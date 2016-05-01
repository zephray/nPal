//
// Copyright (c) 2009, Wei Mingzhi <whistler_wmz@users.sf.net>.
// Portions Copyright (c) 2004, Pierre-Marie Baty.
// Portions Copyright (c) 2009, netwan.
//
// All rights reserved.
//
// This file is part of SDLPAL.
//
// SDLPAL is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//

#include "util.h"

#ifdef PAL_HAS_NATIVEMIDI
#include "midi.h"
#endif

#define INT_MAX 2147483647

#define UTIL_ASSERT(X)          //util_assert_out( (unsigned int)(X),(unsigned int) __LINE__ ,(const char * )__MODULE__)

#define BIT( X )         ( 1 << ( X ) )

#define util_isdigit( X ) (  (  ( (int)(X) >= '0' ) && ( ( int )(X) <= '9' ) ) ? 1 : 0   )

#define util_ishex( X )     (    ( ( (int)(X) >= '0' ) && ( ( int )(X) <= '9' ) ) \
                        || ( ( (int)(X) >= 'a' ) && ( ( int )(X) <= 'f' ) ) \
                       || ( ( (int)(X) >= 'A' ) && ( ( int )(X) <= 'F' ) ) ?  1 : 0   )


static unsigned char c_util_touppertable[] = 
{
 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f,
 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27,
 0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f,
 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,
 0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0x3e, 0x3f,
 0x40, 'A',  'B',  'C',  'D',  'E',  'F',  'G',
 'H',  'I',  'J',  'K',  'L',  'M',  'N',  'O',
 'P',  'Q',  'R',  'S',  'T',  'U',  'V',  'W',
 'X',  'Y',  'Z', 0x5b, 0x5c, 0x5d, 0x5e, 0x5f,
 0x60, 'A',  'B',  'C',  'D',  'E',  'F',  'G',
 'H',  'I',  'J',  'K',  'L',  'M',  'N',  'O',
 'P',  'Q',  'R',  'S',  'T',  'U',  'V',  'W',
 'X',  'Y',  'Z', 0x7b, 0x7c, 0x7d, 0x7e, 0x7f,
 0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87,
 0x88, 0x89, 0x8a, 0x8b, 0x8c, 0x8d, 0x8e, 0x8f,
 0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97,
 0x98, 0x99, 0x9a, 0x9b, 0x9c, 0x9d, 0x9e, 0x9f,
 0xa0, 0xa1, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7,
 0xa8, 0xa9, 0xaa, 0xab, 0xac, 0xad, 0xae, 0xaf,
 0xb0, 0xb1, 0xb2, 0xb3, 0xb4, 0xb5, 0xb6, 0xb7,
 0xb8, 0xb9, 0xba, 0xbb, 0xbc, 0xbd, 0xbe, 0xbf,
 0xc0, 0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7,
 0xc8, 0xc9, 0xca, 0xcb, 0xcc, 0xcd, 0xce, 0xcf,
 0xd0, 0xd1, 0xd2, 0xd3, 0xd4, 0xd5, 0xd6, 0xd7,
 0xd8, 0xd9, 0xda, 0xdb, 0xdc, 0xdd, 0xde, 0xdf,
 0xe0, 0xe1, 0xe2, 0xe3, 0xe4, 0xe5, 0xe6, 0xe7,
 0xe8, 0xe9, 0xea, 0xeb, 0xec, 0xed, 0xee, 0xef,
 0xf0, 0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7,
 0xf8, 0xf9, 0xfa, 0xfb, 0xfc, 0xfd, 0xfe, 0xff
};

#define CONVERT_SIGN          BIT( 0 )
#define CONVERT_SIGN_FORCED     BIT( 1 )
#define CONVERT_LONG          BIT( 2 )
#define CONVERT_LONGLONG       BIT( 3 )
#define CONVERT_LEADING_ZERO    BIT( 4 )
#define CONVERT_LEFT_ADJUST     BIT( 5 )
#define CONVERT_WITH_LIMIT     BIT( 6 )
#define CONVERT_UPPERCASE_IND    BIT( 7 )
#define CONVERT_HEX_HEAD       BIT( 8 )
#define CONVERT_SHORT                    BIT(9) // for"%h"  'h' limit symbol
 

#define CONVERT_ITYPE_CONVERT      BIT( 9 )  //for "%i"

#define CONVERT_START              BIT( 31 )

static char radix_table[][37] = 
         { 
         "0123456789abcdefghijklmnopqrstuvwxyz",
         "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"
         };

