#ifndef __1986BE91_SYS_H
#define __1986BE91_SYS_H

#include "cm3_macro.h"

typedef struct {
  unsigned ENABLE                               : 1;
  unsigned TICKINT                              : 1;
  unsigned CLKSOURCE                            : 1;
  unsigned __dummy0                             : 13;
  unsigned COUNTFLAG                            : 1;
  unsigned __dummy1                             : 15;
} _systick_ctrl;

typedef volatile _systick_ctrl _io_systick_ctrl;

#define offs_SYSTICK_CTRL_ENABLE                   0
#define offs_SYSTICK_CTRL_TICKINT                  1
#define offs_SYSTICK_CTRL_CLKSOURCE                2
#define offs_SYSTICK_CTRL_COUNTFLAG                16

#define size_SYSTICK_CTRL_ENABLE                   1
#define size_SYSTICK_CTRL_TICKINT                  1
#define size_SYSTICK_CTRL_CLKSOURCE                1
#define size_SYSTICK_CTRL_COUNTFLAG                1

#define mask_SYSTICK_CTRL_ENABLE                   ((unsigned long)0x00000001)
#define mask_SYSTICK_CTRL_TICKINT                  ((unsigned long)0x00000002)
#define mask_SYSTICK_CTRL_CLKSOURCE                ((unsigned long)0x00000004)
#define mask_SYSTICK_CTRL_COUNTFLAG                ((unsigned long)0x00010000)


typedef struct {
  unsigned TENMS                                : 24;
  unsigned __dummy0                             : 6;
  unsigned SKEW                                 : 1;
  unsigned NOREF                                : 1;
} _systick_calib;

typedef volatile _systick_calib _io_systick_calib;

#define offs_SYSTICK_CALIB_TENMS                     0
#define offs_SYSTICK_CALIB_SKEW                      30
#define offs_SYSTICK_CALIB_NOREF                     31

#define size_SYSTICK_CALIB_TENMS                     24
#define size_SYSTICK_CALIB_SKEW                      1
#define size_SYSTICK_CALIB_NOREF                     1

#define mask_SYSTICK_CALIB_TENMS                     ((unsigned long)0x00FFFFFF)
#define mask_SYSTICK_CALIB_SKEW                      ((unsigned long)0x40000000)
#define mask_SYSTICK_CALIB_NOREF                     ((unsigned long)0x80000000)


typedef struct {
  unsigned PRI_N                                : 8;
  unsigned PRI_N1                               : 8;
  unsigned PRI_N2                               : 8;
  unsigned PRI_N3                               : 8;
} _nvic_ip;

typedef volatile _nvic_ip _io_nvic_ip;

#define offs_NVIC_IP_PRI_N                      0
#define offs_NVIC_IP_PRI_N1                     8
#define offs_NVIC_IP_PRI_N2                     16
#define offs_NVIC_IP_PRI_N3                     24

#define size_NVIC_IP_PRI_N                      8
#define size_NVIC_IP_PRI_N1                     8
#define size_NVIC_IP_PRI_N2                     8
#define size_NVIC_IP_PRI_N3                     8

#define mask_NVIC_IP_PRI_N                      ((unsigned long)0x000000FF)
#define mask_NVIC_IP_PRI_N1                     ((unsigned long)0x0000FF00)
#define mask_NVIC_IP_PRI_N2                     ((unsigned long)0x00FF0000)
#define mask_NVIC_IP_PRI_N3                     ((unsigned long)0xFF000000)


typedef struct {
  unsigned REVISION                             : 4;
  unsigned PARTNO                               : 12;
  unsigned Const_0xF                            : 4;
  unsigned VARIANT                              : 4;
  unsigned IMPLEMENTER                          : 8;
} _sys_cpuid;

typedef volatile _sys_cpuid _io_sys_cpuid;

#define offs_SYS_CPUID_REVISION                 0
#define offs_SYS_CPUID_PARTNO                   4
#define offs_SYS_CPUID_Const_0xF                16
#define offs_SYS_CPUID_VARIANT                  20
#define offs_SYS_CPUID_IMPLEMENTER              24

