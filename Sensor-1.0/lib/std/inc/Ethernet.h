/*===========================================================================
 *
 *  Network library
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
 *  File Ethernet.h: Ethernet frame functions and definitions.
 *
 *===========================================================================*/

#ifndef __ETHERNET_H
#define __ETHERNET_H

#include "../../bastypes.h"

/* Common Ethernet definitions.
 * Note:
 * 1. All frames end with 4 bytes FCS (frame checksum)
 * 2. Frames with length less 64 bytes called 'Runts'.
 *    Frames with length more 1518 bytes called 'Giants'
 * 4. Other IEEE 802 specifications ('Baby Giants' frames):
 *    - 802.3AC max length (1518 + 4), added Q-tag;
 *      (info 802.1Q (VLAN tag), 802.1p (COS bits))
 *    - 802.1AD max length (1518 + 8), added QinQ;
 *    - 802.1AH (MIM) max length (1518 + 30),
 *      (Provider Bridge Backbone Mac in Mac);
 *    - MPLS max length (1518 + n*4), n - count of stack labels;
 *    - 802.1AE (Mac Security) max length (1518 + 68), added
 *      'Security Tag' and 'Message Authentication Code' fields.
 *        - 802.3AS max length (1518 + 482)
 *        All this specifications with same MTU 1500 bytes.
 */
#define ETH_MACADDR_LENGTH              6
#define ETH_FRAME_MINLENGTH             64
#define ETH_FRAME_MAXLENGTH             1518 //with CRC

//Ethernet frame header length (mac:6 + mac:6 + type:2)
#define ETH_FRAMEHDR_LENGTH             14
//Ethernet frame CRC length 
#define ETH_FRAMECRC_LENGTH             4

//Minimum payload length (all frame types)
#define ETH_PLD_MINLENGTH               46 //64 - hdr:14 - crc:4

//Encapsulated data type (network order)
#define ETH_TYPE_IPV4                   0x0008 //[08 00]
#define ETH_TYPE_ARP                    0x0608 //[08 06]

//Ethernet II definitions
#define ETHII_PLD_MAXLENGTH             1500 //1518 - hdr:14 - crc:4

//Ethernet LLC definitions
#define ETHLLC_PLD_MAXLENGTH            1497
#define ETHLLC_SSAP_IPV4                6
#define ETHLLC_DSAP_IPV4                ETHLLC_SSAP_IPV4

//Ethernet SNAP definitions
#define ETHSNAP_OUI_IEEE                0x000000 //IEEE has OUI [0,0,0]
#define ETHSNAP_PLD_MAXLENGTH                   1492

//Ethernet 'RAW' definitions
#define ETHRAW_PLD_MAXLENGTH                    1500

//Ethernet II header
//(DEC, Intel è Xerox - DIX Ethernet standard)
//Payload: 46..1500 bytes
typedef struct tag_EthernetIIHdr
{
        //Destination MAC
        u8 destMAC[ETH_MACADDR_LENGTH];

        //Source MAC
        u8 srcMAC[ETH_MACADDR_LENGTH];

        //Encapsulated data type
        //(see: http://standards.ieee.org/develop/regauth/ethertype/eth.txt)
        u16 type;

} EthernetIIHdr;



//Ethernet 802.2 (802.3 + LLC header)
//Payload: 46..1497 bytes
typedef struct tag_EthernetLLCHdr
{
        //Destination MAC
        u8 destMAC[ETH_MACADDR_LENGTH];

        //Source MAC
        u8 srcMAC[ETH_MACADDR_LENGTH];

        //Encapsulated data type
        u16 length;

        //Logical Link Control (LLC) header:
        //1. Destination Service Access Point
        //(see: http://standards.ieee.org/develop/regauth/llc/public.html)
        //bits [7..1] - dsap value
        //bit [0] - address type designation (1/0: group/individual)
        u8 dsap;

        //2. Source Service Access Point
        //(see: http://standards.ieee.org/develop/regauth/llc/public.html)
        //bits [7..1] - ssap value
        //bit [0] - 1/0 - command/response
        u8 ssap;

        //3. Connection-(less/oriented).
        //Always set to 0x03
        u8 ctrl;
} EthernetLLCHdr;



//Ethernet 802.2-SNAP
//Payload: 46..1492 bytes
typedef struct tag_EthernetSNAPHdr
{
        //Destination MAC
        u8 destMAC[ETH_MACADDR_LENGTH];

        //Source MAC
        u8 srcMAC[ETH_MACADDR_LENGTH];

        //Encapsulated data type
        u16 length;

        //LLC header: not used
        //dsap,ssap,ctrl is set to 0xAA, 0xAA, 0x03
        u8 dsap;
        u8 ssap;
        u8 ctrl;

        //SNAP fields:
        //Organizationally Unique Identifier (vendor code)
        //IEEE: [0,0,0]
        u8 snapOUI[3];

        //Protocol ID
        //(Whether snapOUI is 0 (IEEE), see PID values at
        //http://standards.ieee.org/develop/regauth/ethertype/eth.txt)
        u8 snapPID[2];
} EthernetSNAPHdr;



