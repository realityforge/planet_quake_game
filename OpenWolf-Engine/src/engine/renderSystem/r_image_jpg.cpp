////////////////////////////////////////////////////////////////////////////////////////
// Copyright(C) 1999 - 2005 Id Software, Inc.
// Copyright(C) 2000 - 2013 Darklegion Development
// Copyright(C) 2011 - 2019 Dusan Jocic <dusanjocic@msn.com>
//
// This file is part of OpenWolf.
//
// OpenWolf is free software; you can redistribute it
// and / or modify it under the terms of the GNU General Public License as
// published by the Free Software Foundation; either version 3 of the License,
// or (at your option) any later version.
//
// OpenWolf is distributed in the hope that it will be
// useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with OpenWolf; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110 - 1301  USA
//
// -------------------------------------------------------------------------------------
// File name:   r_image_jpg.cpp
// Version:     v1.00
// Created:
// Compilers:   Visual Studio 2019, gcc 7.3.0
// Description:
// -------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////

#include <renderSystem/r_precompiled.h>

idRenderSystemImageJPEGLocal renderSystemImageJPEGLocal;

/*
===============
idRenderSystemImageJPEGLocal::idRenderSystemImageJPEGLocal
===============
*/
idRenderSystemImageJPEGLocal::idRenderSystemImageJPEGLocal( void )
{
}

/*
===============
idRenderSystemImageJPEGLocal::~idRenderSystemImageJPEGLocal
===============
*/
idRenderSystemImageJPEGLocal::~idRenderSystemImageJPEGLocal( void )
{
}

void idRenderSystemImageJPEGLocal::JPGErrorExit( j_common_ptr cinfo )
{
    UTF8 buffer[200];
    
    /* cinfo->err really points to a q_jpeg_error_mgr_s struct, so coerce pointer */
    q_jpeg_error_mgr_t* jerr = ( q_jpeg_error_mgr_t* )cinfo->err;
    
    ( *cinfo->err->format_message )( cinfo, buffer );
    
    clientMainSystem->RefPrintf( PRINT_ALL, "Error: %s", buffer );
    
    /* Return control to the setjmp point */
    longjmp( jerr->setjmp_buffer, 1 );
}

void idRenderSystemImageJPEGLocal::JPGOutputMessage( j_common_ptr cinfo )
{
    UTF8 buffer[200];
    
    /* Create the message */
    ( *cinfo->err->format_message )( cinfo, buffer );
    
    /* Send it to stderr, adding a newline */
    clientMainSystem->RefPrintf( PRINT_ALL, "%s\n", buffer );
}