#define size_SYS_CPUID_REVISION                 4
#define size_SYS_CPUID_PARTNO                   12
#define size_SYS_CPUID_Const_0xF                4
#define size_SYS_CPUID_VARIANT                  4
#define size_SYS_CPUID_IMPLEMENTER              8

#define mask_SYS_CPUID_REVISION                 ((unsigned long)0x0000000F)
#define mask_SYS_CPUID_PARTNO                   ((unsigned long)0x0000FFF0)
#define mask_SYS_CPUID_Const_0xF                ((unsigned long)0x000F0000)
#define mask_SYS_CPUID_VARIANT                  ((unsigned long)0x00F00000)
#define mask_SYS_CPUID_IMPLEMENTER              ((unsigned long)0xFF000000)


typedef struct {
  unsigned VECTACTIVE                           : 9;
  unsigned __dummy0                             : 2;
  unsigned RETTOBASE                            : 1;
  unsigned VECTPENDING                          : 9;
  unsigned __dummy1                             : 1;
  unsigned ISRPENDING                           : 1;
  unsigned ISRPREEMPT                           : 1;
  unsigned __dummy2                             : 1;
  unsigned PENDSTCLR                            : 1;
  unsigned PENDSTSET                            : 1;
  unsigned PENDSVCLR                            : 1;
  unsigned PENDSVSET                            : 1;
  unsigned __dummy3                             : 2;
  unsigned NMIPENDSET                           : 1;
} _sys_icsr;

typedef volatile _sys_icsr _io_sys_icsr;

#define offs_SYS_ICSR_VECTACTIVE                0
#define offs_SYS_ICSR_RETTOBASE                 11
#define offs_SYS_ICSR_VECTPENDING               12
#define offs_SYS_ICSR_ISRPENDING                22
#define offs_SYS_ICSR_ISRPREEMPT                23
#define offs_SYS_ICSR_PENDSTCLR                 25
#define offs_SYS_ICSR_PENDSTSET                 26
#define offs_SYS_ICSR_PENDSVCLR                 27
#define offs_SYS_ICSR_PENDSVSET                 28
#define offs_SYS_ICSR_NMIPENDSET                31

#define size_SYS_ICSR_VECTACTIVE                9
#define size_SYS_ICSR_RETTOBASE                 1
#define size_SYS_ICSR_VECTPENDING               9
#define size_SYS_ICSR_ISRPENDING                1
#define size_SYS_ICSR_ISRPREEMPT                1
#define size_SYS_ICSR_PENDSTCLR                 1
#define size_SYS_ICSR_PENDSTSET                 1
#define size_SYS_ICSR_PENDSVCLR                 1
#define size_SYS_ICSR_PENDSVSET                 1
#define size_SYS_ICSR_NMIPENDSET                1

#define mask_SYS_ICSR_VECTACTIVE                ((unsigned long)0x000001FF)
#define mask_SYS_ICSR_RETTOBASE                 ((unsigned long)0x00000800)
#define mask_SYS_ICSR_VECTPENDING               ((unsigned long)0x001FF000)
#define mask_SYS_ICSR_ISRPENDING                ((unsigned long)0x00400000)
#define mask_SYS_ICSR_ISRPREEMPT                ((unsigned long)0x00800000)
#define mask_SYS_ICSR_PENDSTCLR                 ((unsigned long)0x02000000)
#define mask_SYS_ICSR_PENDSTSET                 ((unsigned long)0x04000000)
#define mask_SYS_ICSR_PENDSVCLR                 ((unsigned long)0x08000000)
#define mask_SYS_ICSR_PENDSVSET                 ((unsigned long)0x10000000)
#define mask_SYS_ICSR_NMIPENDSET                ((unsigned long)0x80000000)


typedef struct {
  unsigned TBLOFF                               : 22;
  unsigned TBLBASE                              : 1;
  unsigned __dummy0                             : 2;
} _sys_vtor;

