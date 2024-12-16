#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[]) {

	// Si el nombre d'arguments adjuntat és menor a 3 (nom de comanda, primer arxiu, segon arxiu), no podem operar
	if (argc < 3) {
		// Finalitzem el programa amb el valor -1 com a signe d'error
        	exit(-1);
    	}

	// Obrim els arxius mitjançant el seu nom adjuntat per l'usuari i emmagatzemat a argv[1] i argv[2]
    	// Donat que només es contempla fer-ne la lectura, s'utilitza el flag O_RONLY (només lectura)
	int fd1 = open(argv[1], O_RDONLY);

	// Si l'arxiu no existeix o s'ha provocat un error, es notifica a l'usuari
	if (fd1 == -1) {
        	perror("Error en obrir el primer arxiu");
        	exit(-1);
    	}

    	int fd2 = open(argv[2], O_RDONLY);
    	if (fd2 == -1) {
        	perror("Error en obrir el segon arxiu");
        	close(fd1);
        	exit(-1);
    	}

	// Iniciem les variables de lectura byte a byte per a cada arxiu (per tant, tipus char): byte1 i byte2
    	char byte1, byte2;

	// Iniciem les variables lectura1 i lectura2 per controlar-ne l'extensió i els errors que se'n puguin derivar
	// El tipus ssize_t gestiona valors enters signats de processos que treballen amb E/S (read, write...)
    	ssize_t lectura1, lectura2;

	// Iniciem les variables enteres posicio i linia i els assignem el valor 1
	int posicio = 1, linia = 1;

	// Iniciem dues variables string com els noms dels arxius amb els que es treballen
	char* arxiu1 = argv[1];
	char* arxiu2 = argv[2];

	// Establim un bucle while mentre cap de les lectures ha finalitzat ni s'ha produït un error (0 i -1 respectivament)
	// El programa llegeix byte a byte i s'emmagatzemen els caràcters en les corresponents variables: byte1 i byte2
    	while ((lectura1 = read(fd1, &byte1, 1)) > 0 && (lectura2 = read(fd2, &byte2, 1)) > 0) {

		// Per a cada iteració, si els byte dels arxius és diferent, notifiquem a l'usuari
		if (byte1 != byte2) {
        		printf("%s %s differ: byte %d, line %d\n", arxiu1, arxiu2, posicio, linia);
            		close(fd1);
            		close(fd2);

			// Finalitzem el programa amb un 0 com a operació satisfactòria
            		exit(0);
        	}

		// Pel cas particular del salt de línia (\n), s'incrementa la variable linia en 1
        	if (byte1 == '\n'){
			linia++;
		}

		// Per a cada iteració, s'incrementa la variable posicio en 1
		posicio++;
    	}

	// Informem al kernel del final dels processos sobre els arxius i l'alliberament de recursos
    	close(fd1);
    	close(fd2);

	// Finalment, podem sortir del programa amb el valor 0 com a signe d'operació satisfactòria
    	exit(0);
}
