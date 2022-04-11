#pragma once

#include "Arduino.h"

class _Loggr {
  public:
    void add (const char *fn_name, String content) {
      journal += fn_name;
      journal += " - ";
      add(content);
    }

    void add (String content) {
      journal += content;
      journal += '\n';
    }

    String pop() {
      String ret = journal;
      clear();
      return ret;
    }

    String get() {
      return journal;
    }

    void clear() {
      journal = "";
    }

  private:
    String journal;
};

extern _Loggr Loggr;