typedef volatile _sys_vtor _io_sys_vtor;

#define offs_SYS_VTOR_TBLOFF                    0
#define offs_SYS_VTOR_TBLBASE                   22

#define size_SYS_VTOR_TBLOFF                    22
#define size_SYS_VTOR_TBLBASE                   1

#define mask_SYS_VTOR_TBLOFF                    ((unsigned long)0x003FFFFF)
#define mask_SYS_VTOR_TBLBASE                   ((unsigned long)0x00400000)


typedef struct {
  unsigned VECTRESET                            : 1;
  unsigned VECTCLRACTIVE                        : 1;
  unsigned SYSRESETREQ                          : 1;
  unsigned __dummy0                             : 5;
  unsigned PRIGROUP                             : 3;
  unsigned __dummy1                             : 4;
  unsigned ENDIANESS                            : 1;
  unsigned VECTKEYSTAT                          : 16;
} _sys_aircr;

typedef volatile _sys_aircr _io_sys_aircr;

#define offs_SYS_AIRCR_VECTRESET                0
#define offs_SYS_AIRCR_VECTCLRACTIVE            1
#define offs_SYS_AIRCR_SYSRESETREQ              2
#define offs_SYS_AIRCR_PRIGROUP                 8
#define offs_SYS_AIRCR_ENDIANESS                15
#define offs_SYS_AIRCR_VECTKEYSTAT              16

#define size_SYS_AIRCR_VECTRESET                1
#define size_SYS_AIRCR_VECTCLRACTIVE            1
#define size_SYS_AIRCR_SYSRESETREQ              1
#define size_SYS_AIRCR_PRIGROUP                 3
#define size_SYS_AIRCR_ENDIANESS                1
#define size_SYS_AIRCR_VECTKEYSTAT              16

#define mask_SYS_AIRCR_VECTRESET                ((unsigned long)0x00000001)
#define mask_SYS_AIRCR_VECTCLRACTIVE            ((unsigned long)0x00000002)
#define mask_SYS_AIRCR_SYSRESETREQ              ((unsigned long)0x00000004)
#define mask_SYS_AIRCR_PRIGROUP                 ((unsigned long)0x00000700)
#define mask_SYS_AIRCR_ENDIANESS                ((unsigned long)0x00008000)
#define mask_SYS_AIRCR_VECTKEYSTAT              ((unsigned long)0xFFFF0000)


typedef struct {
  unsigned SLEEPONEXIT                          : 1;
  unsigned SLEEPDEEP                            : 1;
  unsigned __dummy0                             : 1;
  unsigned SEVONPEND                            : 1;
  unsigned __dummy1                             : 27;
} _sys_scr;

typedef volatile _sys_scr _io_sys_scr;

#define offs_SYS_SCR_SLEEPONEXIT                0
#define offs_SYS_SCR_SLEEPDEEP                  1
#define offs_SYS_SCR_SEVONPEND                  3

#define size_SYS_SCR_SLEEPONEXIT                1
#define size_SYS_SCR_SLEEPDEEP                  1
#define size_SYS_SCR_SEVONPEND                  1

#define mask_SYS_SCR_SLEEPONEXIT                ((unsigned long)0x00000001)
#define mask_SYS_SCR_SLEEPDEEP                  ((unsigned long)0x00000002)
#define mask_SYS_SCR_SEVONPEND                  ((unsigned long)0x00000008)


typedef struct {
  unsigned NONBASETHRDENA                       : 1;
  unsigned USERSETMPEND                         : 1;
  unsigned __dummy0                             : 1;
  unsigned UNALIGN_TRP                          : 1;
  unsigned DIV_0_TRP                            : 1;
  unsigned __dummy1                             : 3;
  unsigned BFHFNMIGN                            : 1;
  unsigned STKALIGN                             : 1;
  unsigned __dummy2                             : 22;
} _sys_ccr;

typedef volatile _sys_ccr _io_sys_ccr;

