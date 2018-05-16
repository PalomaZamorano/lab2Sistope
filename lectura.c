#include "funciones.c"

//contenido argv: {numero_imagen,u_bin_char,u_clas_char,bandera_char,NULL}

int main(int argc, char **argv){

    int pipe_lectura_conversion_a[2];
    int pipe_lectura_conversion_b[2];
    pipe(pipe_lectura_conversion_a);
    pipe(pipe_lectura_conversion_b);
    
    bmpInfoHeader* info = (bmpInfoHeader*)malloc(sizeof(bmpInfoHeader));

	/* Se reserva memoria para el nombre de archivo a leer */
    char *nombre_archivo = (char*)malloc(11 + sizeof(argv[0]));

    /* Se da nombre a la cadena */
    Nombrar(nombre_archivo,"imagen_",strlen(argv[0]), argv[0]);
            
    /* Se obtiene la imagen y se redimensiona, con tal de que pueda ser escrito en el pipe */
    unsigned char *img;
    img = CargarBMP(nombre_archivo, info);

    /* Se verifica que la imagen exista*/
    if(img == NULL){
        if(argv[3][0] == '1'){
            printf("|     %s \t |       Not found       |\n", nombre_archivo);
        }
        exit(0);
    }

    img = Redimensionar(info,img);

    /*Se escribe información de imagen e imagen en el pipe*/
    write(pipe_lectura_conversion_a[WRITE],img,info->imgsize);
    write(pipe_lectura_conversion_b[WRITE],info,sizeof(bmpInfoHeader));

    /*Se transforma los pipes a cadenas de char para ser pasados al sgte proceso*/
    char pipe_imagen_char[12]; 
    char pipe_info_char[12];
    snprintf(pipe_imagen_char, 12, "%i", pipe_lectura_conversion_a[READ]);
    snprintf(pipe_info_char, 12, "%i", pipe_lectura_conversion_b[READ]);

    /*Se pasa el numero de imagen y los pipes a sgte proceso*/
    char *argv_lectura[7] = {argv[0],pipe_imagen_char,pipe_info_char,argv[1],argv[2],argv[3],NULL};

    /*Se ejecuta proceso de conversión */
    execvp("./conversion",argv_lectura);

    return 0;
}