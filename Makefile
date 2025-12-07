all:
	g++ -g ./src/*.cpp -I./include -o main -lSDL2 -lSDL2_ttf -ldl -pthread
	./main localhost