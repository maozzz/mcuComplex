/*===========================================================================
 *
 *  Standard library
 *
 *  Marusenkov S.A., "ostfriesland@mail.ru", 2016
 *
 * THIS SOURCE CODE IS PROVIDED AS IS WITH NO WARRANTIES OF ANY KIND
 * INCLUDING THE WARRANTIES OF DESIGN, MERCHANTIBILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE, OR ARISING FROM A COURSE OF DEALING, USAGE OR
 * TRADE PRACTICE.
 *
 *---------------------------------------------------------------------------
 *
 *  File BinDataItem.h: Binary data block structure.
 *
 *===========================================================================*/
 
 #ifndef __BINDATAITEM_H
 #define __BINDATAITEM_H
 
 /* */
 typedef struct tag_BinDataItem
 { 
	//Pointer to data buffer
	const unsigned short *data;
	
	//Data size, words
	unsigned int	wSize;
	
	//Destination address
	unsigned int	dAddr;
 } BinDataItem;
  
#endif /* !__BINDATAITEM_H */
  
 /*===========================================================================
 * End of file BinDataItem.h
 *===========================================================================*/
 