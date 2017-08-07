/* El codigo esta incompleto, queda por añadir los agentes y corregir un pequeño fallo al comprobar que hilos estan libres*/

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <stdbool.h>
#include <sys/sem.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>


int i=0, count;
char fecha[128];
char msg[128];
struct usuarios{int id, estado, hilo;};
struct usuarios usuario[512];
struct {bool available; pthread_t thread;} tid[12];


static void NuevoUsuario(int);
void tiempo();
static void AccionesUsuarios(struct usuarios *usuario);
int ComprobarUsuario();
void imprimir(char msg[]);
int aleatorios();

struct {bool cafe; pthread_t thread;} ta[2];
static void agentes();
int PuedoTomarCafe();

int main(void) {
	FILE *fp;
	fp = fopen("log.txt","w");
	fclose(fp);

	char msg[]=" ----- Abre la oficina del INEM\n";
	imprimir(msg);

	for ( i=0 ; i<12 ; i++ ) {
		tid[i].available = true;
	}

	for ( i=0 ; i<3 ; i++ ) {
	pthread_create(&ta[i].thread, NULL, (void*)agentes, NULL);
	ta[i].cafe = true;
	}

	i=0;
	count=0;
	  if ( signal(SIGUSR1, NuevoUsuario) == SIG_ERR) {
	    perror("Error al recibir la señal");
	    return -1;
	  }
	for ( ; ; )
		pause();
	return 0;
}
int ComprobarUsuario(){
	int a=0;
	bucle:
		if(tid[a].available == true){
			return a;
		}
		a++;
	goto bucle;
	return a;
}
static void NuevoUsuario(int signo){
	if (signo == SIGUSR1){
		signal(SIGUSR1, NuevoUsuario);

		}
	count=ComprobarUsuario();
	if (count<12){
		i++;
		tid[count].available = false;
		usuario[i] = (struct usuarios){i,0,count};
		pthread_create(&tid[count].thread, NULL, (void*)AccionesUsuarios, &usuario[i]);
	}
	else{
		sprintf( msg," ----- El usuario %d se va por falta de sitio\n", usuario->id);
		imprimir(msg);
	}
}
static void AccionesUsuarios(struct usuarios *usuario){

	sprintf( msg," ----- El usuario %d entra en la oficina y ocupa el sitio %d\n", usuario->id, usuario->hilo);
	imprimir(msg);
	bucle:
	sleep(5);

	if (usuario[i].estado==1){
		sprintf( msg," ----- El usuario %d esta siendo atendido\n", usuario->id);
		imprimir(msg);
		//Atenderlo
		sleep(1);
		return;
	}
	else{

		if(aleatorios()<2){
			sprintf( msg," ----- El usuario %d se cansa de esperar y se va, hilo %d queda libre\n", usuario->id, usuario->hilo);
			imprimir(msg);
			tid[count].available = true;
			pthread_join(tid[count].thread, NULL);
			return;
		}

		if (aleatorios()<1){
			sprintf( msg," ----- El usuario %d se enfada y se dirige a colocar una reclamacion\n", usuario->id);
			imprimir(msg);
			//Esperar ser atendido
			return;
		}
	}
	goto bucle;
}
void imprimir(char msg[]){
	tiempo();
	FILE *fp;
	fp = fopen("log.txt","a");
	fputs(fecha, fp);
	fputs(msg, fp);
	//printf("%s %s",fecha, msg);
	fclose(fp);
}
void tiempo(){
	time_t tiempo = time(0);
	struct tm *tlocal = localtime(&tiempo);
	strftime(fecha,128,"%Y/%m/%d %H:%M:%S",tlocal);
}
int aleatorios(){
	srand48 (time(NULL));
	return mrand48() % 11;
}
static void agentes(){
	PuedoTomarCafe();
}
int PuedoTomarCafe(){
	int a=0,b=0;
		bucle:
			if(ta[a].cafe == true){
				b++;
				if(b<2){
					sprintf( msg," ----- El agente %d puede tomar cafe\n", a++);
					imprimir(msg);
					return 0;
				}
			}
			a++;
		goto bucle;
		return -1;
}
