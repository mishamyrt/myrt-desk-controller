// Copyright 2022, Mikhael Khrustik <misha@myrt.co>
//
// All components of Myrt Desk Firmware are licensed under the BSD 3-Clause
// License. See LICENSE.txt for details.

#pragma once

typedef enum {
  MEMORY_TYPE_FLASH = 0x46,
} MemoryType;

typedef enum {
  SYNC_CRC_EOP = 0x20,
  STK_READ_SIGN = 0x75,
  STK_GET_SYNCH = 0x30,
  STK_INSYNC = 0x14,
  STK_OK = 0x10,
  STK_ENTER_PROGMODE = 0x50,
  STK_LOAD_ADDRESS = 0x55,
  STK_PROGRAM_PAGE = 0x64,
  STK_LEAVE_PROGMODE = 0x51,
} OptibootCode;
