/*
===========================================================================
Copyright (C) 2010 James Canete (use.less01@gmail.com)
Copyright (C) 2015-2019 GrangerHub

This file is part of Tremulous.

Tremulous is free software; you can redistribute it
and/or modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 3 of the License,
or (at your option) any later version.

Tremulous is distributed in the hope that it will be
useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Tremulous; if not, see <https://www.gnu.org/licenses/>

===========================================================================
*/
// tr_fbo.h

#ifndef __TR_FBO_H__
#define __TR_FBO_H__

struct image_s;
struct shaderProgram_s;

typedef struct FBO_s
{
	char            name[MAX_QPATH];

	int             index;

	uint32_t        frameBuffer;

	uint32_t        colorBuffers[16];
	int             colorFormat;
	struct image_s  *colorImage[16];

	uint32_t        depthBuffer;
	int             depthFormat;

	uint32_t        stencilBuffer;
	int             stencilFormat;

	uint32_t        packedDepthStencilBuffer;
	int             packedDepthStencilFormat;

	int             width;
	int             height;
} FBO_t;

void FBO_Bind(FBO_t *fbo);
void FBO_Init(void);
void FBO_Shutdown(void);

void FBO_BlitFromTexture(struct image_s *src, ivec4_t srcBox, vec2_t srcTexScale, FBO_t *dst, ivec4_t dstBox, struct shaderProgram_s *shaderProgram, vec4_t color, int blend);
void FBO_Blit(FBO_t *src, ivec4_t srcBox, vec2_t srcTexScale, FBO_t *dst, ivec4_t dstBox, struct shaderProgram_s *shaderProgram, vec4_t color, int blend);
void FBO_FastBlit(FBO_t *src, ivec4_t srcBox, FBO_t *dst, ivec4_t dstBox, int buffers, int filter);


#endif
