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
    clock_t start_t, end_t;
    while(1) {

		if(check){
			int i;
			void *shared_memory; //memoria de cuantas veces se ha precionado
			void *shared_memory2; //memoria de fecha de la ultima vez que se presiono
			int presionado;
			int shmid;
			int shmid2;

			int bool1=0;

			time_t now;
			struct tm *ts;
			char buf[80];

			/* Obtener el tiempo de ahora */
			end_t=clock();
			now = time(NULL);
			/* Formato y escritura del tiempo, "hh:mm:ss" */
			ts = localtime(&now);
			strftime(buf, sizeof(buf), "%H:%M:%S", ts);
			shmid=shmget((key_t)2345, 1024, 0666);
			shmid2=shmget((key_t)2346, 1024, 0666);
			printf("Key of shared memory is %d\n",shmid);
			if(shmid==-1){
				shmid=shmget((key_t)2345, 1024, 0666|IPC_CREAT);
				bool1=1;
			}
			if(shmid2==-1){
				shmid=shmget((key_t)2346, 1024, 0666|IPC_CREAT);
			}

			if(bool1){
				shared_memory=shmat(shmid,NULL,0); //proceso adjunto al segmento de memoria compartida
				presionado = 1;
				char a[2] ;
				*a= presionado+'0';
				strcpy(shared_memory,a);
				printf("You wrote : %s\n",(char *)shared_memory);
				//shared_memory2=shmat(shmid2,NULL,0); //proceso adjunto al segmento de memoria compartida
				//strcpy(shared_memory2,buf);
				//printf("You wrote : %s\n",(char *)shared_memory2);
			}else{
				shared_memory=shmat(shmid,NULL,0);
				shared_memory2=shmat(shmid2,NULL,0);
				printf("Found: %s\n",(char *)shared_memory);
				printf("Found: %s\n",(char *)shared_memory2);
				struct tm tm;
				//strptime(shared_memory2, "%H:%M:%S", &tm);
				//time_t t = mktime(&tm);
				
				//struct tm tm2;
				//strptime(buf, "%H:%M:%S", &tm2);
				//int sec = tm2.tm_sec - tm.tm_sec;
				double sec = ((double) (end_t - start_t)) / CLOCKS_PER_SEC;
				printf("Execution time = %f\n", sec);
				
				if(sec >= 3 || sec < 0){
					presionado = 1;
				} else if (sec >= 0.5){
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
				//shared_memory2=shmat(shmid2,NULL,0); //process attached to shared memory segment
				//strcpy(shared_memory2,buf);
				//printf("You wrote : %s\n",(char *)shared_memory2);

			}
			end:
			printf("end");	
			check=!check;
			start_t = end_t;
		}
    }
    printf("Closing Driver\n");
    close(fd);
}
