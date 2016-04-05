/*
 * The EventManager is a singleton that abstracts away the event notification
 * system: libevent
 */
#ifndef EVENT_MANAGER_H
#define EVENT_MANAGER_H

#include <event2/event.h>

class EventManager {
  public:
    static struct event_base *base;
    static struct event *end_game_event;

    static void init();
    static void trigger_end_game();
};

#endif // EVENT_MANAGER_H
