FUNC := cl
copt :=/c
output := /Fo
FLAGS := /O2
OBJ_DIR :=./../../bin/Textures/

CPP_FILES := $(wildcard *.cpp)
OBJ_FILES := $(addprefix $(OBJ_DIR),$(notdir $(CPP_FILES:.cpp=.obj)))

all: $(OBJ_FILES)

$(OBJ_DIR)texture.obj: texture.cpp texture.h
	$(FUNC) $(output)$(OBJ_DIR)texture.obj $(copt) texture.cpp $(FLAGS)

$(OBJ_DIR)mandeltexture.obj: mandeltexture.cpp mandeltexture.h $(OBJ_DIR)texture.obj
	$(FUNC) $(output)$(OBJ_DIR)mandeltexture.obj $(copt) mandeltexture.cpp $(FLAGS)
   
$(OBJ_DIR)imagetexture.obj: imagetexture.cpp imagetexture.h $(OBJ_DIR)texture.obj
	$(FUNC) $(output)$(OBJ_DIR)imagetexture.obj $(copt) imagetexture.cpp $(FLAGS)

$(OBJ_DIR)fractalnoise.obj: fractalnoise.cpp fractalnoise.h $(OBJ_DIR)texture.obj
	$(FUNC) $(output)$(OBJ_DIR)fractalnoise.obj $(copt) fractalnoise.cpp $(FLAGS)
   
$(OBJ_DIR)colortexture.obj: colortexture.cpp colortexture.h $(OBJ_DIR)texture.obj
	$(FUNC) $(output)$(OBJ_DIR)colortexture.obj $(copt) colortexture.cpp $(FLAGS)

$(OBJ_DIR)functiontexture.obj: functiontexture.cpp functiontexture.h $(OBJ_DIR)texture.obj
	$(FUNC) $(output)$(OBJ_DIR)functiontexture.obj $(copt) functiontexture.cpp $(FLAGS)

clean:
	rm $(OBJ_DIR)*.obj
