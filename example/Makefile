COMPILER   = clang
OPTIONS    = -std=c11 -I../include
WARNINGS   = -Weverything -Wno-unused-parameter
LIBRARIES  = -lglfw3 ../libnanovg.a ../libnanovg_gl3.a
FRAMEWORKS = -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo

# # if installed
# OPTIONS    = -std=c11
# LIBRARIES  = -lglfw3 -lnanovg -lnanovg_gl3

SIMPLE_SOURCES = simple.c
SIMPLE_OBJECTS = $(SIMPLE_SOURCES:.c=.o)
SIMPLE_TARGET  = simple

EXAMPLE_SOURCES = example.c example_ui.c example_perf.c
EXAMPLE_OBJECTS = $(EXAMPLE_SOURCES:.c=.o)
EXAMPLE_TARGET  = example

FBO_SOURCES = fbo.c example_perf.c
FBO_OBJECTS = $(FBO_SOURCES:.c=.o)
FBO_TARGET  = fbo

all: $(SIMPLE_TARGET) $(EXAMPLE_TARGET) $(FBO_TARGET) clean

%.o: %.c
	$(COMPILER) $(OPTIONS) $(WARNINGS) -c $^ -o $@

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
