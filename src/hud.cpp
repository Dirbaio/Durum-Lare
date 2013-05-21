#include "hud.h"
#include "graphics_engine.h"
#include "utils.h"
#include "shop.h"
#include "item_factory.h"
#include "game_scene.h"

void Hud::Init(GameScene* scene) {

    this->scene = scene;
    GraphEng* graphics = GraphEng::getInstance();

    moneyHud.setTexture( *graphics->getTexture("img/hud/money.png") );
    moneyHud.scale(2.0f, 2.0f);

    killsHud.setTexture( *graphics->getTexture("img/hud/kills.png") );
    killsHud.scale(4.0f, 4.0f);

    shopHud.setTexture( *graphics->getTexture("img/hud/shop.png") );

    //shopHud.setOrigin(shopHud.getTextureRect().width*0.5, shopHud.getTextureRect().height*0.5);
    shopHud.setPosition(App->getSize().x*0.5 - shopHud.getTextureRect().width*0.5,
                        App->getSize().y*0.64);


    hudFont.loadFromFile("fonts/BitDarling.TTF");
}

void Hud::Draw() {

    sf::Vector2u scr_size = App->getSize();

    //Draw Money-Score
    for(int i = 0; i < scene->players.size(); i++)
    {
        Player& player = scene->players[i];
        moneyHud.setPosition(scr_size.x*0.05f, scr_size.y*(0.05f+i*0.1f));
        App->draw(moneyHud);

        sf::Text scoreText;
        scoreText.setFont(hudFont);
        scoreText.setString(Utils::intToString( player.getMoney() ));
        while (scoreText.getString().getSize() < 5)
            scoreText.setString("0"+scoreText.getString());
        scoreText.setColor(sf::Color::White);
        scoreText.setPosition(moneyHud.getPosition().x + 48, moneyHud.getPosition().y);
        App->draw(scoreText);
    }

/*
    //Draw Player Special Items
    const std::set<int>& specItems = gameReg->player->getSpecialItems();
    int i = 0;
    for (std::set<int>::iterator it = specItems.begin(); it != specItems.end(); ++it) {
        Item* item = ItemFactory::MakeNewItem(gameReg->scene, (*it));
        item->setPosition(sf::Vector2f(scr_size.x*0.3 + i*scr_size.x*0.05, scr_size.y*0.05f));
        item->setScale(sf::Vector2f(4,4));
        item->Draw();
        delete item;
        ++i;
    }


    //Draw Kill-Score
    killsHud.setPosition(scr_size.x - scr_size.x*0.1f, scr_size.y*0.05f);
    App->draw(killsHud);
    sf::Text killsText;
    killsText.setFont(hudFont);
    killsText.setString(Utils::intToString( gameReg->player->getKills()));
    while (killsText.getString().getSize() < 2)
        killsText.setString("0"+killsText.getString());
    killsText.setColor(sf::Color::White);
    killsText.setPosition(killsHud.getPosition().x + 48, killsHud.getPosition().y + 16);
    App->draw(killsText);



    //Draw Shop-Hud
    std::list<Shop>& shopList = (*gameReg->shopList);
    for (std::list<Shop>::iterator it = shopList.begin(); it != shopList.end(); ++it) {
        Shop& shop = (*it);

        if (shop.isPlayerNear()) {
            //Rectangle
            App->draw(shopHud);
            sf::Text shopText(shop.getStats().message);
            shopText.setColor(sf::Color::Black);
            shopText.setFont(hudFont);
            shopText.setPosition(shopHud.getPosition().x+shopHud.getTextureRect().width*0.2,
                                 shopHud.getPosition().y+shopHud.getTextureRect().height*0.2);
            //Text message
            App->draw(shopText);

            //Items sold
            for (int i = 0; i < shop.getStats().items.size(); ++i) {

                Item& item = shop.getStats().items[i];
                if (item.getItemType() == ITEM_NONE) continue;

                item.setScale(sf::Vector2f(4, 4));
                item.setPosition(sf::Vector2f(shopHud.getPosition().x+shopHud.getTextureRect().width*0.25*(i+1),
                                  shopHud.getPosition().y+shopHud.getTextureRect().height*0.6));
                item.Draw();
                sf::Text itemValue("$ "+Utils::intToString(item.getValue()));
                itemValue.setColor(sf::Color::Green);
                itemValue.setFont(hudFont);
                itemValue.setPosition(item.getPosition().x - 8,
                                      item.getPosition().y + 24);
                //Text message
                App->draw(itemValue);

            }
        }
    }
*/
}
