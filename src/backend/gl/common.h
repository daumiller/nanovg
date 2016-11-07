#ifndef NANOVG_GL_COMMON_H
#define NANOVG_GL_COMMON_H

//==============================================================================
// Enumerations
typedef enum {
    GLNVG_LOC_VIEWSIZE,
    GLNVG_LOC_TEX,
    GLNVG_LOC_FRAG,
    GLNVG_MAX_LOCS
} GLNVGuniformLoc;

typedef enum {
    NSVG_SHADER_FILLGRAD,
    NSVG_SHADER_FILLIMG,
    NSVG_SHADER_SIMPLE,
    NSVG_SHADER_IMG
} GLNVGshaderType;

typedef enum {
    GLNVG_FRAG_BINDING = 0,
} GLNVGuniformBindings;

typedef struct {
    GLuint prog;
    GLuint frag;
    GLuint vert;
    GLint loc[GLNVG_MAX_LOCS];
} GLNVGshader;

typedef struct {
    int id;
    GLuint tex;
    int width, height;
    int type;
    int flags;
} GLNVGtexture;

typedef enum {
    GLNVG_NONE = 0,
    GLNVG_FILL,
    GLNVG_CONVEXFILL,
    GLNVG_STROKE,
    GLNVG_TRIANGLES,
} GLNVGcallType;

//==============================================================================
// Structures
typedef struct {
    int type;
    int image;
    int pathOffset;
    int pathCount;
    int triangleOffset;
    int triangleCount;
    int uniformOffset;
} GLNVGcall;

typedef struct {
    int fillOffset;
    int fillCount;
    int strokeOffset;
    int strokeCount;
} GLNVGpath;

typedef struct {
#ifdef NANOVG_GL_USE_UNIFORMBUFFER
    float scissorMat[12]; // matrices are actually 3 vec4s
    float paintMat[12];
    NVGcolor innerCol;
    NVGcolor outerCol;
    float scissorExt[2];
    float scissorScale[2];
    float extent[2];
    float radius;
    float feather;
    float strokeMult;
    float strokeThr;
    int texType;
    int type;
#else
// note: after modifying layout or size of uniform array,
// don't forget to also update the fragment shader source!
#define NANOVG_GL_UNIFORMARRAY_SIZE 11
    union {
        struct {
            float scissorMat[12]; // matrices are actually 3 vec4s
            float paintMat[12];
            NVGcolor innerCol;
            NVGcolor outerCol;
            float scissorExt[2];
            float scissorScale[2];
            float extent[2];
            float radius;
            float feather;
            float strokeMult;
            float strokeThr;
            float texType;
            float type;
        };
        float uniformArray[NANOVG_GL_UNIFORMARRAY_SIZE][4];
    };
#endif
} GLNVGfragUniforms;

typedef struct {
    GLNVGshader shader;
    GLNVGtexture* textures;
    float view[2];
    int ntextures;
    int ctextures;
    int textureId;
    GLuint vertBuf;
    int fragSize;
    int flags;

    // Per frame buffers
    GLNVGcall* calls;
    int ccalls;
    int ncalls;
    GLNVGpath* paths;
    int cpaths;
    int npaths;
    NVGvertex* verts;
    int cverts;
    int nverts;
    unsigned char* uniforms;
    int cuniforms;
    int nuniforms;

#ifdef NANOVG_GL3
    GLuint vertArr;
#endif

#ifdef NANOVG_GL_USE_UNIFORMBUFFER
    GLuint fragBuf;
#endif

#ifdef NANOVG_GL_USE_STATE_FILTER
    GLuint boundTexture;
    GLuint stencilMask;
    GLenum stencilFunc;
    GLint stencilFuncRef;
    GLuint stencilFuncMask;
#endif
} GLNVGcontext;

//==============================================================================
// Helpers
#define glnvg__maxi(a,b) ((a > b) ? a : b)

