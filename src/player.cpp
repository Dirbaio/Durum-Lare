#include "player.h"
#include <SFML/Graphics.hpp>
#include "graphics_engine.h"
#include "defines.h"
#include "input_engine.h"
#include "game_reg.h"
#include "animation.h"
#include <GL/gl.h>


void Player::Init() {

	GraphEng* graphics = GraphEng::getInstance();

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

    myScore = 0;
}

void Player::LoadAnims() {
    AnimationData* ad = new AnimationData();
    ad->Load("anim/takena.anim");
    m_anim = new Animation();
    m_anim->setAnimData(ad);

    //mySpr.setOrigin(animations.getCurrentFrame()->getLocalBounds().width*0.5,
    // animations.getCurrentFrame()->getLocalBounds().height*0.5);
}

void Player::hitAction()
{
    m_actionDelay = 0.18f;
}

void Player::Update() {
	updateBBox();

	InputEng* input = InputEng::getInstance();

	if (m_actionDelay < 0)
	{
        const sf::Vector2f &pos0 = m_position;
        sf::Vector2f posf = m_position;

            bool hasMoved = false;

            if (input->getKeyState(InputEng::PLAYER_UP) && !input->getKeyState(InputEng::PLAYER_DOWN)) {
            posf.y -= myVel.y*input->getFrameTime().asSeconds();
                    ensureAnim("WalkingUp");
                    m_faceDir = FACE_UP;
                    hasMoved = true;
            }
            if (input->getKeyState(InputEng::PLAYER_DOWN) && !input->getKeyState(InputEng::PLAYER_UP)) {
            posf.y += myVel.y*input->getFrameTime().asSeconds();
                    ensureAnim("WalkingDown");
                    m_faceDir = FACE_DOWN;
                    hasMoved = true;
            }
            if (input->getKeyState(InputEng::PLAYER_LEFT) && !input->getKeyState(InputEng::PLAYER_RIGHT)) {
            posf.x -= myVel.x*input->getFrameTime().asSeconds();
                    ensureAnim("WalkingLeft");
                    m_faceDir = FACE_LEFT;
                    hasMoved = true;
            }
            if (input->getKeyState(InputEng::PLAYER_RIGHT) && !input->getKeyState(InputEng::PLAYER_LEFT)) {
            posf.x += myVel.x*input->getFrameTime().asSeconds();
                    ensureAnim("WalkingRight");
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
		m_actionDelay -= input->getFrameTime().asSeconds();

        if (m_faceDir == FACE_UP)  ensureAnim("AttackUp");
        if (m_faceDir == FACE_DOWN)  ensureAnim("AttackDown");
        if (m_faceDir == FACE_LEFT)  ensureAnim("AttackLeft");
        if (m_faceDir == FACE_RIGHT)  ensureAnim("AttackRight");
	}

	if (input->getKeyDown(InputEng::PLAYER_ACTION)) {
		GameReg* gameReg = GameReg::getInstance();
		gameReg->eventQueue.push(new EventPlayerAction());
	}


        m_anim->Update(input->getFrameTime().asSeconds());
}

void Player::Draw() {

        sf::Sprite* spr = m_anim->getCurrentFrame();

        if (spr == NULL) return;

        spr->setPosition(m_position);
        App->draw(*spr);
}


