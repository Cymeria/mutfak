#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>

class Timer {
  private:
    unsigned long duration;
    unsigned long startTime;
    bool active;
    uint8_t melodyIndex;

  public:
    Timer(unsigned long duration = 0);
    void setDuration(unsigned long d);
    void start();
    void stop();
    void reset();
    void update();
    bool isDone();
    bool isActive();
    void setMelody(uint8_t index);
    unsigned long getRemaining();
    uint8_t getMelody() { return melodyIndex; }
};

#endif
