build: 
	gcc -c t.h ex1open.c t.c 
	gcc -o ex1open ex1open.o t.o 
run: build
	./ex1open test.txt
clean:
	rm *.o
.PHONY: clean
