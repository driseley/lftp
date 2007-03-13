/*
 * lftp and utils
 *
 * Copyright (c) 1996-2007 by Alexander V. Lukyanov (lav@yars.free.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef XSTRING_H
#define XSTRING_H

/* Declare string and memory handling routines.  Take care that an ANSI
   string.h and pre-ANSI memory.h might conflict, and that memory.h and
   strings.h conflict on some systems.  */

#if STDC_HEADERS || HAVE_STRING_H
# include <string.h>
# if !STDC_HEADERS && HAVE_MEMORY_H
#  include <memory.h>
# endif
#else
# include <strings.h>
# ifndef strchr
#  define strchr index
# endif
# ifndef strrchr
#  define strrchr rindex
# endif
# ifndef memcpy
#  define memcpy(Dst, Src, Num) bcopy (Src, Dst, Num)
# endif
# ifndef memcmp
#  define memcmp(Src1, Src2, Num) bcmp (Src1, Src2, Num)
# endif
#endif

#include <stdarg.h>

#if !HAVE_DECL_STRCASECMP
CDECL int strcasecmp(const char *s1,const char *s2);
CDECL int strncasecmp(const char *s1,const char *s2,size_t n);
#endif

#if !HAVE_DECL_VSNPRINTF
CDECL int vsnprintf(char *,size_t,const char *,va_list);
#endif
#if !HAVE_DECL_SNPRINTF
CDECL int snprintf(char *,size_t,const char *,...);
#endif

static inline int xstrcmp(const char *s1,const char *s2)
{
   if(s1==s2)
      return 0;
   if(s1==0 || s2==0)
      return 1;
   return strcmp(s1,s2);
}
static inline int xstrncmp(const char *s1,const char *s2,size_t len)
{
   if(s1==s2 || len==0)
      return 0;
   if(s1==0 || s2==0)
      return 1;
   return strncmp(s1,s2,len);
}
static inline int xstrcasecmp(const char *s1,const char *s2)
{
   if(s1==s2)
      return 0;
   if(s1==0 || s2==0)
      return 1;
   return strcasecmp(s1,s2);
}
static inline size_t xstrlen(const char *s)
{
   if(s==0)
      return 0;
   return strlen(s);
}

#include <stdarg.h>
#include "xmalloc.h"

/* this is a small and fast dynamic string class */
/* mostly used as xstrdup/xfree replacement */

class xstring
{
   char *buf;
   size_t size;
   void get_space(size_t s);
   // make xstring = xstrdup() fail:
   xstring& operator=(char *);
   const char *operator=(const char *s) { return set(s); }
   const char *operator=(const xstring& s) { return set(s.get()); }
   void init(const char *s);
public:
   xstring() { buf=0; size=0; }
   ~xstring() { xfree(buf); }
   xstring(const xstring &s) { init(s); }
   xstring(const char *s) { init(s); }
   operator const char *() const { return buf; }
   const char *get() const { return buf; }
   char *get_non_const() { return buf; }
   const char *set(const char *s);
   const char *nset(const char *s,int len);
   const char *set_allocated(char *s);
   const char *append(const char *s);
   static size_t vstrlen(va_list);
   const char *vappend(va_list);
   const char *vappend(...) __attribute__((sentinel));
   const char *vset(...) __attribute__((sentinel));
   void truncate(size_t n);
   void truncate_at(char c);
};

#endif//XSTRING_H
