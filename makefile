CC = g++
CFLAGS = -std=c++11 -w -g
bin = bin
src = src
build = build
target = $(bin)/main

$(target) : $(build)/glad.o $(build)/main.o
	g++ $(build)/glad.o $(build)/main.o $(CFLAGS) -lglfw -o $@

$(build)/%.o : $(src)/%.c
	g++ -c -o $@ $< $(CFLAGS)

$(build)/%.o : $(src)/%.cpp
	g++ -c -o $@ $< $(CFLAGS)

run:
	./$(target)

clean:
	rm -rf $(bin)
	rm -rf $(build)
