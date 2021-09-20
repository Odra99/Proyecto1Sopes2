/****************************************************************************/
/* term.c   Dr. Juan Gonzalez Gomez. January 2009                           */
/*--------------------------------------------------------------------------*/
/* Example of Serial communications in Linux.                               */
/* Mini serial communication terminal                                       */
/*--------------------------------------------------------------------------*/
/* GPL LICENSE                                                              */
/*--------------------------------------------------------------------------*/
/* This example is a mini serial communication terminal. All the            */
/* characters typed by the user are sent throw the serial port and all the  */
/* characters received by the serial port are printed on the screen.        */
/*                                                                          */
/* It can be tested by attaching a echo device to the serial port. For      */
/* example a microcontroller running a echo-firmware or a wire joining the  */
/* PC tx and rx pins.                                                       */
/*--------------------------------------------------------------------------*/
/* Example of use:                                                          */
/*                                                                          */
/*   ./term /dev/ttyUSB0                                                    */
/*                                                                          */
/*  The serial device name should be passed as a parameter                  */
/*  When executing this example                                             */
/*--------------------------------------------------------------------------*/
/*  In linux, the serial devices names are:                                 */
/*                                                                          */
/*    /dev/ttyS0  --> First native serial port                              */
/*    /dev/ttyS1  --> Second native serial port                             */
/*    ...                                                                   */
/*    /dev/ttyUSB0  --> First USB-RS232 converter                           */
/*    /dev/ttyUSB1  --> Second USB-RS232 converter                          */
/*    ...                                                                   */
/****************************************************************************/

#include <termios.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "serial.h"
#include "console_io.h"


//------------------
//-- CONSTANTS
//------------------

//--TIMEOUT in micro-sec 
#define TIMEOUT 10000

//--------------------
//-- Global variables
//--------------------
//-- Serial port descriptor
int serial_fd;


/**************************************************************************/
/* Main loop. When the user press a key, it is sent to the serial port.   */
/* When a character is received from the serial port, it is printed on    */
/* the screen                                                             */
/**************************************************************************/
void term(void)
{
  char c[1];
  char rec[1];
  int n;
  
  //-- Main loop
  do {
    //-- if there is a key pressed by the user
    if (console_io_kbhit()) {
      
      //-- Read the character
      c[0]=console_io_getch();
      
      //-- Send the character to the serial port, if different from ESC
      if (c[0]!=ESC) 
        serial_send(serial_fd,c,1);
    }
    
    //-- Poll the serial port. A character has been received?
    n=serial_read(serial_fd,rec,1,TIMEOUT);
    
    //-- If so, print it on the screen!!
    if (n==1) {
      printf ("%c",rec[0]);
      fflush(stdout);
    }
    
    //-- Repeat until the user press the ESC key
  } while (c[0]!=ESC);   
}

/**********************/
/*  MAIN PROGRAM      */
/**********************/
int main (int argc, char* argv[])
{
  
  //-- Check if the serial device name is given
  if (argc<2) {
    printf ("No serial device name is given\n");
    exit(0);
  }

  //-- Open the serial port
  serial_fd=serial_open(argv[1],B9600);
  
  //-- Error checking
  if (serial_fd==-1) {
    printf ("Error opening the serial device: %s\n",argv[1]);
    perror("OPEN");
    exit(0);
  }
 
  //-- Configure the console for using the keyboard
  console_io_open();
  
  //-- Execute the main loop
  printf ("Press the ESC key to quit\n");
  term();
  
  //-- Restore the previous console configuration
  console_io_close();
  
  //-- Close the serial port
  serial_close(serial_fd);

  return 0;
}
