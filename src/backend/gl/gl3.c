#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/gl.h>
#endif

#define NANOVG_GL3
#define NANOVG_GL_USE_FBO
#define NANOVG_GL_USE_UNIFORMBUFFER
#define NANOVG_GL_USE_STATE_FILTER

#include <NanoVG/nanovg.h>
#include <NanoVG/nanovg_gl.h>
#include <NanoVG/nanovg_internal.h>
#include "common.h"
#include "common.c"
