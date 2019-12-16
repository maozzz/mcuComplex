/*===========================================================================
 *
 *  Standard library
 *
 *  Marusenkov S.A., "ostfriesland@mail.ru", 2014
 *
 * THIS SOURCE CODE IS PROVIDED AS IS WITH NO WARRANTIES OF ANY KIND
 * INCLUDING THE WARRANTIES OF DESIGN, MERCHANTIBILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE, OR ARISING FROM A COURSE OF DEALING, USAGE OR
 * TRADE PRACTICE.
 *
 *---------------------------------------------------------------------------
 *
 *  File errors.h: Error code definitions.
 *
 *===========================================================================*/

#ifndef __ERRORS_H
#define __ERRORS_H

/*===========================================================================
 * Severity error codes: 0 to 1298.
 *===========================================================================*/

//The operation completed successfully.
#define ERROR_SUCCESS					 0L
#define NO_ERROR						 0L 
#define RESULT_OK						 0L

//The data is invalid.
#define ERROR_INVALID_DATA               13L

//Not enough storage is available to complete this operation.
#define ERROR_OUTOFMEMORY                14L

//The filename, directory name, or volume label syntax is incorrect.
#define ERROR_INVALID_NAME               123L


#ifdef __COMMENT0001
//From "/usr/include/asm-generic/errno-base.h"
#define	EPERM		 1	/* Operation not permitted */
#define	ENOENT		 2	/* No such file or directory */
#define	ESRCH		 3	/* No such process */
#define	EINTR		 4	/* Interrupted system call */
#define	EIO			 5	/* I/O error */
#define	ENXIO		 6	/* No such device or address */
#define	E2BIG		 7	/* Argument list too long */
#define	ENOEXEC		 8	/* Exec format error */
#define	EBADF		 9	/* Bad file number */
#define	ECHILD		10	/* No child processes */
#define	EAGAIN		11	/* Try again */
#define	ENOMEM		12	/* Out of memory */
#define	EACCES		13	/* Permission denied */
#define	EFAULT		14	/* Bad address */
#define	ENOTBLK		15	/* Block device required */
#define	EBUSY		16	/* Device or resource busy */
#define	EEXIST		17	/* File exists */
#define	EXDEV		18	/* Cross-device link */
#define	ENODEV		19	/* No such device */
#define	ENOTDIR		20	/* Not a directory */
#define	EISDIR		21	/* Is a directory */
#define	EINVAL		22	/* Invalid argument */
#define	ENFILE		23	/* File table overflow */
#define	EMFILE		24	/* Too many open files */
#define	ENOTTY		25	/* Not a typewriter */
#define	ETXTBSY		26	/* Text file busy */
#define	EFBIG		27	/* File too large */
#define	ENOSPC		28	/* No space left on device */
#define	ESPIPE		29	/* Illegal seek */
#define	EROFS		30	/* Read-only file system */
#define	EMLINK		31	/* Too many links */
#define	EPIPE		32	/* Broken pipe */
#define	EDOM		33	/* Math argument out of domain of func */
#define	ERANGE		34	/* Math result not representable */

//From "/usr/include/asm-generic/errno.h"
#define	EDEADLK		35	/* Resource deadlock would occur */
#define	ENAMETOOLONG	36	/* File name too long */
#define	ENOLCK		37	/* No record locks available */
#define	ENOSYS		38	/* Function not implemented */
#define	ENOTEMPTY	39	/* Directory not empty */
#define	ELOOP		40	/* Too many symbolic links encountered */
#define	EWOULDBLOCK	EAGAIN	/* Operation would block */
#define	ENOMSG		42	/* No message of desired type */
#define	EIDRM		43	/* Identifier removed */
#define	ECHRNG		44	/* Channel number out of range */
#define	EL2NSYNC	45	/* Level 2 not synchronized */
#define	EL3HLT		46	/* Level 3 halted */
#define	EL3RST		47	/* Level 3 reset */
#define	ELNRNG		48	/* Link number out of range */
#define	EUNATCH		49	/* Protocol driver not attached */
#define	ENOCSI		50	/* No CSI structure available */
#define	EL2HLT		51	/* Level 2 halted */
#define	EBADE		52	/* Invalid exchange */
#define	EBADR		53	/* Invalid request descriptor */
#define	EXFULL		54	/* Exchange full */
#define	ENOANO		55	/* No anode */
#define	EBADRQC		56	/* Invalid request code */
#define	EBADSLT		57	/* Invalid slot */

