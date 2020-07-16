build: 
	gcc -c funcs.h shell.c funcs.c 
	gcc -o shell shell.o funcs.o
run: build
	./shell
clean:
	rm *.o
.PHONY: clean
