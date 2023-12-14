#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>


/*

   Padre
   /  
Hijo1  
 hijo envia a padre
 padre envia a hijo

*/


int main() {
    int fd1[2], fd2[2], numeros, numeros2;
    pid_t pid1, pid2;

    // Creamos el primer pipe (Hijo1 -> Padre)
    pipe(fd1);

    // Creamos el segundo pipe (Padre -> Hijo1)
    pipe(fd2);

    // Se crea un proceso hijo (Hijo1)
    pid1 = fork();

    if (pid1 == 0) {
        // Código del primer hijo (Hijo1)
        close(fd1[0]); // Cierra el descriptor de lectura en el Hijo1
        close(fd2[1]); // Cierra el descriptor de escritura en el Hijo1

        printf("Hijo1: Introduce número: ");
        scanf("%d", &numeros);

        write(fd1[1], &numeros, sizeof(numeros));
        
        // Espera a que el padre lea antes de continuar
        wait(NULL);

        
        read(fd2[0], &numeros2, sizeof(numeros2));
        printf("Hijo1: Recibido número del padre : %d\n", numeros2);

    } else {
        // Código del padre
        close(fd1[1]); // Cierra el descriptor de escritura en el Padre
        close(fd2[0]); // Cierra el descriptor de escritura en el Padre

        // Lee el número enviado por el Hijo1
        read(fd1[0], &numeros, sizeof(numeros));
        printf("Padre: Recibido número del Hijo1: %d\n", numeros);
        

        
        numeros2 = numeros+1;
        
        write(fd2[1], &numeros2, sizeof(numeros2));
        
                // Espera a que el hijo termine antes de continuar
        wait(NULL);

    }


}

