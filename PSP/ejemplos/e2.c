#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>


//de padre a hijo con un while por pantalla

int main() {
    int fd[2], numeros, cont = 0, suma = 0;
    char buffer[10]; // Buffer para almacenar la entrada como cadena
    pid_t pid;

    pipe(fd);

    pid = fork();

    if (pid == 0) {
        // Código del hijo
        close(fd[1]); // Cierra el descriptor de escritura
        do {
            read(fd[0], buffer, sizeof(buffer));
            numeros = atoi(buffer); // Convierte la cadena a entero
            suma += numeros;
        } while (buffer[0] != '+');

        printf("\t Suma total: %d \n", suma);
        close(fd[0]);
    } else {
        // Código del padre
        close(fd[0]); // Cierra el descriptor de lectura

        do {
            printf("\t Introduce números (+ para terminar): ");
            scanf("%s", buffer);
            write(fd[1], buffer, sizeof(buffer));
            cont++;
        } while (buffer[0] != '+');

        close(fd[1]); // Cierra el descriptor de escritura
        wait(NULL);
    }

    return 0;
}

