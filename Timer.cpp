#include "Timer.h"
#include <Arduino.h>
#include <stdint.h>

Timer::Timer(unsigned long duration) {
  this->duration = duration;
  melodyIndex = 0;
  active = false;
  startTime = 0;
}

void Timer::setDuration(unsigned long d) {
  duration = d;
}

void Timer::start() {
  active = true;
  startTime = millis();
}

void Timer::stop() {
  active = false;
}

void Timer::reset() {
  active = false;
  startTime = 0;
}

void Timer::update() {
  if (active && (millis() - startTime >= duration)) {
    active = false;
  }
}

bool Timer::isDone() {
  return !active && startTime > 0;
}

bool Timer::isActive() {
  return active;
}

void Timer::setMelody(uint8_t index) {
  melodyIndex = index;
}

unsigned long Timer::getRemaining() {
  if (!active) return 0;
  unsigned long elapsed = millis() - startTime;
  return elapsed >= duration ? 0 : duration - elapsed;
}
