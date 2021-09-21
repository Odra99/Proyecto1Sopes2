/*****************************************************************************/
/* console_io.h                                                              */
/*---------------------------------------------------------------------------*/
/*  Functions for managing the console (keyboard)                            */
/*---------------------------------------------------------------------------*/
/* GPL LICENSE                                                               */
/*****************************************************************************/

#ifndef _CONSOLA_IO_H
#define _CONSOLA_IO_H

#define ESC 27

/*-----------------*/
/*   PROTOTYPES    */
/*-----------------*/

int  console_io_open(void);
void console_io_close(void);
int  console_io_kbhit(void);
char console_io_getch(void);

#endif 
