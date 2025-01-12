#ifndef __CURL_STRERROR_H
#define __CURL_STRERROR_H
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
 * $Id: strerror.h,v 1.3 2004/10/11 17:26:24 bagder Exp $
 ***************************************************************************/

#include "urldata.h"

const char *Curl_strerror( struct connectdata *conn, int err );

#ifdef USE_LIBIDN
const char *Curl_idn_strerror( struct connectdata *conn, int err );
#endif

#endif
