#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "input_engine.h"

#include <iostream>

#include "defines.h"
#include "game.h"

InputEng::InputEng()
{
    KeyMap[PLAYER_UP] = sf::Keyboard::W;
    KeyMap[PLAYER_DOWN] = sf::Keyboard::S;
    KeyMap[PLAYER_LEFT] = sf::Keyboard::A;
    KeyMap[PLAYER_RIGHT] = sf::Keyboard::D;

    KeyMap[CAM_UP] = sf::Keyboard::Up;
    KeyMap[CAM_DOWN] = sf::Keyboard::Down;
    KeyMap[CAM_LEFT] = sf::Keyboard::Left;
    KeyMap[CAM_RIGHT] = sf::Keyboard::Right;

    KeyMap[CAM_ZOOM_IN] = sf::Keyboard::F1;
    KeyMap[CAM_ZOOM_OUT] = sf::Keyboard::F2;

    KeyMap[SHOP_BUY_1] = sf::Keyboard::Num1;
    KeyMap[SHOP_BUY_2] = sf::Keyboard::Num2;
    KeyMap[SHOP_BUY_3] = sf::Keyboard::Num3;

    KeyMap[NEW_SCENE] = sf::Keyboard::Return;

    KeyMap[PLAYER_ACTION] = sf::Keyboard::Space;

    KeyMap[MENU_START] = sf::Keyboard::Return;

    KeyMap[EXIT] = sf::Keyboard::Escape;

    for (int i = 0; i < K_SIZE; ++i)
        KeyState[i] = KeyStateOld[i] = false;

    firstUpdate = true;

}


std::string InputEng::encodeToString() {

    std::string encoded(K_SIZE, ' ');

    for (int i = 0; i < K_SIZE; ++i)
        encoded[i] = KeyState[i] ? '1' : '0';

    return encoded;
}

void InputEng::decodeFromString(std::string encoded) {

    for (int i = 0; i < K_SIZE; ++i)
    {
        KeyStateOld[i] = KeyState[i];
        KeyState[i] = encoded[i] == '1';
    }

    if(firstUpdate)
        for (int i = 0; i < K_SIZE; ++i)
            KeyStateOld[i] = KeyState[i];
    firstUpdate = false;
}

void InputEng::Update()
{
    sf::Event Event;
    while (App->pollEvent(Event)) {

        switch(Event.type) {
        //Window Closed
        case sf::Event::Closed:
            App->close();
            break;

        default:
            //Nothing
            break;
        }
    }

    MousePos = sf::Mouse::getPosition(*App);

    for (int i = 0; i < sf::Mouse::ButtonCount; ++i)
    {
        MouseStateOld[i] = MouseState[i];
        MouseState[i] = sf::Mouse::isButtonPressed((sf::Mouse::Button)i);
    }

    for (int i = 0; i < K_SIZE; i++)
    {
        KeyStateOld[i] = KeyState[i];
        KeyState[i] = sf::Keyboard::isKeyPressed(KeyMap[i]);
    }


    if(firstUpdate)
        for (int i = 0; i < K_SIZE; ++i)
            KeyStateOld[i] = KeyState[i];
    firstUpdate = false;
}

bool InputEng::getKeyState(int key) {
    return KeyState[key];
}

bool InputEng::getKeyUp(int key) {
    return KeyStateOld[key] && !KeyState[key];
}

bool InputEng::getKeyDown(int key) {
    return !KeyStateOld[key] && KeyState[key];
}

bool InputEng::getMouseDown(sf::Mouse::Button button) {
    return !MouseStateOld[button] && MouseState[button];
}

bool InputEng::getMouseUp(sf::Mouse::Button button) {
    return MouseStateOld[button] && !MouseState[button];
}

sf::Vector2f InputEng::getMousePos() {
    return App->convertCoords(MousePos);
}

sf::Vector2f InputEng::getGlobalMousePos() {
    return globalMousePos;
}

void InputEng::setGlobalMousePos(const sf::Vector2f& pos) {
    globalMousePos = pos;
}


sf::Time InputEng::getFrameTime() {
    return global_frametime;
}