#define offs_SYS_CCR_NONBASETHRDENA             0
#define offs_SYS_CCR_USERSETMPEND               1
#define offs_SYS_CCR_UNALIGN_TRP                3
#define offs_SYS_CCR_DIV_0_TRP                  4
#define offs_SYS_CCR_BFHFNMIGN                  8
#define offs_SYS_CCR_STKALIGN                   9

#define size_SYS_CCR_NONBASETHRDENA             1
#define size_SYS_CCR_USERSETMPEND               1
#define size_SYS_CCR_UNALIGN_TRP                1
#define size_SYS_CCR_DIV_0_TRP                  1
#define size_SYS_CCR_BFHFNMIGN                  1
#define size_SYS_CCR_STKALIGN                   1

#define mask_SYS_CCR_NONBASETHRDENA             ((unsigned long)0x00000001)
#define mask_SYS_CCR_USERSETMPEND               ((unsigned long)0x00000002)
#define mask_SYS_CCR_UNALIGN_TRP                ((unsigned long)0x00000008)
#define mask_SYS_CCR_DIV_0_TRP                  ((unsigned long)0x00000010)
#define mask_SYS_CCR_BFHFNMIGN                  ((unsigned long)0x00000100)
#define mask_SYS_CCR_STKALIGN                   ((unsigned long)0x00000200)


typedef struct {
  unsigned PRI_4                                : 8;
  unsigned PRI_5                                : 8;
  unsigned PRI_6                                : 8;
  unsigned PRI_7                                : 8;
} _sys_shpr1;

typedef volatile _sys_shpr1 _io_sys_shpr1;

#define offs_SYS_SHPR1_PRI_4                    0
#define offs_SYS_SHPR1_PRI_5                    8
#define offs_SYS_SHPR1_PRI_6                    16
#define offs_SYS_SHPR1_PRI_7                    24

#define size_SYS_SHPR1_PRI_4                    8
#define size_SYS_SHPR1_PRI_5                    8
#define size_SYS_SHPR1_PRI_6                    8
#define size_SYS_SHPR1_PRI_7                    8

#define mask_SYS_SHPR1_PRI_4                    ((unsigned long)0x000000FF)
#define mask_SYS_SHPR1_PRI_5                    ((unsigned long)0x0000FF00)
#define mask_SYS_SHPR1_PRI_6                    ((unsigned long)0x00FF0000)
#define mask_SYS_SHPR1_PRI_7                    ((unsigned long)0xFF000000)


typedef struct {
  unsigned PRI_8                                : 8;
  unsigned PRI_9                                : 8;
  unsigned PRI_10                               : 8;
  unsigned PRI_11                               : 8;
} _sys_shpr2;

typedef volatile _sys_shpr2 _io_sys_shpr2;

#define offs_SYS_SHPR2_PRI_8                    0
#define offs_SYS_SHPR2_PRI_9                    8
#define offs_SYS_SHPR2_PRI_10                   16
#define offs_SYS_SHPR2_PRI_11                   24

#define size_SYS_SHPR2_PRI_8                    8
#define size_SYS_SHPR2_PRI_9                    8
#define size_SYS_SHPR2_PRI_10                   8
#define size_SYS_SHPR2_PRI_11                   8

#define mask_SYS_SHPR2_PRI_8                    ((unsigned long)0x000000FF)
#define mask_SYS_SHPR2_PRI_9                    ((unsigned long)0x0000FF00)
#define mask_SYS_SHPR2_PRI_10                   ((unsigned long)0x00FF0000)
#define mask_SYS_SHPR2_PRI_11                   ((unsigned long)0xFF000000)


typedef struct {
  unsigned PRI_12                               : 8;
  unsigned PRI_13                               : 8;
  unsigned PRI_14                               : 8;
  unsigned PRI_15                               : 8;
} _sys_shpr3;

typedef volatile _sys_shpr3 _io_sys_shpr3;

#define offs_SYS_SHPR3_PRI_12                   0
#define offs_SYS_SHPR3_PRI_13                   8
#define offs_SYS_SHPR3_PRI_14                   16
#define offs_SYS_SHPR3_PRI_15                   24

