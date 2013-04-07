#include "player.h"
#include <SFML/Graphics.hpp>
#include "graphics_engine.h"
#include "defines.h"
#include "input_engine.h"
#include "animation.h"
#include <GL/gl.h>
#include "game_scene.h"

AnimationData* s_animData = NULL;

void Player::Init() {

	GraphEng* graphics = GraphEng::getInstance();

    m_jailSpr.setTexture(*graphics->getTexture("img/jail.png"));
    m_jailSpr.setOrigin((float) m_jailSpr.getTextureRect().width *0.5f,
                         (float) m_jailSpr.getTextureRect().height*0.5f);

    mySpr.setTexture(*graphics->getTexture("img/player.png"));

	m_position.x = 0;
	m_position.y = 0;

	myVel.x = 64*1.25;
	myVel.y = 64*1.25;

	m_boundbox.height = 16;
	m_boundbox.width = 16;

        LoadAnims();
        ensureAnim("IdleDown");
	m_actionDelay = 0;

    myMoney = 999;
    myKills = 0;

    m_jailed = false;
}

void Player::LoadAnims() {

	if (s_animData == NULL) {
		s_animData = new AnimationData();
		s_animData->Load("anim/takena.anim");
	}

	m_anim = new Animation();
	m_anim->setAnimData(s_animData);

	//mySpr.setOrigin(animations.getCurrentFrame()->getLocalBounds().width*0.5,
	// animations.getCurrentFrame()->getLocalBounds().height*0.5);
}

void Player::hitAction()
{
    m_actionDelay = 0.18f;
}

void Player::onBuy(Item item) {
    if (item.getItemType() == ITEM_SHOE &&
        (m_specialItems.find(ITEM_SHOE) == m_specialItems.end())) {
            myVel.x*= 1.5f;
            myVel.y*= 1.5f;
            m_specialItems.insert(ITEM_SHOE);
    }

    myMoney -= item.getValue();

}

void Player::Update() {
	updateBBox();

    float delta = playerInput.getFrameTime().asSeconds();

    if(m_jailed)
    {
        m_jailedTime += delta;
        return;
    }

	if (m_actionDelay < 0)
	{
        const sf::Vector2f &pos0 = m_position;
        sf::Vector2f posf = m_position;

            bool hasMoved = false;

            if (playerInput.getKeyState(InputEng::PLAYER_UP) && !playerInput.getKeyState(InputEng::PLAYER_DOWN)) {
            posf.y -= myVel.y*delta;
                    ensureAnim("WalkingUp");
                    m_faceDir = FACE_UP;
                    hasMoved = true;
            }
            if (playerInput.getKeyState(InputEng::PLAYER_DOWN) && !playerInput.getKeyState(InputEng::PLAYER_UP)) {
            posf.y += myVel.y*delta;
                    ensureAnim("WalkingDown");
                    m_faceDir = FACE_DOWN;
                    hasMoved = true;
            }
            if (playerInput.getKeyState(InputEng::PLAYER_LEFT) && !playerInput.getKeyState(InputEng::PLAYER_RIGHT)) {
            posf.x -= myVel.x*delta;
		    if(!hasMoved) ensureAnim("WalkingLeft");
                    m_faceDir = FACE_LEFT;
                    hasMoved = true;
            }
            if (playerInput.getKeyState(InputEng::PLAYER_RIGHT) && !playerInput.getKeyState(InputEng::PLAYER_LEFT)) {
            posf.x += myVel.x*delta;
		    if(!hasMoved) ensureAnim("WalkingRight");
                    m_faceDir = FACE_RIGHT;
                    hasMoved = true;
            }

            if (!hasMoved) {
                if (m_faceDir == FACE_UP)  ensureAnim("IdleUp");
                if (m_faceDir == FACE_DOWN)  ensureAnim("IdleDown");
                if (m_faceDir == FACE_LEFT)  ensureAnim("IdleLeft");
                if (m_faceDir == FACE_RIGHT)  ensureAnim("IdleRight");
            }

		Character::move(posf);
	}
	else
	{
        m_actionDelay -= delta;

        if (m_faceDir == FACE_UP)    ensureAnim("AttackUp");
        if (m_faceDir == FACE_DOWN)  ensureAnim("AttackDown");
        if (m_faceDir == FACE_LEFT)  ensureAnim("AttackLeft");
        if (m_faceDir == FACE_RIGHT) ensureAnim("AttackRight");
	}

    if (playerInput.getKeyDown(InputEng::PLAYER_ACTION)) {

        hitAction();
        std::vector<Person*> persons = scene->getPeopleAround(getPosition(), 20, SEARCH_ANY);
        for (std::vector<Person*>::iterator it = persons.begin(); it != persons.end(); ++it) {
            if (!(*it)->is_alive()) continue;

            (*it)->onHit();
            int n_moneys = Utils::randomInt(1, 3);
            for (int i = 0; i < n_moneys; ++i) scene->spawnNewMoney((*it)->getPosition());
        }
	}


    m_anim->Update(delta);

    for (std::list<Item>::iterator it = scene->itemList.begin(); it != scene->itemList.end(); ++it) {
        if (it->isTakeable()) {

            sf::FloatRect moneyBox = this->getBoundBox();
            moneyBox.left -= moneyBox.width/6;
            moneyBox.width *= 3;
            moneyBox.top -= moneyBox.height/6;
            moneyBox.height *= 3;

            if (Utils::rectCollision(it->getBoundBox(), moneyBox)) {
                it->takeAction();
                myMoney += it->getValue();
            }
        }
    }
}

void Player::Draw() {
        sf::Sprite* spr = m_anim->getCurrentFrame();

        if (spr == NULL) return;

        //spr->setOrigin(sf::Vector2f(16, 32));
        spr->setPosition(m_position);
        App->draw(*spr);


        if(m_jailed)
        {
            float lol = 1-m_jailedTime*5;
            if(lol < 0) lol = 0;
            m_jailSpr.setScale(1+lol, 1-lol);
            m_jailSpr.setPosition(m_position);
            App->draw(m_jailSpr);
        }
}

void Player::gotCaught() {
    if(m_jailed) return;

    m_jailed = true;
    m_jailedTime = 0;
}