#define	EDEADLOCK	EDEADLK

#define	EBFONT		59	/* Bad font file format */
#define	ENOSTR		60	/* Device not a stream */
#define	ENODATA		61	/* No data available */
#define	ETIME		62	/* Timer expired */
#define	ENOSR		63	/* Out of streams resources */
#define	ENONET		64	/* Machine is not on the network */
#define	ENOPKG		65	/* Package not installed */
#define	EREMOTE		66	/* Object is remote */
#define	ENOLINK		67	/* Link has been severed */
#define	EADV		68	/* Advertise error */
#define	ESRMNT		69	/* Srmount error */
#define	ECOMM		70	/* Communication error on send */
#define	EPROTO		71	/* Protocol error */
#define	EMULTIHOP	72	/* Multihop attempted */
#define	EDOTDOT		73	/* RFS specific error */
#define	EBADMSG		74	/* Not a data message */
#define	EOVERFLOW	75	/* Value too large for defined data type */
#define	ENOTUNIQ	76	/* Name not unique on network */
#define	EBADFD		77	/* File descriptor in bad state */
#define	EREMCHG		78	/* Remote address changed */
#define	ELIBACC		79	/* Can not access a needed shared library */
#define	ELIBBAD		80	/* Accessing a corrupted shared library */
#define	ELIBSCN		81	/* .lib section in a.out corrupted */
#define	ELIBMAX		82	/* Attempting to link in too many shared libraries */
#define	ELIBEXEC	83	/* Cannot exec a shared library directly */
#define	EILSEQ		84	/* Illegal byte sequence */
#define	ERESTART	85	/* Interrupted system call should be restarted */
#define	ESTRPIPE	86	/* Streams pipe error */
#define	EUSERS		87	/* Too many users */
#define	ENOTSOCK	88	/* Socket operation on non-socket */
#define	EDESTADDRREQ	89	/* Destination address required */
#define	EMSGSIZE	90	/* Message too long */
#define	EPROTOTYPE	91	/* Protocol wrong type for socket */
#define	ENOPROTOOPT	92	/* Protocol not available */
#define	EPROTONOSUPPORT	93	/* Protocol not supported */
#define	ESOCKTNOSUPPORT	94	/* Socket type not supported */
#define	EOPNOTSUPP	95	/* Operation not supported on transport endpoint */
#define	EPFNOSUPPORT	96	/* Protocol family not supported */
#define	EAFNOSUPPORT	97	/* Address family not supported by protocol */
#define	EADDRINUSE	98	/* Address already in use */
#define	EADDRNOTAVAIL	99	/* Cannot assign requested address */
#define	ENETDOWN	100	/* Network is down */
#define	ENETUNREACH	101	/* Network is unreachable */
#define	ENETRESET	102	/* Network dropped connection because of reset */
#define	ECONNABORTED	103	/* Software caused connection abort */
#define	ECONNRESET	104	/* Connection reset by peer */
#define	ENOBUFS		105	/* No buffer space available */
#define	EISCONN		106	/* Transport endpoint is already connected */
#define	ENOTCONN	107	/* Transport endpoint is not connected */
#define	ESHUTDOWN	108	/* Cannot send after transport endpoint shutdown */
#define	ETOOMANYREFS	109	/* Too many references: cannot splice */
#define	ETIMEDOUT	110	/* Connection timed out */
#define	ECONNREFUSED	111	/* Connection refused */
#define	EHOSTDOWN	112	/* Host is down */
#define	EHOSTUNREACH	113	/* No route to host */
#define	EALREADY	114	/* Operation already in progress */
#define	EINPROGRESS	115	/* Operation now in progress */
#define	ESTALE		116	/* Stale NFS file handle */
#define	EUCLEAN		117	/* Structure needs cleaning */
#define	ENOTNAM		118	/* Not a XENIX named type file */
#define	ENAVAIL		119	/* No XENIX semaphores available */
#define	EISNAM		120	/* Is a named type file */
#define	EREMOTEIO	121	/* Remote I/O error */
#define	EDQUOT		122	/* Quota exceeded */

