DEBUG = -g
CC = gcc -Wall $(DEBUG)

OFILES1 = gnbc.o

SOURCE = *.c *.h

a.out: $(OFILES1)
	$(CC) $(OFILES1) $(ALLOC) 

tar: 
	@tar cf gnbc.tar $(SOURCE)
clean:
	@rm *.o


