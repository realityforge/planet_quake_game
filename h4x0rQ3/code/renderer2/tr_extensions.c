/*
===========================================================================
Copyright (C) 2011 James Canete (use.less01@gmail.com)

This file is part of Quake III Arena source code.

Quake III Arena source code is free software; you can redistribute it
and/or modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2 of the License,
or (at your option) any later version.

Quake III Arena source code is distributed in the hope that it will be
useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Quake III Arena source code; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
===========================================================================
*/
// tr_extensions.c - extensions needed by the renderer not in sdl_glimp.c


#ifdef _MSC_VER
#	pragma warning (disable : 4054 )
#	define SDL_GL_GetProcAddress( a ) qwglGetProcAddress( a )
#else
#	define SDL_GL_GetProcAddress( a ) (void *)qwglGetProcAddress( a )
#endif

#include "tr_local.h"
#include "tr_dsa.h"

#define GLE(ret, name, ...) name##proc * qgl##name;
QGL_1_2_PROCS;
QGL_1_3_PROCS;
QGL_1_4_PROCS;
QGL_1_5_PROCS;
QGL_2_0_PROCS;
QGL_EXT_framebuffer_object_PROCS;
QGL_EXT_framebuffer_blit_PROCS;
QGL_EXT_framebuffer_multisample_PROCS;
QGL_ARB_vertex_array_object_PROCS;
QGL_EXT_direct_state_access_PROCS;
#undef GLE


