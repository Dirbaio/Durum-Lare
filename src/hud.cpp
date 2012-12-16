#include "hud.h"
#include "graphics_engine.h"
#include "defines.h"
#include "utils.h"

void Hud::Init() {

    GraphEng* graphics = GraphEng::getInstance();

    moneyHud.setTexture( *graphics->getTexture("img/hud/money.png") );

}

void Hud::Draw() {

    sf::Vector2u scr_size = App->getSize();

    moneyHud.setPosition(scr_size.x*0.1f, scr_size.y*0.1f);
    App->draw(moneyHud);

    sf::Text scoreText;
    scoreText.setString(Utils::intToString(25));
    while (scoreText.getString().getSize() < 5)
        scoreText.setString("0"+scoreText.getString());
    scoreText.setColor(sf::Color::Green);
    scoreText.setPosition(moneyHud.getPosition().x + 32, moneyHud.getPosition().y-16);
    App->draw(scoreText);


}
