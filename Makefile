wish: wish.o util.o 
	gcc -o wish wish.o util.o 

util.o: util.c
	gcc -c util.c

wish.o: wish.c
	gcc -c wish.c


