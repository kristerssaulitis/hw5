all:
	gcc hw5_2.c
run:
	 ./a.out -c=chunks -s=sizes
clean:
	$(RM) hw5 a.out