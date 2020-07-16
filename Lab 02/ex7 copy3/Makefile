build: 
	gcc -c t.h copy3.c t.c 
	gcc -o copy3 copy3.o t.o
run: build
	./copy3 test.txt test2.txt
clean:
	rm *.o
.PHONY: clean
