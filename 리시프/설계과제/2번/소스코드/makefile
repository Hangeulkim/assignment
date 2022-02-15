ssu_mntr: main.o ssu_mntr.o ssu_check.o
	gcc -o ssu_mntr main.o ssu_mntr.o ssu_check.o

main.o: main.c ssu_mntr.h
	gcc -c main.c

ssu_mntr.o: ssu_mntr.c ssu_mntr.h
	gcc -c ssu_mntr.c

ssu_check.o: ssu_check.c ssu_check.h ssu_mntr.h
	gcc -c ssu_check.c

clean :
	rm *.o
	rm ssu_mntr
