vector_restore:main.o extends.o analyser.o vocab.o
	g++ -o vector_restore main.o extends.o analyser.o vocab.o
main.o:main.cpp
	g++ -c main.cpp
extends.o:extends.cpp extends.h
	g++ -c extends.cpp
analyser.o:extends.o analyser.h analyser.cpp vocab.o
	g++ -c -g analyser.cpp
vocab.o:vocab.cpp vocab.h
	g++ -c vocab.cpp
clean:
	rm main.o extends.o analyser.o vocab.o vector_restore 
