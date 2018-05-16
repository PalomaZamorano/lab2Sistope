#include "funciones.c"


int main(int argc, char **argv){

	/*Se validan los datos ingresados por terminal*/
	int cantidad_imagenes, umbral_binarizacion, umbral_clasificacion, bandera = 0;
	if(LeerTerminal(argc,argv,&cantidad_imagenes,&umbral_binarizacion,&umbral_clasificacion,
    	&bandera) == -1){
        return -1;
    }

    if(bandera == 1){
    	printf("\n|     image \t\t |      nearly black     |\n");
        printf("|------------------------+-----------------------|\n");
    }

	int i, wait;
	for(i = 1; i <= cantidad_imagenes;i++){
		pid_t pid = fork();
		if(pid == 0){
			
			char *numero_imagen = (char*)malloc(sizeof(char)*3);
			char *u_bin_char = (char*)malloc(sizeof(char)*3);
			char *u_clas_char = (char*)malloc(sizeof(char)*3);
			char *bandera_char = (char*)malloc(sizeof(char)*1);

			char *argv_main_lectura[5];
			argv_main_lectura[4] = NULL;

			/*Numero de imagen */
			sprintf(numero_imagen,"%d",i);
			argv_main_lectura[0] = (char*)malloc(sizeof(char)*3);
			argv_main_lectura[0] = numero_imagen;

			/*Umbral binarización */
			sprintf(u_bin_char,"%d",umbral_binarizacion);
			argv_main_lectura[1] = (char*)malloc(sizeof(char)*3);
			argv_main_lectura[1] = u_bin_char;

			/*Umbral clasificación */
			sprintf(u_clas_char,"%d",umbral_clasificacion);
			argv_main_lectura[2] = (char*)malloc(sizeof(char)*3);
			argv_main_lectura[2] = u_clas_char;

			/*Bandera */
			sprintf(bandera_char,"%d",bandera);
			argv_main_lectura[3] = (char*)malloc(sizeof(char)*1);
			argv_main_lectura[3] = bandera_char;

			/*Ejecuta el proceso de lectura */
		    execvp("./lectura",argv_main_lectura);
		    exit(0);
		}
		waitpid(pid, &wait, 0);
	}
	if(bandera == 1){
		printf("+------------------------+-----------------------+\n");
	}

	return 0;
}