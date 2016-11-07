#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <GLFW/glfw3.h>
#include <NanoVG/nanovg.h>
#include <NanoVG/nanovg_gl.h>

static int scaleWidth, scaleHeight;
static int pixelWidth, pixelHeight;
static float pixelScale;

static void drawWindow(NVGcontext *vg, const char *title, float x,float y, float w,float h);
static void loadFonts(NVGcontext *vg);

__attribute__ ((noreturn))
static void onError(int number, const char *description) {
    printf("GLFW Error %d: %s\n", number, description);
    exit(-1);
}

static void onGlfwKey(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if((action == GLFW_PRESS) && (key == GLFW_KEY_ESCAPE)) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}

static void onGlfwResize(GLFWwindow *window, int pxWidth, int pxHeight) {
    pixelWidth  = pxWidth;
    pixelHeight = pxHeight;
    glfwGetWindowSize(window, &scaleWidth, &scaleHeight);
    pixelScale = (float)pixelWidth / (float)scaleWidth;
    glViewport(0, 0, pixelWidth, pixelHeight);
}

int main(int argc, char **argv, char **envp) {
    if(!glfwInit()) { onError(0, "Failed to init GLFW."); }

    glfwSetErrorCallback(onError);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(960, 600, "Test 00", NULL, NULL);
    if(!window) { onError(0, "Failed creating GLFW window."); }

    glfwSetKeyCallback(window, onGlfwKey);
    glfwMakeContextCurrent(window);

    NVGcontext *vg = nvgCreateGL(NVG_ANTIALIAS | NVG_STENCIL_STROKES);
    if(!vg) { onError(0, "Failed to init nanovg."); }

    loadFonts(vg);

    glfwSwapInterval(1); // cap framerate
    glfwSetTime(0);
    double t, dt, prevt = glfwGetTime();
    double mouseX, mouseY;

    glfwSetFramebufferSizeCallback(window, onGlfwResize);
    glfwGetFramebufferSize(window, &pixelWidth, &pixelHeight);
    onGlfwResize(window, pixelWidth, pixelHeight);

    while(!glfwWindowShouldClose(window)) {
        t = glfwGetTime();
        dt = t - prevt;
        prevt = t;

        glfwGetCursorPos(window, &mouseX, &mouseY);

        glClearColor(0.3f, 0.3f, 0.32f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);

        nvgBeginFrame(vg, scaleWidth, scaleHeight, pixelScale);
        drawWindow(vg, "Hello NanoVG!", 32,32, 480,320);
        nvgEndFrame(vg);

        glfwSwapBuffers(window);
        glfwWaitEvents(); //glfwPollEvents();
    }

    nvgDeleteGL(vg);
    glfwTerminate();
    return 0;
}

static NVGpaint shadowPaint, headerPaint;
static bool styled = false;

static void drawWindow(NVGcontext *vg, const char *title, float x,float y, float w,float h)
{
    static float cornerRadius = 3.0f;
    if(!styled) {
        shadowPaint = nvgBoxGradient(vg, x,y+2, w,h, cornerRadius*2, 10, nvgRGBA(0,0,0,128), nvgRGBA(0,0,0,0));
        headerPaint = nvgLinearGradient(vg, x,y,x,y+15, nvgRGBA(255,255,255,8), nvgRGBA(0,0,0,16));
        styled = true;
    }

    // Save State
    nvgSave(vg);

    // Window Frame
    nvgBeginPath(vg);
    nvgRoundedRect(vg, x,y, w,h, cornerRadius);
    nvgFillColor(vg, nvgRGBA(28,30,34,192));
    nvgFill(vg);

    // Drop shadow
    nvgBeginPath(vg);
    nvgRect(vg, x-10,y-10, w+20,h+30);
    nvgRoundedRect(vg, x,y, w,h, cornerRadius);
    nvgPathWinding(vg, NVG_HOLE);
    nvgFillPaint(vg, shadowPaint);
    nvgFill(vg);

    // Header Box
    nvgBeginPath(vg);
    nvgRoundedRect(vg, x+1,y+1, w-2,30, cornerRadius-1);
    nvgFillPaint(vg, headerPaint);
    nvgFill(vg);
    nvgBeginPath(vg);
    nvgMoveTo(vg, x+0.5f, y+0.5f+30);
    nvgLineTo(vg, x+0.5f+w-1, y+0.5f+30);
    nvgStrokeColor(vg, nvgRGBA(0,0,0,32));
    nvgStroke(vg);

    // Title
    nvgFontSize(vg, 18.0f);
    nvgFontFace(vg, "sans-bold");
    nvgTextAlign(vg,NVG_ALIGN_CENTER|NVG_ALIGN_MIDDLE);
    nvgFontBlur(vg,2);
    nvgFillColor(vg, nvgRGBA(0,0,0,128));
    nvgText(vg, x+w/2,y+16+1, title, NULL);
    nvgFontBlur(vg,0);
    nvgFillColor(vg, nvgRGBA(220,220,220,160));
    nvgText(vg, x+w/2,y+16, title, NULL);

    // Restore State
    nvgRestore(vg);
}

static void loadFonts(NVGcontext *vg) {
    if(nvgCreateFont(vg, "icons"    , "fonts/entypo.ttf"           ) == -1) { onError(0, "Error loading font 'icons'."    ); }
    if(nvgCreateFont(vg, "sans"     , "fonts/Roboto-Regular.ttf"   ) == -1) { onError(0, "Error loading font 'sans'."     ); }
    if(nvgCreateFont(vg, "sans-bold", "fonts/Roboto-Bold.ttf"      ) == -1) { onError(0, "Error loading font 'sans-bold'."); }
    if(nvgCreateFont(vg, "emoji"    , "fonts/NotoEmoji-Regular.ttf") == -1) { onError(0, "Error loading font 'emoji'."    ); }
}
