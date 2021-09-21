/******************************************************************************/
/* send_receive.c   Mario Moisés Ramírez Tobar                                */
/*----------------------------------------------------------------------------*/
/* Ejemplo de comunicación con un serial en linux                             */
/* reciviendo data                                                            */
/*----------------------------------------------------------------------------*/
/* GPL LICENSE                                                                */
/*----------------------------------------------------------------------------*/
/* Espera en un loop, la comunicación del pulsador, que espera cada milesima  */
/* y si hay una pulsación proboca una interrupción en el irq                  */
/*                                                                            */
/*----------------------------------------------------------------------------*/
/* Ejemplo de uso:                                                            */
/*                                                                            */
/*   ./send_receive /dev/ttyUSB0                                              */
/*                                                                            */
/*  output will be the following:                                             */
/*                                                                            */
/*    (1bit)                                                                  */
/*                                                                            */
/*  If no data is received, the output will be:                               */
/*                                                                            */
/*                                                                            */
/*----------------------------------------------------------------------------*/
/*  En linux, el nombre de los dispositivos seriales                          */
/*                                                                            */
/*    /dev/ttyS0  --> First native serial port                                */
/*    /dev/ttyS1  --> Second native serial port                               */
/*    ...                                                                     */
/*    /dev/ttyUSB0  --> Primer Convertidor USB-RS232                          */
/*    ...                                                                     */
/******************************************************************************/

#include <termios.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "serial.h"

//------------------
//-- CONSTANTS
//------------------

#define CMD       "a"

//-- tamaño de la información obtenida
#define CMD_LEN   1

//--TIMEOUT in micro-segundos (Esta puesto en 1/10 segundos en este ejemplo)
#define TIMEOUT 100000


/**********************/
/*  MAIN PROGRAM      */
/**********************/
int main (int argc, char* argv[])
{
  int serial_fd;           //-- Descripción del puerto serial
  char data[CMD_LEN+1];    //-- La data recivida
  
  //-- Verificar si el nombre del dispositivo esta disponible
  if (argc<2) {
    printf ("No serial device name is given\n");
    exit(0);
  }

  //-- Abrir el puerto serial
  //-- La rapidez esta configurada en 9600 baudios
  serial_fd=serial_open(argv[1],B9600);
  
  //-- Chequeo de errores
  if (serial_fd==-1) {
    printf ("Error opening the serial device: %s\n",argv[1]);
    perror("OPEN");
    exit(0);
  }
 while(1==1){
  
  //-- Esperar por la data a recibir
  int n;
  n=serial_read(serial_fd,data,CMD_LEN,TIMEOUT);
  
  if (n>0) {
  //-- Mostrar la data recibida
    printf ("(%d bytes)\n",n);
    usleep(TIMEOUT);
  }
 }
  
  //-- Cerrar el puerto serial
  serial_close(serial_fd);

  return 0;
}
