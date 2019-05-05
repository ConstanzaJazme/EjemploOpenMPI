/* Control 02 Computaciòn paralela y distribuida
 *
 * Se modifica codigo de ejemplo y se agrega las funciones para cumplir con los enunciados del control 02.
 *
 * Nombre: Constanza Andrea Jazme Herrera
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* cabecera de las llamadas a MPI */
#include <mpi.h> 
#include <time.h>
#include <iomanip>

using namespace std;

/**
 * Manda un mensaje desde todos los procesos con
 * rango distinto de 0 al proceso 0. El proceso 0
 * los saca por pantalla.
 * Entrada: ninguna
 * Salida: contenido de los mensajes recibidos por 
 * el proceso 0, imprime nombre de la màquina, fecha y hora de esta.
 */
int main(int argc, char** argv) {
    int mi_rango; /* rango del proceso    */
    int p; /* numero de procesos   */
    int fuente; /* rango del que envia  */
    int dest; /* rango del que recibe */
    int tag = 0; /* etiqueta del mensaje */
    int largo_nombre=0; /*Largo del nombre de la maquina*/
    char mensaje[100]; /* mensaje  */
    char nombre[30]; /*Nombre de la maquina*/
    char parcial[40]; /*Fecha que se obtendra*/

    time_t time = std::time(nullptr);

    MPI_Status estado; /* devuelve estado al recibir*/

    /* Comienza las llamadas a MPI */
    MPI_Init(&argc, &argv);

    /* Averiguamos el rango de nuestro proceso */
    MPI_Comm_rank(MPI_COMM_WORLD, &mi_rango);

    /* Averiguamos el número de procesos que estan 
     * ejecutando nuestro porgrama 
     */
    MPI_Comm_size(MPI_COMM_WORLD, &p);
	
   /*Para obtener el nombre de la maquina*/    
    MPI_Get_processor_name(nombre, &largo_nombre);
   
    /*Se agrega el simbolo nulo*/
     nombre[largo_nombre] = '\0';

    if (mi_rango != 0) {
      dest=0;

    /*Se agrega fecha y hora a la variable parcial, en el formato YYY-MM-DD HH:MM:SS*/
      if (std::strftime(parcial, sizeof(parcial), "%F %T", std::localtime(&time))) {
   
	/*Se crea el mensaje*/
        sprintf(mensaje,"%s  %s", nombre, parcial);      
 
	/* Usa strlen+1 para que la marca /0 se transmita */	
        MPI_Send(mensaje, strlen(mensaje)+1, MPI_CHAR, dest, tag, MPI_COMM_WORLD);
      }

    } else { /* mi_rango == 0 */
        for (fuente = 1; fuente < p; fuente++) {
	 MPI_Recv(mensaje, 100, MPI_CHAR, fuente, tag, MPI_COMM_WORLD, &estado);
            printf("%s\n",mensaje);
        }
    }

    /* Termina con MPI. Recordemos que después de 
     * esta llamada no podemos llamar a funciones 
     * MPI, ni siquiera de nuevo a MPI_Init 
     */
    MPI_Finalize();
    return EXIT_SUCCESS;
}
