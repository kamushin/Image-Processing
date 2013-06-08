main: main.o ap_bmp.o filter.o
	g++ main.o ap_bmp.o filter.o -o main
	@echo "******************Make all*******************"
main.o:main.c
	g++ -c main.c
ap_bmp.o:ap_bmp.c ap_bmp.h
	g++ -c ap_bmp.c ap_bmp.h
filter.o:filter.c
	g++ -c filter.c 
.PHONY :clean
clean:
	rm main.o ap_bmp.o filter.o

