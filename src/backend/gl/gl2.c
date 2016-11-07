#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

#define NANOVG_GL2
#define NANOVG_GL_USE_STATE_FILTER

#include <NanoVG/nanovg.h>
#include <NanoVG/nanovg_gl.h>
#include <NanoVG/nanovg_internal.h>
#include "common.h"
#include "common.c"
