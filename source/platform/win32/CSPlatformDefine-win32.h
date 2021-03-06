/****************************************************************************
Copyright (c) 2014 PauT

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/
#ifndef __CSPLATFORMDEFINE_H__
#define __CSPLATFORMDEFINE_H__

#include "platform/CSPlatformConfig.h"
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32

#ifdef __MINGW32__
#include <string.h>
#endif

#if defined(CC_STATIC)
    #define CS_DLL
#else
#if defined(_USRDLL)
    #define CS_DLL     __declspec(dllexport)
#else         /* use a DLL library */
    #define CS_DLL     __declspec(dllimport)
#endif
#endif

#include <assert.h>

#if CC_DISABLE_ASSERT > 0
#define CS_ASSERT(cond)
#else
#define CS_ASSERT(cond)    assert(cond)
#endif
#define CS_UNUSED_PARAM(unusedparam) (void)unusedparam

/* Define NULL pointer value */
#ifndef NULL
#ifdef __cplusplus
#define NULL    0
#else
#define NULL    ((void *)0)
#endif
#endif

#endif //s CC_TARGET_PLATFORM == CC_PLATFORM_WIN32

#endif /* __CCPLATFORMDEFINE_H__*/
