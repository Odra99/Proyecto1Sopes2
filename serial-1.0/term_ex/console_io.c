/**************************************************************************/
/*  console_io.c   (C) 1996-2009. Juan Gonzalez.                          */
/* ---------------------------------------------------------------------- */
/* Functions for managing the console (keyboard)                          */
/*------------------------------------------------------------------------*/
/* This module satisfies the POSIX.1 standard                             */
/*                                                                        */
/* This module is portable to Windows, using the Cygwin library           */
/*------------------------------------------------------------------------*/
/* GPL LICENSE                                                            */
/**************************************************************************/

#include <termios.h>
#include <unistd.h>
#include <sys/select.h>
#include <stdio.h>

#include "console_io.h"

//-- Timeout for the select system call, in micro-seconds
#define TIMEOUT_USEC 10000  

/*--------------------*/
/* GLOBAL VARIABLES   */
/*--------------------*/
static struct termios oldconsole,console;


/*--------------*/
/* FUNCTIONS    */
/*--------------*/

/*************************************************************/
/* Configure the standard input for non blocking readings    */
/* The previous console state is save                        */
/*                                                           */
/*                                                           */
/* RETURNS:                                                  */
/*  0 OK, the console is correctly configured                */
/*  -1 An error occurred                                     */
/*************************************************************/
int console_io_open(void)
{

  //-- Read the console status
  if (tcgetattr(STDIN_FILENO,&console)==-1) {
    perror("tcgetattr Error");
    return -1;
  }
  
  //-- Save the current console status
  oldconsole = console;
  
  //-- Configure the console
  console.c_cc[VMIN]=1;
  console.c_lflag&=~(ECHO|ICANON); 
  
  //-- Write the new console status
  if (tcsetattr(STDIN_FILENO,TCSANOW,&console)==-1) {
    perror("tcsetattr Error");
    return -1;
  }
  
  return 0;
}

void console_io_close(void)
/**********************************************/
/* Restore the previous console status        */
/**********************************************/
{
  if (tcsetattr(STDIN_FILENO,TCSANOW,&oldconsole)==-1) 
    perror("tcsetattr Error");
}

/************************************************************************/
/*  Check if there are pending characters to be read in the buffer      */
/*                                                                      */
/*  RETURNS                                                             */
/*    0: No pending characters                                          */
/*    1: The are characters in the buffer ready to be read              */
/************************************************************************/
int console_io_kbhit(void)
{
  int n;
  fd_set rfd;
  struct timeval timeout;
  
  timeout.tv_sec  = 0;
  timeout.tv_usec = TIMEOUT_USEC;
  
  FD_ZERO(&rfd);
  FD_SET(STDIN_FILENO,&rfd);
  
  n = select(STDIN_FILENO+1,&rfd,NULL,NULL,&timeout);
  
  return n;
}

/******************************************************************/
/* Read one character from the keyboard. This functions waits     */
/* until a key is pressed                                         */
/******************************************************************/
char console_io_getch(void)
{
  int n;
  char c;
    
  while (!console_io_kbhit());
  
  n = read(STDIN_FILENO,&c,1);
  
  return c;
}
