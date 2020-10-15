#include <stdio.h>      // libreria estandar
#include <stdlib.h>     // para usar exit y funciones de la libreria standard
#include <string.h>
#include <pthread.h>    // para usar threads
#include <semaphore.h>  // para usar semaforos
#include <unistd.h>


#define LIMITE 50

//creo estructura de semaforos 
struct semaforos {
    sem_t sem_mezclar;
    sem_t sem_salar;
    sem_t sem_armarMedallones;
    sem_t sem_cocinar; 
    sem_t sem_armarHamburguesas;
	//poner demas semaforos aqui
};

//creo los pasos con los ingredientes
struct paso {
   char accion [LIMITE];
   char ingredientes[4][LIMITE];

};

//creo los parametros de los hilos 
struct parametro {
 int equipo_param;
 struct semaforos semaforos_param;
 struct paso pasos_param[8];
};

void* imprimirAccion(void *data, char *accionIn) {
	struct parametro *mydata = data; 
	int sizeArray = (int)( sizeof(mydata->pasos_param) / sizeof(mydata->pasos_param[0]));
        int i;
	for(i = 0; i < sizeArray; i ++){
		if(strcmp(mydata->pasos_param[i].accion, accionIn) == 0){
		printf("\tEquipo %d - accion %s \n " , mydata->equipo_param, mydata->pasos_param[i].accion);
		int sizeArrayIngredientes = (int)( sizeof(mydata->pasos_param[i].ingredientes) / sizeof(mydata->pasos_param[i].ingredientes[0]) );
		int h;
		printf("\tEquipo %d -----------ingredientes : ----------\n",mydata->equipo_param); 
			for(h = 0; h < sizeArrayIngredientes; h++) {
				if(strlen(mydata->pasos_param[i].ingredientes[h]) != 0) {
							printf("\tEquipo %d ingrediente  %d : %s \n",mydata->equipo_param,h,mydata->pasos_param[i].ingredientes[h]);
				}
			}
		}
	}
}


void* cortar(void *data) {
	char *accion = "cortar"; 
	struct parametro *mydata = data;
	imprimirAccion(mydata,accion);
	usleep( 20000 );
    sem_post(&mydata->semaforos_param.sem_mezclar);

    pthread_exit(NULL);
}

void* mezclar(void *data) {
        struct parametro *mydata = data;
        sem_wait(&mydata->semaforos_param.sem_mezclar);
        char *accion = "mezclar";
        imprimirAccion(mydata,accion);
        usleep( 20000 );
    sem_post(&mydata->semaforos_param.sem_salar);

    pthread_exit(NULL);
}

void* salar(void *data) {
        struct parametro *mydata = data;
        pthread_mutex_lock(&salar); 
        sem_wait(&mydata->semaforos_param.sem_salar);
        char *accion = "salar";
        imprimirAccion(mydata,accion);
        usleep( 20000 );
    sem_post(&mydata->semaforos_param.sem_cocinar);
        pthread_mutex_unlock(&salar);
    pthread_exit(NULL);
}

void* cortarLT(void *data) {
        char *accion = "cortarLT";
        struct parametro *mydata = data;
        imprimirAccion(mydata,accion);
        usleep( 20000 );
    sem_post(&mydata->semaforos_param.sem_armarHamburguesas);

    pthread_exit(NULL);
}

void* hornear(void *data) {
        pthread_mutex_lock(&hornear); 
        char *accion = "hornear";
        struct parametro *mydata = data;
        imprimirAccion(mydata,accion);
        usleep( 20000 );
    sem_post(&mydata->semaforos_param.sem_armarHamburguesas);
        pthread_mutex_unlock(&hornear); 
    pthread_exit(NULL);
}

void* armarMedallones(void *data) {
        struct parametro *mydata = data;
        sem_wait(&mydata->semaforos_param.sem_armarMedallones);
        char *accion = "armarMedallones";
        imprimirAccion(mydata,accion);
        usleep( 20000 );
    sem_post(&mydata->semaforos_param.sem_cocinar);

    pthread_exit(NULL);
}

void* cocinar(void *data) {
        struct parametro *mydata = data;
        pthread_mutex_lock(&cocinar); 
        sem_wait(&mydata->semaforos_param.sem_cocinar); 
        char *accion = "cocinar";
        imprimirAccion(mydata,accion);
        usleep( 20000 );
    sem_post(&mydata->semaforos_param.sem_armarHamburguesas);
        pthread_mutex_unlock(&cocinar);
    pthread_exit(NULL);
}

void* armarHamburguesas(void *data) {
        struct parametro *mydata = data;
        sem_wait(&mydata->semaforos_param.sem_armarHamburguesas);
        sem_wait(&mydata->semaforos_param.sem_armarHamburguesas);
        sem_wait(&mydata->semaforos_param.sem_armarHamburguesas);
        char *accion = "armarHamburguesas";
        imprimirAccion(mydata,accion);
        usleep( 20000 );

    pthread_exit(NULL);
}








