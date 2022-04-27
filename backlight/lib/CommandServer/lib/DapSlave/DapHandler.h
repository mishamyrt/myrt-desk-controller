// Copyright 2022, Mikhael Khrustik <misha@myrt.co>
//
// All components of Myrt Desk Firmware are licensed under the BSD 3-Clause
// License. See LICENSE.txt for details.

#pragma once

#include "stdint.h"

class DapHandler {
 public:
  virtual bool handleMessage(uint8_t *message, uint8_t message_length) = 0;
};
