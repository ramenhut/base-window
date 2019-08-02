/*
//
// Copyright (c) 1998-2019 Joe Bertolami. All Right Reserved.
//
//   Redistribution and use in source and binary forms, with or without
//   modification, are permitted provided that the following conditions are met:
//
//   * Redistributions of source code must retain the above copyright notice,
//     this list of conditions and the following disclaimer.
//
//   * Redistributions in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other materials provided with the distribution.
//
//   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
//   AND ANY EXPRESS OR IMPLIED WARRANTIES, CLUDG, BUT NOT LIMITED TO, THE
//   IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
//   ARE DISCLAIMED.  NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
//   LIABLE FOR ANY DIRECT, DIRECT, CIDENTAL, SPECIAL, EXEMPLARY, OR
//   CONSEQUENTIAL DAMAGES (CLUDG, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
//   GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSESS TERRUPTION)
//   HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER  CONTRACT, STRICT
//   LIABILITY, OR TORT (CLUDG NEGLIGENCE OR OTHERWISE) ARISG  ANY WAY  OF THE
//   USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// Additional Information:
//
//   For more information, visit http://www.bertolami.com.
//
*/

#ifndef __BASE_GRAPHICS_H__
#define __BASE_GRAPHICS_H__

#include "base_window.h"

#if defined(BASE_PLATFORM_WINDOWS)
#include <gl/GL.h>
#elif defined(BASE_PLATFORM_MACOS)
#include "OpenGL/gl.h"
#endif

namespace base {

class GraphicsWindow : public BaseWindow {
 public:
  GraphicsWindow(const ::std::string& title, uint32 x, uint32 y, uint32 width, uint32 height,
                 uint32 render_bpp, uint32 depth_stencil_bpp, uint32 style_flags = 0);
  GraphicsWindow(const GraphicsWindow& rhs) = delete;
  ~GraphicsWindow();

  // Acquires the graphics context for the current frame.
  void BeginScene();
  // Releases the graphics context for the current frame.
  void EndScene();
  // Careful with this call -- we're double buffering so this should never
  // be used (swap will implicitly force a pipeline stall), except in the
  // circumstance of multi-threaded resource rendering.
  void Resolve();

 private:
  // Creates and initializes the graphical subsystem of the window.
  void CreateGraphics(uint32 render_bpp, uint32 depth_stencil_bpp);
  // Tears down the graphics subsystem of the window and releases any connected
  // operating system resources.
  void DestroyGraphics();

#if defined(BASE_PLATFORM_WINDOWS)
  HDC device_context_handle_;
  HGLRC graphics_handle_;
#elif defined(BASE_PLATFORM_IOS)
  // iOS does not provide a default framebuffer. We create one to use
  // whenever no other framebuffer is set. This default framebuffer uses
  // window (layer) memory for backing storage.
  NSOpenGLView* hView;
#elif defined(BASE_PLATFORM_MACOSX)
  NSOpenGLView* hView;
#endif
};

}  // namespace base

/* Implementation */

namespace base {

#if defined(BASE_PLATFORM_WINDOWS)
#pragma comment(lib, "OpenGL32.lib")
#endif

GraphicsWindow::GraphicsWindow(const ::std::string& title, uint32 x, uint32 y, uint32 width,
                               uint32 height, uint32 render_bpp, uint32 depth_stencil_bpp,
                               uint32 style_flags) {
  Create(title, x, y, width, height, style_flags);
  CreateGraphics(render_bpp, depth_stencil_bpp);
}

GraphicsWindow::~GraphicsWindow() { DestroyGraphics(); }

void GraphicsWindow::CreateGraphics(uint32 render_bpp, uint32 depth_stencil_bpp) {
#if defined(BASE_PLATFORM_WINDOWS)
  PIXELFORMATDESCRIPTOR pfd;
  device_context_handle_ = (HDC)GetDC(window_handle_);

  memset(&pfd, 0, sizeof(pfd));

  pfd.nSize = sizeof(pfd);
  pfd.nVersion = 1;
  pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER | PFD_SWAP_EXCHANGE;
  pfd.iPixelType = PFD_TYPE_RGBA;
  pfd.cColorBits = render_bpp;
  pfd.cDepthBits = depth_stencil_bpp;
  pfd.iLayerType = PFD_MAIN_PLANE;

  SetPixelFormat(device_context_handle_, ChoosePixelFormat(device_context_handle_, &pfd), &pfd);

  graphics_handle_ = wglCreateContext(device_context_handle_);
  wglMakeCurrent(device_context_handle_, graphics_handle_);
#endif

  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glViewport(0, 0, width_, height_);
  glPointSize(45.0);

  glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
  glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
  glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
}

void GraphicsWindow::DestroyGraphics() {
#if defined(BASE_PLATFORM_WINDOWS)
  wglMakeCurrent(NULL, NULL);
  wglDeleteContext(graphics_handle_);
  ReleaseDC(window_handle_, device_context_handle_);
#endif
}

void GraphicsWindow::BeginScene() {
#if defined(BASE_PLATFORM_WINDOWS)
  wglMakeCurrent(device_context_handle_, graphics_handle_);
#endif

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glEnable(GL_BLEND);
  glCullFace(GL_BACK);
  glFrontFace(GL_CCW);
  glDepthFunc(GL_LEQUAL);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glDepthMask(TRUE);

#if defined(BASE_PLATFORM_WINDOWS) || defined(BASE_PLATFORM_MACOS)
  glEnable(GL_TEXTURE_2D);
  glDisable(GL_LIGHTING);
  glClearDepth(1.0f);
#endif
}

void GraphicsWindow::EndScene() {
#if defined(BASE_PLATFORM_WINDOWS)
  SwapBuffers(device_context_handle_);
#elif defined(BASE_PLATFORM_MACOS)
  [[m_hView openGLContext] flushBuffer];
#elif defined(BASE_PLATFORM_IOS)
  // On iOS, a render buffer must be bound, which will be presented by
  // flushBuffer. This behavior differs from OSX and Win, which automatically
  // render the attached color buffer of the presently bound framebuffer.
  [m_hView flushBuffer];
#endif
}

void GraphicsWindow::Resolve() {
#if defined(BASE_PLATFORM_WINDOWS)
  wglMakeCurrent(device_context_handle_, graphics_handle_);
#endif

  glFlush();
}

}  // namespace base

#endif  // __BASE_GRAPHICS_H__
