CC = g++
CFLAGS = -std=c++11 -w -g
bin = bin
src = src
build = build
target = $(bin)/main

objects := $(build)/camera.o $(build)/loader.o $(build)/model.o $(build)/SkyboxRenderer.o \
$(build)/shader.o $(build)/SkyboxShader.o $(build)/stb_image.o $(build)/tiny_obj_loader.o \
$(build)/glad.o $(build)/main.o $(build)/imgui.o $(build)/imgui_draw.o $(build)/imgui_impl_glfw.o \
$(build)/imgui_impl_opengl3.o $(build)/imgui_widgets.o $(build)/mloader.o $(build)/mesh.o \
$(build)/entityRenderer.o $(build)/car.o $(build)/RendererManager.o $(build)/entity.o \
$(build)/plane.o $(build)/city.o

$(target) : $(objects)
	g++ $(objects) $(CFLAGS) -lglfw -lassimp -o $@

# build glad
$(build)/glad.o : $(src)/glad.c
	g++ -c -o $@ $< $(CFLAGS)

# build camera
$(build)/camera.o : $(src)/camera/camera.cpp
	g++ -c -o $@ $< $(CFLAGS)

# build imgui
$(build)/%.o : $(src)/imgui/%.cpp
	g++ -c -o $@ $< $(CFLAGS)

# -----------------------------
# build loader
# build mesh
$(build)/mesh.o : $(src)/mesh/mesh.cpp
	g++ -c -o $@ $< $(CFLAGS)

# build model loader
$(build)/mloader.o : $(src)/model_loader/loader.cpp
	g++ -c -o $@ $< $(CFLAGS)

# build sky loader
$(build)/loader.o : $(src)/skyloader/loader.cpp
	g++ -c -o $@ $< $(CFLAGS)

# build model
$(build)/model.o : $(src)/model/model.cpp
	g++ -c -o $@ $< $(CFLAGS)

# build tiny_obj_loader
$(build)/tiny_obj_loader.o : $(src)/tiny_obj_loader/tiny_obj_loader.cpp
	g++ -c -o $@ $< $(CFLAGS)
# -----------------------------

# -----------------------------
# build shaders
# build shader
$(build)/shader.o : $(src)/shaders/shader.cpp $(src)/shaders/shader.h
	g++ -c -o $@ $< $(CFLAGS)
$(build)/SkyboxShader.o : $(src)/shaders/SkyboxShader.cpp
	g++ -c -o $@ $< $(CFLAGS)
# -----------------------------

# -----------------------------
# build stb_image
$(build)/stb_image.o : $(src)/stb_image/stb_image.cpp
	g++ -c -o $@ $< $(CFLAGS)
# -----------------------------


# -----------------------------
# build renderers
# build renderer manager
$(build)/RendererManager.o : $(src)/renderers/RendererManager.cpp $(src)/renderers/RendererManager.h
	g++ -c -o $@ $< $(CFLAGS)

# build car renderer
$(build)/entityRenderer.o : $(src)/renderers/entityRenderer.cpp $(src)/renderers/entityRenderer.h
	g++ -c -o $@ $< $(CFLAGS)

# build sky renderer
$(build)/SkyboxRenderer.o : $(src)/renderers/SkyboxRenderer.cpp $(src)/renderers/SkyboxRenderer.h
	g++ -c -o $@ $< $(CFLAGS)
# -----------------------------

# -----------------------------
# build game object
# build entity obj
$(build)/entity.o : $(src)/game_object/entity.cpp $(src)/game_object/entity.h
	g++ -c -o $@ $< $(CFLAGS)

# build car obj
$(build)/car.o : $(src)/game_object/car.cpp $(src)/game_object/car.h
	g++ -c -o $@ $< $(CFLAGS)

# build plane obj
$(build)/plane.o : $(src)/game_object/plane.cpp $(src)/game_object/plane.h
	g++ -c -o $@ $< $(CFLAGS)

# build city
$(build)/city.o : $(src)/game_object/City.cpp $(src)/game_object/City.h
	g++ -c -o $@ $< $(CFLAGS)
# -----------------------------

# -----------------------------
# build main
$(build)/main.o : $(src)/main.cpp
	g++ -c -o $@ $< $(CFLAGS)
# -----------------------------

run:
	./$(target)

clean:
	rm -rf $(bin)
	rm -rf $(build)
	mkdir $(bin)
	mkdir $(build)
