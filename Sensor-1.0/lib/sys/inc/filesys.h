/*===========================================================================
 *
 *  File system library
 *
 *  Marusenkov S.A., "ostfriesland@mail.ru", 2016-2019
 *
 * THIS SOURCE CODE IS PROVIDED AS IS WITH NO WARRANTIES OF ANY KIND
 * INCLUDING THE WARRANTIES OF DESIGN, MERCHANTIBILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE, OR ARISING FROM A COURSE OF DEALING, USAGE OR
 * TRADE PRACTICE.
 *
 *---------------------------------------------------------------------------
 *
 *  File filesys.h: File system definitions and functions prototypes.
 *
 *===========================================================================*/

#ifndef __FILESYS_H
#define __FILESYS_H

#include <globaldef.h>
#include <stdlib.h>
#include <stdcf.h>
#include "../../bastypes.h"
#include "../../std/inc/DateTime.h"
#include "../../std/inc/path.h"

//Enable/disable selected file system
//FAT32/16
#ifndef FS_FAT_EN
#define FS_FAT_EN					1
#endif /* !FS_FAT_EN */

//dev
#ifndef FS_DEV_EN
#define FS_DEV_EN					1
#endif /* !FS_DEV_EN */

//proc
#ifndef FS_PROC_EN
#define FS_PROC_EN					1
#endif /* !FS_PROC_EN */

//Enable applications for virtual root filesystem
#ifndef FS_VROOT_APPS_EN
#define FS_VROOT_APPS_EN			1
#endif /* !FS_VROOT_APPS_EN */

//Other file system
//#ifndef FS_x_EN
//#define FS_x_EN		1
//#endif /* !FS_x_EN */

#ifndef FS_VERSION_STRING
#define FS_VERSION_STRING			"Filesys v1.2"
#endif /* !FS_VERSION_STRING */

//Mount table size, elements
#ifndef FS_MTAB_MAX_SIZE
#define FS_MTAB_MAX_SIZE			8
#endif /* !FS_MTAB_MAX_SIZE */

//Maximum count of opened files (directories)
#ifndef FS_OPENFILES_MAX_COUNT
#define FS_OPENFILES_MAX_COUNT      10
#endif /* FS_OPENFILES_MAX_COUNT */

//Maximum path length, chars
#ifndef FS_PATH_MAX
#define FS_PATH_MAX					(256 - 64)
#endif /* !FS_PATH_MAX */

//Directory buffer size, bytes (aligned by 32 bytes)
#ifndef FS_DIR_BUF_SIZE
#define FS_DIR_BUF_SIZE				512
#endif /* !FS_DIR_BUF_SIZE */

//Internal buffer of "fs_fprintf" function
#ifndef FS_FPRINTF_BUFFER_SIZE
#define FS_FPRINTF_BUFFER_SIZE		128
#endif /* !FS_FPRINTF_BUFFER_SIZE */

//Long File Name (LFN) maximum length, chars
#define FS_LFN_MAX_LENGTH			(256 - 64)

//Count of partitions at disk
#define FS_DISK_PART_COUNT			4

//File system IDs
//Empty partition
#define FS_EMPTY_ID					0x00
//FAT-12 (logical or placed at first 32MB, otherwise used ID=0x06)
#define FS_FAT12_ID					0x01
//FAT-16 less 32MB (primary and placed at first 32MB, otherwise used ID=0x06)
#define FS_FAT16_ID					0x04
//Extended
#define FS_EXTENDED_ID				0x05
//FAT-16B
#define FS_FAT16B_ID				0x06
//HPFS/NTFS/exFAT
#define FS_NTFS_ID					0x07
//FAT-32
#define FS_FAT32_ID					0x0B
//FAT-32X (LBA)
#define FS_FAT32X_ID				0x0C
//FAT-16X (LBA, VFAT)
#define FS_FAT16X_ID				0x0E
//Extended (same as 0x05 with LBA)
#define FS_EXTENDEDX_ID				0x0F
//Linux swap, Sun Solaris
#define FS_LINUX_SWAP_ID			0x82
//Linux
#define FS_LINUX_ID					0x83
//Linux extended
#define FS_LINUX_EXT_ID				0x85

//vroot
#define FS_VROOT_ID					0xC8
//dev
#define FS_DEV_ID					0xC9
//proc
#define FS_PROC_ID					0xCA

