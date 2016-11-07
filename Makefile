COMPILER = clang
LIBTOOL  = AR
CFLAGS = -std=c11 -O2 -m64 -I$(shell pwd)/include
CWARNS = -Wall -Wextra -Wno-unused-parameter
CDEFS  = -DNDEBUG
LFLAGS =

LIB_SOURCES = src/nanovg.c
LIB_OBJECTS = $(LIB_SOURCES:.c=.o)
LIB_TARGET  = libnanovg.a

GL2_SOURCES = src/backend/gl/gl2.c
GL2_OBJECTS = $(GL2_SOURCES:.c=.o)
GL2_TARGET  = libnanovg_gl2.a

GL3_SOURCES = src/backend/gl/gl3.c
GL3_OBJECTS = $(GL3_SOURCES:.c=.o)
GL3_TARGET  = libnanovg_gl3.a

GLES2_SOURCES = src/backend/gl/gles2.c
GLES2_OBJECTS = $(GLES2_SOURCES:.c=.o)
GLES2_TARGET  = libnanovg_gles2.a

GLES3_SOURCES = src/backend/gl/gles3.c
GLES3_OBJECTS = $(GLES3_SOURCES:.c=.o)
GLES3_TARGET  = libnanovg_gles3.a

%.o: %.c
	$(COMPILER) $(CDEFS) $(CFLAGS) $(CWARNS) -c $^ -o $@

all: nanovg gl2 gl3 gles2 gles3 clean

nanovg: $(LIB_TARGET)
$(LIB_TARGET): $(LIB_OBJECTS)
	$(LIBTOOL) -rcs $(LIB_TARGET) $(LIB_OBJECTS)

gl2: $(GL2_TARGET)
$(GL2_TARGET): $(GL2_OBJECTS)
	$(LIBTOOL) -rcs $(GL2_TARGET) $(GL2_OBJECTS)

gl3: $(GL3_TARGET)
$(GL3_TARGET): $(GL3_OBJECTS)
	$(LIBTOOL) -rcs $(GL3_TARGET) $(GL3_OBJECTS)

gles2: $(GLES2_TARGET)
$(GLES2_TARGET): $(GLES2_OBJECTS)
	$(LIBTOOL) -rcs $(GLES2_TARGET) $(GLES2_OBJECTS)

gles3: $(GLES3_TARGET)
$(GLES3_TARGET): $(GLES3_OBJECTS)
	$(LIBTOOL) -rcs $(GLES3_TARGET) $(GLES3_OBJECTS)

install:
	cp -R include/NanoVG /usr/local/include/
	cp ./*.a /usr/local/lib

clean:
	rm -f $(LIB_OBJECTS)
	rm -f $(GL2_OBJECTS)
	rm -f $(GL3_OBJECTS)
	rm -f $(GLES2_OBJECTS)
	rm -f $(GLES3_OBJECTS)

veryclean: clean
	rm -f $(LIB_TARGET) $(GL2_TARGET) $(GL3_TARGET) $(GLES2_TARGET) $(GLES3_TARGET)

remake: veryclean all