//==============================================================================
// Shared Functions
static void glnvg__bindTexture(GLNVGcontext* gl, GLuint tex);
static void glnvg__stencilMask(GLNVGcontext* gl, GLuint mask);
static void glnvg__stencilFunc(GLNVGcontext* gl, GLenum func, GLint ref, GLuint mask);
static GLNVGtexture* glnvg__allocTexture(GLNVGcontext* gl);
static GLNVGtexture* glnvg__findTexture(GLNVGcontext* gl, int id);
static int glnvg__deleteTexture(GLNVGcontext* gl, int id);
static void glnvg__dumpShaderError(GLuint shader, const char* name, const char* type);
static void glnvg__dumpProgramError(GLuint prog, const char* name);
static void glnvg__checkError(GLNVGcontext* gl, const char* str);
static int glnvg__createShader(GLNVGshader* shader, const char* name, const char* header, const char* opts, const char* vshader, const char* fshader);
static void glnvg__deleteShader(GLNVGshader* shader);
static void glnvg__getUniforms(GLNVGshader* shader);
static int glnvg__renderDeleteTexture(void* uptr, int image);
static int glnvg__renderGetTextureSize(void* uptr, int image, int* w, int* h);
static void glnvg__xformToMat3x4(float* m3, float* t);
static NVGcolor glnvg__premulColor(NVGcolor c);
static int glnvg__convertPaint(GLNVGcontext* gl, GLNVGfragUniforms* frag, NVGpaint* paint, NVGscissor* scissor, float width, float fringe, float strokeThr);
static void glnvg__setUniforms(GLNVGcontext* gl, int uniformOffset, int image);
static void glnvg__renderViewport(void* uptr, int width, int height, float devicePixelRatio);
static void glnvg__fill(GLNVGcontext* gl, GLNVGcall* call);
static void glnvg__convexFill(GLNVGcontext* gl, GLNVGcall* call);
static void glnvg__stroke(GLNVGcontext* gl, GLNVGcall* call);
static void glnvg__triangles(GLNVGcontext* gl, GLNVGcall* call);
static void glnvg__renderCancel(void* uptr);
static GLenum glnvg_convertBlendFuncFactor(int factor);
static void glnvg__blendCompositeOperation(NVGcompositeOperationState op);
static void glnvg__renderFlush(void* uptr, NVGcompositeOperationState compositeOperation);
static int glnvg__maxVertCount(const NVGpath* paths, int npaths);
static GLNVGcall* glnvg__allocCall(GLNVGcontext* gl);
static int glnvg__allocPaths(GLNVGcontext* gl, int n);
static int glnvg__allocVerts(GLNVGcontext* gl, int n);
static int glnvg__allocFragUniforms(GLNVGcontext* gl, int n);
static GLNVGfragUniforms* nvg__fragUniformPtr(GLNVGcontext* gl, int i);
static void glnvg__vset(NVGvertex* vtx, float x, float y, float u, float v);
static void glnvg__renderFill(void* uptr, NVGpaint* paint, NVGscissor* scissor, float fringe, const float* bounds, const NVGpath* paths, int npaths);
static void glnvg__renderStroke(void* uptr, NVGpaint* paint, NVGscissor* scissor, float fringe, float strokeWidth, const NVGpath* paths, int npaths);
static void glnvg__renderTriangles(void* uptr, NVGpaint* paint, NVGscissor* scissor, const NVGvertex* verts, int nverts);
static void glnvg__renderDelete(void* uptr);

//==============================================================================
// Very-Conditional Shared Functions
static int glnvg__renderCreate(void* uptr);
static int glnvg__renderCreateTexture(void* uptr, int type, int w, int h, int imageFlags, const unsigned char* data);
static int glnvg__renderUpdateTexture(void* uptr, int image, int x, int y, int w, int h, const unsigned char* data);

#endif // NANOVG_GL_COMMON_H
