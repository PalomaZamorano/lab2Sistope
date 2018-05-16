#include "funciones.h"


/*Función que dado un nombre de archivo, lee el archivo y si corresponde al formato 0x4D42, se leerá la 
imagen que contenga y retorna su contenido en una forma de arreglo de caracteres */
unsigned char *CargarBMP(char *nombre_archivo, bmpInfoHeader *bInfoHeader){

    FILE *archivo;
    bmpFileHeader header;
    unsigned char *imagen;
    unsigned short tipo;

    /* Se abre el archivo y si no se encuentra, finaliza la ejecución */
    archivo = fopen(nombre_archivo, "r");
    if(!archivo){
        return NULL;
    }
    
    /* Si lo encuentra, comienza la lectura de los datos pertinentes y la imagen*/
    fread(&tipo, sizeof(unsigned short), 1, archivo);
    if(tipo != FORMATO){
        fclose(archivo);
        return NULL;
    }

    fread(&header, sizeof(bmpFileHeader), 1, archivo);

    fread(bInfoHeader, sizeof(bmpInfoHeader), 1, archivo);

    imagen=(unsigned char*)malloc(bInfoHeader->imgsize*4);

    if(imagen == NULL){
        printf("Memoria insuficiente para %s \n", nombre_archivo);
        return NULL;
    }

    fseek(archivo, header.offset, SEEK_SET);
    
    fread(imagen, bInfoHeader->imgsize,1, archivo);

    if(imagen == NULL){
        return NULL;
    }

    fclose(archivo);

    return imagen;
}

unsigned char *RedimensionarGrande(bmpInfoHeader *info, unsigned char *img, int reduccion){
    
    int anchog = info->bpmx ;
    int altog = info->bpmx ;
    reduccion = info->compress;

    int tama = anchog*altog*4; //64byts
 

    unsigned char *redimensionado = (unsigned char*)malloc(tama);

    int x,y,v,r,g,b;
    int n,m;
    
    for(y=0; y < altog; y++){
        for(x=0; x < anchog; x++){

        	(redimensionado[4*(x+y*info->alto)]) = 255;
            (redimensionado[4*(x+y*info->alto)+1]) = 255;
            (redimensionado[4*(x+y*info->alto)+2]) = 255;
            (redimensionado[4*(x+y*info->alto)+3]) = 255;
        }
    }        

    for(y=0; y < info->alto; y++){
        for(x=0; x < info->ancho; x++){

        	r=(img[4*(x+y*info->alto)]);
            g=(img[4*(x+y*info->alto)+1]);
            b=(img[4*(x+y*info->alto)+2]);
            v=(img[4*(x+y*info->alto)+3]);

        	for(n=0;n<reduccion;n++){
        		for(m=0;m<reduccion;m++){
          			(redimensionado[4*(((x*reduccion)+n)+((y*reduccion)+m)*altog)]) = r;
           			(redimensionado[4*(((x*reduccion)+n)+((y*reduccion)+m)*altog)+1]) = g;
           			(redimensionado[4*(((x*reduccion)+n)+((y*reduccion)+m)*altog)+2]) = b;
           			(redimensionado[4*(((x*reduccion)+n)+((y*reduccion)+m)*altog)+3]) = v;
           		}	
        
	   		}

        }          
    }

   
    return redimensionado;
}





/*Función que dada una matriz que contiene solo binarios y una posición, asigna un 0 o 1 según la 
comparación entre la escala de gris y el umbral de binariación */
void Binariza(bmpInfoHeader *info, unsigned char *imagen, int **binarizado, int umbral_binarizacion, 
    int numero_imagen){

    int pixel;
    int x, y;
    int alto = info->alto;
    int ancho = info->ancho;

    for(y = alto; y > 0; y--){
        for(x = 0; x < ancho; x++){

            /* Posicion del pixel */
            pixel = 4*(x+y*ancho);
            if((pixel + 3) < info->imgsize){
                /* Según el valor de gris, se binariza la posición y se transforma a blanco o negro el pixel */
                if(imagen[pixel] > umbral_binarizacion){
                    binarizado[x][y] = 1;
                    imagen[pixel] = 255;
                    imagen[pixel+1] = 255;
                    imagen[pixel+2] = 255;
                    imagen[pixel+3] = 255;
                }
                else{
                    binarizado[x][y] = 0;
                    imagen[pixel] = 0;
                    imagen[pixel+1] = 0;
                    imagen[pixel+2] = 0;
                    imagen[pixel+3] = 255;
                }
            }
        }
    }

  	 unsigned char *img1=(unsigned char*)malloc(info->bpmx*info->bpmx*4);	
     img1 = RedimensionarGrande(info,imagen,info->compress);
	    
    /* Preparamos los datos pertinentes a la imagen a guardar */
    bmpInfoHeader *save = malloc(sizeof(bmpInfoHeader));
    save->headersize=sizeof(bmpInfoHeader);
    save->ancho=info->bpmx;
    save->alto=info->bpmx;
    save->planos=1;
    save->bpp=32;
    save->compress=0;
    save->imgsize=save->ancho*save->ancho*4;
    save->bpmx=(unsigned)round((double)50*100/2.54);
    save->bpmy=save->bpmx;
    save->colors=0;
    save->imxtcolors=0;
    
    /* Se prepara el nombre para guardar la nueva imagen */
    char numero_char[3];
    sprintf(numero_char,"%d",numero_imagen);
    char *nuevo_archivo = (char*)malloc(17 + sizeof(numero_char));
    Nombrar(nuevo_archivo,"Nueva_imagen_",strlen(numero_char),numero_char);

    /* Se guarda la imagen en escala de grises */
    GuardarBMP(nuevo_archivo,save,img1);

}