#define size_SYS_SHPR3_PRI_12                   8
#define size_SYS_SHPR3_PRI_13                   8
#define size_SYS_SHPR3_PRI_14                   8
#define size_SYS_SHPR3_PRI_15                   8

#define mask_SYS_SHPR3_PRI_12                   ((unsigned long)0x000000FF)
#define mask_SYS_SHPR3_PRI_13                   ((unsigned long)0x0000FF00)
#define mask_SYS_SHPR3_PRI_14                   ((unsigned long)0x00FF0000)
#define mask_SYS_SHPR3_PRI_15                   ((unsigned long)0xFF000000)


typedef struct {
  unsigned MEMFAULTACT                          : 1;
  unsigned BUSFAULTACT                          : 1;
  unsigned __dummy0                             : 1;
  unsigned USGFAULTACT                          : 1;
  unsigned __dummy1                             : 3;
  unsigned SVCALLACT                            : 1;
  unsigned MONITORACT                           : 1;
  unsigned __dummy2                             : 1;
  unsigned PENDSVACT                            : 1;
  unsigned SYSTICKACT                           : 1;
  unsigned USGFAULTPENDED                       : 1;
  unsigned MEMFAULTPENDED                       : 1;
  unsigned BUSFAULTPENDED                       : 1;
  unsigned SVCALLPENDED                         : 1;
  unsigned MEMFAULTENA                          : 1;
  unsigned BUSFAULTENA                          : 1;
  unsigned USGFAULTENA                          : 1;
  unsigned __dummy3                             : 13;
} _sys_shcsr;

typedef volatile _sys_shcsr _io_sys_shcsr;

#define offs_SYS_SHCSR_MEMFAULTACT              0
#define offs_SYS_SHCSR_BUSFAULTACT              1
#define offs_SYS_SHCSR_USGFAULTACT              3
#define offs_SYS_SHCSR_SVCALLACT                7
#define offs_SYS_SHCSR_MONITORACT               8
#define offs_SYS_SHCSR_PENDSVACT                10
#define offs_SYS_SHCSR_SYSTICKACT               11
#define offs_SYS_SHCSR_USGFAULTPENDED           12
#define offs_SYS_SHCSR_MEMFAULTPENDED           13
#define offs_SYS_SHCSR_BUSFAULTPENDED           14
#define offs_SYS_SHCSR_SVCALLPENDED             15
#define offs_SYS_SHCSR_MEMFAULTENA              16
#define offs_SYS_SHCSR_BUSFAULTENA              17
#define offs_SYS_SHCSR_USGFAULTENA              18

#define size_SYS_SHCSR_MEMFAULTACT              1
#define size_SYS_SHCSR_BUSFAULTACT              1
#define size_SYS_SHCSR_USGFAULTACT              1
#define size_SYS_SHCSR_SVCALLACT                1
#define size_SYS_SHCSR_MONITORACT               1
#define size_SYS_SHCSR_PENDSVACT                1
#define size_SYS_SHCSR_SYSTICKACT               1
#define size_SYS_SHCSR_USGFAULTPENDED           1
#define size_SYS_SHCSR_MEMFAULTPENDED           1
#define size_SYS_SHCSR_BUSFAULTPENDED           1
#define size_SYS_SHCSR_SVCALLPENDED             1
#define size_SYS_SHCSR_MEMFAULTENA              1
#define size_SYS_SHCSR_BUSFAULTENA              1
#define size_SYS_SHCSR_USGFAULTENA              1

