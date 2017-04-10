CC = clang
INCLUDE = .
CFLAGS = -Wall -std=c11

hw2: hw2.o linkedlist.o termctrl.o stringutil.o
	$(CC) -o hw2 hw2.o linkedlist.o termctrl.o stringutil.o
hw2.o: hw2.c termctrl.h stringutil.h linkedlist.h
	$(CC) -I$(INCLUDE) $(CFLAGS) -c hw2.c
termctrl.o: termctrl.c termctrl.h
	$(CC) -I$(INCLUDE) $(CFLAGS) -c termctrl.c
stringutil.o: stringutil.c stringutil.h
	$(CC) -I$(INCLUDE) $(CFLAGS) -c stringutil.c
linkedlist.o: linkedlist.c linkedlist.h
	$(CC) -I$(INCLUDE) $(CFLAGS) -c linkedlist.c
clean:
	rm -f hw2.o linkedlist.o termctrl.o stringutil.o hw2
