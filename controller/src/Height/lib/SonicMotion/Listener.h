#pragma once

class MotionListener {
  public:
    virtual void onUpMotion() = 0;
    virtual void onDownMotion() = 0;
    virtual void onHoldMotion() = 0;
    virtual void onMotionEnd(size_t distance) = 0;
};
