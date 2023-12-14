#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

/*

   Padre
   /   \
Hijo1  Hijo2

del hijo1 al padre, del padre al hijo2 y del hijo2 al hijo1

*/

int main() {
    int fd1[2], fd2[2], fd3[2], numeros;

    pid_t pid1, pid2;

    // Creamos el primer pipe (Hijo1 -> Padre)
    pipe(fd1);

    // Creamos el segundo pipe (Padre -> Hijo2)
    pipe(fd2);

    // Creamos el tercer pipe (Hijo2 -> Hijo1)
    pipe(fd3);

    // Se crea un proceso hijo (Hijo1)
    pid1 = fork();

    if (pid1 == 0) {
        // Código del primer hijo (Hijo1)
        close(fd1[0]); // Cierra el descriptor de lectura en el Hijo1

        printf("Hijo1: Introduce número: ");
        scanf("%d", &numeros);

        write(fd1[1], &numeros, sizeof(numeros));

        close(fd3[1]); // Cierra el descriptor de escritura en el Hijo1
        read(fd3[0], &numeros, sizeof(numeros));
        printf("Hijo1: Recibido número del Hijo2: %d\n", numeros);
    } else {
        // Código del padre
        close(fd1[1]); // Cierra el descriptor de escritura en el Padre

        // Lee el número enviado por el Hijo1
        read(fd1[0], &numeros, sizeof(numeros));
        printf("\n");
        printf("Padre: Recibido número del Hijo1: %d\n", numeros);

        // Se crea un segundo proceso hijo (Hijo2)
        pid2 = fork();

        if (pid2 == 0) {
            // Código del segundo hijo (Hijo2)
            close(fd2[1]); // Cierra el descriptor de escritura en el Hijo2

            read(fd2[0], &numeros, sizeof(numeros));
            printf("\n");
            printf("Hijo2: Recibido el número: %d\n", numeros);

            // Devuelve el número al Hijo1
            write(fd3[1], &numeros, sizeof(numeros));
        } else {
            close(fd2[0]); // Cierra el descriptor de lectura en el Padre

            // Envía el número al Hijo2
            write(fd2[1], &numeros, sizeof(numeros));

            // Espera a que ambos hijos terminen
            wait(NULL);
            wait(NULL);

            // Lee el número devuelto por el Hijo2
            read(fd3[0], &numeros, sizeof(numeros));
            printf("\n");
            printf("Padre: Recibido número del Hijo2: %d\n", numeros);

            // Suma 1 al número
            numeros += 1;
            printf("Padre: Número incrementado por 1: %d\n", numeros);
        }
    }

    return 0;
}

