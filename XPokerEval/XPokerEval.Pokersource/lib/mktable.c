/*
 *  Copyright 2006 Michael Maurer <mjmaurer@yahoo.com>, 
 *                 Brian Goetz <brian@quiotix.com>, 
 *                 Loic Dachary <loic@dachary.org>, 
 *                 Tim Showalter <tjs@psaux.com>
 *
 *  This package is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; version 2 dated June, 1991.
 *
 *  This package is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this package; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston,
 *  MA 02110-1301, USA.
 */
#include <stdio.h>
#include <string.h>

#include "poker_defs.h"
#include "mktable.h"


static const char *gTableName, *gFileName, *gTableType;
static int gTableSize;
static int gInUse = 0, gHeaderDone=0, gLineCount;
static FILE *gCFile;

void
MakeTable_begin(const char *tableName,
                const char *fileName, 
                const char *tableType,
                int tableSize) {
  char fnBuf[128];

  if (gInUse) 
    fprintf(stderr, "MakeTable_begin called before previous call to _end\n");
  gTableName = tableName;
  gFileName  = fileName;
  gTableType = tableType;
  gTableSize = tableSize;
  gInUse = 1;
  gLineCount = 0;
  gHeaderDone = 0;

  strcpy(fnBuf, fileName);
  strcat(fnBuf, ".c");
  gCFile = fopen(fnBuf, "w");
  if (!gCFile) 
    fprintf(stderr, "Could not open %s for writing\n", fnBuf);
  else {
    fprintf(gCFile, "#include \"poker_defs.h\"\n\n");
    fprintf(gCFile, "/* \n * Table %s\n */\n\n", gTableName);
  };
}


static void 
outputHeader(void) {
  if (gCFile) {
    fprintf(gCFile, "%s %s[%d] = { \n", gTableType, gTableName, gTableSize);
  };
  gHeaderDone = 1;
}

void
MakeTable_comment(const char *commentString) {
  if (!gInUse) 
    fprintf(stderr, "MakeTable_comment called before _begin\n");
  else if (gHeaderDone) 
    fprintf(stderr, "MakeTable_comment called after first table entry\n");
  else 
    if (gCFile) 
      fprintf(gCFile, "/*\n%s\n */\n\n", commentString);
}

void
MakeTable_extraCode(const char *codeString) {
  if (!gInUse) 
    fprintf(stderr, "MakeTable_extraCode called before _begin\n");
  else if (gHeaderDone) 
    fprintf(stderr, "MakeTable_extraCode called after first table entry\n");
  else 
    if (gCFile) 
      fprintf(gCFile, "%s \n", codeString);
}

void 
MakeTable_outputString(const char *string) {
  if (!gHeaderDone)
    outputHeader();
  ++gLineCount;
  if (gCFile)
    fprintf(gCFile, "     %s %s\n", string,
            (gLineCount == gTableSize ? "" : ","));
}

#ifdef USE_INT64
void 
MakeTable_outputUInt64(uint64 arg) {
  uint32 high, low;
  char buf[80];

  high = arg >> 32;
  low  = (uint32) arg;
#if defined(MSDOS)
  sprintf(buf, " { 0x%08x%08x } ", high, low);
#else
#if defined(WIN32) && !defined(__MINGW32__)
  sprintf(buf, " { 0x%08x%08xi64 } ", high, low);
#else
  sprintf(buf, " { 0x%08x%08xLL } ", high, low);
#endif
#endif
  MakeTable_outputString(buf);
}
#endif

void 
MakeTable_outputUInt32(uint32 arg) {
  char buf[80];
  sprintf(buf, "0x%08x", arg);
  MakeTable_outputString(buf);
}

void 
MakeTable_outputUInt16(uint16 arg) {
  char buf[80];
  sprintf(buf, "0x%04x", arg);
  MakeTable_outputString(buf);
}

void 
MakeTable_outputUInt8(uint8 arg) {
  char buf[80];
  sprintf(buf, "0x%02x", arg);
  MakeTable_outputString(buf);
}

void 
MakeTable_end(void) {
  if (gCFile) 
    fprintf(gCFile, "};\n");
  fclose(gCFile);

  gInUse = 0;
}
    