//Ethernet 'RAW'. (Novell, IPX as payload)
//Payload: 46..1500 bytes
typedef struct tag_EthernetRawHdr
{
        //Destination MAC
        u8 destMAC[ETH_MACADDR_LENGTH];

        //Source MAC
        u8 srcMAC[ETH_MACADDR_LENGTH];

        //Encapsulated data type
        u16 length;
} EthernetRawHdr;


//Group MAC address bit (multicast/broadcast)
#define ETH_MACADDR_IGBIT               0x01

//Locally administered MAC address bit
#define ETH_MACADDR_LGBIT               0x02


/* Copy src MAC address to dest MAC address */
#define CopyMAC(dest, src) { *((u32*)dest) = *((u32*)src); \
                             *((u16*)(dest + 4)) = *((u16*)(src + 4)); }

/* Fill MAC address with zeros */
#define FillMACZeros(mac)       {   *((u32*)mac) = (u32)0; \
                                    *((u16*)(mac + 4)) = (u16)0; }

/* Fill MAC address as broadcast (FF..FF) */
#define FillMACBroadcast(mac) { *((u32*)mac) = 0xFFFFFFFFU; \
                                *((u16*)(mac + 4)) = 0xFFFF; }

/* Fill MAC address with specified multicast IP address
   Multicast IP : 234.1.2.3 -> 0x030201EA, mask 0xFFFF7F00
   Multicast MAC: 01-00-5e-0xx-xx-xx,
   xx-xx-xx - least 23 bits of multicast IP. */
#define FillMACMulticast(mac, maddr) { *((u32*)mac) = 0x005e0001U; \
                                       mac[3] = (u8)((maddr >> 8) & 0x7F); \
                                       mac[4] = (u8)(maddr >> 16); \
                                       mac[5] = (u8)(maddr >> 24); }


/* Indicating that MAC1 and MAC2 adresses is equal */
#define IsMACEqual(macaddr1, macaddr2) \
                (*((u32*)(macaddr1)) == *((u32*)(macaddr2)) && \
                 *((u16*)((macaddr1) + 4)) == *((u16*)((macaddr2) + 4)))


/* Indicating that MAC address has all zeros */
#define IsMACZero(macaddr) (*((u32*)(macaddr)) == (u32)0 && \
                            *((u16*)((macaddr) + 4)) == (u16)0)

/* Indicating that MAC address is broadcast */
#define IsMACBCast(macaddr) (*((u32*)(macaddr)) == 0xFFFFFFFFU && \
                             *((u16*)((macaddr) + 4)) == 0xFFFF)

/* Indicating that MAC address is multicast */
#define IsMACMCast(macaddr) ((*((u32*)(macaddr)) & 0x80FFFFFFU) == 0x005e0001U)

/* Indicating that MAC address is locally administered
   (LG bit is set) */
#define IsMACLocal(macaddr) ((macaddr) & ETH_MACADDR_LGBIT)

/* Convert string at hexadecimal format to MAC address */
s32 ParseMACString(const char *src, u8 *mac);

/* Parse MAC address using specified OUI and NIC.
   @oui: 24-bit vendor identifier;
   @nic: 24-bit device identifier; */
u8 *ParseMACAddr(u32 oui, u32 nic, u8 *mac);

/* Copy src MAC address to dest MAC address */
void CopyMACF(u8 *dest, const u8 *src);

/* Fill MAC address with zeros */
void FillMACZerosF(u8 *mac);

/* Fill MAC address as broadcast (FF..FF) */
void FillMACBroadcastF(u8 *mac);

/* Fill MAC address with specified multicast IP address */
void FillMACMulticastF(u8 *mac, u32 maddr);

/* Indicating that MAC1 and MAC2 adresses is equal */
u8 IsMACEqualF(const u8 *mac1, const u8 *mac2);

/* Indicating that MAC address has all zeros */
u8 IsMACZeroF(const u8 *mac);

/* Indicating that MAC address is broadcast */
u8 IsMACBCastF(const u8 *mac);

/* Indicating that MAC address is multicast */
u8 IsMACMCastF(const u8 *mac);

#endif /* __ETHERNET_H */

/*===========================================================================
 * End of file Ethernet.h
 *===========================================================================*/

