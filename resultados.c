#include "bibliotecas.c"


int main(int argc, char **argv){

	//Se determina resultado final.
	if(argv[1][0] == '1' ){
		//nearly black
		printf("|     imagen_%s.bmp 	 |       Yes             |\n", argv[0]);
	}
	else{
		printf("|     imagen_%s.bmp 	 |       No              |\n", argv[0]);
	}

}