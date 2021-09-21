#------------------------------------------------------------------------------
# Mario Moisés Ramírez Tobar                         
#------------------------------------------------------------------------------
# GPL LICENSE                                                               
#------------------------------------------------------------------------------

CC = gcc
CFLAGS =  -Iserial

#-- Librería serial
SERIAL = serial.o

#-- Ejemplo: send_receive
NAME1 = send_receive
DEP1 = send_receive_ex/$(NAME1).o 

#-- Ejemplo: term
NAME2 = term
DEP2 = term_ex/$(NAME2).o term_ex/console_io.o


all: $(SERIAL) $(NAME1) $(NAME2)


#------------------------------
#-- serial.o
#------------------------------
$(SERIAL): serial/serial.c serial/serial.h
	    $(CC) -o $(SERIAL) -c serial/serial.c

clean::
	  rm -f $(SERIAL) *.bak *~

#---------------------------
#-- Ejemplo: send_receive
#---------------------------
$(NAME1): $(DEP1) $(SERIAL)
	$(CC) -o $(NAME1) $(DEP1) $(SERIAL)
  
clean::
	  rm -f $(NAME1) $(DEP1) *.bak *~

#-------------------------------
#-- Ejemplo: term
#-------------------------------
$(NAME2): $(DEP2) $(SERIAL)
	$(CC) -o $(NAME2) $(DEP2) $(SERIAL)
  
clean::
	  rm -f $(NAME2) $(DEP2) *.bak *~    