//File system readable names
//Empty partition
#define FS_EMPTY_NAME				"Empty"
//FAT-12 (logical or placed at first 32MB, otherwise used ID=0x06)
#define FS_FAT12_NAME				"W95 FAT12"
//FAT-16 less 32MB (primary and placed at first 32MB, otherwise used ID=0x06)
#define FS_FAT16_NAME				"W95 FAT16"
//Extended
#define FS_EXTENDED_NAME			"Extended"
//FAT-16B
#define FS_FAT16B_NAME				"W95 FAT16B"
//HPFS/NTFS/exFAT
#define FS_NTFS_NAME				"HPFS/NTFS/exFAT"
//FAT-32
#define FS_FAT32_NAME				"W95 FAT32"
//FAT-32X (LBA)
#define FS_FAT32X_NAME				"W95 FAT32 Ext'd"
//FAT-16X (LBA, VFAT)
#define FS_FAT16X_NAME				"W95 FAT16 Ext'd"
//Extended (same as 0x05 with LBA)
#define FS_EXTENDEDX_NAME			"W95 Ext'd (LBA)"
//Linux swap, Sun Solaris
#define FS_LINUX_SWAP_NAME			"Linux swap / Solaris"
//Linux
#define FS_LINUX_NAME				"Linux"
//Linux extended
#define FS_LINUX_EXT_NAME			"Linux extended"

//#ifndef __mode_t_defined
//typedef u32		mode_t;
//#define __mode_t_defined
//#endif

#ifndef EOF
#define EOF (-1)
#endif /* !EOF */

//64-bit offset/size is used
typedef s64		foff_t;

//(PartConfig::status, MBRPTRecord::status)
//Active partition (bootable)
#define FS_MBRREC_ACTIVE	0x80

/* MBR Partition table record
   Offset   Description
   [00]     Status: (0x80 - active (bootable), 0x00 - inactive)
   [01..03] CHS address of first sector in partition
	 [01]   Head
	 [02]   Bits: [7..6] cylinder [9..8] bits, [5..0] sector
	 [03]   Cylinder [7..0] bits
   [04]     Partition type (file system id)
   [05..07] CHS address of last sector in partition
	 [05]   Head
	 [06]   Bits: [7..6] cylinder [9..8] bits, [5..0] sector
	 [07]   Cylinder [7..0] bits
   [08..0B] LBA of first sector in partition
   [0C..0F] Number of sectors in partition */
typedef struct tag_MBRPTRecord
{
	//Status
	u8 status;

	//CHS address of first sector in partition
	u8 fCHS[3];

	//Partition type
	u8 id;

	//CHS address of last sector in partition
	u8 lCHS[3];

	//LBA of Partition Boot Record sector number
	u32 pbr;

	//Partition size, sectors
	u32 size;
} MBRPTRecord;



/* Block device configuration */
typedef struct tag_BDevConfig
{
	//Physical block size, bytes
	u32 blockSize;

	//Read block from device
	s32 (*ReadBlock)(u32 index, u8 *data);

	//Write block to device
	s32 (*WriteBlock)(u32 index, const u8 *data);
} BDevConfig;



/* Partition configuration */
typedef struct tag_PartConfig
{	
	//Device configuration
	BDevConfig *dev;

	//Partition Boot Record (PBR) sector number
	u32 pbr;

	//Partition size, sectors
	u32 size;

	//FAT table size, sectors
	u32 fatSize;	

	//FSINFO sector number (FAT32 only)
	u16 fsinfo;

	//System size, sectors
	u16 systemSize;

	//Sector size, bytes
	u16 sectorSize;

	//Cluster size, sectors
	u8 clusterSize;

	//Count of FAT tables
	u8 fatCount;

	//Partition id
	u8 id;

	//Status
	u8 status;

	//Reserved
	u8 reserved[2];
} PartConfig;



/* Disk configuration */
typedef struct tag_HDiskConfig
{
	//Physical device configuration
	BDevConfig dev;

	//Disk identifier
	u32 id;

	//Disk size, sectors
	u32 size;

	//Disk partitions
	PartConfig part[FS_DISK_PART_COUNT];
} HDiskConfig;


typedef struct tag_FSFILE	FSFILE;
typedef struct tag_FSDIR	FSDIR;

/* */
typedef struct tag_FSFILERoutines
{
	//Open file routine
	FSFILE *(*pfopen)(const char *filename, const char *modes, FSFILE *stream);

	//Close file routine
	int     (*pfclose)(FSFILE *stream);

	//Flush routine
	int     (*pfflush)(FSFILE *stream);

	//Seek position routine
	int     (*pfseek)(FSFILE *stream, foff_t offset, int whence);

	//Tell position routine
	foff_t  (*pftell)(FSFILE *stream);

	//Read file routine
	size_t  (*pfread)(void *ptr, size_t sz, size_t n, FSFILE *stream);

	//Write file routine
	size_t (*pfwrite)(const void *ptr, size_t sz, size_t n, FSFILE *stream);

    //Create new file or directory routine
    FSFILE *(*pfcreate)(const char *filename, u32 opflags, void *opargs, FSDIR *hdir, FSFILE *stream);

    //Remove exist file or directory routine
    FSFILE *(*pfremove)(const char *filename, u32 opflags, void *opargs, FSDIR *hdir, FSFILE *stream);
} FSFILERoutines;



