CC		= gcc
LIBS	= -li2c -lpthread
WARN	= -Wall
#################################################

all: main

led_matrix:	led_matrix.o
	$(CC) $(WARN) $? -o $@ $(LIBS)

sock: sock.o
	$(CC) $(WARN) $? -o $@ $(LIBS)

main: main.o sock.o led_matrix.o gaz.o finalDecision.o
	$(CC) $(WARN) $? -o $@ $(LIBS)

%.o:	%.c 
	$(CC) -c $(WARN) $? -o $@

clean:
	rm -f *.o sock led_matrix main
