main: main.o algo.o bitmap.o
	g++ main.o algo.o bitmap.o -o main
	@echo "******************Make all*******************"
main.o:main.c
	g++ -c main.c
algo.o:algo.c
	g++ -c algo.c
bitmap.o:bitmap.c
	g++ -c bitmap.c 
.PHONY :clean
clean:
	rm *.o

