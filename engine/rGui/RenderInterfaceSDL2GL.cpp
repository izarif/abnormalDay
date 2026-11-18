/*
 * This source file is part of libRocket, the HTML/CSS Interface Middleware
 *
 * For the latest information, see http://www.librocket.com
 *
 * Copyright (c) 2008-2010 Nuno Silva
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */

#include <Rocket/Core/Core.h>
#include <SDL_image.h>
#include <SDL_opengl.h>

#include <zlib.h>
#include <enet/enet.h>
#include "tools.h"
#include "geom.h"
#include "glexts.h"
#include "glemu.h"

#include "RenderInterfaceSDL2GL.h"

#if !(SDL_VIDEO_RENDER_OGL)
    #error "Only the opengl sdl backend is supported. To add support for others, see http://mdqinc.com/blog/2013/01/integrating-librocket-with-sdl-2/"
#endif

RocketSDL2GLRenderer::RocketSDL2GLRenderer(SDL_GLContext glContext)
{
  mGlContext = glContext;
}

// Called by Rocket when it wants to render geometry that it does not wish to optimise.
void RocketSDL2GLRenderer::RenderGeometry(Rocket::Core::Vertex* vertices, int verticesNum, int* indices, int indicesNum, const Rocket::Core::TextureHandle texture, const Rocket::Core::Vector2f& translation)
{
  ROCKET_UNUSED(verticesNum);

  glPushMatrix();
  glTranslatef(translation.x, translation.y, 0);

  glVertexPointer(2, GL_FLOAT, sizeof(Rocket::Core::Vertex), &vertices[0].position);
  glEnableClientState(GL_COLOR_ARRAY);
  glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(Rocket::Core::Vertex), &vertices[0].colour);

  if (!texture)
  {
    glDisable(GL_TEXTURE_2D);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
  }
  else
  {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, (GLuint) texture);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(2, GL_FLOAT, sizeof(Rocket::Core::Vertex), &vertices[0].tex_coord);
  }

  glDrawElements(GL_TRIANGLES, indicesNum, GL_UNSIGNED_INT, indices);

  glPopMatrix();
}


// Called by Rocket when it wants to enable or disable scissoring to clip content.
void RocketSDL2GLRenderer::EnableScissorRegion(bool enable)
{
  if (enable)
  {
    glEnable(GL_SCISSOR_TEST);
  }
  else
  {
    glDisable(GL_SCISSOR_TEST);
  }
}

// Called by Rocket when it wants to change the scissor region.
void RocketSDL2GLRenderer::SetScissorRegion(int x, int y, int width, int height)
{
  glScissor(x, y, width, height);
}

// Called by Rocket when a texture is required by the library.
bool RocketSDL2GLRenderer::LoadTexture(Rocket::Core::TextureHandle& textureHandle, Rocket::Core::Vector2i& textureDimensions, const Rocket::Core::String& source)
{
  SDL_Surface* surface = IMG_Load(source.CString());

  if (!surface)
  {
    return false;
  }

  SDL_Surface* formattedSurface = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGBA32, 0);
  SDL_FreeSurface(surface);

  if (!formattedSurface)
  {
    return false;
  }

  textureDimensions.x = formattedSurface->w;
  textureDimensions.y = formattedSurface->h;

  GLuint textureId = 0;
  glGenTextures(1, &textureId);

  if (textureId == 0)
  {
    printf("Failed to generate texture\n");
    return false;
  }

  glBindTexture(GL_TEXTURE_2D, textureId);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, formattedSurface->w, formattedSurface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, formattedSurface->pixels);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  SDL_FreeSurface(formattedSurface);

  textureHandle = (Rocket::Core::TextureHandle)textureId;

  return true;
}

// Called by Rocket when a texture is required to be built from an internally-generated sequence of pixels.
bool RocketSDL2GLRenderer::GenerateTexture(Rocket::Core::TextureHandle& textureHandle, const Rocket::Core::byte* source, const Rocket::Core::Vector2i& sourceDimensions)
{
  GLuint textureId = 0;
  glGenTextures(1, &textureId);

  if (textureId == 0)
  {
    printf("Failed to generate texture\n");
    return false;
  }

  glBindTexture(GL_TEXTURE_2D, textureId);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, sourceDimensions.x, sourceDimensions.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, source);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  textureHandle = (Rocket::Core::TextureHandle)textureId;

  return true;
}

// Called by Rocket when a loaded texture is no longer required.
void RocketSDL2GLRenderer::ReleaseTexture(Rocket::Core::TextureHandle textureHandle)
{
  glDeleteTextures(1, (GLuint*)&textureHandle);
}
