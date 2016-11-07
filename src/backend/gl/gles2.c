#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

#define NANOVG_GLES2
#define NANOVG_GL_USE_FBO
#define NANOVG_GL_USE_STATE_FILTER

static unsigned int glnvg__nearestPow2(unsigned int num)
{
    unsigned n = num > 0 ? num - 1 : 0;
    n |= n >> 1;
    n |= n >> 2;
    n |= n >> 4;
    n |= n >> 8;
    n |= n >> 16;
    n++;
    return n;
}

#include <NanoVG/nanovg.h>
#include <NanoVG/nanovg_gl.h>
#include <NanoVG/nanovg_internal.h>
#include "common.h"
#include "common.c"
