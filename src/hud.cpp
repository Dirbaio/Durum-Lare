#include "hud.h"
#include "graphics_engine.h"
#include "defines.h"
#include "utils.h"

#include "game_reg.h"


void Hud::Init() {

    GraphEng* graphics = GraphEng::getInstance();

    moneyHud.setTexture( *graphics->getTexture("img/hud/money.png") );
    moneyHud.scale(2.0f, 2.0f);

    killsHud.setTexture( *graphics->getTexture("img/hud/kills.png") );
    killsHud.scale(4.0f, 4.0f);

    hudFont.loadFromFile("fonts/BitDarling.TTF");
}

void Hud::Draw() {

    GameReg* gameReg = GameReg::getInstance();
    sf::Vector2u scr_size = App->getSize();

    moneyHud.setPosition(scr_size.x*0.05f, scr_size.y*0.05f);
    App->draw(moneyHud);

    sf::Text scoreText;
    scoreText.setFont(hudFont);
    scoreText.setString(Utils::intToString( gameReg->player->getScore() ));
    while (scoreText.getString().getSize() < 5)
        scoreText.setString("0"+scoreText.getString());
    scoreText.setColor(sf::Color::Green);
    scoreText.setPosition(moneyHud.getPosition().x + 48, moneyHud.getPosition().y);
    App->draw(scoreText);


    killsHud.setPosition(scr_size.x - scr_size.x*0.1f, scr_size.y*0.05f);
    App->draw(killsHud);

    sf::Text killsText;
    killsText.setFont(hudFont);
    killsText.setString(Utils::intToString( gameReg->player->getKills()));
    while (killsText.getString().getSize() < 2)
        killsText.setString("0"+killsText.getString());
    killsText.setColor(sf::Color::Green);
    killsText.setPosition(killsHud.getPosition().x + 48, killsHud.getPosition().y + 16);
    App->draw(killsText);

}
