ngram_and_vector_restore:main.o lm.o extends.o analyser.o vocab.o
	g++ -o ngram_and_vector_restore main.o lm.o extends.o analyser.o vocab.o
main.o:main.cpp
	g++ -c main.cpp
lm.o:lm.cpp lm.h
	g++ -c lm.cpp
extends.o:extends.cpp extends.h
	g++ -c extends.cpp
analyser.o:lm.o extends.o analyser.h analyser.cpp vocab.o
	g++ -c -g analyser.cpp
vocab.o:vocab.cpp vocab.h
	g++ -c vocab.cpp
clean:
	rm main.o lm.o extends.o analyser.o vocab.o ngram_and_vector_restore 
