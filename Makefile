CC := gcc
CCOPTS :=  -Wall -Wextra -Wconversion -O


Factory : Factory.o assemblers.o packers.o
	$(CC) -pthread -o Factory Factory.o assemblers.o packers.o
# how to compile .c files
Factory.o: factory.c  assemblers.h packers.h factory.h
	$(CC) $(CCOPTS) -pthread -c -o Factory.o factory.c
assemblers.o: assemblers.c  assemblers.h factory.h
	$(CC) $(CCOPTS) -pthread -c -o assemblers.o assemblers.c
packers.o: packers.c  packers.h factory.h
	$(CC) $(CCOPTS) -pthread -c -o packers.o packers.c

clean:
	rm -rf *.o Factory
