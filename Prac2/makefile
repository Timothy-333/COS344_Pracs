#add all the files that you need to the files makefile variable below as a space seperated list
files = shapes.cpp

main: main.cpp glad.c
	g++ -g Shader.cpp $(files) main.cpp glad.c -lglfw3 -pthread -lGLEW -ldl -lGL -o main

clean:
	rm -f *.o main

run:
	./main

all:
	make clean
	make
	make run