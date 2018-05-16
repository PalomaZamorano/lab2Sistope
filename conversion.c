#include "funciones.c"

//contenido argv: {numero_imagen,pipe_imagen,pipe_info,u_bin,u_clas,bandera,NULL}

int main(int argc, char **argv){
	
	/* Se obtiene la parte de lectura de los pipes */
	int pipe_lectura_conversion_a = atoi(argv[1]);
	int pipe_lectura_conversion_b = atoi(argv[2]);
	
	/* Leyendo info*/
	bmpInfoHeader* info = (bmpInfoHeader*)malloc(sizeof(bmpInfoHeader));
	read(pipe_lectura_conversion_b,info,sizeof(bmpInfoHeader));

	/* Leyendo imagen*/
	unsigned char *img = (unsigned char*)malloc(sizeof(unsigned char)*info->imgsize);
	read(pipe_lectura_conversion_a,img,info->imgsize);

	/* Se transforma la imagen a gris */
    EscalaGrises(img,info);

    /* Se guarda la imagen e info en sus respectivos pipes */
    int pipe_conversion_bin_a[2];
    int pipe_conversion_bin_b[2];
    pipe(pipe_conversion_bin_a);
    pipe(pipe_conversion_bin_b);

    write(pipe_conversion_bin_a[WRITE],img,info->imgsize);
    write(pipe_conversion_bin_b[WRITE],info,sizeof(bmpInfoHeader));
	
	/* Se transforma los pipes a cadenas de char para ser pasados al sgte proceso*/
    char pipe_imagen_char[12]; 
    char pipe_info_char[12];
    snprintf(pipe_imagen_char, 12, "%i", pipe_conversion_bin_a[READ]);
    snprintf(pipe_info_char, 12, "%i", pipe_conversion_bin_b[READ]);

    /* Solo se cambian los pipes a pasar al sgte proceso */
    argv[1] = pipe_imagen_char;
    argv[2] = pipe_info_char;

    /* Se ejecuta proceso de binarización */
    execvp("./binarizacion",argv);

    return 0;
}