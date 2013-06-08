objects=test.o hs.o imageio.o util.o filter.o deriv.o sor.o fmg.o writeflow.o
horn:$(objects)
	gcc -o A1 $(objects) -lm
test.o:hs.h imageio.h util.h 
hs.o: hs.h imageio.h util.h filter.h deriv.h sor.h writeflow.h 
imageio.o: imageio.h
util.o: util.h imageio.h
filter.o:filter.h util.h
deriv.o: deriv.h util.h filter.h
sor.o: sor.h util.h 
fmg.o: fmg.h filter.h util.h
writeflow.o: writeflow.h util.h
.PHONY:clean
clean:
	rm -rf A1 $(objects)