/* */
typedef struct tag_FSDIRRoutines
{
	FSDIR *(*popendir)(const char *dirname, FSDIR *dirstream);

	/* */
	int (*pclosedir)(FSDIR *dirstream);

	/* */
	struct fsdirent *(*preaddir)(FSDIR *dirstream, struct fsdirent *entry);

	/* */
	void (*prewinddir)(FSDIR *dirstream);

	/* */
	foff_t (*ptelldir)(FSDIR *dirstream);

	/* */
	void (*pseekdir)(FSDIR *dirstream, foff_t pos);

	/* */
	void (*pgetdtime)(DateTime *destdt, s64 *srcftm);
} FSDIRRoutines;



/* */
typedef struct tag_FSFILE
{
	//Pointer to partition that contains file
	PartConfig *pt;

	//Stream flags
	u32 flags;

	//Current offset at file, bytes
	foff_t offset;

	//File size, bytes
	foff_t size;

	//Cluster contains file descriptor
	u32 clFdNumber;

    //File descriptor cluster offset, bytes
	u32 clFdOffset;

	//Start cluster of data
	u32 clStNumber;

	//Current cluster number;
	u32 clNumber;	

	//Routines
	FSFILERoutines *rt;
} FSFILE;



/* */
typedef struct tag_FSDIR
{
	//Directory stream
	FSFILE fstream;

    //Offset at directory stream, bytes
	foff_t offset;

    //Current cluster (when directory read)
	u32 fdCluster;

    //Directory routines
	FSDIRRoutines *rt;

	//Directory buffered data
	u8 buf[FS_DIR_BUF_SIZE];

} FSDIR;



//fsdirent::d_type values
#define FS_DIRENT_DTYPE_PIPE		1
#define FS_DIRENT_DTYPE_CHDEV		2
#define FS_DIRENT_DTYPE_DIRECTORY	4
#define FS_DIRENT_DTYPE_BLDEV		6
#define FS_DIRENT_DTYPE_FILE		8
#define FS_DIRENT_DTYPE_SYMLINK		10
#define FS_DIRENT_DTYPE_VOLUMENAME  11

/* */
struct fsdirent
{	
	//File serial number
	foff_t d_ino;

	//Offset of next record, bytes
	foff_t d_off;

	//Record length, bytes
	unsigned short int d_reclen;

	//Entry type (see FS_DIRENT_DTYPE_x)
	unsigned char d_type;

	//Entry name
	char d_name[FS_LFN_MAX_LENGTH + 1];

	//Additional fields
	//access
	u16 d_access;

	//Start cluster
	u32 d_stcl;

	//Descriptor cluster
	u32 d_fdcl;

	//Modification time
	s64 d_mtime;

	//File size
	foff_t d_fsize;
};

//BSD Backwards compatibility
#define d_fileno d_ino


//Mount options (FSMTABEntry::opts)
//Read only
#define FS_MT_OPT_RDONLY		0
//Read/write
#define FS_MT_OPT_RDWR			1

/* */
typedef struct tag_FSMTABEntry
{
	//Path to mount point
	const char *dir;

	//Pointer to partition
	PartConfig *pt;

	//Options (see FS_MT_OPT_x)
	u32 opts;
} FSMTABEntry;


typedef DateTime* (*pFsGetTime)(DateTime *destdt);
typedef pFsGetTime		LpFsGetTime;

typedef FSFILE* (*pGetStdStream)();
typedef pGetStdStream	LpGetStdStream;

typedef int (*pCopyProgress)(const char *srcpath, foff_t cpcount, foff_t size);
typedef pCopyProgress   LpCopyProgress;

//(FileSysConfig::opts)
//#define FS_OPTS_x			0

//Default options
#define FS_OPTS_DEFAULT		0

/* Represents configuration of file system module */
typedef struct tag_FileSysConfig
{
	//Disk list
	HDiskConfig		**disk;

	//Options (FS_OPTS_x)
	u32				opts;

	//Get current date/time routine
	LpFsGetTime		get_time;

	//Get stdin
	LpGetStdStream	get_stdin;

	//Get stdout
	LpGetStdStream	get_stdout;

	//Get stderr
	LpGetStdStream	get_stderr;

    //Notification
    LpCopyProgress  copy_notify;
} FileSysConfig;

