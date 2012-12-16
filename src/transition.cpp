#include "transition.h"
#include "input_engine.h"

#include <iostream>

//--TRANSITION-------------------------------
Transition::Transition() {
        pos = 0;
        posToGo = 0;
}

//--TRANSITION-CONSTANT-VELOCITY--------------
void TransitionVelConst::update(float dTime) {

        float distLeft = pos - posToGo;
        if (distLeft < 0) {
                float distToMove = -vel*dTime;
                if (distLeft >= distToMove) distLeft = 0;
                else distLeft -= distToMove;
        }
        else if (distLeft > 0) {
                float distToMove = vel * dTime;
                if (distLeft <= distToMove) distLeft = 0;
                else distLeft -= distToMove;
        }

        pos = distLeft + posToGo;
}

//--TRANSITION-LINEAR------------------------
void TransitionLinear::setPos(float _pos) {
        Transition::setPos(_pos);
        percent = 1.0f;
}

void TransitionLinear::goPos(float _pos) {
        percent = 0.0f;
        initPos = pos;
        Transition::goPos(_pos);
}

void TransitionLinear::update(float dTime) {

        if (percent < 1.0f) {
                percent += dTime/time;
                if (percent >= 1.0f) {
                        percent = 1.0f;
                        pos = posToGo;
                }
                else pos =
                        initPos * (1.0f-percent) +
                        posToGo * (percent);
        }

}