typedef union
 {
#ifdef _LONG_LONG_SP_
 ULLONG     m_ulldat;
 ULLONG  *  m_pulldat;
 SLLONG     m_slldat; 
 SLLONG  *  m_pslldat;
#endif 
 float     m_ftdat;
    float  *  m_pftdat;
    double    m_dbdat;
    double *  m_pdbdat;
 ULONG      m_uldat;
 ULONG   *  m_puldat;
 long     m_sldat;
 long   *  m_psldat; 
 unsigned int        m_u32dat;
 unsigned int     *  m_pu32dat; 
 int     m_s32dat;
 int     *  m_ps32dat;    
 char      *  m_ps8dat;
} TYPE_NUM_T;

 

static TYPE_NUM_T  g_type_data;

void
trim(
   char *str
)
/*++
  Purpose:

    Remove the leading and trailing spaces in a string.

  Parameters:

    str - the string to proceed.

  Return value:

    None.

--*/
{
   int pos = 0;
   char *dest = str;

   //
   // skip leading blanks
   //
   while (str[pos] <= ' ' && str[pos] > 0)
      pos++;

   while (str[pos])
   {
      *(dest++) = str[pos];
      pos++;
   }

   *(dest--) = '\0'; // store the null

   //
   // remove trailing blanks
   //
   while (dest >= str && *dest <= ' ' && *dest > 0)
      *(dest--) = '\0';
}

char *
va(
   const char *format,
   ...
)
/*++
  Purpose:

    Does a varargs printf into a temp buffer, so we don't need to have
    varargs versions of all text functions.

  Parameters:

    format - the format string.

  Return value:

    Pointer to the result string.

--*/
{
   static char string[256];
   va_list     argptr;

   va_start(argptr, format);
   vsprintf(string, format, argptr);
   va_end(argptr);

   return string;
}

/*
 * RNG code based on RACC by Pierre-Marie Baty.
 * http://racc.bots-united.com
 *
 * Copyright (c) 2004, Pierre-Marie Baty
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or
 * without modification, are permitted provided that the following
 * conditions are met:
 *
 * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in
 * the documentation and/or other materials provided with the
 * distribution.
 *
 * Neither the name of the RACC nor the names of its contributors
 * may be used to endorse or promote products derived from this
 * software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
 * CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 * TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

//
// Our random number generator's seed.
//
static int glSeed = 0;

static void
lsrand(
   unsigned int iInitialSeed
)
/*++
  Purpose:

    This function initializes the random seed based on the initial seed value passed in the
    iInitialSeed parameter.

  Parameters:

    [IN]  iInitialSeed - The initial random seed.

  Return value:

    None.

--*/
{
   //
   // fill in the initial seed of the random number generator
   //
   glSeed = 1664525L * iInitialSeed + 1013904223L;
}

static int
lrand(
   void
)
/*++
  Purpose:

    This function is the equivalent of the rand() standard C library function, except that
    whereas rand() works only with short integers (i.e. not above 32767), this function is
    able to generate 32-bit random numbers.

  Parameters:

    None.

  Return value:

    The generated random number.

--*/
{
   if (glSeed == 0) // if the random seed isn't initialized...
      lsrand((unsigned int)SDL_GetTicks()); // initialize it first
   glSeed = 1664525L * glSeed + 1013904223L; // do some twisted math (infinite suite)
   return ((glSeed >> 1) + 1073741824L); // and return the result.
}

int
RandomLong(
   int from,
   int to
)
/*++
  Purpose:

    This function returns a random integer number between (and including) the starting and
    ending values passed by parameters from and to.

  Parameters:

    from - the starting value.

    to - the ending value.

  Return value:

    The generated random number.

--*/
{
   if (to <= from)
      return from;

   return from + lrand() / (INT_MAX / (to - from + 1));
}

float
RandomFloat(
   float from,
   float to
)
/*++
  Purpose:

    This function returns a random floating-point number between (and including) the starting
    and ending values passed by parameters from and to.

  Parameters:

    from - the starting value.

    to - the ending value.

  Return value:

    The generated random number.

--*/
{
   if (to <= from)
      return from;

   return from + (float)lrand() / (INT_MAX / (to - from));
}

void
UTIL_Delay(
   unsigned int ms
)
{
   unsigned int t = SDL_GetTicks() + ms;

   while (SDL_PollEvent(NULL));

   while (SDL_GetTicks() < t)
   {
      SDL_Delay(1);
      while (SDL_PollEvent(NULL));
   }

#ifdef PAL_HAS_NATIVEMIDI
   MIDI_CheckLoop();
#endif
}