/*Función que dado un nombre de archivo y una imagen, crea un archivo y guarda la imagen recibida */
void GuardarBMP(char *nombre_archivo, bmpInfoHeader *info, unsigned char *imagen){
  
  	bmpFileHeader header;
  	FILE *archivo;
  	unsigned short tipo;
  	archivo = fopen(nombre_archivo, "w+");
  	header.size = info->imgsize+sizeof(bmpFileHeader)+sizeof(bmpInfoHeader);

  	/* El offset será el tamaño de las dos cabeceras + 2 (información de fichero)*/
  	header.offset=sizeof(bmpFileHeader)+sizeof(bmpInfoHeader)+2;

  	/* Escribimos la identificación del archivo */
  	tipo = FORMATO;
  	fwrite(&tipo, sizeof(tipo),1,archivo);

  	/* Escribimos la cabecera de fichero */
  	fwrite(&header, sizeof(bmpFileHeader),1,archivo);

  	/* Escribimos la información básica de la imagen */
  	fwrite(info, sizeof(bmpInfoHeader),1,archivo);

  	/* Escribimos la imagen y guardamos*/
  	fwrite(imagen, info->imgsize, 1, archivo);
  	fclose(archivo);
}

/*Función que dado un nombre de archivo en limpio, un borrador y un número en forma de cadena de 
caracteres, guarda en nombre archivo el borrador seguido del numero de imagen y con formato .bmp incluido */
void Nombrar(char *nombre_archivo, char* borrador, char largo_numero, char numero_char[largo_numero]){
    
    int i;
    int largo_borrador = strlen(borrador);
    for(i = 0; i < largo_borrador; i++){
        nombre_archivo[i] = borrador[i];
    }

    for(i = 0; i < largo_numero; i++){
        nombre_archivo[largo_borrador+i] = numero_char[i];
    }

    /* Se añade el formato */
    nombre_archivo[largo_borrador+largo_numero] = '.';
    nombre_archivo[largo_borrador+largo_numero+1] = 'b';
    nombre_archivo[largo_borrador+largo_numero+2] = 'm';
    nombre_archivo[largo_borrador+largo_numero+3] = 'p';
    nombre_archivo[largo_borrador+largo_numero+4] = '\0';
}

//No sé como explicar esto
void EscalaGrises(unsigned char *imagen, bmpInfoHeader*info){

    int pixel;
    int x, y;
    int r, g, b;
    int gris;
	
	
    for(y=info->alto; y > 0; y-=1){
        for(x=0; x < info->ancho; x+=1){
            
        	/* Posicion del pixel */
            pixel= 4*(x+y*info->ancho);
            
            /* Se obtienen los valores de cada componente, excepto el valor de suavizado ya que no 
            es utilizado */
            b=imagen[pixel];
            g=imagen[pixel+1];
            r=imagen[pixel+2];	

            /* Se convierte a gris con la formula y se transforma los valores de cada componente*/
            gris = (r*0.3) + (g*0.59) + (b*0.11);
			imagen[pixel] = gris;
			imagen[pixel+1] = gris;
			imagen[pixel+2]= gris;
			imagen[pixel+3]= 255;
        }
    }
}

/*Función que valida si una cadena de caracteres recibida por pantalla corresponde a un número antes de 
transformarlo a entero*/
int ValidarNumero(int largo,char cadena[largo]){

    int i;
    for(i = 0; i < largo; i++){
        /* '0' -> 48 , '9' -> 57 */
        if(!(cadena[i] > 47 && cadena[i] < 58)){
            /* Si no se encuentra en el rango, mandar inmediatamente FALSE */
            return FALSE;
        }
    }
    /* Si nunca se encontró un caracter fuera de rango, mandar TRUE, ya que siempre se halló numeros */
    return TRUE;
}



/*Función que dada una matriz que contiene numeros binarios, determina según el umbral de clasificación si
es Nearly Black o no*/
int ClasificarImagen(bmpInfoHeader *info, int **binarizado, int umbral_clasificacion){

    int i,j;
    int alto = info->alto, ancho = info->ancho;
    float negros = 0, total = ((float)alto)*((float)ancho), porcentaje_negros;
    
    for(i = 0; i < alto; i++){
        for(j = 0; j < ancho; j++){
            /* Si es 0, negro */
            if(binarizado[i][j] == 0){
                negros++;
            }
        }
    }

    porcentaje_negros = negros*100/total;
    /* Según el porcentaje de pixeles negros, se califica como Nearly Black o no*/
    if(porcentaje_negros >= umbral_clasificacion){
        return 1;
    }
    else{
        return 0;
    }
}

