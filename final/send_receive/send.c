/***************************************************************************//**
*  \file       test_app.c
*
*  \details    Userspace application to test the Device driver
*
*  \author     EmbeTronicX
*
* *******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <signal.h>
#include<sys/shm.h>
#include <time.h>

#define REG_CURRENT_TASK _IOW('a','a',int32_t*)
 
#define SIGETX 44
 
static int done = 0;
int check = 0;
 
void ctrl_c_handler(int n, siginfo_t *info, void *unused)
{
    if (n == SIGINT) {
        printf("\nrecieved ctrl-c\n");
        done = 1;
    }
}
 
void sig_event_handler(int n, siginfo_t *info, void *unused)
{
    if (n == SIGETX) {
        check = info->si_int;
        printf ("Recivir la señal del kernel: Valor = %u\n", check);
    }
}
 
int main(){
    int fd;
    int32_t value, number;
    struct sigaction act;
 
    printf("****************SENDER****************\n");
 
    sigemptyset (&act.sa_mask);
    act.sa_flags = (SA_SIGINFO | SA_RESETHAND);
    act.sa_sigaction = ctrl_c_handler;
    sigaction (SIGINT, &act, NULL);
 
    /* install custom signal handler */
    sigemptyset(&act.sa_mask);
    act.sa_flags = (SA_SIGINFO | SA_RESTART);
    act.sa_sigaction = sig_event_handler;
    sigaction(SIGETX, &act, NULL);
 
    printf("Instalar manejador de señales por SIGETX = %d\n", SIGETX);
 
    printf("\nAbriendo driver\n");
    fd = open("/dev/etx_device", O_RDWR);
    if(fd < 0) {
            printf("No se puede abrir archivo de dispositivo...\n");
            return 0;
    }
 
    printf("Registrando aplicación ...");
    /* register this task with kernel for signal */
    if (ioctl(fd, REG_CURRENT_TASK,(int32_t*) &number)) {
        printf("Fallido\n");
        close(fd);
        exit(1);
    }
    printf("Hecho!!!\n");
    
    //Codigo de memoria compartida
    void *shared_memory; //memoria de cuantas veces se ha precionado
    void *shared_memory2; //memoria de fecha de la ultima vez que se presiono
    clock_t start_t, end_t, int_t;
    int start = 0;
    int shmid;
    while(1) {
		if(check){
			int i;
			int presionado; //numero de veces que se ha presionado
			int bool1=0;
			/* Obtener el tiempo de ahora */
			end_t=clock();
            //Obtener la memoria compartida
			shmid=shmget((key_t)2222, 1024, 0666);
			printf("Key of shared memory is %d\n",shmid);
			if(shmid==-1){
				shmid=shmget((key_t)2222, 1024, 0666|IPC_CREAT);
				bool1=1;
			}
			if(bool1){ //primera vez que se presiona
				shared_memory=shmat(shmid,NULL,0); //proceso adjunto al segmento de memoria compartida
				presionado = 1;
				char a[2] ;
				*a= presionado+'0';
				strcpy(shared_memory,a);
				printf("You wrote : %s\n",(char *)shared_memory);
			}else{ // cuando ya se haya presionado una vez.
				shared_memory=shmat(shmid,NULL,0);
				printf("Found: %s\n",(char *)shared_memory);
                // Visualizar tiempo transcurrido
				double sec = ((double) (end_t - start_t)) / CLOCKS_PER_SEC;
				printf("Execution time = %f\n", sec);
				
				if(sec >= 3 || sec < 0){
					presionado = 1;
				} else if (sec >= 0.1){
					if(strcmp(shared_memory, "1") == 0){
						presionado = 2;
					} else if (strcmp(shared_memory, "2") == 0){
						presionado = 3;
					} else {
						presionado = 1;
					}	
				} else {
					goto end;
				}
				char a[2] ;
				*a= presionado+'0';
				strcpy(shared_memory,a);
				printf("You wrote : %s\n",(char *)shared_memory);

			}
			end:
			printf("end");	
			check=!check;
			start_t = end_t;
            start = 1;
		}
        if(start == 1){
            int_t = clock();
            double sec = ((double) (int_t - start_t)) / CLOCKS_PER_SEC;
            if(sec>=3){
                char a[2];
                int press = 0;
                *a=press+'0';
				shared_memory=shmat(shmid,NULL,0);
				strcpy(shared_memory,a);
				printf("You wrote : %s\n",(char *)shared_memory);
            }
        }
    }
    printf("Closing Driver\n");
    close(fd);
}
