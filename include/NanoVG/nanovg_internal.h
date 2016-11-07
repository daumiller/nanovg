#ifndef NANOVG_INTERNAL_H
#define NANOVG_INTERNAL_H

#include <NanoVG/nanovg.h>

typedef enum {
    NVG_TEXTURE_ALPHA = 0x01,
    NVG_TEXTURE_RGBA = 0x02,
} NVGtexture;

typedef struct {
    float xform[6];
    float extent[2];
} NVGscissor;

typedef struct {
    float x,y,u,v;
} NVGvertex;

typedef struct {
    int first;
    int count;
    unsigned char closed;
    int nbevel;
    NVGvertex* fill;
    int nfill;
    NVGvertex* stroke;
    int nstroke;
    int winding;
    int convex;
} NVGpath;

typedef struct {
    void* userPtr;
    int edgeAntiAlias;
    int (*renderCreate)(void* uptr);
    int (*renderCreateTexture)(void* uptr, int type, int w, int h, int imageFlags, const unsigned char* data);
    int (*renderDeleteTexture)(void* uptr, int image);
    int (*renderUpdateTexture)(void* uptr, int image, int x, int y, int w, int h, const unsigned char* data);
    int (*renderGetTextureSize)(void* uptr, int image, int* w, int* h);
    void (*renderViewport)(void* uptr, int width, int height, float devicePixelRatio);
    void (*renderCancel)(void* uptr);
    void (*renderFlush)(void* uptr, NVGcompositeOperationState compositeOperation);
    void (*renderFill)(void* uptr, NVGpaint* paint, NVGscissor* scissor, float fringe, const float* bounds, const NVGpath* paths, int npaths);
    void (*renderStroke)(void* uptr, NVGpaint* paint, NVGscissor* scissor, float fringe, float strokeWidth, const NVGpath* paths, int npaths);
    void (*renderTriangles)(void* uptr, NVGpaint* paint, NVGscissor* scissor, const NVGvertex* verts, int nverts);
    void (*renderDelete)(void* uptr);
} NVGparams;


NVGcontext* nvgCreateInternal(NVGparams* params);
void nvgDeleteInternal(NVGcontext* ctx);
NVGparams* nvgInternalParams(NVGcontext* ctx);

#endif // NANOVG_INTERNAL_H
