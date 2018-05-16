#ifndef __funciones_h__
#define __funciones_h__

#include <math.h>
#include <ctype.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/types.h>

#define READ 0
#define WRITE 1
#define TRUE 2
#define FALSE 3
#define FORMATO 0x4D42

typedef struct bmpFileHeader{
    /* 2 bytes de identificación */
    unsigned int size;		        /* Tamaño del archivo */
    unsigned short int resv1;       /* Reservado */
    unsigned short int resv2;       /* Reservado */
    unsigned int offset;    		/* Offset hasta los datos de imagen */
} bmpFileHeader;

typedef struct bmpInfoHeader{
    unsigned int headersize;      	/* Tamaño de la cabecera */
    int ancho;
    int alto;
    unsigned short planos;          /* Planos de color (Siempre 1) */
    unsigned short bpp;             /* Bits por pixel */
    unsigned int compress;        	/* Compresión */
    unsigned int imgsize;     		/* Tamaño de los datos de imagen */
    int bpmx;        				/* Resolución X en bits por metro */
    int bpmy;        				/* Resolución Y en bits por metro */
    unsigned int colors;      		/* Colores usados en la paleta */
    unsigned int imxtcolors;   
   	/* Colores importantes. 0 si son todos */
} bmpInfoHeader;

    int altoa;
    int anchoa;
    int reducciona;

unsigned char *CargarBMP(char *nombre_archivo, bmpInfoHeader *bInfoHeader);
void Binariza(bmpInfoHeader *info, unsigned char *imagen, int **binarizado, int umbral_binarizacion, 
    int numero_imagen);
void GuardarBMP(char *nombre_archivo, bmpInfoHeader *info, unsigned char *imagen);
void Nombrar(char *nombre_archivo, char* borrador, char largo_numero, char numero_char[largo_numero]);
void EscalaGrises(unsigned char *imagen, bmpInfoHeader*info);
int ValidarNumero(int largo,char cadena[largo]);
int ClasificarImagen(bmpInfoHeader *info, int **binarizado, int umbral_clasificacion);
int VerificarValores(int verificador_C, int verificador_U, int verificador_N, int c, int u, int n);
int LeerTerminal(int argc, char **argv, int *cantidad_imagenes, int *umbral_binarizacion, 
    int *umbral_clasificacion, int *bandera);
unsigned char *Redimensionar(bmpInfoHeader *info, unsigned char *img);
#endif
