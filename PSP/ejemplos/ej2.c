#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h> 


/*
hijo envia a padre
y hay otro hijo del padre para controlar las salidas

 P1 (Padre)
        / \
     P2   P3

*/

void main(){

     int fd[2], fd2[2], numeros, numeros2; 
     pid_t pid, pid2;
    
     // Creamos el pipe
     pipe(fd); 
     
     //Se crea un proceso hijo
     pid = fork();

     if (pid==0)
     
     {
                close(fd[0]); // Cierra el descriptor de lectura
                printf("El hijo escribe en el PIPE \n");
                for(int i=0; i<3; i++)
                {
                printf("\t Introduce número: ");
                scanf("%d", &numeros);
                write(fd[1], &numeros, sizeof(numeros)); 
                }
     
     }
     
     else
     
     {
                close(fd[1]); // Cierra el descriptor de lectura
                wait(NULL);
                printf("El padre lee el PIPE\n");
                for(int i=0; i<3; i++)
                {
                read(fd[0],&numeros2, sizeof(numeros2));  
                printf("\t Mensaje leído del pipe: %d \n", numeros2);
                }


     pid2 = fork();
     if (pid2==0)
 
     {
                printf("Este es el ultimo mensaje a leer del padre\n");
     }
     else
     
     {
         wait(NULL);
         
     }
    
     }
     

      
      
        
}
