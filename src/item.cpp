#include "item.h"
#include "defines.h"
#include "input_engine.h"
#include "game_reg.h"

void Item::Init() {

    transPosX = NULL;
    transPosY = NULL;
    transScale = NULL;

    m_boundbox.width = 8;
    m_boundbox.height = 8;

    flagDie = false;
    dieTimer = 0.0f;

    spawnTimer = 0.5f;
    m_isTakeable = false; //Es falso al principio para que no
                          //se coja hasta que acabe la transicion de spawnear
}


void Item::Update() {

    updateBBox();

    InputEng* input = InputEng::getInstance();
    float delta = input->getFrameTime().asSeconds();

    if (spawnTimer > 0) {
        spawnTimer -= delta;
        if (spawnTimer <= 0) m_isTakeable = true;
    }


    if (flagDie) {
        dieTimer -= delta;
         if (dieTimer <= 0) markForDelete();
    }



    //Update transitions
    if (transPosX != NULL) {
        transPosX->update(delta);
        m_position.x = transPosX->getPos();
        if (transPosX->reached()) {
            delete transPosX;
            transPosX = NULL;
        }

    }

    if (transPosY != NULL) {
        transPosY->update(delta);
        m_position.y = transPosY->getPos();
        if (transPosY->reached()) {
            delete transPosY;
            transPosY = NULL;
        }

    }

    if (transScale != NULL) {
        transScale->update(delta);
        m_scale.x = m_scale.y = transScale->getPos();
        if (transScale->reached()) {
            delete transScale;
            transScale = NULL;
            m_scale.x = 1.0f;
            m_scale.y = 1.0f;
        }
    }



}

void Item::takeAction() {
    m_isTakeable = false;

    flagDie = true;
    dieTimer = 0.2f;
    setTransScale(1.0f, 3.0f, dieTimer);

}

void Item::Draw() {

    mySpr.setPosition(m_position);
    if (transScale != NULL) mySpr.setScale(transScale->getPos(), transScale->getPos());
    else mySpr.setScale(1.0f, 1.0f);
    App->draw(mySpr);

}

void Item::setSprite(sf::Sprite spr) {
    mySpr = spr;
}

void Item::setTransPos(sf::Vector2f from, sf::Vector2f to) {
    if (transPosX != NULL) delete transPosX;
    transPosX = new TransitionLinear();
    transPosX->setPos(from.x);
    transPosX->goPos(to.x);
    transPosX->setTime(0.2f);

    if (transPosY != NULL) delete transPosY;
    transPosY = new TransitionLinear();
    transPosY->setPos(from.y);
    transPosY->goPos(to.y);
    transPosY->setTime(0.2f);

}

void Item::setTransScale(float from, float to, float time) {
    if (transScale != NULL) delete transScale;
    transScale = new TransitionLinear();
    transScale->setPos(from);
    transScale->goPos(to);
    transScale->setTime(time);
}