#define	ENOMEDIUM	123	/* No medium found */
#define	EMEDIUMTYPE	124	/* Wrong medium type */
#define	ECANCELED	125	/* Operation Canceled */
#define	ENOKEY		126	/* Required key not available */
#define	EKEYEXPIRED	127	/* Key has expired */
#define	EKEYREVOKED	128	/* Key has been revoked */
#define	EKEYREJECTED	129	/* Key was rejected by service */

/* for robust mutexes */
#define	EOWNERDEAD	130	/* Owner died */
#define	ENOTRECOVERABLE	131	/* State not recoverable */

#define ERFKILL		132	/* Operation not possible due to RF-kill */

#define EHWPOISON	133	/* Memory page has hardware error */

#endif


#ifdef __COMMENT0002
//From "arm-none-abi/include/sys/errno.h"
#ifndef _REENT_ONLY
#define errno (*__errno())
extern int *__errno _PARAMS ((void));
#endif

#define	EPERM 1		/* Not owner */
#define	ENOENT 2	/* No such file or directory */
#define	ESRCH 3		/* No such process */
#define	EINTR 4		/* Interrupted system call */
#define	EIO 5		/* I/O error */
#define	ENXIO 6		/* No such device or address */
#define	E2BIG 7		/* Arg list too long */
#define	ENOEXEC 8	/* Exec format error */
#define	EBADF 9		/* Bad file number */
#define	ECHILD 10	/* No children */
#define	EAGAIN 11	/* No more processes */
#define	ENOMEM 12	/* Not enough space */
#define	EACCES 13	/* Permission denied */
#define	EFAULT 14	/* Bad address */
#ifdef __LINUX_ERRNO_EXTENSIONS__
#define	ENOTBLK 15	/* Block device required */
#endif
#define	EBUSY 16	/* Device or resource busy */
#define	EEXIST 17	/* File exists */
#define	EXDEV 18	/* Cross-device link */
#define	ENODEV 19	/* No such device */
#define	ENOTDIR 20	/* Not a directory */
#define	EISDIR 21	/* Is a directory */
#define	EINVAL 22	/* Invalid argument */
#define	ENFILE 23	/* Too many open files in system */
#define	EMFILE 24	/* File descriptor value too large */
#define	ENOTTY 25	/* Not a character device */
#define	ETXTBSY 26	/* Text file busy */
#define	EFBIG 27	/* File too large */
#define	ENOSPC 28	/* No space left on device */
#define	ESPIPE 29	/* Illegal seek */
#define	EROFS 30	/* Read-only file system */
#define	EMLINK 31	/* Too many links */
#define	EPIPE 32	/* Broken pipe */
#define	EDOM 33		/* Mathematics argument out of domain of function */
#define	ERANGE 34	/* Result too large */
#define	ENOMSG 35	/* No message of desired type */
#define	EIDRM 36	/* Identifier removed */
#ifdef __LINUX_ERRNO_EXTENSIONS__
#define	ECHRNG 37	/* Channel number out of range */
#define	EL2NSYNC 38	/* Level 2 not synchronized */
#define	EL3HLT 39	/* Level 3 halted */
#define	EL3RST 40	/* Level 3 reset */
#define	ELNRNG 41	/* Link number out of range */
#define	EUNATCH 42	/* Protocol driver not attached */
#define	ENOCSI 43	/* No CSI structure available */
#define	EL2HLT 44	/* Level 2 halted */
#endif
#define	EDEADLK 45	/* Deadlock */
#define	ENOLCK 46	/* No lock */
#ifdef __LINUX_ERRNO_EXTENSIONS__
#define EBADE 50	/* Invalid exchange */
#define EBADR 51	/* Invalid request descriptor */
#define EXFULL 52	/* Exchange full */
#define ENOANO 53	/* No anode */
#define EBADRQC 54	/* Invalid request code */
#define EBADSLT 55	/* Invalid slot */
#define EDEADLOCK 56	/* File locking deadlock error */
#define EBFONT 57	/* Bad font file fmt */
#endif
#define ENOSTR 60	/* Not a stream */
#define ENODATA 61	/* No data (for no delay io) */
#define ETIME 62	/* Stream ioctl timeout */
#define ENOSR 63	/* No stream resources */
#ifdef __LINUX_ERRNO_EXTENSIONS__
#define ENONET 64	/* Machine is not on the network */
#define ENOPKG 65	/* Package not installed */
#define EREMOTE 66	/* The object is remote */
#endif
#define ENOLINK 67	/* Virtual circuit is gone */
#ifdef __LINUX_ERRNO_EXTENSIONS__
#define EADV 68		/* Advertise error */
#define ESRMNT 69	/* Srmount error */
#define	ECOMM 70	/* Communication error on send */
#endif
#define EPROTO 71	/* Protocol error */
#define	EMULTIHOP 74	/* Multihop attempted */
#ifdef __LINUX_ERRNO_EXTENSIONS__
#define	ELBIN 75	/* Inode is remote (not really error) */
#define	EDOTDOT 76	/* Cross mount point (not really error) */
#endif
#define EBADMSG 77	/* Bad message */
#define EFTYPE 79	/* Inappropriate file type or format */
#ifdef __LINUX_ERRNO_EXTENSIONS__
#define ENOTUNIQ 80	/* Given log. name not unique */
#define EBADFD 81	/* f.d. invalid for this operation */
#define EREMCHG 82	/* Remote address changed */
#define ELIBACC 83	/* Can't access a needed shared lib */
#define ELIBBAD 84	/* Accessing a corrupted shared lib */
#define ELIBSCN 85	/* .lib section in a.out corrupted */
#define ELIBMAX 86	/* Attempting to link in too many libs */
#define ELIBEXEC 87	/* Attempting to exec a shared library */
#endif
#define ENOSYS 88	/* Function not implemented */
#ifdef __CYGWIN__
#define ENMFILE 89      /* No more files */
#endif
#define ENOTEMPTY 90	/* Directory not empty */
#define ENAMETOOLONG 91	/* File or path name too long */
#define ELOOP 92	/* Too many symbolic links */
#define EOPNOTSUPP 95	/* Operation not supported on socket */
#define EPFNOSUPPORT 96 /* Protocol family not supported */
#define ECONNRESET 104  /* Connection reset by peer */
#define ENOBUFS 105	/* No buffer space available */
#define EAFNOSUPPORT 106 /* Address family not supported by protocol family */
#define EPROTOTYPE 107	/* Protocol wrong type for socket */
#define ENOTSOCK 108	/* Socket operation on non-socket */
#define ENOPROTOOPT 109	/* Protocol not available */
#ifdef __LINUX_ERRNO_EXTENSIONS__
#define ESHUTDOWN 110	/* Can't send after socket shutdown */
#endif
#define ECONNREFUSED 111	/* Connection refused */
#define EADDRINUSE 112		/* Address already in use */
#define ECONNABORTED 113	/* Software caused connection abort */
#define ENETUNREACH 114		/* Network is unreachable */
#define ENETDOWN 115		/* Network interface is not configured */
#define ETIMEDOUT 116		/* Connection timed out */
#define EHOSTDOWN 117		/* Host is down */
#define EHOSTUNREACH 118	/* Host is unreachable */
#define EINPROGRESS 119		/* Connection already in progress */
#define EALREADY 120		/* Socket already connected */
#define EDESTADDRREQ 121	/* Destination address required */
#define EMSGSIZE 122		/* Message too long */
#define EPROTONOSUPPORT 123	/* Unknown protocol */
#ifdef __LINUX_ERRNO_EXTENSIONS__
#define ESOCKTNOSUPPORT 124	/* Socket type not supported */
#endif
#define EADDRNOTAVAIL 125	/* Address not available */
#define ENETRESET 126		/* Connection aborted by network */
#define EISCONN 127		/* Socket is already connected */
#define ENOTCONN 128		/* Socket is not connected */
#define ETOOMANYREFS 129
#ifdef __LINUX_ERRNO_EXTENSIONS__
#define EPROCLIM 130
#define EUSERS 131
#endif
#define EDQUOT 132
#define ESTALE 133
#define ENOTSUP 134		/* Not supported */
#ifdef __LINUX_ERRNO_EXTENSIONS__
#define ENOMEDIUM 135   /* No medium (in tape drive) */
#endif
#ifdef __CYGWIN__
#define ENOSHARE 136    /* No such host or network path */
#define ECASECLASH 137  /* Filename exists with different case */
#endif
#define EILSEQ 138		/* Illegal byte sequence */
#define EOVERFLOW 139	/* Value too large for defined data type */
#define ECANCELED 140	/* Operation canceled */
#define ENOTRECOVERABLE 141	/* State not recoverable */
#define EOWNERDEAD 142	/* Previous owner died */
#ifdef __LINUX_ERRNO_EXTENSIONS__
#define ESTRPIPE 143	/* Streams pipe error */
#endif
#define EWOULDBLOCK EAGAIN	/* Operation would block */

