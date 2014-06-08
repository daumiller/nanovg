//
// Copyright (c) 2009-2013 Mikko Mononen memon@inside.org
//
// This software is provided 'as-is', without any express or implied
// warranty.  In no event will the authors be held liable for any damages
// arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgment in the product documentation would be
//    appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be
//    misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.
//
#ifndef NANOVG_GL_UTILS_H
#define NANOVG_GL_UTILS_H

struct NVGLUframebuffer {
	struct NVGcontext* ctx;
	GLuint fbo;
	GLuint rbo;
	GLuint texture;
	int image;
};

// Helper function to create GL frame buffer to render to.
struct NVGLUframebuffer* nvgluCreateFramebuffer(struct NVGcontext* ctx, int w, int h);
void nvgluDeleteFramebuffer(struct NVGcontext* ctx, struct NVGLUframebuffer* fb);

#endif // NANOVG_GL_UTILS_H

#ifdef NANOVG_GL_IMPLEMENTATION

#if defined(NANOVG_GL3)
// FBO is core in OpenGL 3>.
#	define NANOVG_FBO_VALID 1
#elif defined(NANOVG_GL2)
// On OS X including glext defines FBO on GL2 too.
#	ifdef __APPLE__
#		include <OpenGL/glext.h>
#		define NANOVG_FBO_VALID 1
#	endif
#endif

struct NVGLUframebuffer* nvgluCreateFramebuffer(struct NVGcontext* ctx, int w, int h)
{
#ifdef NANOVG_FBO_VALID
	struct NVGLUframebuffer* fb = NULL;
	fb = (struct NVGLUframebuffer*)malloc(sizeof(struct NVGLUframebuffer));
	if (fb == NULL) goto error;
	memset(fb, 0, sizeof(struct NVGLUframebuffer));

	fb->image = nvgCreateImageRGBA(ctx, w, h, NULL);
	fb->texture = nvglImageHandle(ctx, fb->image);
	nvglImageFlags(ctx, fb->image, NVGL_TEXTURE_FLIP_Y);

	// frame buffer object
	glGenFramebuffers(1, &fb->fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fb->fbo);

	// render buffer object
	glGenRenderbuffers(1, &fb->rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, fb->rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, w, h);

	// combine all
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fb->texture, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, fb->rbo);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) goto error;

	return fb;
error:
	nvgluDeleteFramebuffer(ctx, fb);
	return NULL;
#else
	NVG_NOTUSED(ctx);
	NVG_NOTUSED(w);
	NVG_NOTUSED(h);
	return NULL;
#endif
}

void nvgluBindFramebuffer(struct NVGLUframebuffer* fb)
{
#ifdef NANOVG_FBO_VALID
	glBindFramebuffer(GL_FRAMEBUFFER, fb != NULL ? fb->fbo : 0);
#else
	NVG_NOTUSED(fb);
#endif
}

void nvgluDeleteFramebuffer(struct NVGcontext* ctx, struct NVGLUframebuffer* fb)
{
#ifdef NANOVG_FBO_VALID
	if (fb == NULL) return;
	if (fb->fbo != 0)
		glDeleteFramebuffers(1, &fb->fbo);
	if (fb->rbo != 0)
		glDeleteRenderbuffers(1, &fb->rbo);
	if (fb->image >= 0)
		nvgDeleteImage(ctx, fb->image);
	fb->fbo = 0;
	fb->rbo = 0;
	fb->texture = 0;
	fb->image = -1;
	free(fb);
#else
	NVG_NOTUSED(ctx);
	NVG_NOTUSED(fb);
#endif
}

#endif // NANOVG_GL_IMPLEMENTATION
