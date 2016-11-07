#ifndef NANOVG_GL_H
#define NANOVG_GL_H

//==============================================================================
// Enumerations
typedef enum  {
    NVG_ANTIALIAS       = 1<<0, // use geometry based anti-aliasing (may not be needed when using MSAA)
    NVG_STENCIL_STROKES = 1<<1, // strokes should be drawn using stencil buffer
    NVG_DEBUG           = 1<<2, // additional debug checks
} NVGcreateFlags;

//==============================================================================
// Backend Management
NVGcontext* nvgCreateGL(int flags);
void nvgDeleteGL(NVGcontext* ctx);
int nvglCreateImageFromHandleGL(NVGcontext* ctx, GLuint textureId, int w, int h, int imageFlags);
GLuint nvglImageHandleGL(NVGcontext* ctx, int image);

//==============================================================================
// Framebuffer Management
#ifdef NANOVG_GL_USE_FBO
    typedef struct {
        NVGcontext* ctx;
        GLuint fbo;
        GLuint rbo;
        GLuint texture;
        int image;
    } NVGLUframebuffer;

    // Helper function to create GL frame buffer to render to.
    void nvgluBindFramebuffer(NVGLUframebuffer* fb);
    NVGLUframebuffer* nvgluCreateFramebuffer(NVGcontext* ctx, int w, int h, int imageFlags);
    void nvgluDeleteFramebuffer(NVGLUframebuffer* fb);
#endif

#endif // NANOVG_GL_H
