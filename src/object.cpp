#include "object.h"
#include "input_engine.h"
#include "graphics_engine.h"
#include "utils.h"

const sf::Vector2i dirInc[] = {
	sf::Vector2i(0,-1),
	sf::Vector2i(0, 1),
	sf::Vector2i(-1,0),
	sf::Vector2i( 1,0)
};

int Object::idCount = 0;

Object::Object(GameScene* sc) :
    scene(sc),
	uniqueID(idCount++),
	m_center(sf::Vector2f(0,0)),
	m_rotation(0),
	m_scale(sf::Vector2f(1.0f, 1.0f)),
	m_canCollide(false),
	m_isTakeable(false),
	m_isUsable(false),
	m_isTaken(false)
{
    //uniqueID = Utils::getNewUniqueID();

	m_toBeDeleted = false;
    m_isTakeable = false;
    m_isUsable = false;
    m_isTaken = false;
    m_canCollide = false;

    //myTransitionX = NULL;
    //myTransitionY = NULL;

    m_boundbox.width = 16;
    m_boundbox.height = 16;

    m_faceDir = FACE_RIGHT;

    m_anim = NULL;
    m_prio = 0;
}


void Object::Draw() {
    sf::Sprite* spr = m_anim->getCurrentFrame();

    if (spr == NULL) return;

    //spr->setOrigin(sf::Vector2f(16, 32));
    spr->setPosition(m_position);
    sf::Texture::bind(spr->getTexture());
    //sf::Texture::bind(NULL);
    vec3 pos = Utils::to3(m_position);
    float tx = spr->getTextureRect().width;
    float ty = spr->getTextureRect().height;
//    tx = ty = 500;
    glColor3f(1, 1, 1);

    glMatrixMode(GL_TEXTURE);
    glLoadIdentity();
    glScalef(1.0f/spr->getTexture()->getSize().x, 1.0f/spr->getTexture()->getSize().y, 1.0f);

    glMatrixMode(GL_MODELVIEW);

    glBegin(GL_QUADS);
    sf::IntRect r = spr->getTextureRect();
    glTexCoord2f(r.left, r.top+r.height);
    glVertex3f(pos.x-tx/2, pos.y, pos.z);
    glTexCoord2f(r.left, r.top);
    glVertex3f(pos.x-tx/2, pos.y+ty, pos.z);
    glTexCoord2f(r.left+r.width, r.top);
    glVertex3f(pos.x+tx/2, pos.y+ty, pos.z);
    glTexCoord2f(r.left+r.width, r.top+r.height);
    glVertex3f(pos.x+tx/2, pos.y, pos.z);
    glEnd();
//    App->draw(*spr);

/*
    if(m_jailed)
    {
        float lol = 1-m_jailedTime*5;
        if(lol < 0) lol = 0;
        m_jailSpr.setScale(1+lol, 1-lol);
        m_jailSpr.setPosition(m_position);
        App->draw(m_jailSpr);
    }*/
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
        m_boundbox.top = m_position.y - m_boundbox.height;
}

void Object::ensureAnim(std::string anim) {
    if (currentAnim != anim) {
       m_anim->SelectAnim(anim);
       currentAnim = anim;
    }

}
