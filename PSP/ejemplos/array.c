#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define ARRAY_SIZE 5

int main() {
    int myArray[ARRAY_SIZE] = {1, 2, 3, 4, 5};
    int fd[2];
    pid_t pid;

    // Crear la tubería
    if (pipe(fd) == -1) {
        perror("Error al crear la tubería");
        exit(EXIT_FAILURE);
    }

    // Crear un proceso hijo
    pid = fork();

    if (pid == -1) {
        perror("Error al crear el proceso hijo");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        // Código del proceso hijo
        close(fd[0]); // Cerrar el extremo de lectura de la tubería en el proceso hijo
        write(fd[1], myArray, sizeof(myArray)); // Escribir el array en la tubería
        close(fd[1]); // Cerrar el extremo de escritura de la tubería en el proceso hijo
    } else {
        // Código del proceso padre
        close(fd[1]); // Cerrar el extremo de escritura de la tubería en el proceso padre

        int receivedArray[ARRAY_SIZE];

        read(fd[0], receivedArray, sizeof(receivedArray)); // Leer el array desde la tubería

        close(fd[0]); // Cerrar el extremo de lectura de la tubería en el proceso padre

        // Imprimir el array recibido
        printf("Array recibido por el proceso padre: ");
        for (int i = 0; i < ARRAY_SIZE; i++) {
            printf("%d ", receivedArray[i]);
        }
        printf("\n");
    }

    return 0;
}

