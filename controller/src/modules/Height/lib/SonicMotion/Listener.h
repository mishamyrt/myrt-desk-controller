// Copyright 2022, Mikhael Khrustik <misha@myrt.co>
//
// All components of Myrt Desk Firmware are licensed under the BSD 3-Clause
// License. See LICENSE.txt for details.

#pragma once

class MotionListener {
 public:
  virtual void onUpMotion() = 0;
  virtual void onDownMotion() = 0;
  virtual void onHoldMotion() = 0;
  virtual void onMotionEnd(size_t distance) = 0;
};