#define __ELASTERROR 2000	/* Users can add values starting here */
#endif

/*===========================================================================
 * DNS error codes: 9000 to 9999.
 *===========================================================================*/

//DNS server unable to interpret format.
#define DNS_ERROR_RCODE_FORMAT_ERROR     9001L

//DNS server failure.
#define DNS_ERROR_RCODE_SERVER_FAILURE   9002L

//DNS name does not exist.
#define DNS_ERROR_RCODE_NAME_ERROR       9003L

//DNS request not supported by name server.
#define DNS_ERROR_RCODE_NOT_IMPLEMENTED  9004L

//DNS operation refused.
#define DNS_ERROR_RCODE_REFUSED          9005L

//DNS name that ought not exist, does exist.
#define DNS_ERROR_RCODE_YXDOMAIN         9006L

//DNS RR set that ought not exist, does exist.
#define DNS_ERROR_RCODE_YXRRSET          9007L

//DNS RR set that ought to exist, does not exist.
#define DNS_ERROR_RCODE_NXRRSET          9008L

//DNS server not authoritative for zone.
#define DNS_ERROR_RCODE_NOTAUTH          9009L

//DNS name in update or prereq is not in zone.
#define DNS_ERROR_RCODE_NOTZONE          9010L

//DNS signature failed to verify.
#define DNS_ERROR_RCODE_BADSIG           9016L

