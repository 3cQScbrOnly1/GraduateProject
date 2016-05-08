ngram_restore:main.o lm.o extends.o analyser.o
	g++ -o ngram_restore main.o lm.o extends.o analyser.o
main.o:main.cpp
	g++ -c main.cpp
lm.o:lm.cpp lm.h
	g++ -c lm.cpp
extends.o:extends.cpp extends.h
	g++ -c extends.cpp
analyser.o:lm.o extends.o analyser.h analyser.cpp
	g++ -c -g analyser.cpp
clean:
	rm ngram_restore main.o lm.o extends.o analyser.o