void GLimp_InitExtraExtensions( void )
{
	char *extension;
	const char* result[3] = { "...ignoring %s\n", "...using %s\n", "...%s not found\n" };

	// Check OpenGL version
	sscanf(glConfig.version_string, "%d.%d", &glRefConfig.openglMajorVersion, &glRefConfig.openglMinorVersion);
	if (glRefConfig.openglMajorVersion < 2)
		ri.Error(ERR_FATAL, "OpenGL 2.0 required!");
	ri.Printf(PRINT_ALL, "...using OpenGL %s\n", glConfig.version_string);

	// set DSA fallbacks
#define GLE(ret, name, ...) qgl##name = GLDSA_##name;
	QGL_EXT_direct_state_access_PROCS;
#undef GLE

	// GL function loader, based on https://gist.github.com/rygorous/16796a0c876cf8a5f542caddb55bce8a
#define GLE(ret, name, ...) qgl##name = (name##proc *) SDL_GL_GetProcAddress("gl" #name);

	// OpenGL 1.2, was GL_EXT_draw_range_elements
	QGL_1_2_PROCS;
	glRefConfig.drawRangeElements = !!r_ext_draw_range_elements->integer;
	ri.Printf(PRINT_ALL, result[glRefConfig.drawRangeElements], "glDrawRangeElements()");

	// OpenGL 1.3, was GL_ARB_texture_compression
	QGL_1_3_PROCS;

	// OpenGL 1.4, was GL_EXT_multi_draw_arrays
	QGL_1_4_PROCS;
	glRefConfig.multiDrawArrays = !!r_ext_multi_draw_arrays->integer;
	ri.Printf(PRINT_ALL, result[glRefConfig.multiDrawArrays], "glMultiDrawElements()");

	// OpenGL 1.5, was GL_ARB_vertex_buffer_object and GL_ARB_occlusion_query
	QGL_1_5_PROCS;
	glRefConfig.occlusionQuery = qtrue;

	// OpenGL 2.0, was GL_ARB_shading_language_100, GL_ARB_vertex_program, GL_ARB_shader_objects, and GL_ARB_vertex_shader
	QGL_2_0_PROCS;
	
	// Determine GLSL version
	if (1)
	{
		char version[256];

		Q_strncpyz(version, (char *)qglGetString(GL_SHADING_LANGUAGE_VERSION), sizeof(version));

		sscanf(version, "%d.%d", &glRefConfig.glslMajorVersion, &glRefConfig.glslMinorVersion);

		ri.Printf(PRINT_ALL, "...using GLSL version %s\n", version);
	}

	glRefConfig.memInfo = MI_NONE;

	// GL_NVX_gpu_memory_info
	extension = "GL_NVX_gpu_memory_info";
	if( GLimp_HaveExtension( extension ) )
		{
		glRefConfig.memInfo = MI_NVX;

		ri.Printf(PRINT_ALL, result[1], extension);
	}
	else
	{
		ri.Printf(PRINT_ALL, result[2], extension);
	}

	// GL_ATI_meminfo
	extension = "GL_ATI_meminfo";
	if( GLimp_HaveExtension( extension ) )
	{
		if (glRefConfig.memInfo == MI_NONE)
		{
			glRefConfig.memInfo = MI_ATI;

			ri.Printf(PRINT_ALL, result[1], extension);
	}
	else
	{
			ri.Printf(PRINT_ALL, result[0], extension);
	}
	}
	else
	{
		ri.Printf(PRINT_ALL, result[2], extension);
	}

	// GL_ARB_texture_non_power_of_two
	extension = "GL_ARB_texture_non_power_of_two";
	glRefConfig.textureNonPowerOfTwo = qfalse;
	if( GLimp_HaveExtension( extension ) )
	{
		glRefConfig.textureNonPowerOfTwo = qtrue; // !!r_ext_texture_non_power_of_two->integer

		ri.Printf(PRINT_ALL, result[glRefConfig.textureNonPowerOfTwo], extension);
	}
	else
	{
		ri.Printf(PRINT_ALL, result[2], extension);
	}

	// GL_ARB_texture_float
	extension = "GL_ARB_texture_float";
	glRefConfig.textureFloat = qfalse;
	if( GLimp_HaveExtension( extension ) )
	{
		glRefConfig.textureFloat = !!r_ext_texture_float->integer;

		ri.Printf(PRINT_ALL, result[glRefConfig.textureFloat], extension);
	}
	else
	{
		ri.Printf(PRINT_ALL, result[2], extension);
	}

	// GL_ARB_half_float_pixel
	extension = "GL_ARB_half_float_pixel";
	glRefConfig.halfFloatPixel = qfalse;
	if( GLimp_HaveExtension( extension ) )
	{
		glRefConfig.halfFloatPixel = !!r_arb_half_float_pixel->integer;

		ri.Printf(PRINT_ALL, result[glRefConfig.halfFloatPixel], extension);
	}
	else
	{
		ri.Printf(PRINT_ALL, result[2], extension);
	}

	// GL_EXT_framebuffer_object
	extension = "GL_EXT_framebuffer_object";
	glRefConfig.framebufferObject = qfalse;
	if( GLimp_HaveExtension( extension ) )
	{
		glRefConfig.framebufferObject = !!r_ext_framebuffer_object->integer;

		qglGetIntegerv(GL_MAX_RENDERBUFFER_SIZE_EXT, &glRefConfig.maxRenderbufferSize);
		qglGetIntegerv(GL_MAX_COLOR_ATTACHMENTS_EXT, &glRefConfig.maxColorAttachments);

		QGL_EXT_framebuffer_object_PROCS;

		ri.Printf(PRINT_ALL, result[glRefConfig.framebufferObject], extension);
	}
	else
	{
		ri.Printf(PRINT_ALL, result[2], extension);
	}

	// GL_EXT_framebuffer_blit
	extension = "GL_EXT_framebuffer_blit";
	glRefConfig.framebufferBlit = qfalse;
	if (GLimp_HaveExtension(extension))
	{
		glRefConfig.framebufferBlit = qtrue;

		QGL_EXT_framebuffer_blit_PROCS;

		ri.Printf(PRINT_ALL, result[glRefConfig.framebufferBlit], extension);
	}
	else
	{
		ri.Printf(PRINT_ALL, result[2], extension);
	}

	// GL_EXT_framebuffer_multisample
	extension = "GL_EXT_framebuffer_multisample";
	glRefConfig.framebufferMultisample = qfalse;
	if (GLimp_HaveExtension(extension))
	{
		glRefConfig.framebufferMultisample = qtrue;

		QGL_EXT_framebuffer_multisample_PROCS;

		ri.Printf(PRINT_ALL, result[glRefConfig.framebufferMultisample], extension);
	}
	else
	{
		ri.Printf(PRINT_ALL, result[2], extension);
	}

	glRefConfig.textureCompression = TCR_NONE;

	// GL_ARB_texture_compression_rgtc
	extension = "GL_ARB_texture_compression_rgtc";
	if (GLimp_HaveExtension(extension))
	{
		qboolean useRgtc = r_ext_compressed_textures->integer >= 1;

		if (useRgtc)
			glRefConfig.textureCompression |= TCR_RGTC;

		ri.Printf(PRINT_ALL, result[useRgtc], extension);
	}
	else
	{
		ri.Printf(PRINT_ALL, result[2], extension);
	}

	glRefConfig.swizzleNormalmap = r_ext_compressed_textures->integer && !(glRefConfig.textureCompression & TCR_RGTC);

	// GL_ARB_texture_compression_bptc
	extension = "GL_ARB_texture_compression_bptc";
	if (GLimp_HaveExtension(extension))
	{
		qboolean useBptc = r_ext_compressed_textures->integer >= 2;

		if (useBptc)
			glRefConfig.textureCompression |= TCR_BPTC;

		ri.Printf(PRINT_ALL, result[useBptc], extension);
	}
	else
	{
		ri.Printf(PRINT_ALL, result[2], extension);
	}

	// GL_ARB_depth_clamp
	extension = "GL_ARB_depth_clamp";
	glRefConfig.depthClamp = qfalse;
	if( GLimp_HaveExtension( extension ) )
	{
		glRefConfig.depthClamp = qtrue;

		ri.Printf(PRINT_ALL, result[glRefConfig.depthClamp], extension);
	}
	else
	{
		ri.Printf(PRINT_ALL, result[2], extension);
	}

	// GL_ARB_seamless_cube_map
	extension = "GL_ARB_seamless_cube_map";
	glRefConfig.seamlessCubeMap = qfalse;
	if( GLimp_HaveExtension( extension ) )
	{
		glRefConfig.seamlessCubeMap = !!r_arb_seamless_cube_map->integer;

		ri.Printf(PRINT_ALL, result[glRefConfig.seamlessCubeMap], extension);
	}
	else
	{
		ri.Printf(PRINT_ALL, result[2], extension);
	}

	// use float lightmaps?
	glRefConfig.floatLightmap = (glRefConfig.textureFloat && glRefConfig.halfFloatPixel && r_floatLightmap->integer && r_hdr->integer);

	// GL_ARB_vertex_array_object
	extension = "GL_ARB_vertex_array_object";
	glRefConfig.vertexArrayObject = qfalse;
	if( GLimp_HaveExtension( extension ) )
	{
		glRefConfig.vertexArrayObject = !!r_arb_vertex_array_object->integer;

		QGL_ARB_vertex_array_object_PROCS;

		ri.Printf(PRINT_ALL, result[glRefConfig.vertexArrayObject], extension);
	}
	else
	{
		ri.Printf(PRINT_ALL, result[2], extension);
	}

	// GL_ARB_half_float_vertex
	extension = "GL_ARB_half_float_vertex";
	glRefConfig.packedTexcoordDataType = GL_FLOAT;
	glRefConfig.packedTexcoordDataSize = sizeof(float) * 2;
	glRefConfig.packedColorDataType    = GL_FLOAT;
	glRefConfig.packedColorDataSize    = sizeof(float) * 4;
	if( GLimp_HaveExtension( extension ) )
	{
		qboolean useExt = !!r_arb_half_float_vertex->integer;

		if (useExt)
		{
			glRefConfig.packedTexcoordDataType = GL_HALF_FLOAT;
			glRefConfig.packedTexcoordDataSize = sizeof(uint16_t) * 2;
			glRefConfig.packedColorDataType    = GL_HALF_FLOAT;
			glRefConfig.packedColorDataSize    = sizeof(uint16_t) * 4;
		}

		ri.Printf(PRINT_ALL, result[useExt], extension);
	}
	else
	{
		ri.Printf(PRINT_ALL, result[2], extension);
	}

	// GL_EXT_direct_state_access
	extension = "GL_EXT_direct_state_access";
	glRefConfig.directStateAccess = qfalse;
	if (GLimp_HaveExtension(extension))
	{
		glRefConfig.directStateAccess = !!r_ext_direct_state_access->integer;

		// QGL_*_PROCS becomes several functions, do not remove {}
		if (glRefConfig.directStateAccess)
		{
			QGL_EXT_direct_state_access_PROCS;
		}

		ri.Printf(PRINT_ALL, result[glRefConfig.directStateAccess], extension);
	}
	else
	{
		ri.Printf(PRINT_ALL, result[2], extension);
	}

#undef GLE
}
#ifdef _MSC_VER
#pragma warning (default : 4054 )
#endif
