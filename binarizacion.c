#include "funciones.c"

//contenido argv: {numero_imagen,pipe_imagen,pipe_info,u_bin,u_clas,bandera,NULL}

int main(int argc, char **argv){

	/* Se obtiene la parte de lectura de los pipes */
	int pipe_conversion_bin_a = atoi(argv[1]);
	int pipe_conversion_bin_b = atoi(argv[2]);
	
	/* Leyendo info*/
	bmpInfoHeader* info = (bmpInfoHeader*)malloc(sizeof(bmpInfoHeader));
	read(pipe_conversion_bin_b,info,sizeof(bmpInfoHeader));

	/* Leyendo imagen*/
	unsigned char *img = (unsigned char*)malloc(sizeof(unsigned char)*info->imgsize);
	read(pipe_conversion_bin_a,img,info->imgsize);

	/* Se reserva memoria para una matriz que representará la imagen binarizada para
       su posterior clasificación */
	int **bin = (int**)malloc((info->alto)*sizeof(int*));
    if(bin == NULL){
        /*En caso de que no se pueda otorgar memoria, finaliza su ejecucion*/
        if(argv[5][0] == '1'){
        	printf("|     imagen_%s.bmp 	 |       Out of memory   |\n", argv[0]);
        }
        exit(0);
    }

    int x;
    for(x = 0; x < info->alto; x++){
        bin[x] = (int*)malloc(info->ancho*sizeof(int));
    }

    /* Binarizamos la imagen */
    int u_binarizacion = atoi(argv[3]);
    int numero_imagen = atoi(argv[0]);
    Binariza(info,img,bin,u_binarizacion,numero_imagen);

    /* Se libera la memoria utilizada en la imagen */

    /* Se guarda la imagen binarizada e info en sus respectivos pipes */
    int pipe_bin_clas_a[2];
    int pipe_bin_clas_b[2];
    pipe(pipe_bin_clas_a);
    pipe(pipe_bin_clas_b);

    /* Una matriz debe ser escrita fila por fila, posteriormente se lee de la misma manera */
    for(x = 0; x < info->alto; x++){
    	write(pipe_bin_clas_a[WRITE],bin[x],info->ancho*sizeof(int));
    }
    write(pipe_bin_clas_b[WRITE],info,sizeof(bmpInfoHeader));
	
	/* Se transforma los pipes a cadenas de char para ser pasados al sgte proceso*/
    char pipe_imagen_char[12]; 
    char pipe_info_char[12];
    snprintf(pipe_imagen_char, 12, "%i", pipe_bin_clas_a[READ]);
    snprintf(pipe_info_char, 12, "%i", pipe_bin_clas_b[READ]);

	/*Se pasa los datos de matriz binarizada e info mediante pipes a sgte proceso*/
    char *argv_clas[6] = {argv[0],pipe_imagen_char,pipe_info_char,argv[4],argv[5],NULL};

    execvp("./clasificacion",argv_clas);

    return 0;
}