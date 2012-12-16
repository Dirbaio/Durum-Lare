#include "hud.h"
#include "graphics_engine.h"
#include "defines.h"
#include "utils.h"

void Hud::Init() {

    GraphEng* graphics = GraphEng::getInstance();

    moneyHud.setTexture( *graphics->getTexture("img/hud/money.png") );
    moneyHud.scale(2.0f, 2.0f);

    hudFont.loadFromFile("fonts/BitDarling.TTF");
}

void Hud::Draw() {

    sf::Vector2u scr_size = App->getSize();

    moneyHud.setPosition(scr_size.x*0.05f, scr_size.y*0.05f);
    App->draw(moneyHud);

    sf::Text scoreText;
    scoreText.setFont(hudFont);
    scoreText.setString(Utils::intToString(25));
    while (scoreText.getString().getSize() < 5)
        scoreText.setString("0"+scoreText.getString());
    scoreText.setColor(sf::Color::Green);
    scoreText.setPosition(moneyHud.getPosition().x + 48, moneyHud.getPosition().y);
    App->draw(scoreText);


}
