#ifndef __TIMEVAL_H
#define __TIMEVAL_H
/***************************************************************************
 *                                  _   _ ____  _
 *  Project                     ___| | | |  _ \| |
 *                             / __| | | | |_) | |
 *                            | (__| |_| |  _ <| |___
 *                             \___|\___/|_| \_\_____|
 *
 * Copyright (C) 1998 - 2004, Daniel Stenberg, <daniel@haxx.se>, et al.
 *
 * This software is licensed as described in the file COPYING, which
 * you should have received as part of this distribution. The terms
 * are also available at http://curl.haxx.se/docs/copyright.html.
 *
 * You may opt to use, copy, modify, merge, publish, distribute and/or sell
 * copies of the Software, and permit persons to whom the Software is
 * furnished to do so, under the terms of the COPYING file.
 *
 * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
 * KIND, either express or implied.
 *
 * $Id: timeval.h,v 1.18 2004/06/02 11:36:07 bagder Exp $
 ***************************************************************************/

/*
 * CAUTION: this header is designed to work when included by the app-side
 * as well as the library. Do not mix with library internals!
 */

#include "setup.h"

#if defined( WIN32 ) && !defined( __GNUC__ ) || defined( __MINGW32__ )
#include <time.h>
#else
#include <sys/time.h>
#endif

#ifndef HAVE_GETTIMEOFDAY
#if !defined( _WINSOCKAPI_ ) && !defined( __MINGW32__ ) && !defined( _AMIGASF ) && \
	!defined( __LCC__ )
struct timeval {
	long tv_sec;
	long tv_usec;
};
#endif
#endif

struct timeval curlx_tvnow( void );

/*
 * Make sure that the first argument (t1) is the more recent time and t2 is
 * the older time, as otherwise you get a weird negative time-diff back...
 *
 * Returns: the time difference in number of milliseconds.
 */
long curlx_tvdiff( struct timeval t1, struct timeval t2 );

/*
 * Same as curlx_tvdiff but with full usec resolution.
 *
 * Returns: the time difference in seconds with subsecond resolution.
 */
double curlx_tvdiff_secs( struct timeval t1, struct timeval t2 );

long Curl_tvlong( struct timeval t1 );

/* These two defines below exist to provide the older API for library
   internals only. */
#define Curl_tvnow() curlx_tvnow()
#define Curl_tvdiff( x,y ) curlx_tvdiff( x,y )
#define Curl_tvdiff_secs( x,y ) curlx_tvdiff_secs( x,y )

#endif
