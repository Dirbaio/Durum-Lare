#ifndef EVENT_H
#define EVENT_H

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <set>

class Person;
class Item;

enum EventType {

    EVENT_MOVE,

    EVENT_GAME_OVER,

    EVENT_PLAYER_ACTION,

    EVENT_DELETE_PERSON,
    EVENT_DELETE_ITEM,

    //Total events number
    EVENT_TYPE_SIZE
};

struct Event {
    int type;
    Event(int _type) : type(_type){}
};

struct EventGameOver : public Event {
    EventGameOver() : Event(EVENT_GAME_OVER) {}
};


struct EventPlayerAction : public Event {
    EventPlayerAction() : Event(EVENT_PLAYER_ACTION) {}
};

struct EventDeletePerson : public Event {
    EventDeletePerson(Person* _person) : Event(EVENT_DELETE_PERSON), person(_person) {}

    Person* person;
};

struct EventDeleteItem : public Event {
    EventDeleteItem(Item* _item) : Event(EVENT_DELETE_ITEM), item(_item) {}

    Item* item;
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
