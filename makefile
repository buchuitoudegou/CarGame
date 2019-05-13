CC = g++
CFLAGS = -std=c++11 -w -g
bin = bin
src = src
build = build
target = $(bin)/main

objects := $(build)/camera.o $(build)/loader.o $(build)/model.o $(build)/SkyboxRenderer.o \
$(build)/shader.o $(build)/SkyboxShader.o $(build)/stb_image.o $(build)/tiny_obj_loader.o \
$(build)/glad.o $(build)/main.o

# build glad
$(build)/glad.o : $(src)/glad.c
	g++ -c -o $@ $< $(CFLAGS)

# build camera
$(build)/camera.o : $(src)/camera/camera.cpp
	g++ -c -o $@ $< $(CFLAGS)

# # build imgui
# $(build)/imgui.o : $(src)/imgui/imgui.cpp
# 	g++ -c -o $@ $< $(CFLAGS)

# $(build)/%.o : $(src)/imgui/%.cpp
# 	g++ -c -o $@ $< $(CFLAGS)

# build loader
$(build)/loader.o : $(src)/loader/loader.cpp
	g++ -c -o $@ $< $(CFLAGS)

# build model
$(build)/model.o : $(src)/model/model.cpp
	g++ -c -o $@ $< $(CFLAGS)

# build renderers
$(build)/SkyboxRenderer.o : $(src)/renderers/SkyboxRenderer.cpp
	g++ -c -o $@ $< $(CFLAGS)

# build shaders
$(build)/shader.o : $(src)/shaders/shader.cpp
	g++ -c -o $@ $< $(CFLAGS)
$(build)/SkyboxShader.o : $(src)/shaders/SkyboxShader.cpp
	g++ -c -o $@ $< $(CFLAGS)

# build stb_image
$(build)/stb_image.o : $(src)/stb_image/stb_image.cpp
	g++ -c -o $@ $< $(CFLAGS)

# build tiny_obj_loader
$(build)/tiny_obj_loader.o : $(src)/tiny_obj_loader/tiny_obj_loader.cpp
	g++ -c -o $@ $< $(CFLAGS)

# build main
$(build)/main.o : $(src)/main.cpp
	g++ -c -o $@ $< $(CFLAGS)

$(target) : $(objects)
	g++ $(objects) $(CFLAGS) -lglfw -o bin/main

run:
	./$(target)

clean:
	rm -rf $(bin)
	rm -rf $(build)
	mkdir $(bin)
	mkdir $(build)
