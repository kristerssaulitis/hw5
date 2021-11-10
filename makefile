all:
	gcc hw5.c
run:
	./a.out -c=chunks -s=sizes
clean:
	$(RM) hw5 a.out