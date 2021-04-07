CC = gcc
CFLAGS = -std=gnu99 -Wpedantic -g

all: FSreport

FSreport: FSreport.o
	$(CC) $(CFLAGS) FSreport.o -o FSreport

FSreport.o: FSreport.c FSreport.h
	$(CC) $(CFLAGS) -c FSreport.c

clean:
	rm -rf *.o FSreport vgcore*
