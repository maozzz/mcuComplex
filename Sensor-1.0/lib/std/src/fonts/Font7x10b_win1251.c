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
 *  File Font7x10b_win1251.c: Font 7x10 bold. MS Windows DOSApp-105 analog.
 *  Notes:
 *  Each byte at font7x10b_win1251_data array is column of 8 pixels.
 *  Minor bit is upper pixel.
 *  Y      X 0     6 ('|' - column of 8 pixels)
 *  0        |||||||
 *  8        ||||||| 
 *
 *===========================================================================*/

#include "../../inc/fonts.h"

static const u8 font7x10b_win1251_data[] =
{
  /* 0x00 - ������ ����������.*/
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

  /* 0x01 - ���� � �������.*/
  0x78, 0xa4, 0x4a, 0x42, 0x4a, 0xa4, 0x78, 0x00, 0x00, 0x01, 0x01, 0x01, 0x00, 0x00,

  /* 0x02 - ���� � ������� �����������.*/
  0x78, 0xdc, 0xb6, 0xbe, 0xb6, 0xdc, 0x78, 0x00, 0x00, 0x01, 0x01, 0x01, 0x00, 0x00,

  /* 0x03 - ����� (������).*/
  0x1c, 0x3e, 0x7e, 0xfc, 0x7e, 0x3e, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

  /* 0x04 - �����.*/
  0x10, 0x38, 0x7c, 0xfe, 0x7c, 0x38, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

  /* 0x05 - ������.*/
  0x30, 0x78, 0x77, 0xbf, 0x77, 0x78, 0x30, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00,

  /* 0x06 - ����.*/
  0x38, 0x7c, 0x7e, 0xbf, 0x7e, 0x7c, 0x38, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00,

  /* 0x07 - ����������� ���� �� ������.*/
  0x00, 0x70, 0xf8, 0xf8, 0xf8, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

  /* 0x08 - ����������� ���� �� ������ � ��������.*/
  0xff, 0x8f, 0x07, 0x07, 0x07, 0x8f, 0xff, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f,

  /* 0x09 - ������������� ���� �� ������.*/
  0x00, 0x70, 0x88, 0x88, 0x88, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

  /* 0x0a - ������������� ���� �� ������ � �������� (������).*/
  0xff, 0x8f, 0x77, 0x77, 0x77, 0x8f, 0xff, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f,

  /* 0x0b - ������� ������ (���� �� �������� �����).*/
  0xe0, 0x10, 0x10, 0x1a, 0xe6, 0x0e, 0x00, 0x00, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00,

  /* 0x0c - ������� ������ (���� � ������� �����).*/
  0x00, 0x4e, 0x51, 0xf1, 0x51, 0x4e, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,

  /* 0x0d - ���� I.*/
  0x00, 0x80, 0x80, 0xfe, 0x04, 0x38, 0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00,

  /* 0x0e - ���� II.*/
  0x80, 0x80, 0xfe, 0x0a, 0xc5, 0xc5, 0x7f, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,

  /* 0x0f - ������ (���� � ������������� ������).*/
  0x10, 0xba, 0x44, 0xc7, 0x44, 0xba, 0x10, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,

  /* 0x10 - ������� ������� ������.*/
  0x00, 0x00, 0xfc, 0xf8, 0x70, 0x20, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00,

  /* 0x11 - ������� ������� �����.*/
  0x00, 0x20, 0x70, 0xf8, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00,

  /* 0x12 - ������ ������� �����-����.*/
  0x00, 0x44, 0xc6, 0xff, 0xc6, 0x44, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,

  /* 0x13 - ��� ��������������� �����.*/
  0x00, 0xbf, 0xbf, 0x00, 0xbf, 0xbf, 0x00, 0x00, 0x01, 0x01, 0x00, 0x01, 0x01, 0x00,

  /* 0x14 - ������ "��".*/
  0x00, 0x0e, 0x1f, 0x11, 0xff, 0x01, 0xff, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01,

  /* 0x15 - ������ ���������.*/
  0x00, 0x8e, 0xbf, 0x29, 0xfb, 0xe3, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00,

  /* 0x16 - ������� �������������.*/
  0x00, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00,

  /* 0x17 - ������ ������� �����-���� � ��������������.*/
  0x00, 0x24, 0x66, 0xff, 0x66, 0x24, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00,

  /* 0x18 - ������ ������� �����.*/
  0x00, 0x04, 0x06, 0xff, 0x06, 0x04, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,

  /* 0x19 - ������ ������� ����.*/
  0x00, 0x40, 0xc0, 0xff, 0xc0, 0x40, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,

  /* 0x1a - ������ ������� ������.*/
  0x20, 0x20, 0x20, 0xf8, 0x70, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

  /* 0x1b - ������ ������� �����.*/
  0x20, 0x70, 0xf8, 0x20, 0x20, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

  /* 0x1c - ������ �������.*/
  0x00, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00,

  /* 0x1d - ������ ������� �����-������.*/
  0x20, 0x70, 0xf8, 0x20, 0xf8, 0x70, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

  /* 0x1e - ������� ������� �����.*/
  0xc0, 0xf0, 0xfc, 0xff, 0xfc, 0xf0, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

  /* 0x1f - ������� ������� ����.*/
  0x06, 0x1e, 0x7e, 0xfe, 0x7e, 0x1e, 0x06, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,

  /* 0x20 - ������ (������ ����������).*/
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

  /* 0x21 - ��������������� ����.*/
  0x00, 0x00, 0xbf, 0xbf, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0x00,

  /* 0x22 - ������� �������.*/
  0x00, 0x07, 0x07, 0x00, 0x07, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

  /* 0x23 - �������.*/
  0x00, 0x48, 0xfe, 0x48, 0xfe, 0x48, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x00,

  /* 0x24 - ������.*/
  0x00, 0x4c, 0x9e, 0x93, 0xf2, 0x64, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00,

  /* 0x25 - �������.*/
  0x00, 0xc6, 0xe9, 0x36, 0xd8, 0x2f, 0xc7, 0x00, 0x01, 0x01, 0x00, 0x00, 0x01, 0x00,

  /* 0x26 - ���������.*/
  0x00, 0xe6, 0xff, 0x19, 0xff, 0xe6, 0x30, 0x00, 0x00, 0x01, 0x01, 0x00, 0x01, 0x01,

  /* 0x27 - ��������.*/
  0x00, 0x00, 0x04, 0x07, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

  /* 0x28 - ����������� ������.*/
  0x00, 0x7c, 0xfe, 0x83, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x00, 0x00,

  /* 0x29 - ����������� ������.*/
  0x00, 0x01, 0x83, 0xfe, 0x7c, 0x00, 0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00,

  /* 0x2a - ��������� (���������).*/
  0x10, 0x54, 0x7c, 0x38, 0x7c, 0x54, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

  /* 0x2b - ����.*/
  0x00, 0x20, 0x20, 0xf8, 0xf8, 0x20, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

  /* 0x2c - �������.*/
  0x00, 0x00, 0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x05, 0x07, 0x03, 0x00, 0x00,

  /* 0x2d - ����.*/
  0x00, 0x20, 0x20, 0x20, 0x20, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

  /* 0x2e - �����.*/
  0x00, 0x00, 0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x00, 0x00,

  /* 0x2f - ���� �����-������� ('/').*/
  0x00, 0xc0, 0xf0, 0x3c, 0x0f, 0x03, 0x00, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,

  /* 0x30 - '0'.*/
  0x00, 0xfe, 0xff, 0x01, 0xff, 0xff, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00,

  /* 0x31 - '1'.*/
  0x00, 0x04, 0x06, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x00, 0x00,

  /* 0x32 - '2'.*/
  0x00, 0x86, 0xc7, 0x71, 0x3f, 0x0e, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00,

  /* 0x33 - '3'.*/
  0x00, 0x82, 0x93, 0x11, 0xff, 0xee, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x00, 0x00,

  /* 0x34 - '4'.*/
  0x00, 0x78, 0x7f, 0x47, 0xf8, 0xf8, 0x40, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x00,

  /* 0x35 - '5'.*/
  0x00, 0x9f, 0x9f, 0x11, 0xf1, 0xe1, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x00, 0x00,

  /* 0x36 - '6'.*/
  0x00, 0xf8, 0xfe, 0x17, 0xf1, 0xe0, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x00, 0x00,

  /* 0x37 - '7'.*/
  0x00, 0x01, 0xc1, 0xf9, 0x3f, 0x07, 0x00, 0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0x00,

  /* 0x38 - '8'.*/
  0x00, 0xee, 0xff, 0x11, 0xff, 0xee, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x00, 0x00,

  /* 0x39 - '9'.*/
  0x00, 0x0e, 0x9f, 0xf1, 0x7f, 0x1e, 0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00,

  /* 0x3a - ���������.*/
  0x00, 0x00, 0x8c, 0x8c, 0x8c, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x00, 0x00,

  /* 0x3b - ����� � �������.*/
  0x00, 0x00, 0x8c, 0x8c, 0x8c, 0x00, 0x00, 0x00, 0x00, 0x05, 0x07, 0x03, 0x00, 0x00,

  /* 0x3c - ������.*/
  0x00, 0x10, 0x38, 0x6c, 0xc6, 0x83, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00,

  /* 0x3d - �����.*/
  0x00, 0x48, 0x48, 0x48, 0x48, 0x48, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

  /* 0x3e - ������.*/
  0x00, 0x83, 0xc6, 0x6c, 0x38, 0x10, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,

  /* 0x3f - �������������� ����.*/
  0x00, 0x06, 0xa7, 0xb1, 0x1f, 0x0e, 0x00, 0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0x00,

  /* 0x40 - "������" ('@').*/
  0x00, 0x7c, 0xfe, 0x83, 0x39, 0x45, 0x3e, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01,

  /* 0x41 - 'A'.*/
  0x00, 0xfc, 0xfe, 0x23, 0xfe, 0xfc, 0x00, 0x00, 0x01, 0x01, 0x00, 0x01, 0x01, 0x00,

  /* 0x42 - 'B'.*/
  0x00, 0xff, 0xff, 0x11, 0xff, 0xee, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00,

  /* 0x43 - 'C'.*/
  0x00, 0xfe, 0xff, 0x01, 0xc7, 0xc6, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x00, 0x00,

  /* 0x44 - 'D'.*/
  0x00, 0xff, 0xff, 0x01, 0xff, 0xfe, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00,

  /* 0x45 - 'E'.*/
  0x00, 0xff, 0xff, 0x11, 0x11, 0x01, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00,

  /* 0x46 - 'F'.*/
  0x00, 0xff, 0xff, 0x11, 0x11, 0x01, 0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00,

  /* 0x47 - 'G'.*/
  0x00, 0xfe, 0xff, 0x01, 0xf3, 0xf2, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x00, 0x00,

  /* 0x48 - 'H'.*/
  0x00, 0xff, 0xff, 0x10, 0xff, 0xff, 0x00, 0x00, 0x01, 0x01, 0x00, 0x01, 0x01, 0x00,

  /* 0x49 - 'I'.*/
  0x00, 0x00, 0x01, 0xff, 0xff, 0x01, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x00,

  /* 0x4a - 'J'.*/
  0x00, 0xc0, 0xc0, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x00, 0x00,

  /* 0x4b - 'K'.*/
  0x00, 0xff, 0xff, 0x38, 0xef, 0xc7, 0x00, 0x00, 0x01, 0x01, 0x00, 0x01, 0x01, 0x00,

  /* 0x4c - 'L'.*/
  0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00,

  /* 0x4d - 'M'.*/
  0xff, 0xff, 0x1c, 0x70, 0x1c, 0xff, 0xff, 0x01, 0x01, 0x00, 0x00, 0x00, 0x01, 0x01,

  /* 0x4e - 'N'.*/
  0x00, 0xff, 0xff, 0x1c, 0x30, 0xff, 0xff, 0x00, 0x01, 0x01, 0x00, 0x00, 0x01, 0x01,

  /* 0x4f - 'O'.*/
  0x00, 0xfe, 0xff, 0x01, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x00, 0x00,

  /* 0x50 - 'P'.*/
  0x00, 0xff, 0xff, 0x11, 0x1f, 0x0e, 0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00,

  /* 0x51 - 'Q'.*/
  0x00, 0xfe, 0xff, 0x01, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x01, 0x03, 0x07, 0x04, 0x00,

  /* 0x52 - 'R'.*/
  0x00, 0xff, 0xff, 0x11, 0xff, 0xee, 0x00, 0x00, 0x01, 0x01, 0x00, 0x01, 0x01, 0x00,

  /* 0x53 - 'S'.*/
  0x00, 0xce, 0x9f, 0x39, 0xf3, 0xe6, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x00, 0x00,

  /* 0x54 - 'T'.*/
  0x00, 0x01, 0x01, 0xff, 0xff, 0x01, 0x01, 0x00, 0x00, 0x00, 0x01, 0x01, 0x00, 0x00,

  /* 0x55 - 'U'.*/
  0x00, 0xff, 0xff, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x00, 0x00,

  /* 0x56 - 'V'.*/
  0x00, 0x7f, 0xff, 0x80, 0xff, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,

  /* 0x57 - 'W'.*/
  0x3f, 0xff, 0xc0, 0x3c, 0xc0, 0xff, 0x3f, 0x00, 0x01, 0x01, 0x00, 0x01, 0x01, 0x00,

  /* 0x58 - 'X'.*/
  0x00, 0xc7, 0xef, 0x38, 0xef, 0xc7, 0x00, 0x00, 0x01, 0x01, 0x00, 0x01, 0x01, 0x00,

  /* 0x59 - 'Y'.*/
  0x00, 0x0f, 0x1f, 0xf0, 0xf0, 0x1f, 0x0f, 0x00, 0x00, 0x00, 0x01, 0x01, 0x00, 0x00,

  /* 0x5a - 'Z'.*/
  0x00, 0xc1, 0xf1, 0x3d, 0x0f, 0x03, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00,

  /* 0x5b - '['.*/
  0x00, 0x00, 0xff, 0xff, 0x01, 0x01, 0x00, 0x00, 0x00, 0x03, 0x03, 0x02, 0x02, 0x00,

  /* 0x5c - '\'.*/
  0x00, 0x03, 0x0f, 0x3c, 0xf0, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01,

  /* 0x5d - ']'.*/
  0x00, 0x01, 0x01, 0xff, 0xff, 0x00, 0x00, 0x00, 0x02, 0x02, 0x03, 0x03, 0x00, 0x00,

  /* 0x5e - '^'.*/
  0x00, 0x04, 0x06, 0x03, 0x06, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

  /* 0x5f - '_'.*/
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,

  /* 0x60 - �������� ��������.*/
  0x00, 0x00, 0x03, 0x07, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

  /* 0x61 - 'a'.*/
  0x00, 0xc0, 0xe8, 0x28, 0xf8, 0xf0, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x00,

  /* 0x62 - 'b'.*/
  0x00, 0xff, 0xff, 0x08, 0xf8, 0xf0, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00,

  /* 0x63 - 'c'.*/
  0x00, 0xf0, 0xf8, 0x08, 0x98, 0x90, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x00, 0x00,

  /* 0x64 - 'd'.*/
  0x00, 0xf0, 0xf8, 0x08, 0xff, 0xff, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x00,

  /* 0x65 - 'e'.*/
  0x00, 0xf0, 0xf8, 0x28, 0x38, 0xb0, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x00, 0x00,

  /* 0x66 - 'f'.*/
  0x00, 0x08, 0xfe, 0xff, 0x09, 0x09, 0x00, 0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0x00,

  /* 0x67 - 'g'.*/
  0x00, 0x38, 0x7c, 0x44, 0xfc, 0xfc, 0x00, 0x00, 0x01, 0x02, 0x02, 0x03, 0x01, 0x00,

  /* 0x68 - 'h'.*/
  0x00, 0xff, 0xff, 0x08, 0xf8, 0xf0, 0x00, 0x00, 0x01, 0x01, 0x00, 0x01, 0x01, 0x00,

  /* 0x69 - 'i'.*/
  0x00, 0x00, 0x08, 0xf9, 0xf9, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x00,

  /* 0x6a - 'j'.*/
  0x00, 0x00, 0x08, 0xf9, 0xf9, 0x00, 0x00, 0x00, 0x02, 0x02, 0x03, 0x01, 0x00, 0x00,

  /* 0x6b - 'k'.*/
  0x00, 0xff, 0xff, 0x70, 0xd8, 0x88, 0x00, 0x00, 0x01, 0x01, 0x00, 0x01, 0x01, 0x00,

  /* 0x6c - 'l'.*/
  0x00, 0x00, 0x01, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x00,

  /* 0x6d - 'm'.*/
  0xf8, 0xf8, 0x18, 0xf0, 0x18, 0xf8, 0xf0, 0x01, 0x01, 0x00, 0x00, 0x00, 0x01, 0x01,

  /* 0x6e - 'n'.*/
  0x00, 0xf8, 0xf8, 0x08, 0xf8, 0xf0, 0x00, 0x00, 0x01, 0x01, 0x00, 0x01, 0x01, 0x00,

  /* 0x6f - 'o'.*/
  0x00, 0xf0, 0xf8, 0x08, 0xf8, 0xf0, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x00, 0x00,

  /* 0x70 - 'p'.*/
  0x00, 0xf8, 0xf8, 0x08, 0xf8, 0xf0, 0x00, 0x00, 0x07, 0x07, 0x01, 0x01, 0x00, 0x00,

  /* 0x71 - 'q'.*/
  0x00, 0xf0, 0xf8, 0x08, 0xf8, 0xf8, 0x00, 0x00, 0x00, 0x01, 0x01, 0x07, 0x07, 0x00,

  /* 0x72 - 'r'.*/
  0x00, 0xf8, 0xf8, 0x10, 0x18, 0x18, 0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00,

  /* 0x73 - 's'.*/
  0x00, 0x30, 0x78, 0x68, 0xe8, 0xc8, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00,

  /* 0x74 - 't'.*/
  0x00, 0x08, 0xfe, 0xfe, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x00,

  /* 0x75 - 'u'.*/
  0x00, 0xf8, 0xf8, 0x00, 0xf8, 0xf8, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x00,

  /* 0x76 - 'v'.*/
  0x00, 0x78, 0xf8, 0x80, 0xf8, 0x78, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,

  /* 0x77 - 'w'.*/
  0x78, 0xf8, 0x80, 0xf0, 0x80, 0xf8, 0x78, 0x00, 0x01, 0x01, 0x00, 0x01, 0x01, 0x00,

  /* 0x78 - 'x'.*/
  0x00, 0x98, 0xf8, 0x60, 0xf8, 0x98, 0x00, 0x00, 0x01, 0x01, 0x00, 0x01, 0x01, 0x00,

  /* 0x79 - 'y'.*/
  0x00, 0x78, 0xf8, 0x80, 0xf8, 0x78, 0x00, 0x00, 0x04, 0x06, 0x03, 0x01, 0x00, 0x00,

  /* 0x7a - 'z'.*/
  0x00, 0x88, 0xc8, 0x68, 0x38, 0x18, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00,

  /* 0x7b - '{'.*/
  0x00, 0x10, 0xfe, 0xef, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x00,

  /* 0x7c - '|'.*/
  0x00, 0x00, 0xdf, 0xdf, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x07, 0x00, 0x00, 0x00,

  /* 0x7d - '}'.*/
  0x00, 0x01, 0xef, 0xfe, 0x10, 0x00, 0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00,

  /* 0x7e - '~'.*/
  0x00, 0x06, 0x03, 0x07, 0x06, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

  /* 0x7f - "�����".*/
  0x00, 0xf0, 0x98, 0x8c, 0x98, 0xf0, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00,

  /* 0x80 - ����� �� ����� �����������.*/
  0x44, 0x00, 0x11, 0x44, 0x00, 0x11, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x01, 0x00,

  /* 0x81 - ����� �� ����� ������.*/
  0xaa, 0x00, 0x55, 0xaa, 0x00, 0x55, 0x00, 0x02, 0x00, 0x01, 0x02, 0x00, 0x01, 0x00,

  /* 0x82 - ����� �� ��������.*/
  0xaa, 0x55, 0x55, 0xaa, 0x55, 0xaa, 0x55, 0x02, 0x01, 0x01, 0x02, 0x01, 0x02, 0x01,

  /* 0x83 - ������������� - ������������ �����.*/
  0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00,

  /* 0x84 - ������������� - ������������ ����� � ������� �� ������ �����.*/
  0x20, 0x20, 0x20, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00,

  /* 0x85 - ������������� - ������������ ����� � ������� ������� �� ������ �����.*/
  0x50, 0x50, 0x50, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00,

  /* 0x86 - ������������� - ������� ������������ ����� � ������� �� ������ �����.*/
  0x20, 0x20, 0xff, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x0f, 0x00, 0x00,

  /* 0x87 - ������������� - ������� ������ ���� � ������� ������������ ������.*/
  0x20, 0x20, 0xe0, 0x20, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x0f, 0x00, 0x00,

  /* 0x88 - ������������� - ������� ������ ���� � ������� �������������� ������.*/
  0x50, 0x50, 0x50, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00,

  /* 0x89 - ������������� - ������� ������������ ����� � ������� ������� �� ������ �����.*/
  0x50, 0x50, 0xdf, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x03, 0x00, 0x00,

  /* 0x8a - ������������� - ������� ������������ �����.*/
  0x00, 0x00, 0xff, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x03, 0x00, 0x00,

  /* 0x8b - ������������� - ������� ������� ������ ����.*/
  0x50, 0x50, 0xd0, 0x10, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x03, 0x00, 0x00,

  /* 0x8c - ������������� - ������� ������ ������ ����.*/
  0x50, 0x50, 0x5f, 0x40, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

  /* 0x8d - ������������� - ������ ������ ���� � ������� ������������ ������.*/
  0x20, 0x20, 0x3f, 0x20, 0x3f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

  /* 0x8e - ������������� - ������ ������ ���� � ������� �������������� ������.*/
  0x50, 0x50, 0x50, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

  /* 0x8f - ������������� - ������� ������ ����.*/
  0x20, 0x20, 0x20, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00,

  /* 0x90 - ������������� - ������ ����� ����.*/
  0x00, 0x00, 0x00, 0x3f, 0x20, 0x20, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

  /* 0x91 - ������������� - �������������� ����� � ������� �� ������ �����.*/
  0x20, 0x20, 0x20, 0x3f, 0x20, 0x20, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

  /* 0x92 - ������������� - �������������� ����� � ������� �� ������ ����.*/
  0x20, 0x20, 0x20, 0xe0, 0x20, 0x20, 0x20, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00,

  /* 0x93 - ������������� - ������������ ����� � ������� �� ������ ������.*/
  0x00, 0x00, 0x00, 0xff, 0x20, 0x20, 0x20, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00,

  /* 0x94 - ������������� - �������������� ����� �� ������.*/
  0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

  /* 0x95 - ������������� - �����������.*/
  0x20, 0x20, 0x20, 0xff, 0x20, 0x20, 0x20, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00,

  /* 0x96 - ������������� - ������������ ����� � ������� ������� �� ������ ������.*/
  0x00, 0x00, 0x00, 0xff, 0x50, 0x50, 0x50, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00,

  /* 0x97 - ������������� - ������� ������������ ����� � ������� �� ������ ������.*/
  0x00, 0x00, 0xff, 0x00, 0xff, 0x20, 0x20, 0x00, 0x00, 0x0f, 0x00, 0x0f, 0x00, 0x00,

  /* 0x98 - ������������� - ������� ������ ����� ����.*/
  0x00, 0x00, 0x7f, 0x40, 0x5f, 0x50, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

  /* 0x99 - ������������� - ������� ������� ����� ����.*/
  0x00, 0x00, 0xf0, 0x10, 0xd0, 0x50, 0x50, 0x00, 0x00, 0x03, 0x00, 0x03, 0x00, 0x00,

  /* 0x9a - ������������� - ������� �������������� ����� � ������� ������� �� ������ �����.*/
  0x50, 0x50, 0x5f, 0x40, 0x5f, 0x50, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

  /* 0x9b - ������������� - ������� �������������� ����� � ������� ������� �� ������ ����.*/
  0x50, 0x50, 0xd0, 0x10, 0xd0, 0x50, 0x50, 0x00, 0x00, 0x0f, 0x00, 0x0f, 0x00, 0x00,

  /* 0x9c - ������������� - ������� ������������ ����� � ������� ������� �� ������ ������.*/
  0x00, 0x00, 0xff, 0x00, 0xdf, 0x50, 0x50, 0x00, 0x00, 0x0f, 0x00, 0x0f, 0x00, 0x00,

  /* 0x9d - ������������� - ������� �������������� ����� �� ������.*/
  0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

  /* 0x9e - ������������� - ������� �����������.*/
  0x50, 0x50, 0xdf, 0x00, 0xdf, 0x50, 0x50, 0x00, 0x00, 0x0f, 0x00, 0x0f, 0x00, 0x00,

  /* 0x9f - ������������� - ������� �������������� ����� � ������� �� ������ �����.*/
  0x50, 0x50, 0x50, 0x5f, 0x50, 0x50, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

  /* 0xa0 - ������������� - �������������� ����� � ������� ������� �� ������ �����.*/
  0x20, 0x20, 0x3f, 0x20, 0x3f, 0x20, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

  /* 0xa1 - ������������� - ������� �������������� ����� � ������� �� ������ ����.*/
  0x50, 0x50, 0x50, 0xd0, 0x50, 0x50, 0x50, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00,

  /* 0xa2 - ������������� - �������������� ����� � ������� ������� �� ������ ����.*/
  0x20, 0x20, 0xe0, 0x20, 0xe0, 0x20, 0x20, 0x00, 0x00, 0x0f, 0x00, 0x0f, 0x00, 0x00,

  /* 0xa3 - ������������� - ������ ����� ����, ������� ������������ �����.*/
  0x00, 0x00, 0x3f, 0x20, 0x3f, 0x20, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

  /* 0xa4 - ������������� - ������ ����� ����, ������� �������������� �����.*/
  0x00, 0x00, 0x00, 0x7f, 0x50, 0x50, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

  /* 0xa5 - ������������� - ������� ����� ����, ������� �������������� �����.*/
  0x00, 0x00, 0x00, 0xf0, 0x50, 0x50, 0x50, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00,

  /* 0xa6 - ������������� - ������� ����� ����, ������� ������������ �����.*/
  0x00, 0x00, 0xe0, 0x20, 0xe0, 0x20, 0x20, 0x00, 0x00, 0x0f, 0x00, 0x0f, 0x00, 0x00,

  /* 0xa7 - ������������� - ������� ������������ ����� �� ������ � �������� ������ � �����.*/
  0x20, 0x20, 0xff, 0x00, 0xff, 0x20, 0x20, 0x00, 0x00, 0x0f, 0x00, 0x0f, 0x00, 0x00,

  /* 0xa8 - '�'.*/
  0x00, 0xff, 0xff, 0x12, 0x13, 0x03, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00,

  /* 0xa9 - ������������� - ������� �������������� ����� �� ������ � �������� ����� � ����.*/
  0x50, 0x50, 0x50, 0xdf, 0x50, 0x50, 0x50, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00,

  /* 0xaa - ������������� - ������ ������ ����.*/
  0x20, 0x20, 0x20, 0x3f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

  /* 0xab - ������������� - ������� ����� ����.*/
  0x00, 0x00, 0x00, 0xe0, 0x20, 0x20, 0x20, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00,

  /* 0xac - ������������� - ����������� ����������.*/
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,

  /* 0xad - ������������� - ����������� ������ �������� ����������.*/
  0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,

  /* 0xae - ������������� - ����������� ����� �������� ����������.*/
  0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x03, 0x03, 0x03, 0x03, 0x00, 0x00, 0x00,

  /* 0xaf - ������������� - ����������� ������ �������� ����������.*/
  0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x03, 0x03, 0x03,

  /* 0xb0 - ������������� - ����������� ������� �������� ����������.*/
  0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

  /* 0xb1 - ���������� '�'.*/
  0x00, 0xfe, 0xff, 0x11, 0x93, 0x82, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x00, 0x00,

  /* 0xb2 - ���������� '�'.*/
  0x00, 0xf0, 0xf8, 0x28, 0x28, 0x08, 0x90, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x00,

  /* 0xb3 - 'I' � ����� ������� ������.*/
  0x00, 0x01, 0x01, 0xfc, 0xfc, 0x01, 0x01, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x00,

  /* 0xb4 - 'i' � ����� ������� ������.*/
  0x00, 0x03, 0x0b, 0xf8, 0xfb, 0x03, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x00,

  /* 0xb5 - 'Y' � ������� ������ ('~').*/
/*  0x00, 0x9c, 0xbd, 0x22, 0xfd, 0xfc, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x00, 0x00,*/

  /* 0xb5 - ������, ������������ �� ������ ��������� ����� "(*)".*/
  0x00, 0x78, 0x84, 0x30, 0x30, 0x84, 0x78, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

  /* 0xb6 - 'y' � ������� ������ ('~').*/
  0x00, 0x78, 0xfa, 0x84, 0xfa, 0x78, 0x00, 0x00, 0x04, 0x06, 0x03, 0x01, 0x00, 0x00,

  /* 0xb7 - ��������� ������ ������.*/
  0x00, 0x0e, 0x1f, 0x11, 0x1f, 0x0e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

  /* 0xb8 - '�'.*/
  0x00, 0xf3, 0xfb, 0x28, 0x3b, 0xb3, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x00, 0x00,

  /* 0xb9 - ������� ����������� ���� �� ������.*/
  0x00, 0x00, 0x38, 0x7c, 0x7c, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

  /* 0xba - ��������� ����������� ���� �� ������.*/
  0x00, 0x00, 0x00, 0x30, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

  /* 0xbb - ������ ����� �����������.*/
  0x00, 0x40, 0x80, 0x00, 0xfe, 0x02, 0x02, 0x00, 0x00, 0x00, 0x01, 0x01, 0x00, 0x00,

  /* 0xbc - '�'.*/
  0xff, 0xff, 0x1c, 0xff, 0xff, 0x27, 0x27, 0x01, 0x01, 0x00, 0x01, 0x01, 0x00, 0x00,

  /* 0xbd - "������".*/
  0x74, 0x88, 0x04, 0x04, 0x88, 0x74, 0x00, 0x01, 0x00, 0x01, 0x01, 0x00, 0x01, 0x00,

  /* 0xbe - ����������� ������� �� ������.*/
  0x00, 0x00, 0x78, 0x78, 0x78, 0x78, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

  /* 0xbf - ������ ����������.*/
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

  /* 0xc0 - '�'.*/
  0x00, 0xfc, 0xfe, 0x23, 0xfe, 0xfc, 0x00, 0x00, 0x01, 0x01, 0x00, 0x01, 0x01, 0x00,

  /* 0xc1 - '�'.*/
  0x00, 0xff, 0xff, 0x11, 0xf1, 0xe1, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00,

  /* 0xc2 - '�'.*/
  0x00, 0xff, 0xff, 0x11, 0xff, 0xee, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00,

  /* 0xc3 - '�'.*/
  0x00, 0xff, 0xff, 0x01, 0x01, 0x01, 0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00,

  /* 0xc4 - '�'.*/
  0x00, 0xfe, 0xff, 0x01, 0xff, 0xff, 0x00, 0x03, 0x03, 0x01, 0x01, 0x01, 0x03, 0x03,

  /* 0xc5 - '�'.*/
  0x00, 0xff, 0xff, 0x11, 0x11, 0x01, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00,

  /* 0xc6 - '�'.*/
  0x00, 0xef, 0x10, 0xff, 0xff, 0x10, 0xef, 0x00, 0x01, 0x00, 0x01, 0x01, 0x00, 0x01,

  /* 0xc7 - '�'.*/
  0x00, 0x82, 0x11, 0x11, 0xff, 0xee, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x00, 0x00,

  /* 0xc8 - '�'.*/
  0x00, 0xff, 0xff, 0x70, 0x1c, 0xff, 0xff, 0x00, 0x01, 0x01, 0x00, 0x00, 0x01, 0x01,

  /* 0xc9 - '�'.*/
  0x00, 0xff, 0xff, 0x71, 0x1d, 0xff, 0xff, 0x00, 0x01, 0x01, 0x00, 0x00, 0x01, 0x01,

  /* 0xca - '�'.*/
  0x00, 0xff, 0xff, 0x38, 0xef, 0xc7, 0x00, 0x00, 0x01, 0x01, 0x00, 0x01, 0x01, 0x00,

  /* 0xcb - '�'.*/
  0x00, 0xfe, 0xff, 0x01, 0xff, 0xff, 0x00, 0x00, 0x01, 0x01, 0x00, 0x01, 0x01, 0x00,

  /* 0xcc - '�'.*/
  0xff, 0xff, 0x1c, 0x70, 0x1c, 0xff, 0xff, 0x01, 0x01, 0x00, 0x00, 0x00, 0x01, 0x01,

  /* 0xcd - '�'.*/
  0x00, 0xff, 0xff, 0x10, 0xff, 0xff, 0x00, 0x00, 0x01, 0x01, 0x00, 0x01, 0x01, 0x00,

  /* 0xce - '�'.*/
  0x00, 0xfe, 0xff, 0x01, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x00, 0x00,

  /* 0xcf - '�'.*/
  0x00, 0xff, 0xff, 0x01, 0xff, 0xff, 0x00, 0x00, 0x01, 0x01, 0x00, 0x01, 0x01, 0x00,

  /* 0xd0 - '�'.*/
  0x00, 0xff, 0xff, 0x11, 0x1f, 0x0e, 0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00,

  /* 0xd1 - '�'.*/
  0x00, 0xfe, 0xff, 0x01, 0xc7, 0xc6, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x00, 0x00,

  /* 0xd2 - '�'.*/
  0x00, 0x01, 0x01, 0xff, 0xff, 0x01, 0x01, 0x00, 0x00, 0x00, 0x01, 0x01, 0x00, 0x00,

  /* 0xd3 - '�'.*/
  0x00, 0x8f, 0x9f, 0x10, 0xff, 0xff, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x00, 0x00,

  /* 0xd4 - '�'.*/
  0x00, 0x3c, 0x42, 0xff, 0xff, 0x42, 0x3c, 0x00, 0x00, 0x00, 0x01, 0x01, 0x00, 0x00,

  /* 0xd5 - '�'.*/
  0x00, 0xc7, 0xef, 0x38, 0xef, 0xc7, 0x00, 0x00, 0x01, 0x01, 0x00, 0x01, 0x01, 0x00,

  /* 0xd6 - '�'.*/
  0x00, 0xff, 0xff, 0x00, 0xff, 0xff, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x03, 0x03,

  /* 0xd7 - '�'.*/
  0x00, 0x0f, 0x1f, 0x10, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x00,

  /* 0xd8 - '�'.*/
  0xff, 0xff, 0x00, 0xff, 0x00, 0xff, 0xff, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,

  /* 0xd9 - '�'.*/
  0xff, 0xff, 0x00, 0xff, 0x00, 0xff, 0xff, 0x01, 0x01, 0x01, 0x01, 0x01, 0x03, 0x03,

  /* 0xda - '�'.*/
  0x07, 0x01, 0xff, 0xff, 0x10, 0xf0, 0xe0, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x00,

  /* 0xdb - '�'.*/
  0xff, 0xff, 0x10, 0xf0, 0xe0, 0xff, 0xff, 0x01, 0x01, 0x01, 0x01, 0x00, 0x01, 0x01,

  /* 0xdc - '�'.*/
  0x00, 0xff, 0xff, 0x10, 0xf0, 0xe0, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00,

  /* 0xdd - '�'.*/
  0x00, 0x82, 0x93, 0x11, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x00, 0x00,

  /* 0xde - '�'.*/
  0xff, 0xff, 0x18, 0xff, 0x01, 0xff, 0xfe, 0x01, 0x01, 0x00, 0x01, 0x01, 0x01, 0x00,

  /* 0xdf - '�'.*/
  0x00, 0xee, 0xff, 0x11, 0xff, 0xff, 0x00, 0x00, 0x01, 0x01, 0x00, 0x01, 0x01, 0x00,

  /* 0xe0 - '�'.*/
  0x00, 0xc0, 0xe8, 0x28, 0xf8, 0xf0, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x00,

  /* 0xe1 - '�'.*/
  0x00, 0xf0, 0xf8, 0x28, 0xe8, 0xc4, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x00, 0x00,

  /* 0xe2 - '�'.*/
  0x00, 0xf8, 0xf8, 0x28, 0xf8, 0xd0, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00,

  /* 0xe3 - '�'.*/
  0x00, 0xf8, 0xf8, 0x08, 0x08, 0x08, 0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00,

  /* 0xe4 - '�'.*/
  0x00, 0xf0, 0xf8, 0x08, 0xf8, 0xf8, 0x00, 0x03, 0x03, 0x01, 0x01, 0x01, 0x03, 0x03,

  /* 0xe5 - '�'.*/
  0x00, 0xf0, 0xf8, 0x28, 0x38, 0xb0, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x00, 0x00,

  /* 0xe6 - '�'.*/
  0x00, 0xd8, 0x20, 0xf8, 0xf8, 0x20, 0xd8, 0x00, 0x01, 0x00, 0x01, 0x01, 0x00, 0x01,

  /* 0xe7 - '�'.*/
  0x00, 0x90, 0x08, 0x28, 0xf8, 0xd0, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x00, 0x00,

  /* 0xe8 - '�'.*/
  0x00, 0xf8, 0xf8, 0xc0, 0x60, 0xf8, 0xf8, 0x00, 0x01, 0x01, 0x00, 0x00, 0x01, 0x01,

  /* 0xe9 - '�'.*/
  0x00, 0xf8, 0xf8, 0xc0, 0x66, 0xfb, 0xf8, 0x00, 0x01, 0x01, 0x00, 0x00, 0x01, 0x01,

  /* 0xea - '�'.*/
  0x00, 0xf8, 0xf8, 0x70, 0xd8, 0x88, 0x00, 0x00, 0x01, 0x01, 0x00, 0x01, 0x01, 0x00,

  /* 0xeb - '�'.*/
  0x00, 0xf0, 0xf8, 0x08, 0xf8, 0xf8, 0x00, 0x00, 0x01, 0x01, 0x00, 0x01, 0x01, 0x00,

  /* 0xec - '�'.*/
  0xf8, 0xf8, 0x18, 0x30, 0x18, 0xf8, 0xf8, 0x01, 0x01, 0x00, 0x00, 0x00, 0x01, 0x01,

  /* 0xed - '�'.*/
  0x00, 0xf8, 0xf8, 0x20, 0xf8, 0xf8, 0x00, 0x00, 0x01, 0x01, 0x00, 0x01, 0x01, 0x00,

  /* 0xee - '�'.*/
  0x00, 0xf0, 0xf8, 0x08, 0xf8, 0xf0, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x00, 0x00,

  /* 0xef - '�'.*/
  0x00, 0xf8, 0xf8, 0x08, 0xf8, 0xf8, 0x00, 0x00, 0x01, 0x01, 0x00, 0x01, 0x01, 0x00,

  /* 0xf0 - '�'.*/
  0x00, 0xf8, 0xf8, 0x08, 0xf8, 0xf0, 0x00, 0x00, 0x07, 0x07, 0x01, 0x01, 0x00, 0x00,

  /* 0xf1 - '�'.*/
  0x00, 0xf0, 0xf8, 0x08, 0x98, 0x90, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x00, 0x00,

  /* 0xf2 - '�'.*/
  0x00, 0x08, 0x08, 0xf8, 0xf8, 0x08, 0x08, 0x00, 0x00, 0x00, 0x01, 0x01, 0x00, 0x00,

  /* 0xf3 - '�'.*/
  0x00, 0x78, 0xf8, 0x80, 0xf8, 0x78, 0x00, 0x00, 0x04, 0x06, 0x03, 0x01, 0x00, 0x00,

  /* 0xf4 - '�'.*/
  0x00, 0xf0, 0x08, 0xf8, 0xf8, 0x08, 0xf0, 0x00, 0x00, 0x01, 0x07, 0x07, 0x01, 0x00,

  /* 0xf5 - '�'.*/
  0x00, 0x98, 0xf8, 0x60, 0xf8, 0x98, 0x00, 0x00, 0x01, 0x01, 0x00, 0x01, 0x01, 0x00,

  /* 0xf6 - '�'.*/
  0x00, 0xf8, 0xf8, 0x00, 0xf8, 0xf8, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x03, 0x03,

  /* 0xf7 - '�'.*/
  0x00, 0x38, 0x78, 0x40, 0xf8, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x00,

  /* 0xf8 - '�'.*/
  0xf8, 0xf8, 0x00, 0xf8, 0x00, 0xf8, 0xf8, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,

  /* 0xf9 - '�'.*/
  0xf8, 0xf8, 0x00, 0xf8, 0x00, 0xf8, 0xf8, 0x01, 0x01, 0x01, 0x01, 0x01, 0x03, 0x03,

  /* 0xfa - '�'.*/
  0x18, 0x18, 0xf8, 0xf8, 0x20, 0xe0, 0xc0, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x00,

  /* 0xfb - '�'.*/
  0xf8, 0xf8, 0x20, 0xe0, 0xc0, 0xf8, 0xf8, 0x01, 0x01, 0x01, 0x01, 0x00, 0x01, 0x01,

  /* 0xfc - '�'.*/
  0x00, 0xf8, 0xf8, 0x20, 0xe0, 0xc0, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00,

  /* 0xfd - '�'.*/
  0x00, 0x90, 0x08, 0x28, 0x28, 0xf8, 0xf0, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x00,

  /* 0xfe - '�'.*/
  0xf8, 0xf8, 0x60, 0xf8, 0x08, 0xf8, 0xf0, 0x01, 0x01, 0x00, 0x01, 0x01, 0x01, 0x00,

  /* 0xff - '�'.*/
  0x00, 0xb0, 0xf8, 0x48, 0xf8, 0xf8, 0x00, 0x00, 0x01, 0x01, 0x00, 0x01, 0x01, 0x00
};


FONT font7x10b_win1251 =
{
   7,                           // Width, pixels
  10,                           // Height, pixels

  2,                            // Symbol row count
  14,                           // Bytes per symbol

  &font7x10b_win1251_data[0],   // Font data address
  3584                          // Font data length
};

/*===========================================================================
 * End of file Font7x10b_win1251.c
 *===========================================================================*/

