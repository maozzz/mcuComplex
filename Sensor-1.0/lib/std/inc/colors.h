/*===========================================================================
 *
 *  Screen library
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
 *  File colors.h: Colors definitions.
 *
 *===========================================================================*/

#ifndef __COLORS_H
#define __COLORS_H

#include "../../bastypes.h"

//Names of colors (format: 0xAARRGGBB)
#define COL_TRANSPARENT           0xFFFFFFFF
#define COL_ALICEBLUE             0x00F0F8FF
#define COL_ANTIQUEWHITE          0x00FAEBD7
#define COL_AQUA                  0x0000FFFF
#define COL_AQUAMARINE            0x007FFFD4
#define COL_AZURE                 0x00F0FFFF
#define COL_BEIGE                 0x00F5F5DC
#define COL_BISQUE                0x00FFE4C4
#define COL_BLACK                 0x00000000
#define COL_BLANCHEDALMOND        0x00FFEBCD
#define COL_BLUE                  0x000000FF
#define COL_BLUEVIOLET            0x008A2BE2
#define COL_BROWN                 0x00A52A2A
#define COL_BURLYWOOD             0x00DEB887
#define COL_CADETBLUE             0x005F9EA0
#define COL_CHARTREUSE            0x007FFF00
#define COL_CHOCOLATE             0x00D2691E
#define COL_CORAL                 0x00FF7F50
#define COL_CORNFLOWERBLUE        0x006495ED
#define COL_CORNSILK              0x00FFF8DC
#define COL_CRIMSON               0x00DC143C
#define COL_CYAN                  0x0000FFFF
#define COL_DARKBLUE              0x0000008B
#define COL_DARKCYAN              0x00008B8B
#define COL_DARKGOLDENROD         0x00B8860B
#define COL_DARKGRAY              0x00A9A9A9
#define COL_DARKGREEN             0x00006400
#define COL_DARKKHAKI             0x00BDB76B
#define COL_DARKMAGENTA           0x008B008B
#define COL_DARKOLIVEGREEN        0x00556B2F
#define COL_DARKORANGE            0x00FF8C00
#define COL_DARKORCHID            0x009932CC
#define COL_DARKRED               0x008B0000
#define COL_DARKSALMON            0x00E9967A
#define COL_DARKSEAGREEN          0x008FBC8B
#define COL_DARKSLATEBLUE         0x00483D8B
#define COL_DARKSLATEGRAY         0x002F4F4F
#define COL_DARKTURQUOISE         0x0000CED1
#define COL_DARKVIOLET            0x009400D3
#define COL_DEEPPINK              0x00FF1493
#define COL_DEEPSKYBLUE           0x0000BFFF
#define COL_DIMGRAY               0x00696969
#define COL_DODGERBLUE            0x001E90FF
#define COL_FIREBRICK             0x00B22222
#define COL_FLORALWHITE           0x00FFFAF0
#define COL_FORESTGREEN           0x00228B22
#define COL_FUCHSIA               0x00FF00FF
#define COL_GAINSBORO             0x00DCDCDC
#define COL_GHOSTWHITE            0x00F8F8FF
#define COL_GOLD                  0x00FFD700
#define COL_GOLDENROD             0x00DAA520
#define COL_GRAY                  0x00808080
#define COL_GREEN                 0x00008000
#define COL_GREENYELLOW           0x00ADFF2F
#define COL_HONEYDEW              0x00F0FFF0
#define COL_HOTPINK               0x00FF69B4
#define COL_INDIANRED             0x00CD5C5C
#define COL_INDIGO                0x004B0082
#define COL_IVORY                 0x00FFFFF0
#define COL_KHAKI                 0x00F0E68C
#define COL_LAVENDER              0x00E6E6FA
#define COL_LAVENDERBLUSH         0x00FFF0F5
#define COL_LAWNGREEN             0x007CFC00
#define COL_LEMONCHIFFON          0x00FFFACD
#define COL_LIGHTBLUE             0x00ADD8E6
#define COL_LIGHTCORAL            0x00F08080
#define COL_LIGHTCYAN             0x00E0FFFF
#define COL_LIGHTGOLDENRODYELLOW  0x00FAFAD2
#define COL_LIGHTGREEN            0x0090EE90
#define COL_LIGHTGRAY             0x00D3D3D3
#define COL_LIGHTPINK             0x00FFB6C1
#define COL_LIGHTSALMON           0x00FFA07A
#define COL_LIGHTSEAGREEN         0x0020B2AA
#define COL_LIGHTSKYBLUE          0x0087CEFA
#define COL_LIGHTSLATEGRAY        0x00778899
#define COL_LIGHTSTEELBLUE        0x00B0C4DE
#define COL_LIGHTYELLOW           0x00FFFFE0
#define COL_LIME                  0x0000FF00
#define COL_LIMEGREEN             0x0032CD32
#define COL_LINEN                 0x00FAF0E6
#define COL_MAGENTA               0x00FF00FF
#define COL_MAROON                0x00800000
#define COL_MEDIUMAQUAMARINE      0x0066CDAA
#define COL_MEDIUMBLUE            0x000000CD
#define COL_MEDIUMORCHID          0x00BA55D3
#define COL_MEDIUMPURPLE          0x009370DB
#define COL_MEDIUMSEAGREEN        0x003CB371
#define COL_MEDIUMSLATEBLUE       0x007B68EE
#define COL_MEDIUMSPRINGGREEN     0x0000FA9A
#define COL_MEDIUMTURQUOISE       0x0048D1CC
#define COL_MEDIUMVIOLETRED       0x00C71585
#define COL_MIDNIGHTBLUE          0x00191970
#define COL_MINTCREAM             0x00F5FFFA
#define COL_MISTYROSE             0x00FFE4E1
#define COL_MOCCASIN              0x00FFE4B5
#define COL_NAVAJOWHITE           0x00FFDEAD
#define COL_NAVY                  0x00000080
#define COL_OLDLACE               0x00FDF5E6
#define COL_OLIVE                 0x00808000
#define COL_OLIVEDRAB             0x006B8E23
#define COL_ORANGE                0x00FFA500
#define COL_ORANGERED             0x00FF4500
#define COL_ORCHID                0x00DA70D6
#define COL_PALEGOLDENROD         0x00EEE8AA
#define COL_PALEGREEN             0x0098FB98
#define COL_PALETURQUOISE         0x00AFEEEE
#define COL_PALEVIOLETRED         0x00DB7093
#define COL_PAPAYAWHIP            0x00FFEFD5
#define COL_PEACHPUFF             0x00FFDAB9
#define COL_PERU                  0x00CD853F
#define COL_PINK                  0x00FFC0CB
#define COL_PLUM                  0x00DDA0DD
#define COL_POWDERBLUE            0x00B0E0E6
#define COL_PURPLE                0x00800080
#define COL_RED                   0x00FF0000
#define COL_ROSYBROWN             0x00BC8F8F
#define COL_ROYALBLUE             0x004169E1
#define COL_SADDLEBROWN           0x008B4513
#define COL_SALMON                0x00FA8072
#define COL_SANDYBROWN            0x00F4A460
#define COL_SEAGREEN              0x002E8B57
#define COL_SEASHELL              0x00FFF5EE
#define COL_SIENNA                0x00A0522D
#define COL_SILVER                0x00C0C0C0
#define COL_SKYBLUE               0x0087CEEB
#define COL_SLATEBLUE             0x006A5ACD
#define COL_SLATEGRAY             0x00708090
#define COL_SNOW                  0x00FFFAFA
#define COL_SPRINGGREEN           0x0000FF7F
#define COL_STEELBLUE             0x004682B4
#define COL_TAN                   0x00D2B48C
#define COL_TEAL                  0x00008080
#define COL_THISTLE               0x00D8BFD8
#define COL_TOMATO                0x00FF6347
#define COL_TURQUOISE             0x0040E0D0
#define COL_VIOLET                0x00EE82EE
#define COL_WHEAT                 0x00F5DEB3
#define COL_WHITE                 0x00FFFFFF
#define COL_WHITESMOKE            0x00F5F5F5
#define COL_YELLOW                0x00FFFF00
#define COL_YELLOWGREEN           0x009ACD32
#define COL_SALMONORANGE          0x00E1523D
#define COL_BRILLYELLOWGREEN      0x008CCB5E
#define COL_PURPLEBLUE            0x00354D73
#define COL_GRAYBROWN             0x00403A3A
#define COL_CORNFLOWERBLUE        0x006495ED
#define COL_MUSTARD               0x00FFDB58
#define COL_VKONTAKTE             0x004D7198
#define COL_LIGHTGRAY2            0x00D7D7D7



//4-bit color table
extern const color_t colorTable4bit[];

//8-bit Windows color table
extern const color_t colorTableWin8bit[];

//8-bit Macintosh color table
extern const color_t colorTableMac8bit[];

#endif /* __COLORS_H */

/*===========================================================================
 * End of file colors.h
 *===========================================================================*/