//DNS bad key.
#define DNS_ERROR_RCODE_BADKEY           9017L

//DNS signature validity expired.
#define DNS_ERROR_RCODE_BADTIME          9018L

/*===========================================================================
 * DNS packet format errors.
 *===========================================================================*/

//No records found for given DNS query.
#define DNS_INFO_NO_RECORDS              9501L

//Bad DNS packet.
#define DNS_ERROR_BAD_PACKET             9502L

//No DNS packet.
#define DNS_ERROR_NO_PACKET              9503L

//DNS error, check rcode.
#define DNS_ERROR_RCODE                  9504L

//Unsecured DNS packet.
#define DNS_ERROR_UNSECURE_PACKET        9505L

/*===========================================================================
 * DNS general API errors.
 *===========================================================================*/

//General API errors
#define DNS_ERROR_NO_MEMORY				 ERROR_OUTOFMEMORY
#define DNS_ERROR_INVALID_NAME			 ERROR_INVALID_NAME
#define DNS_ERROR_INVALID_DATA			 ERROR_INVALID_DATA

//Invalid DNS type.
#define DNS_ERROR_INVALID_TYPE           9551L

//Invalid IP address.
#define DNS_ERROR_INVALID_IP_ADDRESS     9552L

//Invalid property.
#define DNS_ERROR_INVALID_PROPERTY       9553L