void* ejecutarReceta(void *i) {

	//variables semaforos
	sem_t sem_mezclar;
        sem_t sem_salar;
        sem_t sem_armarMedallones;
        sem_t sem_cocinar;
        sem_t sem_armarHamburguesas;

	//variables hilos
	pthread_t p1;
        pthread_t p2;
        pthread_t p3;
        pthread_t p4;
        pthread_t p5;
        pthread_t p6;
        pthread_t p7;
        pthread_t p8;

	//numero del equipo (casteo el puntero a un int)
	int p = *((int *) i);

	printf("Ejecutando equipo %d \n", p);

	//reservo memoria para el struct
	struct parametro *pthread_data = malloc(sizeof(struct parametro));

	//seteo los valores al struct

	//seteo numero de grupo
	pthread_data->equipo_param = p;

	//seteo semaforos
	pthread_data->semaforos_param.sem_mezclar = sem_mezclar;
	//setear demas semaforos al struct aqui


	//seteo las acciones y los ingredientes (Faltan acciones e ingredientes) ¿Se ve hardcodeado no? ¿Les parece bien?
        FILE* fp= fopen("receta.txt","r"); 
        char buffer[100];
        int cont=0;
        int contIngredientes=0;
        int num=0;
        char delimitador[]="/";
        char *token;
        while(cont<8){
            fscanf(fp,"%s",buffer);
            token = strtok(buffer,delimitador);
            while(token!= NULL){
             if (num<1){
            strcpy(pthread_data->pasos_param[cont].accion,token);

            printf(token,"%s");
            num++;
            }
             else{
            printf(token,"%s");
            strcpy(pthread_data->pasos_param[cont].ingredientes[contIngredientes],token);
            contIngredientes++;
            }

            token = strtok(NULL,delimitador);
            }

           memset (buffer,0,100);
            cont++;
            num=0;
            contIngredientes=0;

        }


         fclose(fp);

       /* strcpy(pthread_data->pasos_param[0].accion, "cortar");
	strcpy(pthread_data->pasos_param[0].ingredientes[0], "ajo");
        strcpy(pthread_data->pasos_param[0].ingredientes[1], "perejil");
 	strcpy(pthread_data->pasos_param[0].ingredientes[2], "cebolla");

	strcpy(pthread_data->pasos_param[1].accion, "mezclar");
	strcpy(pthread_data->pasos_param[1].ingredientes[0], "ajo");
        strcpy(pthread_data->pasos_param[1].ingredientes[1], "perejil");
 	strcpy(pthread_data->pasos_param[1].ingredientes[2], "cebolla");
	strcpy(pthread_data->pasos_param[1].ingredientes[3], "carne picada");
*/

	//inicializo los semaforos

	sem_init(&(pthread_data->semaforos_param.sem_mezclar),0,0);
        sem_init(&(pthread_data->semaforos_param.sem_salar),0,0);
        sem_init(&(pthread_data->semaforos_param.sem_armarMedallones),0,0);
        sem_init(&(pthread_data->semaforos_param.sem_cocinar),0,0);
        sem_init(&(pthread_data->semaforos_param.sem_armarHamburguesas),0,0);



        //creo los hilos a todos les paso el struct creado (el mismo a todos los hilos) ya que todos comparten los semaforos 
    int rc;
    rc = pthread_create(&p1,NULL,cortar,pthread_data);                     //parametros de la funcion a ejecutar, pasado por referencia
    rc = pthread_create(&p2,NULL,mezclar,pthread_data);
    rc = pthread_create(&p3,NULL,salar,pthread_data);
    rc = pthread_create(&p4,NULL,cortarLT,pthread_data);
    rc = pthread_create(&p5,NULL,hornear,pthread_data);
    rc = pthread_create(&p6,NULL,armarMedallones,pthread_data);
    rc = pthread_create(&p7,NULL,cocinar,pthread_data);
    rc = pthread_create(&p8,NULL,armarHamburguesas,pthread_data);

	//join de todos los hilos
	pthread_join (p1,NULL);
        pthread_join (p2,NULL);
        pthread_join (p3,NULL);
        pthread_join (p4,NULL);
        pthread_join (p5,NULL);
        pthread_join (p6,NULL);
        pthread_join (p7,NULL);
        pthread_join (p8,NULL);

	//valido que el hilo se alla creado bien 
    if (rc){
       printf("Error:unable to create thread, %d \n", rc);
       exit(-1);
     }


	//destruccion de los semaforos 
	sem_destroy(&sem_mezclar);
        sem_destroy(&sem_salar);
        sem_destroy(&sem_armarMedallones);
        sem_destroy(&sem_cocinar);
        sem_destroy(&sem_armarHamburguesas);

	//salida del hilo
	 pthread_exit(NULL);
}


int main ()
{
	//creo los nombres de los equipos 
	int rc;
	int *equipoNombre1 =malloc(sizeof(*equipoNombre1));
	int *equipoNombre2 =malloc(sizeof(*equipoNombre2));
	int *equipoNombre3 =malloc(sizeof(*equipoNombre3));
	*equipoNombre1 = 1;
	*equipoNombre2 = 2;
	*equipoNombre3 = 3;

	//creo las variables los hilos de los equipos
	pthread_t equipo1; 
	pthread_t equipo2;
	pthread_t equipo3;

	//inicializo los hilos de los equipos
    rc = pthread_create(&equipo1,NULL,ejecutarReceta,equipoNombre1); 
    rc = pthread_create(&equipo2,NULL,ejecutarReceta,equipoNombre2);
    rc = pthread_create(&equipo3,NULL,ejecutarReceta,equipoNombre3);

   if (rc){
       printf("Error:unable to create thread, %d \n", rc);
       exit(-1);
     } 

	//join de todos los hilos
	pthread_join (equipo1,NULL);
	pthread_join (equipo2,NULL);
	pthread_join (equipo3,NULL);


    pthread_exit(NULL);
}


//Para compilar:   gcc HellsBurgers.c -o ejecutable -lpthread
//Para ejecutar:   ./ejecutable
