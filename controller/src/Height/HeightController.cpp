#include "HeightController.h"

HeightController::HeightController(Bekant *motor, SonicMotion *motion) {
  _motor = motor;
  _motion = motion;
  _motion->setListener(this);
  _state.headroom = 2000;
  _motion->enable();
  _updateHeightLater();
}

bool HeightController::set(size_t height) {
  if (!_motion->disable()) {
    return false;
  }
  if (height == _state.height) {
    return true;
  } else if (height < _state.height) {
    _motor->moveDown();
  } else if (height > _state.height) {
    _motor->moveUp();
  }
  _target_height = height;
  _moving_to_target = true;
  return true;
}

void HeightController::handle() {
  if (_moving_to_target) {
    _updateHeight();
    if ((_motor->getState() == STATE_MOVE_UP && _state.height >= _target_height)
      || (_motor->getState() == STATE_MOVE_DOWN && _state.height <= _target_height)) {
      _motor->stop();
      _moving_to_target = false;
      _motion->enable();
    }
    return;
  }
  if (_should_update && _measure_height.isDone()) {
    _should_update = false;
    _updateHeight();
  }
  _motion->handle();
};

void HeightController::onUpMotion() {
  _motor->moveUp();
}

void HeightController::onDownMotion() {
  _motor->moveDown();
}

void HeightController::onHoldMotion() {
  _motor->stop();
}

void HeightController::onMotionEnd(size_t distance) {
  _motor->stop();
  _updateHeightLater();
}

size_t HeightController::get() {
  return _state.height;
}
