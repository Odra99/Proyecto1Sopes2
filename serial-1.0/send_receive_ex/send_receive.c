/******************************************************************************/
/* send_receive.c   Dr. Juan Gonzalez Gomez. January 2009                     */
/*----------------------------------------------------------------------------*/
/* Example of Serial communications in Linux.                                 */
/* Sending and receiving data strings                                         */
/*----------------------------------------------------------------------------*/
/* GPL LICENSE                                                                */
/*----------------------------------------------------------------------------*/
/* This example sends a ASCII string to the serial port. It waits for the     */
/* same string to be echoed by another device (For example a microcontroller  */
/* running an echo-firmware or a wire joining the PC tx and rx pins           */
/* The received string is print on the screen. If no data is received         */
/* during the TIMEOUT time, a timeout message is printed                      */
/*                                                                            */
/* The serial port speed is configured to 9600 baud                           */
/*----------------------------------------------------------------------------*/
/* Example of use:                                                            */
/*                                                                            */
/*   ./send_receive /dev/ttyUSB0                                              */
/*                                                                            */
/*  The serial device name should be passed as a parameter                    */
/*  When executing this example, if the echoed data is received the           */
/*  output will be the following:                                             */
/*                                                                            */
/*    String sent------> ASCII Command test                                   */
/*    String received--> ASCII Command test (18 bytes)                        */
/*                                                                            */
/*  If no data is received, the output will be:                               */
/*    String sent------> ASCII Command test                                   */
/*    String received--> Timeout!                                             */
/*                                                                            */
/*----------------------------------------------------------------------------*/
/*  In linux, the serial devices names are:                                   */
/*                                                                            */
/*    /dev/ttyS0  --> First native serial port                                */
/*    /dev/ttyS1  --> Second native serial port                               */
/*    ...                                                                     */
/*    /dev/ttyUSB0  --> First USB-RS232 converter                             */
/*    /dev/ttyUSB1  --> Second USB-RS232 converter                            */
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

//-- ASCII string to send through the serial port
#define CMD       "a"

//-- ASCII String length
#define CMD_LEN   1

//--TIMEOUT in micro-sec (It is set to 1 sec in this example)
#define TIMEOUT 100000


/**********************/
/*  MAIN PROGRAM      */
/**********************/
int main (int argc, char* argv[])
{
  int serial_fd;           //-- Serial port descriptor
  char data[CMD_LEN+1];    //-- The received command
  
  //-- Check if the serial device name is given
  if (argc<2) {
    printf ("No serial device name is given\n");
    exit(0);
  }

  //-- Open the serial port
  //-- The speed is configure at 9600 baud
  serial_fd=serial_open(argv[1],B9600);
  
  //-- Error checking
  if (serial_fd==-1) {
    printf ("Error opening the serial device: %s\n",argv[1]);
    perror("OPEN");
    exit(0);
  }
 while(1==1){
  //-- Send the command to the serial port
  /*serial_send(serial_fd, CMD, CMD_LEN);
  printf ("String sent------> %s\n",CMD);*/
  
  //-- Wait for the received data
  int n;
  n=serial_read(serial_fd,data,CMD_LEN,TIMEOUT);
  
  //-- Show the received data
  //printf ("String received--> \n");
  //fflush(stdout);
  if (n>0) {
    printf ("(%d bytes)\n",n);
    usleep(TIMEOUT);
  }
  else {
    //printf ("Timeout!\n");
  }
 }
  
  //-- Close the serial port
  serial_close(serial_fd);

  return 0;
}
