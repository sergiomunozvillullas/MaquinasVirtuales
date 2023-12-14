#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h> 
#include <time.h>
#include <string.h>


//P1-P2-P3
//p1 a p2    p2 a p3 y p3 a p2

void main() {
    // Declaración de tuberías
    int fd[2], fd2[2], fd3[2];
    char buffer[5], num1;

    // Declaración de identificadores de procesos
    pid_t pid, pid2;

    // Creamos el pipe
    pipe(fd); 

    // Se crea un proceso hijo
    pid = fork();

    if (pid == 0) {
        // Código del primer hijo
        pipe(fd2); 
        pipe(fd3);
        pid2 = fork();

        if (pid2 == 0) {
            // Código del segundo hijo
            close(fd2[1]);
            int cantNum2 = 0, numero2 = 0, suma = 0, total = 0;
            read(fd2[0], &cantNum2, sizeof(cantNum2));

            for (int i = 0; i < cantNum2; i++) {
                read(fd2[0], &numero2, sizeof(numero2));
                if (numero2 % 5 == 0) {
                    suma += numero2;
                }
                if (numero2 % 5 != 0) {
                    total += 1;
                }
            }

            close(fd3[0]);
            write(fd3[1], &suma, sizeof(suma));
            write(fd3[1], &total, sizeof(total));
        }
        else {
            // Código del primer hijo
            close(fd[1]); // Cierra el descriptor de escritura
            int cantNum, total = 0, suma = 0, numero2 = 0;
            read(fd[0], &cantNum, sizeof(cantNum));

            close(fd2[0]);
            write(fd2[1], &cantNum, sizeof(cantNum));
            for (int i = 0; i < cantNum; i++) {
                printf("Introduce número: ");
                scanf("%d", &numero2);
                write(fd2[1], &numero2, sizeof(numero2));
            }

            wait(NULL); // Espera al tercer proceso

            close(fd3[1]);
            read(fd3[0], &suma, sizeof(suma));
            printf("La suma de los múltiplos de 5 es igual a = %d \n", suma);
            read(fd3[0], &total, sizeof(total));
            printf("Se han procesado números %d no múltiplos de 5 \n", total);
        }
    }
    else {
        // Código del proceso padre
        int numero = 0;
        printf("Introduce la cantidad de numeros a procesar: ");
        scanf("%d", &numero);
        close(fd[0]); // Cierra el descriptor de lectura
        write(fd[1], &numero, sizeof(numero));
        wait(NULL);
    }
}