void idRenderSystemImageJPEGLocal::LoadJPG( StringEntry filename, U8** pic, S32* width, S32* height )
{
    /* This struct contains the JPEG decompression parameters and pointers to
     * working space (which is allocated as needed by the JPEG library).
     */
    struct jpeg_decompress_struct cinfo = {};
    
    /* We use our private extension JPEG error handler.
     * Note that this struct must live as long as the main JPEG parameter
     * struct, to avoid dangling-pointer problems.
     */
    /* This struct represents a JPEG error handler.  It is declared separately
     * because applications often want to supply a specialized error handler
     * (see the second half of this file for an example).  But here we just
     * take the easy way out and use the standard error handler, which will
     * print a message on stderr and call exit() if compression fails.
     * Note that this struct must live as long as the main JPEG parameter
     * struct, to avoid dangling-pointer problems.
     */
    q_jpeg_error_mgr_t jerr;
    /* More stuff */
    JSAMPARRAY buffer;		/* Output row buffer */
    U32 row_stride;	/* physical row width in output buffer */
    U32 pixelcount, memcount;
    U32 sindex, dindex;
    U8* out;
    S32 len;
    union
    {
        U8* b;
        void* v;
    } fbuffer;
    U8*  buf;
    
    /* In this example we want to open the input file before doing anything else,
     * so that the setjmp() error recovery below can assume the file is open.
     * VERY IMPORTANT: use "b" option to fopen() if you are on a machine that
     * requires it in order to read binary files.
     */
    
    len = fileSystem->ReadFile( const_cast< UTF8* >( filename ), &fbuffer.v );
    if ( !fbuffer.b || len < 0 )
    {
        return;
    }
    
    /* Step 1: allocate and initialize JPEG decompression object */
    
    /* We have to set up the error handler first, in case the initialization
     * step fails.  (Unlikely, but it could happen if you are out of memory.)
     * This routine fills in the contents of struct jerr, and returns jerr's
     * address which we place into the link field in cinfo.
     */
    cinfo.err = jpeg_std_error( &jerr.pub );
    cinfo.err->error_exit = JPGErrorExit;
    cinfo.err->output_message = JPGOutputMessage;
    
    /* Establish the setjmp return context for R_JPGErrorExit to use. */
    if ( setjmp( jerr.setjmp_buffer ) )
    {
        /* If we get here, the JPEG code has signaled an error.
         * We need to clean up the JPEG object, close the input file, and return.
         */
        jpeg_destroy_decompress( &cinfo );
        fileSystem->FreeFile( fbuffer.v );
        
        /* Append the filename to the error for easier debugging */
        clientMainSystem->RefPrintf( PRINT_ALL, ", loading file %s\n", filename );
        return;
    }
    
    /* Now we can initialize the JPEG decompression object. */
    jpeg_create_decompress( &cinfo );
    
    /* Step 2: specify data source (eg, a file) */
    
    jpeg_mem_src( &cinfo, fbuffer.b, len );
    
    /* Step 3: read file parameters with jpeg_read_header() */
    
    ( void ) jpeg_read_header( &cinfo, TRUE );
    /* We can ignore the return value from jpeg_read_header since
     *   (a) suspension is not possible with the stdio data source, and
     *   (b) we passed TRUE to reject a tables-only JPEG file as an error.
     * See libjpeg.doc for more info.
     */
    
    /* Step 4: set parameters for decompression */
    
    /*
     * Make sure it always converts images to RGB color space. This will
     * automatically convert 8-bit greyscale images to RGB as well.
     */
    cinfo.out_color_space = JCS_RGB;
    
    /* Step 5: Start decompressor */
    
    ( void ) jpeg_start_decompress( &cinfo );
    /* We can ignore the return value since suspension is not possible
     * with the stdio data source.
     */
    
    /* We may need to do some setup of our own at this point before reading
     * the data.  After jpeg_start_decompress() we have the correct scaled
     * output image dimensions available, as well as the output colormap
     * if we asked for color quantization.
     * In this example, we need to make an output work buffer of the right size.
     */
    /* JSAMPLEs per row in output buffer */
    
    pixelcount = cinfo.output_width * cinfo.output_height;
    
    if ( !cinfo.output_width || !cinfo.output_height || ( ( pixelcount * 4 ) / cinfo.output_width ) / 4 != cinfo.output_height
            || pixelcount > 0x1FFFFFFF || cinfo.output_components != 3
       )
    {
        // Free the memory to make sure we don't leak memory
        fileSystem->FreeFile( fbuffer.v );
        jpeg_destroy_decompress( &cinfo );
        
        Com_Error( ERR_DROP, "LoadJPG: %s has an invalid image format: %dx%d*4=%d, components: %d", filename,
                   cinfo.output_width, cinfo.output_height, pixelcount * 4, cinfo.output_components );
    }
    
    memcount = pixelcount * 4;
    row_stride = cinfo.output_width * cinfo.output_components;
    
    out = ( U8* )clientMainSystem->RefMalloc( memcount );
    
    *width = cinfo.output_width;
    *height = cinfo.output_height;
    
    /* Step 6: while (scan lines remain to be read) */
    /*           jpeg_read_scanlines(...); */
    
    /* Here we use the library's state variable cinfo.output_scanline as the
     * loop counter, so that we don't have to keep track ourselves.
     */
    while ( cinfo.output_scanline < cinfo.output_height )
    {
        /* jpeg_read_scanlines expects an array of pointers to scanlines.
         * Here the array is only one element long, but you could ask for
         * more than one scanline at a time if that's more convenient.
         */
        buf = ( ( out + ( row_stride * cinfo.output_scanline ) ) );
        buffer = &buf;
        ( void ) jpeg_read_scanlines( &cinfo, buffer, 1 );
    }
    
    buf = out;
    
    // Expand from RGB to RGBA
    sindex = pixelcount * cinfo.output_components;
    dindex = memcount;
    
    do
    {
        buf[--dindex] = 255;
        buf[--dindex] = buf[--sindex];
        buf[--dindex] = buf[--sindex];
        buf[--dindex] = buf[--sindex];
    }
    while ( sindex );
    
    *pic = out;
    
    /* Step 7: Finish decompression */
    
    jpeg_finish_decompress( &cinfo );
    /* We can ignore the return value since suspension is not possible
     * with the stdio data source.
     */
    
    /* Step 8: Release JPEG decompression object */
    
    /* This is an important step since it will release a good deal of memory. */
    jpeg_destroy_decompress( &cinfo );
    
    /* After finish_decompress, we can close the input file.
     * Here we postpone it until after no more JPEG errors are possible,
     * so as to simplify the setjmp error logic above.  (Actually, I don't
     * think that jpeg_destroy can do an error exit, but why assume anything...)
     */
    fileSystem->FreeFile( fbuffer.v );
    
    /* At this point you may want to check to see whether any corrupt-data
     * warnings occurred (test whether jerr.pub.num_warnings is nonzero).
     */
    
    /* And we're done! */
}


