#include "funciones.c"

//contenido argv: {numero_imagen, bin, info, u_clas, bandera, NULL}

int main(int argc, char **argv){

	/* Se obtiene la parte de lectura de los pipes */
	int pipe_bin_clas_a = atoi(argv[1]);
	int pipe_bin_clas_b = atoi(argv[2]);
	
	/* Leyendo info*/
	bmpInfoHeader* info = (bmpInfoHeader*)malloc(sizeof(bmpInfoHeader));
	read(pipe_bin_clas_b,info,sizeof(bmpInfoHeader));

	/* Leyendo matriz*/
	int **bin = (int**)malloc((info->alto)*sizeof(int*));
	if(bin == NULL){
		if(argv[5][0] == '1'){
			printf("|     imagen_%s.bmp 	 |       Out of memory   |\n", argv[0]);
		}
		exit(0);
	}
    
    /* Se lee fila a fila desde el pipe*/
    int x;
    for(x = 0; x < info->alto; x++){
        bin[x] = (int*)malloc(info->ancho*sizeof(int));
        read(pipe_bin_clas_a,bin[x],info->ancho*sizeof(int));
    }

    /* Se clasifica la imagen*/
    int u_clasificacion = atoi(argv[3]);
    int resultado_clasificacion = ClasificarImagen(info,bin,u_clasificacion);
   	
    /* Se libera la memoria utilizada en la matriz binarizada */
    for(x = 0; x < info->alto; x++){
    	free(bin[x]);
    	bin[x] = NULL;
    }
    /* Si se desea mostrar resultados */
    if(argv[4][0] == '1'){

    	/* Se envía el numero de imagen y el resultado de la clasificación al sgte proceso */
	    char *resultado = (char*)malloc(sizeof(char)*1);
	    sprintf(resultado,"%d",resultado_clasificacion);
		
	    char *argv_resultados[3] = {argv[0],resultado,NULL};

	   	execvp("./resultados",argv_resultados);
    }
    return 0;
}