/*Función que verifica si los valores ingresados por pantalla, son válidos para la ejecución del programa*/
int VerificarValores(int verificador_C, int verificador_U, int verificador_N, int c, int u, int n){
    
    if(verificador_C == 0 || verificador_U == 0 || verificador_N == 0){
        printf("Falta asignar valor a: ");
        if(verificador_C == 0){
            printf("cantidad de imagenes ");
        }
        if(verificador_U == 0){
            printf("umbral de binarización ");
        }
        if(verificador_N == 0){
            printf("umbral de clasificación. ");
        }    
        printf("\n");
        return -1;
    }

    if(n >= 100){
        printf("El umbral de clasificación es demasiado alto. \n");
        return -1;
    }
    if(c < 1){
        printf("Debe ingresar al menos una imagen a procesar. \n");
        return -1;   
    }

    return 0;
}

/*Función que contiene las operaciones de lectura de datos en la terminal y verifica si son validos o no*/
int LeerTerminal(int argc, char **argv, int *cantidad_imagenes, int *umbral_binarizacion, 
    int *umbral_clasificacion, int *bandera){

    int a;
    opterr = 0;
    char *c = NULL, *u = NULL, *n = NULL;
    int verC = 0, verU = 0, verN = 0;

    while ((a = getopt (argc, argv, "c:u:n:b")) != -1){
        switch (a){
        	/* Se valida si es entero cada parametro ingresado, y si lo son, 
        	se asigna al valor correspondiente */
            case 'c':
                c = optarg;
                if(ValidarNumero(strlen(c),c) == FALSE){
                    printf("La cantidad de imagenes ingresadas, no es un tipo de dato válido \n");
                    return -1;
                }
                else{
                    verC = 1;
                    *cantidad_imagenes = atoi(c);
                }
                break;
            case 'u':
                u = optarg;
                if(ValidarNumero(strlen(u),u) == FALSE){
                    printf("El umbral de binarizacion ingresado, no es un tipo de dato válido \n");
                    return -1;
                }
                else{
                    verU = 1;
                    *umbral_binarizacion = atoi(u);
                }
                break;
            case 'n':
                n = optarg;
                if(ValidarNumero(strlen(n),n) == FALSE){
                    printf("El umbral de clasificación ingresado, no es un tipo de dato válido \n");
                    return -1;
                }
                else{
                    verN = 1;
                    *umbral_clasificacion = atoi(n);
                }
                break;
            case 'b':
                *bandera = 1;
                break;
            case '?':
                if (optopt == 'c' || optopt == 'u' || optopt == 'n' || optopt == 'b'){
                    fprintf (stderr, "Opcion -%c requiere un argumento.\n", optopt);
                }
                else if (isprint (optopt)){
                    fprintf (stderr, "Opcion desconocida `-%c'.\n", optopt);
                }
                else{
                    fprintf (stderr, "Opcion con caracter desconocido `\\x%x'.\n", optopt);
                }
            return -1;

            default:
                abort ();
        }
    }

    /* Si los valores no son válidos, se retorna -1 */
    if(VerificarValores(verC, verU, verN, *cantidad_imagenes, *umbral_binarizacion, 
        *umbral_clasificacion) == -1){
        return -1;
    }

    return 0;
}

unsigned char *Redimensionar(bmpInfoHeader *info, unsigned char *img){

    int ancho = info->ancho, alto = info->alto;
    int reduccionX = ancho/128;
    int reduccionY = alto/128;
    int nuevo_ancho = 128;
    int nuevo_alto = 128;
    info->imgsize = 128*128*4; //64byts
    info->bpmx= alto;
    info->compress = reduccionY;
    unsigned char *redimensionado = (unsigned char*)malloc(info->imgsize);

    int x,y,b,g,r,v;

    for(y=0; y < nuevo_alto; y++){
        for(x=0; x < nuevo_alto; x++){

        	b=(img[4*((x*reduccionX)+(y*reduccionY)* info->alto)]);
            g=(img[4*((x*reduccionX)+(y*reduccionY)* info->alto)+1]);
            r=(img[4*((x*reduccionX)+(y*reduccionY)* info->alto)+2]);
            v=(img[4*((x*reduccionX)+(y*reduccionY)* info->alto)+3]);
            
            (redimensionado [4*(x+y*nuevo_alto)]) = b;
            (redimensionado [4*(x+y*nuevo_alto)+1]) = g;
            (redimensionado [4*(x+y*nuevo_alto)+2]) = r;
            (redimensionado [4*(x+y*nuevo_alto)+3])= v;
    	}
    }	

    info->ancho = nuevo_ancho;
    info->alto = nuevo_alto;

    /*Libera memoria de la antigua imagen*/
    free(img);
    img = NULL;

    return redimensionado;
}