/*
 * Initialize destination --- called by jpeg_start_compress
 * before any data is actually written.
 */

void idRenderSystemImageJPEGLocal::voidinit_destination( j_compress_ptr cinfo )
{
    my_dest_ptr dest = ( my_dest_ptr ) cinfo->dest;
    
    dest->pub.next_output_byte = dest->outfile;
    dest->pub.free_in_buffer = dest->size;
}

/*
 * Empty the output buffer --- called whenever buffer fills up.
 *
 * In typical applications, this should write the entire output buffer
 * (ignoring the current state of next_output_byte & free_in_buffer),
 * reset the pointer & count to the start of the buffer, and return TRUE
 * indicating that the buffer has been dumped.
 *
 * In applications that need to be able to suspend compression due to output
 * overrun, a FALSE return indicates that the buffer cannot be emptied now.
 * In this situation, the compressor will return to its caller (possibly with
 * an indication that it has not accepted all the supplied scanlines).  The
 * application should resume compression after it has made more room in the
 * output buffer.  Note that there are substantial restrictions on the use of
 * suspension --- see the documentation.
 *
 * When suspending, the compressor will back up to a convenient restart point
 * (typically the start of the current MCU). next_output_byte & free_in_buffer
 * indicate where the restart point will be if the current call returns FALSE.
 * Data beyond this point will be regenerated after resumption, so do not
 * write it out when emptying the buffer externally.
 */

boolean idRenderSystemImageJPEGLocal::empty_output_buffer( j_compress_ptr cinfo )
{
    my_dest_ptr dest = ( my_dest_ptr ) cinfo->dest;
    
    jpeg_destroy_compress( cinfo );
    
    // Make crash fatal or we would probably leak memory.
    Com_Error( ERR_FATAL, "Output buffer for encoded JPEG image has insufficient size of %d bytes", dest->size );
    
    return FALSE;
}

/*
 * Terminate destination --- called by jpeg_finish_compress
 * after all data has been written.  Usually needs to flush buffer.
 *
 * NB: *not* called by jpeg_abort or jpeg_destroy; surrounding
 * application must deal with any cleanup that should happen even
 * for error exit.
 */

void idRenderSystemImageJPEGLocal::term_destination( j_compress_ptr cinfo )
{
}

/*
 * Prepare for output to a stdio stream.
 * The caller must have already opened the stream, and is responsible
 * for closing it after finishing compression.
 */

void idRenderSystemImageJPEGLocal::jpegDest( j_compress_ptr cinfo, U8* outfile, S32 size )
{
    my_dest_ptr dest;
    
    /* The destination object is made permanent so that multiple JPEG images
     * can be written to the same file without re-executing jpeg_stdio_dest.
     * This makes it dangerous to use this manager and a different destination
     * manager serially with the same JPEG object, because their private object
     * sizes may be different.  Caveat programmer.
     */
    if ( cinfo->dest == NULL )  	/* first time for this JPEG object? */
    {
        cinfo->dest = ( struct jpeg_destination_mgr* )
                      ( *cinfo->mem->alloc_small )( ( j_common_ptr ) cinfo, JPOOL_PERMANENT,
                              sizeof( my_destination_mgr ) );
    }
    
    dest = ( my_dest_ptr ) cinfo->dest;
    dest->pub.init_destination = voidinit_destination;
    dest->pub.empty_output_buffer = empty_output_buffer;
    dest->pub.term_destination = term_destination;
    dest->outfile = outfile;
    dest->size = size;
}