#define mask_SYS_SHCSR_MEMFAULTACT              ((unsigned long)0x00000001)
#define mask_SYS_SHCSR_BUSFAULTACT              ((unsigned long)0x00000002)
#define mask_SYS_SHCSR_USGFAULTACT              ((unsigned long)0x00000008)
#define mask_SYS_SHCSR_SVCALLACT                ((unsigned long)0x00000080)
#define mask_SYS_SHCSR_MONITORACT               ((unsigned long)0x00000100)
#define mask_SYS_SHCSR_PENDSVACT                ((unsigned long)0x00000400)
#define mask_SYS_SHCSR_SYSTICKACT               ((unsigned long)0x00000800)
#define mask_SYS_SHCSR_USGFAULTPENDED           ((unsigned long)0x00001000)
#define mask_SYS_SHCSR_MEMFAULTPENDED           ((unsigned long)0x00002000)
#define mask_SYS_SHCSR_BUSFAULTPENDED           ((unsigned long)0x00004000)
#define mask_SYS_SHCSR_SVCALLPENDED             ((unsigned long)0x00008000)
#define mask_SYS_SHCSR_MEMFAULTENA              ((unsigned long)0x00010000)
#define mask_SYS_SHCSR_BUSFAULTENA              ((unsigned long)0x00020000)
#define mask_SYS_SHCSR_USGFAULTENA              ((unsigned long)0x00040000)


typedef struct {
  unsigned IACCVIOL                             : 1;
  unsigned DACCVIOL                             : 1;
  unsigned __dummy0                             : 1;
  unsigned MUNSTKERR                            : 1;
  unsigned MSTKERR                              : 1;
  unsigned __dummy1                             : 2;
  unsigned MMARVALID                            : 1;
  unsigned IBUSERR                              : 1;
  unsigned PRECISERR                            : 1;
  unsigned IMPRECISERR                          : 1;
  unsigned UNSTKERR                             : 1;
  unsigned STKERR                               : 1;
  unsigned __dummy2                             : 2;
  unsigned BFARVALID                            : 1;
  unsigned UNDEFINSTR                           : 1;
  unsigned INVSTATE                             : 1;
  unsigned INVPC                                : 1;
  unsigned NOCP                                 : 1;
  unsigned __dummy3                             : 4;
  unsigned UNALIGNED                            : 1;
  unsigned DIVBYZERO                            : 1;
  unsigned __dummy4                             : 6;
} _sys_cfsr;

typedef volatile _sys_cfsr _io_sys_cfsr;

#define offs_SYS_CFSR_IACCVIOL                  0
#define offs_SYS_CFSR_DACCVIOL                  1
#define offs_SYS_CFSR_MUNSTKERR                 3
#define offs_SYS_CFSR_MSTKERR                   4
#define offs_SYS_CFSR_MMARVALID                 7
#define offs_SYS_CFSR_IBUSERR                   8
#define offs_SYS_CFSR_PRECISERR                 9
#define offs_SYS_CFSR_IMPRECISERR               10
#define offs_SYS_CFSR_UNSTKERR                  11
#define offs_SYS_CFSR_STKERR                    12
#define offs_SYS_CFSR_BFARVALID                 15
#define offs_SYS_CFSR_UNDEFINSTR                16
#define offs_SYS_CFSR_INVSTATE                  17
#define offs_SYS_CFSR_INVPC                     18
#define offs_SYS_CFSR_NOCP                      19
#define offs_SYS_CFSR_UNALIGNED                 24
#define offs_SYS_CFSR_DIVBYZERO                 25

#define size_SYS_CFSR_IACCVIOL                  1
#define size_SYS_CFSR_DACCVIOL                  1
#define size_SYS_CFSR_MUNSTKERR                 1
#define size_SYS_CFSR_MSTKERR                   1
#define size_SYS_CFSR_MMARVALID                 1
#define size_SYS_CFSR_IBUSERR                   1
#define size_SYS_CFSR_PRECISERR                 1
#define size_SYS_CFSR_IMPRECISERR               1
#define size_SYS_CFSR_UNSTKERR                  1
#define size_SYS_CFSR_STKERR                    1
#define size_SYS_CFSR_BFARVALID                 1
#define size_SYS_CFSR_UNDEFINSTR                1
#define size_SYS_CFSR_INVSTATE                  1
#define size_SYS_CFSR_INVPC                     1
#define size_SYS_CFSR_NOCP                      1
#define size_SYS_CFSR_UNALIGNED                 1
#define size_SYS_CFSR_DIVBYZERO                 1

