COMPILER   = clang
OPTIONS    = -std=c11 -I../include
WARNINGS   = -Weverything -Wno-unused-parameter
LIBRARIES  = glfw3.dylib nanovg.dylib nanovg_gl3.dylib
FRAMEWORKS = -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo -rpath @executable_path

SIMPLE_SOURCES = simple.c
SIMPLE_OBJECTS = $(SIMPLE_SOURCES:.c=.o)
SIMPLE_TARGET  = simple

EXAMPLE_SOURCES = example.c example_ui.c example_perf.c
EXAMPLE_OBJECTS = $(EXAMPLE_SOURCES:.c=.o)
EXAMPLE_TARGET  = example

FBO_SOURCES = fbo.c example_perf.c
FBO_OBJECTS = $(FBO_SOURCES:.c=.o)
FBO_TARGET  = fbo

all: $(LIBRARIES) $(SIMPLE_TARGET) $(EXAMPLE_TARGET) $(FBO_TARGET) clean

%.o: %.c
	$(COMPILER) $(OPTIONS) $(WARNINGS) -c $^ -o $@

%.dylib: ../%.dylib
	ln -s $^ $@

$(SIMPLE_TARGET): $(SIMPLE_OBJECTS)
	$(COMPILER) $(SIMPLE_OBJECTS) $(LIBRARIES) $(FRAMEWORKS) -o $(SIMPLE_TARGET)

$(EXAMPLE_TARGET): $(EXAMPLE_OBJECTS)
	$(COMPILER) $(EXAMPLE_OBJECTS) $(LIBRARIES) $(FRAMEWORKS) -o $(EXAMPLE_TARGET)

$(FBO_TARGET): $(FBO_OBJECTS)
	$(COMPILER) $(FBO_OBJECTS) $(LIBRARIES) $(FRAMEWORKS) -o $(FBO_TARGET)

clean:
	rm -f $(SIMPLE_OBJECTS)
	rm -f $(EXAMPLE_OBJECTS)
	rm -f $(FBO_OBJECTS)

veryclean: clean
	rm -f $(SIMPLE_TARGET) $(EXAMPLE_TARGET) $(FBO_TARGET)

remake: veryclean all
