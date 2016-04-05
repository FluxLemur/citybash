#include "event_manager.h"

struct event_base *EventManager::base;
struct event *EventManager::end_game_event;

void EventManager::init() {
  base = event_base_new();
}

void EventManager::trigger_end_game() {
  struct timeval tv = {0,0};
  event_add(end_game_event, &tv);
}
