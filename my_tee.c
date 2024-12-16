#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[]) {

	// Si el nombre d'arguments adjunts són menor a 2 (nom de comanda, nom arxiu), no podem operar
	if (argc < 2) {
		// Finalizem el programa amb el valor -1 com a signe d'error
        	exit(-1);
    	}

	// Gestionem la sobreescriptura (O_WRONLY) o creació (O_CREAT) del nom d'arxiu adjuntat mitjançant flags
	// Afegim el mode 0644 que significa rw-r--r-- en base octal
		// És a dir, permisos de lectura i escriptura pel propietari i només lectura pel grup i altres usuaris
	// Iniciem la variable entera fd i l'assignem el descriptor d'arxiu per fer-ne la gestió dels recursos
    	int fd = open(argv[1], O_WRONLY | O_CREAT, 0644);

	// Si no s'ha pogut obrir o crear l'arxiu, es notifica a l'usuari
    	if (fd == -1) {
        	perror("Error creant l'arxiu");
		// Finalitzem el programa amb el valor -1 com a signe d'error
        	exit(-1);
    	}

	// Iniciem la variable buffer que emmagatzema byte a byte (per tant, continguts char)
    	char buffer;

	// Iniciem la variable lectura, que gestiona els valors enters signats retornats pel procés read
	ssize_t lectura;

	// Establim un bucle while mentre la lectura de l'arxiu no ha finalitzat o s'obté un error (0 i -1 respectivament)
	// Llegim directament des de l'entrada estàndard, STDIN_FILENO, que constitueix la ruta proveïda
	while ((lectura = read(STDIN_FILENO, &buffer, 1)) > 0) {

		// Per a cada iteració, escrivim un byte de l'entrada estàndard a l'arxiu mitjançant write
		// Si rebem un error (-1) d'escriptura, notifiquem a l'usuari
        	if (write(fd, &buffer, 1) == -1) {
            		perror("Error escrivint a l'arxiu");

			// Tanquem l'arxiu i finaltizem el programa
			close(fd);
            		exit(-1);
        	}

		// Per a cada iteració, enviem un byte de tornada a l'entrada estàndard
		// Si rebem un error (-1) d'escriptura, notifiquem a l'usuari
        	if (write(STDIN_FILENO, &buffer, 1) == -1) {
            		perror("Error escrivint a la sortida estàndard");

			// Tanquem l'arxiu i finaltizem el programa
			close(fd);
            		exit(-1);
        	}
    	}

	// Ja fora del bucle, si s'ha sortit per un error de lectura (-1), notifiquem a l'usuari
    	if (lectura == -1) {
        	perror("Error llegint l'entrada estàndard");

		// Tanquem l'arxiu i finalitzem el programa
		close(fd);
        	exit(-1);
    	}

	// Notifiquem al kernel de la finalització de processos i alliberament de recursos
    	close(fd);

	// Finalitzem el programa amb el valor 0 com a operació satisfactòria
	exit(0);
}
