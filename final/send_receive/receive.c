 #include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/shm.h>
#include<string.h>

int main()
{
	int i, b;
	void *shared_memory;
	int shmid;
	i=-1;
	while(1){
		//Memoria compartida con el id 2222
		shmid=shmget((key_t)2222, 1024, 0666);
		if(shmid==-1){
			if(i==-1){
				printf("El numero ha sido presionado 0 veces\n");
				i=0;
			}
		}else{
			shared_memory=shmat(shmid,NULL,0); //proceso adjunto al segmento de memoria compartida
			// compartir la información
			b = atoi((char *) shared_memory);
			if(i != b){
				if(b==1){
					printf("El boton ha sido presionado %d vez\n",b);
				} else {
					printf("El boton ha sido presionado %d veces\n", b);
				}
				i=b;
			}
		}
  		usleep(100000);	
	}
}
