#ifndef EVENT_H
#define EVENT_H

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <set>

enum EventType {

    EVENT_MOVE,


    //Total events number
    EVENT_TYPE_SIZE
};

struct Event {
    int type;
    Event(int _type) : type(_type){}
};
/*
//A object changes his position (not residing in map world IE quadtree)
struct EventMove : public Event {
    EventMove(Object* _obj, sf::Vector2f _vel, sf::Vector2f _dir) :
        Event(EVENT_MOVE), obj(_obj), velocity(_vel), direction(_dir) {}

    Object* obj;
    sf::Vector2f velocity;
    sf::Vector2f direction;
};*/

#endif // EVENT_H
