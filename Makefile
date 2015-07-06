FUNC := g++
copt := -c 
OBJ_DIR := ./bin/
FLAGS := -fopenmp -O3 -lm -g

CPP_FILES := $(wildcard src/*.cpp)
OBJ_FILES := $(addprefix $(OBJ_DIR),$(notdir $(CPP_FILES:.cpp=.obj)))

TEXTURE_CPP_FILES := $(wildcard src/Textures/*.cpp)
TEXTURE_OBJ_FILES := $(addprefix $(OBJ_DIR)Textures/,$(notdir $(TEXTURE_CPP_FILES:.cpp=.obj)))

all:
	cd ./src && make
	$(FUNC) ./main.cpp -o ./main.exe $(OBJ_DIR)*.obj $(OBJ_DIR)Textures/*.obj $(FLAGS)

clean:
	cd ./src && make clean
	rm ./*.exe
	rm ./*.obj
