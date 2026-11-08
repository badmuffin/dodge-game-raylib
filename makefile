build:
	g++ main.cpp -o game $(shell pkg-config --cflags --libs raylib)

run: build
	./game

clean:
	rm -rf game