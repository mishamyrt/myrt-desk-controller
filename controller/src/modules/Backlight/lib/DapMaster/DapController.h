#pragma once

class DapController {
  public:
    virtual void onError() = 0;
    virtual void onConnect() = 0;
};
