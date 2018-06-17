COMPILER = clang
CFLAGS   = -std=c11 -O2 -m64 -I$(shell pwd)/include
CWARNS   = -Wall -Wextra -Wno-unused-parameter
CDEFS    = -DNDEBUG
LIBTOOL  = clang
LFLAGS   = -shared
LEXT     = .so

ifeq ($(OS),Windows_NT)
	LEXT := .dll
else
	UNAME_S := $(shell uname -s)
	ifeq ($(UNAME_S),Darwin)
		LEXT := .dylib
		LFLAGS := -dynamiclib
	endif
endif

LIB_SOURCES = src/nanovg.c
LIB_OBJECTS = $(LIB_SOURCES:.c=.o)
LIB_TARGET  = nanovg$(LEXT)
LIB_EXTRA   =

GL2_SOURCES = src/backend/gl/gl2.c
GL2_OBJECTS = $(GL2_SOURCES:.c=.o)
GL2_TARGET  = nanovg_gl2$(LEXT)
GL2_EXTRA   =

GL3_SOURCES = src/backend/gl/gl3.c
GL3_OBJECTS = $(GL3_SOURCES:.c=.o)
GL3_TARGET  = nanovg_gl3$(LEXT)
GL3_EXTRA   =

GLES2_SOURCES = src/backend/gl/gles2.c
GLES2_OBJECTS = $(GLES2_SOURCES:.c=.o)
GLES2_TARGET  = nanovg_gles2$(LEXT)
GLES2_EXTRA   =

GLES3_SOURCES = src/backend/gl/gles3.c
GLES3_OBJECTS = $(GLES3_SOURCES:.c=.o)
GLES3_TARGET  = nanovg_gles3$(LEXT)
GLES3_EXTRA   =

ifneq ($(OS),Windows_NT)
	UNAME_S := $(shell uname -s)
	ifeq ($(UNAME_S),Darwin)
		LIB_EXTRA   := -install_name @rpath/$(LIB_TARGET)
		GL2_EXTRA   := -install_name @rpath/$(GL2_TARGET)   -framework OpenGL
		GL3_EXTRA   := -install_name @rpath/$(GL3_TARGET)   -framework OpenGL
		GLES2_EXTRA := -install_name @rpath/$(GLES2_TARGET) -framework OpenGL
		GLES3_EXTRA := -install_name @rpath/$(GLES3_TARGET) -framework OpenGL
	endif
endif

%.o: %.c
	$(COMPILER) $(CDEFS) $(CFLAGS) $(CWARNS) -c $^ -o $@

all: nanovg gl2 gl3 gles2 gles3 clean

nanovg: $(LIB_TARGET)
$(LIB_TARGET): $(LIB_OBJECTS)
	$(LIBTOOL) $(LFLAGS) -o $(LIB_TARGET) $(LIB_OBJECTS) $(LIB_EXTRA)

gl2: $(GL2_TARGET)
$(GL2_TARGET): $(GL2_OBJECTS)
	$(LIBTOOL) $(LFLAGS) -o $(GL2_TARGET) $(GL2_OBJECTS) $(LIB_TARGET) $(GL2_EXTRA)

gl3: $(GL3_TARGET)
$(GL3_TARGET): $(GL3_OBJECTS)
	$(LIBTOOL) $(LFLAGS) -o $(GL3_TARGET) $(GL3_OBJECTS) $(LIB_TARGET) $(GL3_EXTRA)

gles2: $(GLES2_TARGET)
$(GLES2_TARGET): $(GLES2_OBJECTS)
	$(LIBTOOL) $(LFLAGS) -o $(GLES2_TARGET) $(GLES2_OBJECTS) $(LIB_TARGET) $(GLES2_EXTRA)

gles3: $(GLES3_TARGET)
$(GLES3_TARGET): $(GLES3_OBJECTS)
	$(LIBTOOL) $(LFLAGS) -o $(GLES3_TARGET) $(GLES3_OBJECTS) $(LIB_TARGET) $(GLES3_EXTRA)

install:
	cp -R include/NanoVG /usr/local/include/
	cp ./*$(LEXT) /usr/local/lib

clean:
	rm -f $(LIB_OBJECTS)
	rm -f $(GL2_OBJECTS)
	rm -f $(GL3_OBJECTS)
	rm -f $(GLES2_OBJECTS)
	rm -f $(GLES3_OBJECTS)

veryclean: clean
	rm -f $(LIB_TARGET) $(GL2_TARGET) $(GL3_TARGET) $(GLES2_TARGET) $(GLES3_TARGET)

remake: veryclean all
