OPTIMIZE=-O2
CC = gcc
CFLAGS = $(OPTIMIZE) -g3 -std=gnu11 -lrt  -pthread 
DEPS = sltest.h SortedList.h parser.h
OBJ = main.o sltest.o SortedList.o parser.o
SOURCES = Makefile sltest.c sltest.h main.c SortedList.h SortedList.c parser.h parser.c

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $< 

sltest: $(OBJ)
	gcc $(CFLAGS) -o $@ $^


check:
	

dist: lab4-Part2-GarimaLunawatBreannaNery.tar.gz
lab4.2-GarimaLunawatBreannaNery.tar.gz: $(SOURCES)
	mkdir lab4-Part2-GarimaLunawatBreannaNery
	cp $(SOURCES) lab-Part2-GarimaLunawatBreannaNery
	tar -czf $@ lab4-Part2-GarimaLunawatBreannaNery
	rm -r lab4-Part2-GarimaLunawatBreannaNery

clean:
	rm -f *.o *.so *.so.* sltest lab4-Part2-GarimaLunawatBreannaNery.tar.gz lab4-Part2-GarimaLunawatBreannaNery