#define mask_SYS_CFSR_IACCVIOL                  ((unsigned long)0x00000001)
#define mask_SYS_CFSR_DACCVIOL                  ((unsigned long)0x00000002)
#define mask_SYS_CFSR_MUNSTKERR                 ((unsigned long)0x00000008)
#define mask_SYS_CFSR_MSTKERR                   ((unsigned long)0x00000010)
#define mask_SYS_CFSR_MMARVALID                 ((unsigned long)0x00000080)
#define mask_SYS_CFSR_IBUSERR                   ((unsigned long)0x00000100)
#define mask_SYS_CFSR_PRECISERR                 ((unsigned long)0x00000200)
#define mask_SYS_CFSR_IMPRECISERR               ((unsigned long)0x00000400)
#define mask_SYS_CFSR_UNSTKERR                  ((unsigned long)0x00000800)
#define mask_SYS_CFSR_STKERR                    ((unsigned long)0x00001000)
#define mask_SYS_CFSR_BFARVALID                 ((unsigned long)0x00008000)
#define mask_SYS_CFSR_UNDEFINSTR                ((unsigned long)0x00010000)
#define mask_SYS_CFSR_INVSTATE                  ((unsigned long)0x00020000)
#define mask_SYS_CFSR_INVPC                     ((unsigned long)0x00040000)
#define mask_SYS_CFSR_NOCP                      ((unsigned long)0x00080000)
#define mask_SYS_CFSR_UNALIGNED                 ((unsigned long)0x01000000)
#define mask_SYS_CFSR_DIVBYZERO                 ((unsigned long)0x02000000)


typedef struct {
  unsigned VECTTBL                              : 1;
  unsigned __dummy0                             : 28;
  unsigned FORCED                               : 1;
  unsigned DEBUGEVT                             : 1;
} _sys_hfsr;

typedef volatile _sys_hfsr _io_sys_hfsr;

#define offs_SYS_HFSR_VECTTBL                   0
#define offs_SYS_HFSR_FORCED                    29
#define offs_SYS_HFSR_DEBUGEVT                  30

#define size_SYS_HFSR_VECTTBL                   1
#define size_SYS_HFSR_FORCED                    1
#define size_SYS_HFSR_DEBUGEVT                  1

#define mask_SYS_HFSR_VECTTBL                   ((unsigned long)0x00000001)
#define mask_SYS_HFSR_FORCED                    ((unsigned long)0x20000000)
#define mask_SYS_HFSR_DEBUGEVT                  ((unsigned long)0x40000000)


typedef struct {
  unsigned SEPARATE                             : 1;
  unsigned __dummy0                             : 7;
  unsigned DREGION                              : 8;
  unsigned IREGION                              : 8;
  unsigned __dummy1                             : 8;
} _mpu_type;

typedef volatile _mpu_type _io_mpu_type;

#define offs_MPU_TYPE_SEPARATE                 0
#define offs_MPU_TYPE_DREGION                  8
#define offs_MPU_TYPE_IREGION                  16

#define size_MPU_TYPE_SEPARATE                 1
#define size_MPU_TYPE_DREGION                  8
#define size_MPU_TYPE_IREGION                  8

#define mask_MPU_TYPE_SEPARATE                 ((unsigned long)0x00000001)
#define mask_MPU_TYPE_DREGION                  ((unsigned long)0x0000FF00)
#define mask_MPU_TYPE_IREGION                  ((unsigned long)0x00FF0000)


typedef struct {
  unsigned ENABLE                               : 1;
  unsigned HFNMIENA                             : 1;
  unsigned PRIVDEFENA                           : 1;
  unsigned __dummy0                             : 29;
} _mpu_ctrl;

typedef volatile _mpu_ctrl _io_mpu_ctrl;

#define offs_MPU_CTRL_ENABLE                   0
#define offs_MPU_CTRL_HFNMIENA                 1
#define offs_MPU_CTRL_PRIVDEFENA               2