//Try DNS operation again later.
#define DNS_ERROR_TRY_AGAIN_LATER        9554L

//Record for given name and type is not unique.
#define DNS_ERROR_NOT_UNIQUE             9555L

//DNS name does not comply with RFC specifications.
#define DNS_ERROR_NON_RFC_NAME           9556L

//DNS name is a fully-qualified DNS name.
#define DNS_STATUS_FQDN                  9557L

//DNS name is dotted (multi-label).
#define DNS_STATUS_DOTTED_NAME           9558L

//DNS name is a single-part name.
#define DNS_STATUS_SINGLE_PART_NAME      9559L

//DNS name contains an invalid character.
#define DNS_ERROR_INVALID_NAME_CHAR      9560L

//DNS name is entirely numeric.
#define DNS_ERROR_NUMERIC_NAME           9561L

//The operation requested is not permitted on a DNS root server.
#define DNS_ERROR_NOT_ALLOWED_ON_ROOT_SERVER 9562L

//The record could not be created because this part of the DNS namespace 
//has been delegated to another server.
#define DNS_ERROR_NOT_ALLOWED_UNDER_DELEGATION 9563L

//The DNS server could not find a set of root hints.
#define DNS_ERROR_CANNOT_FIND_ROOT_HINTS 9564L

//The DNS server found root hints but they were not consistent 
//across all adapters.
#define DNS_ERROR_INCONSISTENT_ROOT_HINTS 9565L

//The specified value is too small for this parameter.
#define DNS_ERROR_DWORD_VALUE_TOO_SMALL  9566L

//The specified value is too large for this parameter.
#define DNS_ERROR_DWORD_VALUE_TOO_LARGE  9567L

//This operation is not allowed while the DNS server is loading zones in 
//the background. Please try again later.
#define DNS_ERROR_BACKGROUND_LOADING     9568L

//The operation requested is not permitted on against a DNS server running 
//on a read-only DC.
#define DNS_ERROR_NOT_ALLOWED_ON_RODC    9569L

//No data is allowed to exist underneath a DNAME record.
#define DNS_ERROR_NOT_ALLOWED_UNDER_DNAME 9570L

/*===========================================================================
 * DNS zone errors
 *===========================================================================*/

//DNS zone does not exist.
#define DNS_ERROR_ZONE_DOES_NOT_EXIST		9601L

//DNS zone information not available.
#define DNS_ERROR_NO_ZONE_INFO				9602L

//Invalid operation for DNS zone.
#define DNS_ERROR_INVALID_ZONE_OPERATION	9603L

//Invalid DNS zone configuration.
#define DNS_ERROR_ZONE_CONFIGURATION_ERROR	9604L

//DNS zone has no start of authority (SOA) record.
#define DNS_ERROR_ZONE_HAS_NO_SOA_RECORD	9605L

//DNS zone has no Name Server (NS) record.
#define DNS_ERROR_ZONE_HAS_NO_NS_RECORDS	9606L

//DNS zone is locked.
#define DNS_ERROR_ZONE_LOCKED				9607L

//DNS zone creation failed.
#define DNS_ERROR_ZONE_CREATION_FAILED		 9608L

//DNS zone already exists.
#define DNS_ERROR_ZONE_ALREADY_EXISTS		9609L

//DNS automatic zone already exists.
#define DNS_ERROR_AUTOZONE_ALREADY_EXISTS	9610L

//Invalid DNS zone type.
#define DNS_ERROR_INVALID_ZONE_TYPE			9611L

//Secondary DNS zone requires master IP address.
#define DNS_ERROR_SECONDARY_REQUIRES_MASTER_IP 9612L

//DNS zone not secondary.
#define DNS_ERROR_ZONE_NOT_SECONDARY		9613L

//Need secondary IP address.
#define DNS_ERROR_NEED_SECONDARY_ADDRESSES	9614L

//WINS initialization failed.
#define DNS_ERROR_WINS_INIT_FAILED			9615L

