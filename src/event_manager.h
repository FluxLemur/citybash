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
    static void init();
};

#endif // EVENT_MANAGER_H
