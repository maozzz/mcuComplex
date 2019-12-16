/*===========================================================================
 *
 *  Standard library.
 *
 *  Marusenkov S.A., "ostfriesland@mail.ru", 2014-2016
 *
 * THIS SOURCE CODE IS PROVIDED AS IS WITH NO WARRANTIES OF ANY KIND
 * INCLUDING THE WARRANTIES OF DESIGN, MERCHANTIBILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE, OR ARISING FROM A COURSE OF DEALING, USAGE OR
 * TRADE PRACTICE.
 *
 *---------------------------------------------------------------------------
 *
 *  File path.h: File path functions prototypes.
 *
 *===========================================================================*/

#ifndef __PATH_H
#define __PATH_H

/* Get directory name */
char *GetDirectoryName(const char *path, char *dir);

/* Get filename */
char *GetFileName(const char *path, char *fname);

/* Get filename without extension */
char *GetFileNameWithoutExtension(const char *path, char *fname);

/* Get file extension */
char *GetExtension(const char *path, char *ext);

/* Indicates that specified file path has extension  */
int HasExtension(const char *path);

/* Get normalized path (without '.', '..', '////') */
char *GetNormPath(char *outPath, const char *inPath);

#endif /* __PATH_H */

/*===========================================================================
 * End of file path.c
 *===========================================================================*/
