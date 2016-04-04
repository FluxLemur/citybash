#include "event_manager.h"

struct event_base * EventManager::base;

void EventManager::init() {
  base = event_base_new();
}