void
TerminateOnError(
   const char *fmt,
   ...
)
// This function terminates the game because of an error and
// prints the message string pointed to by fmt both in the
// console and in a messagebox.
{
   va_list argptr;
   char string[256];
   extern VOID PAL_Shutdown(VOID);

   // concatenate all the arguments in one string
   va_start(argptr, fmt);
   vsprintf(string, fmt, argptr);
   va_end(argptr);


   fprintf(stderr, "\nFATAL ERROR: %s\n", string);

#ifdef _WIN32
   MessageBoxA(0, string, "FATAL ERROR", MB_ICONERROR);
#endif

#ifdef __linux__
   system(va("beep; xmessage -center \"FATAL ERROR: %s\"", string));
#endif

#ifdef NSPIRE
   show_msgbox("FATAL ERROR", string);
#endif

#if defined(__SYMBIAN32__)
   UTIL_WriteLog(LOG_DEBUG,"[0x%08x][%s][%s] - %s",(long)TerminateOnError,"TerminateOnError",__FILE__, string);
   SDL_Delay(3000);
#endif

#ifdef _DEBUG
   assert(!"TerminateOnError()"); // allows jumping to debugger
#endif


PAL_Shutdown();

#if defined (NDS)
   while (1);
#else
   exit(255);
#endif
}

void *
UTIL_malloc(
   size_t               buffer_size
)
{
   // handy wrapper for operations we always forget, like checking malloc's returned pointer.

   void *buffer;

   // first off, check if buffer size is valid
   if (buffer_size == 0)
      TerminateOnError("UTIL_malloc() called with invalid buffer size: %d\n", buffer_size);

   buffer = malloc(buffer_size); // allocate real memory space

   // last check, check if malloc call succeeded
   if (buffer == NULL)
      TerminateOnError("UTIL_malloc() failure for %d bytes (out of memory?)\n", buffer_size);

   return buffer; // nothing went wrong, so return buffer pointer
}

void *
UTIL_calloc(
   size_t               n,
   size_t               size
)
{
   // handy wrapper for operations we always forget, like checking calloc's returned pointer.

   void *buffer;

   // first off, check if buffer size is valid
   if (n == 0 || size == 0)
      TerminateOnError ("UTIL_calloc() called with invalid parameters\n");

   buffer = calloc(n, size); // allocate real memory space

   // last check, check if malloc call succeeded
   if (buffer == NULL)
      TerminateOnError("UTIL_calloc() failure for %d bytes (out of memory?)\n", size * n);

   return buffer; // nothing went wrong, so return buffer pointer
}

FILE *
UTIL_OpenRequiredFile(
   LPCSTR            lpszFileName
)
/*++
  Purpose:

    Open a required file. If fails, quit the program.

  Parameters:

    [IN]  lpszFileName - file name to open.

  Return value:

    Pointer to the file.

--*/
{
   FILE         *fp;

   fp = fopen(va("%s%s", PAL_PREFIX, lpszFileName), "rb");
	
   if (fp == NULL)
   {
      TerminateOnError("File not found: %s!\n", va("%s%s", PAL_PREFIX, lpszFileName));
   }
#ifdef ENABLE_LOG
   else
   {
      printf("Successfully opened file: %s\n", va("%s%s", PAL_PREFIX, lpszFileName));
   }
#endif

   return fp;
}

VOID
UTIL_CloseFile(
   FILE             *fp
)
/*++
  Purpose:

    Close a file.

  Parameters:

    [IN]  fp - file handle to be closed.

  Return value:

    None.

--*/
{
   if (fp != NULL)
   {
      fclose(fp);
   }
}

#ifdef ENABLE_LOG

static FILE *pLogFile = NULL;

FILE *
UTIL_OpenLog(
   VOID
)
{
   if ((pLogFile = fopen(_PATH_LOG, "a+")) == NULL)
   {
      return NULL;
   }

   return pLogFile;
}

VOID
UTIL_CloseLog(
   VOID
)
{
   if (pLogFile != NULL)
   {
      fclose(pLogFile);
   }
}

VOID
UTIL_WriteLog(
   int             Priority,
   const char     *Fmt,
   ...
)
{
   va_list       vaa;
   time_t        lTime;
   struct tm    *curTime;
   char          szDateBuf[260];

   time(&lTime);

   if ((Priority < LOG_EMERG) || (Priority >= LOG_LAST_PRIORITY))
   {
      return;
   }

   curTime = localtime(&lTime);
   strftime(szDateBuf, 128, "%Y-%m-%d   %H:%M:%S", curTime);
   szDateBuf[strlen(szDateBuf) - 1] = '\0'; //remove the

   va_start(vaa,Fmt);

   fprintf(pLogFile, "[%s]", szDateBuf);
   vfprintf(pLogFile, Fmt, vaa);
   fprintf(pLogFile, "\n");
   fflush(pLogFile);

   va_end(vaa);
}

#endif