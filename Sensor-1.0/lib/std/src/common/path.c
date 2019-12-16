/*===========================================================================
 *
 *  Standard library
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
 *  File path.c: File path functions.
 *
 *===========================================================================*/
#define _CRT_SECURE_NO_WARNINGS	

#include "../../inc/path.h"
#include "../../../bastypes.h"

#include <string.h>

/* */
char *GetDirectoryName(const char *path, char *dir)
{
	s32 i, len;

	if (!path || (len = strlen(path)) == 0)
	{
		dir[0] = 0;
		return dir;
	}

	i = len - 1;

	while(i >= 0)
	{
		if (path[i] == '\\' || path[i] == '/')
		{			
			memcpy(dir, path, i);
			dir[i] = 0;		
			break;
		}

		i--;
	}

	if (i < 0)
	{
		memcpy(dir, path, len);
		dir[len] = 0;
	}

	return dir;
}



/* */
char *GetFileName(const char *path, char *fname)
{
	s32 i, len, fnamei;	

	if (!path || (len = strlen(path)) == 0)
	{
		fname[0] = 0;
		return fname;
	}

	i      = len - 1;
	fnamei = 0;	

	while(i >= 0)
	{
		if (path[i] == '\\' || path[i] == '/')
		{
			fnamei = i + 1;
			if (fnamei < len)	
			{
				memcpy(fname, &path[fnamei], len - fnamei);
                fname[(len - fnamei)] = 0;
			}
			else			
				fname[0] = 0;					

			break;
		}

		i--;
	}

	if (i < 0)
	{
		memcpy(fname, path, len);
		fname[len] = 0;
	}

	return fname;
}



/* */
char *GetFileNameWithoutExtension(const char *path, char *fname)
{
	s32 i, len, fnamei, exti;
	u32 extFound = 0;

	if (!path || (len = strlen(path)) == 0)
	{
		fname[0] = 0;
		return fname;
	}

	i      = len - 1;
	fnamei = 0;
	exti   = len;

	while(i >= 0)
	{
		if (path[i] == '.' && !extFound)
		{
			exti = i;
			extFound = 1;			
		}
		else if (path[i] == '\\' || path[i] == '/')
		{
			fnamei = i + 1;
			if (fnamei < len)	
			{
				memcpy(fname, &path[fnamei], exti - fnamei);
                fname[(exti - fnamei)] = 0;
			}
			else			
				fname[0] = 0;					

			break;
		}

		i--;
	}

	if (i < 0)
	{
		memcpy(fname, path, exti);
		fname[exti] = 0;
	}

	return fname;
}



/* */
char *GetExtension(const char *path, char *ext)
{
	s32 i, len, exti;	

	if (!path || (len = strlen(path)) == 0)
	{
		ext[0] = 0;
		return ext;
	}

	i = len - 1;		

	while(i >= 0)
	{
		if (path[i] == '.')
		{
			exti = i + 1;			
			if (exti < len)
            {
				memcpy(ext, &path[exti], len - exti);
                ext[(len - exti)] = 0;
            }
			else
				ext[0] = 0;

			break;
		}
		else if (path[i] == '\\' || path[i] == '/')
		{
			ext[0] = 0;
			break;
		}

		i--;
	}

	//No extension
	if (i < 0)			
		ext[0] = 0;	

	return ext;
}



/* */
int HasExtension(const char *path)
{
	s32 i, len;	

	if (!path || (len = strlen(path)) == 0)			
		return 0;	

	i = len - 1;		

	while(i >= 0)
	{
		if (path[i] == '.')			
			return ((i + 1) < len) ? 1 : 0;
		else if (path[i] == '\\' || path[i] == '/')
			return 0;

		i--;
	}

	return 0;
}



/* Get normalized path (without '.', '..', '////') */
char *GetNormPath(char *outPath, const char *inPath)
{
	s32 k, n, nmlen, inPathLength;

	//Length of input buffer
	inPathLength = strlen(inPath);

	//outPath contains normalized path
	outPath[0] = 0;

	//Set counters
	nmlen  = 0;
	n = 0;
	k = 0;

	while(n < inPathLength)
    {
        if (inPath[n] == '/' || inPath[n] == '\\')
		{
			k++;
			n++;
		}

		//Skip chars before '/' or '\'
		while(n < inPathLength && inPath[n] != '/' && inPath[n] != '\\') n++;

		//Name length
		nmlen  = n - k;

		if (nmlen == 0 ||
			(nmlen == 1 && inPath[k] == '.'))
		{
			//Stay at current directory: no actions
		}
		else if (nmlen == 2 && strncmp("..", &inPath[k], 2) == 0)
		{
			//Go to parent directory: remove previous name and '/'
			s32 end = (s32)strlen(outPath);

            while(end >= 0 && outPath[end] != '/' && outPath[end] != '\\') end--;

			outPath[end] = 0;
		}
		else
		{
			s32 end = strlen(outPath);
			outPath[end] = '/';
			strncpy(&outPath[end + 1], &inPath[k], nmlen);
			outPath[end + 1 + nmlen] = 0;
		}

		k = n;
	}

	if (outPath[0] == 0)
	{
		outPath[0] = '/';
		outPath[1] = 0;
	}

	return outPath;
}

/*===========================================================================
 * End of file path.c
 *===========================================================================*/

