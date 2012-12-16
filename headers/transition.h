#pragma once

#include <SFML/System.hpp>

class Transition;

struct Transition2D {
        Transition* x;
        Transition* y;
};

class Transition {
public:
        Transition();
        ~Transition(){}

        virtual void setPos(float _pos){pos = posToGo = _pos;}
        virtual void goPos(float _pos){posToGo = _pos;}


        virtual void update(float dTime) = 0;

        virtual float getPos() {return pos;}
        virtual float getGoPos() {return posToGo;}
        virtual bool reached() {return pos == posToGo;}

protected:
        sf::Clock timer;
        float pos;
        float posToGo;

};

class TransitionVelConst : public Transition {
public:
        TransitionVelConst() : vel(1.0f) {}
        ~TransitionVelConst() {}

        void update(float dTime);

        void setVel(float secsTotal) {vel = secsTotal;}

protected:

        float vel;
};

class TransitionLinear : public Transition {
public:

        TransitionLinear(): time(1.0f), percent(1.0f) {}
        ~TransitionLinear() {}

        void setPos(float _pos);
        void goPos(float _pos);
        void update(float dTime);

        void setTime(float timeToReach) {time = timeToReach;}

protected:

        float time;
        float percent;
        float initPos;
};
