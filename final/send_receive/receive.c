 #include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/shm.h>
#include<string.h>

int main()
{
int i, b;
void *shared_memory;
char buff[100];
int shmid;
	while(1){
		//Memoria compartida con el id 2345
		shmid=shmget((key_t)2345, 1024, 0666);
		if(shmid==-1){
			printf("0\n");
			i=0;
		}else{
			shared_memory=shmat(shmid,NULL,0); //proceso adjunto al segmento de memoria compartida
			// compartir la información
			b = atoi((char *) shared_memory);
			if(i != b){
				printf("El boton ha sido presionado %s vez\n",(char *)shared_memory);
			}
		}
  		usleep(400000);	
	}
}