#define size_MPU_CTRL_ENABLE                   1
#define size_MPU_CTRL_HFNMIENA                 1
#define size_MPU_CTRL_PRIVDEFENA               1

#define mask_MPU_CTRL_ENABLE                   ((unsigned long)0x00000001)
#define mask_MPU_CTRL_HFNMIENA                 ((unsigned long)0x00000002)
#define mask_MPU_CTRL_PRIVDEFENA               ((unsigned long)0x00000004)


typedef struct {
  unsigned REGION                               : 4;
  unsigned VALID                                : 1;
  unsigned ADDR                                 : 27;
} _mpu_rbar;

typedef volatile _mpu_rbar _io_mpu_rbar;

#define offs_MPU_RBAR_REGION                 0
#define offs_MPU_RBAR_VALID                  4
#define offs_MPU_RBAR_ADDR                   5

#define size_MPU_RBAR_REGION                 4
#define size_MPU_RBAR_VALID                  1
#define size_MPU_RBAR_ADDR                   27

#define mask_MPU_RBAR_REGION                 ((unsigned long)0x0000000F)
#define mask_MPU_RBAR_VALID                  ((unsigned long)0x00000010)
#define mask_MPU_RBAR_ADDR                   ((unsigned long)0xFFFFFFE0)


typedef struct {
  unsigned ENABLE                               : 1;
  unsigned SIZE                                 : 5;
  unsigned __dummy0                             : 2;
  unsigned SRD                                  : 8;
  unsigned ATTRS                                : 16;
} _mpu_rasr;

typedef volatile _mpu_rasr _io_mpu_rasr;

#define offs_MPU_RASR_ENABLE                 0
#define offs_MPU_RASR_SIZE                   1
#define offs_MPU_RASR_SRD                    8
#define offs_MPU_RASR_ATTRS                  16

#define size_MPU_RASR_ENABLE                 1
#define size_MPU_RASR_SIZE                   5
#define size_MPU_RASR_SRD                    8
#define size_MPU_RASR_ATTRS                  16

#define mask_MPU_RASR_ENABLE                 ((unsigned long)0x00000001)
#define mask_MPU_RASR_SIZE                   ((unsigned long)0x0000003E)
#define mask_MPU_RASR_SRD                    ((unsigned long)0x0000FF00)
#define mask_MPU_RASR_ATTRS                  ((unsigned long)0xFFFF0000)

typedef struct 
{
    __io_reg __dummy0;
    __io_reg __dummy1;
    __io_reg ACTLR;
    __io_reg __dummy2;
} _sysctrl;

typedef struct
{
  __io_reg CTRL;
  __io_reg LOAD;
  __io_reg VAL;
  __io_reg CALIB;
} _systick;

typedef struct
{
	__io_reg ISER[1];       //0x0000
	__io_reg __dummy0[31];
    __io_reg ICER[1];       //0x0080
	__io_reg __dummy1[31];
    __io_reg ISPR[1];       //0x0100
	__io_reg __dummy2[31];
    __io_reg ICPR[1];       //0x0180
	__io_reg __dummy3[31];
    __io_reg IABR[1];       //0x0200
	__io_reg __dummy4[31];    
    __io_reg __dummy5[32];  //0x0280
    __io_reg IP[8];         //0x0300
} _nvic;

typedef struct
{
	__io_reg CPUID;
    __io_reg ICSR;
    __io_reg VTOR;
    __io_reg AIRCR;
    __io_reg SCR;
    __io_reg CCR;
    __io_reg SHPR[3];    
    __io_reg SHCSR;
    __io_reg CFSR;
    __io_reg HFSR;
    __io_reg __dummy0;
    __io_reg MMFAR;
    __io_reg BFAR;
} _sys;

typedef struct 
{
    __io_reg TYPE;    
    __io_reg CTRL;
    __io_reg RNR;
    __io_reg RBAR;
    __io_reg RASR; 
} _mpu;

#endif /* __1986BE91_SYS_H */
