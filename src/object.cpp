#include "object.h"
#include "input_engine.h"
#include "graphics_engine.h"
#include "utils.h"

Object::Object() :
	m_center(sf::Vector2f(0,0)),
	m_rotation(0),
	m_scale(sf::Vector2f(1.0f, 1.0f)),
	m_canCollide(false),
	m_isTakeable(false),
	m_isUsable(false),
	m_isTaken(false)
{
    //uniqueID = Utils::getNewUniqueID();

    m_isTakeable = false;
    m_isUsable = false;
    m_isTaken = false;
    m_canCollide = false;

    //myTransitionX = NULL;
    //myTransitionY = NULL;

    m_boundbox.width = 16;
    m_boundbox.height = 16;

    m_anim = NULL;
    m_prio = 0;
}


/*
void Object::setDrawTransition(Transition* tx, Transition* ty) {
        cancelTransition();
        myTransitionX = tx;
        myTransitionY = ty;
}*/
/*
void Object::checkTransition() {
        InputEng* input = InputEng::getInstance();

        if (myTransitionX != NULL) {
                myTransitionX->update(input->getFrameTime().asSeconds());

                this->setPosition(sf::Vector2f(myTransitionX->getPos(), m_position.y));

                if (myTransitionX->reached()) {
                        delete myTransitionX;
                        myTransitionX = NULL;
                }
        }

        if (myTransitionY != NULL) {
                myTransitionY->update(input->getFrameTime().asSeconds());

                this->setPosition(sf::Vector2f(m_position.x, myTransitionY->getPos()));

                if (myTransitionY->reached()) {
                        delete myTransitionY;
                        myTransitionY = NULL;
                }
        }

}

void Object::cancelTransition() {
        if (myTransitionX != NULL) {
                this->setPosition(sf::Vector2f(myTransitionX->getGoPos(),m_position.y));
                delete myTransitionX;
                myTransitionX = NULL;
        }

        if (myTransitionY != NULL) {
                this->setPosition(sf::Vector2f(m_position.x, myTransitionY->getGoPos()));
                delete myTransitionY;
                myTransitionY = NULL;
        }
}
*/
void Object::updateBBox() {

        m_boundbox.left = m_position.x - m_boundbox.width*0.5f;
        m_boundbox.top = m_position.y - m_boundbox.height*0.5f;
}
