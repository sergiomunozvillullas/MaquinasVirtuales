#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h> 
#include <time.h>


int main(){
    int fd1[2];
    int fd2[2];
    int fd3[2];

    int status;
   
    int entero;
    int totalNumero=0;
    int suma=0;
    int noMultiplo=0;
    int numeros[4];
    
    pid_t p1,p2,p3;

    pipe(fd1);
    

    p1=fork();

    if(p1==0){
        /*IMPORTANTE CREAR LOS PIPES SOLO CUANDO SE VAN A DAR USO DE LO CONTRARIO
        DEPENDIENDO DE LO QUE SE PIDA*/
        pipe(fd2);
         pipe(fd3);
        p2=fork();
         
        if(p2==0){
            close(fd2[1]);
            
            /*4.Como dije antes al abrir el pipe fd2 enviamos primero la variable de NUMEROS 
            y luego la de TOTALNUMEROS en caso contrario dará ERROR*/
            read(fd2[0], &numeros,sizeof(numeros));
            read(fd2[0], &totalNumero,sizeof(totalNumero));
            
            //Linea solo para comprobar que llega el numero
            printf("proceso p3 enteros :%d\n",numeros[1]);
           
           printf("totalNumero %d recibido por p3 \n",totalNumero);

            for(int i =0;i<totalNumero;i++){
                if(numeros[i]%5==0){
                    suma+=numeros[i];
                }
                if(numeros[i]%5!=0){
                    noMultiplo++;
                }
            }
            //printf("suma multiplos %d\n",suma);
            //printf("suma no multiplos %d\n",noMultiplo);

            /*5.Finalmente para terminar de la misma manera que se explico antes 
            enviamos los datos de manera ordenada por el tercer pipe FD3*/
            close(fd3[0]);
            write(fd3[1],&suma,sizeof(suma));
            write(fd3[1],&noMultiplo,sizeof(noMultiplo));

            
        }
        else{
            /*2.Aqui recibe mediante el red y leyendo del pipe1 que se creo en 
            el padre*/
            close(fd1[1]);
           
            read(fd1[0],&totalNumero,sizeof(totalNumero));
            //la linea de abajo es solo para verificar que en efecto ha llegado el numero
            printf("numero %d recibido por p2\n",totalNumero);
            
            close(fd1[0]);
            close(fd2[0]);
            
            
           for(int i =0;i < totalNumero;i++)
           {
                printf("escribe los numeros \n");
               scanf(" %d",&numeros[i]);
              
           }
           /*3.Aqui esta parte es muy IMPORTANTE los pipes son secuenciales es decir no almacenan
           solo leen uno a uno asi que si se envia primero la variable de numeros se tiene que
           leer por el otro lado del pipe en la misma secuencia sino tendremos ERRORES*/
            write(fd2[1],&numeros,sizeof(numeros));
            write(fd2[1],&totalNumero,sizeof(totalNumero));
            
            
            close(fd2[1]);

            /*6.Al leerlo lo leemos en el mismo orden en lo que lo hemos enviado para 
            no tener errores*/
            close(fd3[1]);
            read(fd3[0],&suma,sizeof(suma));
            read(fd3[0],&noMultiplo,sizeof(noMultiplo));
            printf("suma de multiplos %d\n",suma);
            printf("Cantidad de no multiplos %d\n",noMultiplo);
            
        }
    }
    else{
        //1.abrimos el primer pipi para enviarlo al proceso p2
       close(fd1[0]);
       printf("Introduce la cantidad de numeros;\n");
       scanf("%d",&totalNumero);
       write(fd1[1],&totalNumero,sizeof(totalNumero));

       //1.si no se ponen los wait el proceso padre se ejecuta y da error.
       //1.almenos en el padre si hace falta poner la espera.
        p1=wait(NULL);
        p2=wait(NULL);
        p3=wait(NULL);


    }
}
