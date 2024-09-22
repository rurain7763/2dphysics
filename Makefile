build:
	g++ -std=c++17 -Wall -I/opt/homebrew/include ./src/*.cpp ./src/Physics/*.cpp -L/opt/homebrew/lib -lm -lSDL2 -lSDL2_image -lSDL2_gfx -o app

run:
	./app

clean:
	rm app