//Need WINS servers.
#define DNS_ERROR_NEED_WINS_SERVERS			9616L

//NBTSTAT initialization call failed.
#define DNS_ERROR_NBSTAT_INIT_FAILED		9617L

//Invalid delete of start of authority (SOA)
#define DNS_ERROR_SOA_DELETE_INVALID		9618L

//A conditional forwarding zone already exists for that name.
#define DNS_ERROR_FORWARDER_ALREADY_EXISTS	9619L

//This zone must be configured with one or more master DNS server 
//IP addresses.
#define DNS_ERROR_ZONE_REQUIRES_MASTER_IP	9620L

//The operation cannot be performed because this zone is shutdown.
#define DNS_ERROR_ZONE_IS_SHUTDOWN			9621L

/*===========================================================================
 * DNS datafile errors
 *===========================================================================*/

//Primary DNS zone requires datafile.
#define DNS_ERROR_PRIMARY_REQUIRES_DATAFILE		9651L

//Invalid datafile name for DNS zone.
#define DNS_ERROR_INVALID_DATAFILE_NAME			9652L

//Failed to open datafile for DNS zone.
#define DNS_ERROR_DATAFILE_OPEN_FAILURE			9653L

//Failed to write datafile for DNS zone.
#define DNS_ERROR_FILE_WRITEBACK_FAILED			9654L

//Failure while reading datafile for DNS zone.
#define DNS_ERROR_DATAFILE_PARSING				9655L

/*===========================================================================
 * DNS database errors
 *===========================================================================*/

//DNS record does not exist.
#define DNS_ERROR_RECORD_DOES_NOT_EXIST  9701L

//DNS record format error.
#define DNS_ERROR_RECORD_FORMAT          9702L

//Node creation failure in DNS.
#define DNS_ERROR_NODE_CREATION_FAILED   9703L

//Unknown DNS record type.
#define DNS_ERROR_UNKNOWN_RECORD_TYPE    9704L

//DNS record timed out.
#define DNS_ERROR_RECORD_TIMED_OUT       9705L

//Name not in DNS zone.
#define DNS_ERROR_NAME_NOT_IN_ZONE       9706L

//CNAME loop detected.
#define DNS_ERROR_CNAME_LOOP             9707L

//Node is a CNAME DNS record.
#define DNS_ERROR_NODE_IS_CNAME          9708L

//A CNAME record already exists for given name.
#define DNS_ERROR_CNAME_COLLISION        9709L

//Record only at DNS zone root.
#define DNS_ERROR_RECORD_ONLY_AT_ZONE_ROOT 9710L

//DNS record already exists.
#define DNS_ERROR_RECORD_ALREADY_EXISTS  9711L

//Secondary DNS zone data error.
#define DNS_ERROR_SECONDARY_DATA         9712L

//Could not create DNS cache data.
#define DNS_ERROR_NO_CREATE_CACHE_DATA   9713L

//DNS name does not exist.
#define DNS_ERROR_NAME_DOES_NOT_EXIST    9714L

//Could not create pointer (PTR) record.
#define DNS_WARNING_PTR_CREATE_FAILED    9715L

//DNS domain was undeleted.
#define DNS_WARNING_DOMAIN_UNDELETED     9716L

//The directory service is unavailable.
#define DNS_ERROR_DS_UNAVAILABLE         9717L

//DNS zone already exists in the directory service.
#define DNS_ERROR_DS_ZONE_ALREADY_EXISTS 9718L

//DNS server not creating or reading the boot file for the directory service 
//integrated DNS zone.
#define DNS_ERROR_NO_BOOTFILE_IF_DS_ZONE 9719L

//Node is a DNAME DNS record.
#define DNS_ERROR_NODE_IS_DNAME          9720L

//A DNAME record already exists for given name.
#define DNS_ERROR_DNAME_COLLISION        9721L

//An alias loop has been detected with either CNAME or DNAME records.
#define DNS_ERROR_ALIAS_LOOP             9722L

/*===========================================================================
 * DNS operation errors
 *===========================================================================*/

//DNS AXFR (zone transfer) complete.
#define DNS_INFO_AXFR_COMPLETE           9751L