/*===========================================================================
 * Basic functions
 *===========================================================================*/

/* Protocol stack configuration */
extern FileSysConfig *fileSysConfig;

/* Initialize filesystem module */
int fs_initialize();

/* Apply configuration */
int fs_applyconfig();

/* Get disk partitions */
int fs_get_partitions(HDiskConfig *disk);

/* Mount partition to specified point */
int fs_mount(PartConfig *pt, const char *dir, u32 opts);

/* Remount specified point */
int fs_remount(const char *dir, u32 opts);

/* Unmount specified point */
int fs_umount(const char *dir);

/* Indicates that current directory is mount point */
u32 fs_ismount(const char *dir);

/*===========================================================================
 * File functions
 *===========================================================================*/

/* Opens the file by specified path and associates a stream with it */
FSFILE *fs_fopen(const char *filename, const char *modes, FSFILE *stream);

/* Flushes the stream and closes the underlying file descriptor */
int fs_fclose(FSFILE *stream);

/* Write of all buffered data for the given output or
   update stream via the stream's underlying write function */
int fs_fflush(FSFILE *stream);

/* Sets the file position indicator for the stream pointed
   to by stream to the beginning of the file. */
void fs_rewind(FSFILE *stream);

/* Sets the file position indicator for the stream pointed to by stream. */
int fs_fseek(FSFILE *stream, foff_t offset, int whence);

/* Obtains the current value of the file position indicator
   for the stream pointed to by stream. */
foff_t fs_ftell(FSFILE *stream);

/* Reads data from the specified stream */
size_t fs_fread(void *ptr, size_t sz, size_t n, FSFILE *stream);

/* Writes data to the specified stream */
size_t fs_fwrite(const void *ptr, size_t sz, size_t n, FSFILE *stream);

/* Writes the string s to stream, without its terminating null byte */
int fs_fputs(const char *s, FSFILE *stream);

/* Writes the character c, cast to an unsigned char, to stream */
int fs_fputc(int c, FSFILE *stream);

/* Writes the character c, cast to an unsigned char, to stream */
#define fs_putc(c, stream)		fs_fputc(c, stream)

/* Writes the string s and a trailing newline to stdout */
int fs_puts(const char *s);

/* Is equivalent to fs_fputc(c, stdout) */
int fs_putchar(int c);

/* Write output to the given output stream */
int fs_vfprintf(FSFILE *stream, const char *format, va_list argv);

/* Write output to the given output stream */
_STDCF_ATTR((__format__(__printf__, 2, 3)))
int fs_fprintf(FSFILE *stream, const char *format, ...);

/* Remove a file or directory */
int fs_remove(const char *filename);


/* Rename/move file or directory */
int fs_rename(const char *oldname, const char *newname);

/* Indicates that specified file exists */
u32 fs_isfexist(const char *filename);

/* Indicates that file or directory with specified path exists */
u32 fs_isexist(const char *filename);

/* Generate new filename (that not exist at current filesystem) */
char *fs_gen_newfilename(const char *dirpath,
                         const char *reqname,
                         char *outname);

/* Copy file or directory to specified destination path */
int fs_copy(const char *srcpath,  //source path
            const char *destdir,  //destination directory
            const char *destname, //destination name
            u32 isRewrite,        //rewrite existed file
            u32 isRecursive);     //recursive copy (for directories)

/*===========================================================================
 * Directory functions
 *===========================================================================*/

/* Opens a directory stream and returns a pointer to the directory stream */
FSDIR *fs_opendir(const char *dirname, FSDIR *dir);

/* Closes the directory stream */
int fs_closedir(FSDIR *dirstream);

/* Returns a pointer to a fsdirent structure */
struct fsdirent *fs_readdir(FSDIR *dirstream, struct fsdirent *entry);

/* Resets the position of the directory stream
   to the beginning of the directory */
void fs_rewinddir(FSDIR *dirstream);

/* Set the position of the next fs_readdir() call in the directory stream.*/
void fs_seekdir(FSDIR *dirstream, foff_t pos);

/* Return current location in directory stream */
foff_t fs_telldir(FSDIR *dirstream);

/* Get current working directory */
char *fs_getcwd(char *buffer, size_t size);

/* Get current working directory */
char *fs_getwd(char *buffer);

/* Change working directory */
int fs_chdir(const char *filename);

/* Make a directory */
int fs_mkdir(const char *dirname, u32 mode);

/* Indicates that specified directory exists */
u32 fs_isdexist(const char *dirname);

/* Indicates that two path are equals */
int fs_isequalpath(const char *path1, const char *path2);

#endif /* !__FILESYS_H */

/*===========================================================================
 * End of file filesys.h
 *===========================================================================*/