/*
=================
idRenderSystemImageJPEGLocal::SaveJPGToBuffer

Encodes JPEG from image in image_buffer and writes to buffer.
Expects RGB input data
=================
*/
size_t idRenderSystemImageJPEGLocal::SaveJPGToBuffer( U8* buffer, U64 bufSize, S32 quality, S32 image_width, S32 image_height, U8* image_buffer, S32 padding )
{
    size_t outcount;
    
    struct jpeg_compress_struct cinfo;
    q_jpeg_error_mgr_t jerr;
    /* pointer to JSAMPLE row[s] */
    JSAMPROW row_pointer[1];
    my_dest_ptr dest;
    /* physical row width in image buffer */
    S32 row_stride;
    
    /* Step 1: allocate and initialize JPEG compression object */
    cinfo.err = jpeg_std_error( &jerr.pub );
    cinfo.err->error_exit = JPGErrorExit;
    cinfo.err->output_message = JPGOutputMessage;
    
    /* Establish the setjmp return context for R_JPGErrorExit to use. */
    if ( setjmp( jerr.setjmp_buffer ) )
    {
        /* If we get here, the JPEG code has signaled an error.
         * We need to clean up the JPEG object and return.
         */
        jpeg_destroy_compress( &cinfo );
        
        clientMainSystem->RefPrintf( PRINT_ALL, "\n" );
        return 0;
    }
    
    /* Now we can initialize the JPEG compression object. */
    jpeg_create_compress( &cinfo );
    
    /* Step 2: specify data destination (eg, a file) */
    /* Note: steps 2 and 3 can be done in either order. */
    jpegDest( &cinfo, buffer, bufSize );
    
    /* Step 3: set parameters for compression */
    /* image width and height, in pixels */
    cinfo.image_width = image_width;
    cinfo.image_height = image_height;
    /* # of color components per pixel */
    cinfo.input_components = 3;
    /* colorspace of input image */
    cinfo.in_color_space = JCS_RGB;
    
    jpeg_set_defaults( &cinfo );
    jpeg_set_quality( &cinfo, quality, TRUE /* limit to baseline-JPEG values */ );
    
    /* If quality is set high, disable chroma subsampling */
    if ( quality >= 85 )
    {
        cinfo.comp_info[0].h_samp_factor = 1;
        cinfo.comp_info[0].v_samp_factor = 1;
    }
    
    /* Step 4: Start compressor */
    jpeg_start_compress( &cinfo, TRUE );
    
    /* Step 5: while (scan lines remain to be written) */
    /*           jpeg_write_scanlines(...); */
    row_stride = image_width * cinfo.input_components + padding; /* JSAMPLEs per row in image_buffer */
    
    while ( cinfo.next_scanline < cinfo.image_height )
    {
        /* jpeg_write_scanlines expects an array of pointers to scanlines.
         * Here the array is only one element long, but you could pass
         * more than one scanline at a time if that's more convenient.
         */
        row_pointer[0] = &image_buffer[( ( cinfo.image_height - 1 ) * row_stride ) - cinfo.next_scanline * row_stride];
        ( void ) jpeg_write_scanlines( &cinfo, row_pointer, 1 );
    }
    
    /* Step 6: Finish compression */
    jpeg_finish_compress( &cinfo );
    
    dest = ( my_dest_ptr ) cinfo.dest;
    outcount = dest->size - dest->pub.free_in_buffer;
    
    /* Step 7: release JPEG compression object */
    jpeg_destroy_compress( &cinfo );
    
    /* And we're done! */
    return outcount;
}

void idRenderSystemImageJPEGLocal::SaveJPG( UTF8* filename, S32 quality, S32 image_width, S32 image_height, U8* image_buffer, S32 padding )
{
    U8* out = nullptr;
    size_t bufSize;
    
    bufSize = image_width * image_height * 3;
    out = ( U8* )memorySystem->AllocateTempMemory( bufSize );
    
    bufSize = SaveJPGToBuffer( out, ( U64 )bufSize, quality, image_width, image_height, image_buffer, padding );
    fileSystem->WriteFile( filename, out, ( S32 )bufSize );
    
    memorySystem->FreeTempMemory( out );
}