//DNS zone transfer failed.
#define DNS_ERROR_AXFR                   9752L

//Added local WINS server.
#define DNS_INFO_ADDED_LOCAL_WINS        9753L

/*===========================================================================
 * DNS secure update
 *===========================================================================*/

//Secure update call needs to continue update request.
#define DNS_STATUS_CONTINUE_NEEDED       9801L

/*===========================================================================
 * DNS setup errors
 *===========================================================================*/

//TCP/IP network protocol not installed.
#define DNS_ERROR_NO_TCPIP               9851L

//No DNS servers configured for local system.
#define DNS_ERROR_NO_DNS_SERVERS         9852L

//Directory partition (DP) errors
//The specified directory partition does not exist.
#define DNS_ERROR_DP_DOES_NOT_EXIST      9901L

//The specified directory partition already exists.
#define DNS_ERROR_DP_ALREADY_EXISTS      9902L

//This DNS server is not enlisted in the specified directory partition.
#define DNS_ERROR_DP_NOT_ENLISTED        9903L

//This DNS server is already enlisted in the specified directory partition.
#define DNS_ERROR_DP_ALREADY_ENLISTED    9904L

//The directory partition is not available at this time. Please wait 
//a few minutes and try again.
#define DNS_ERROR_DP_NOT_AVAILABLE       9905L

//The application directory partition operation failed. The domain 
//controller holding the domain naming master role is down or unable 
//to service the request or is not running Windows Server 2003.
#define DNS_ERROR_DP_FSMO_ERROR          9906L

/*===========================================================================
 * Network error codes: 10000 to 11999.
 *===========================================================================*/

//Detected an invalid pointer address in attempting to use a 
//pointer argument in a call.
#define EFAULT                        10014L

//An invalid argument was supplied.
#define EINVAL                        10022L

//A required address was omitted from an operation on a socket.
#define EDESTADDRREQ                  10039L

//A message sent on a datagram socket was larger than the internal message 
//buffer or some other network limit, or the buffer used to receive a 
//datagram into was smaller than the datagram itself.
#define EMSGSIZE                      10040L

//Only one usage of each socket address (protocol/network address/port) is 
//normally permitted.
#define EADDRINUSE                    10048L

//The requested address is not valid in its context.
#define EADDRNOTAVAIL                 10049L

//A socket operation encountered a dead network.
#define ENETDOWN                      10050L

//A socket operation was attempted to an unreachable network.
#define ENETUNREACH                   10051L

//The connection has been broken due to keep-alive activity detecting 
//a failure while the operation was in progress.
#define ENETRESET                     10052L

//An established connection was aborted by the software in your host machine.
#define ECONNABORTED                  10053L

//An existing connection was forcibly closed by the remote host.
#define ECONNRESET                    10054L

//An operation on a socket could not be performed because the system 
//lacked sufficient buffer space or because a queue was full.
#define ENOBUFS						  10055L

//A connect request was made on an already connected socket.
#define EISCONN                       10056L

//A request to send or receive data was disallowed because the socket is not 
//connected and (when sending on a datagram socket using a sendto call) no 
//address was supplied.
#define ENOTCONN                      10057L

//A request to send or receive data was disallowed because the socket had 
//already been shut down in that direction with a previous shutdown call.
#define ESHUTDOWN                     10058L

//Too many references to some kernel object.
#define ETOOMANYREFS                  10059L

//A connection attempt failed because the connected party did not properly 
//respond after a period of time, or established connection failed because 
//connected host has failed to respond.
#define ETIMEDOUT                     10060L

//No connection could be made because the target machine actively refused it.
#define ECONNREFUSED                  10061L

//Name component or name was too long.
#define ENAMETOOLONG                  10063L

//A socket operation failed because the destination host was down.
#define EHOSTDOWN                     10064L

//A socket operation was attempted to an unreachable host.
#define EHOSTUNREACH                  10065L

#endif /* __ERRORS_H */

/*===========================================================================
 * End of file errors.h
 *===========================================================================*/
















