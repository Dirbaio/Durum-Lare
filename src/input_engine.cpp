#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "input_engine.h"

#include <iostream>

#include "defines.h"
#include "game.h"

InputEng* InputEng::m_pInstance = NULL;

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

    KeyMap[EXIT] = sf::Keyboard::Escape;

}

InputEng::~InputEng() { m_pInstance = NULL; }

InputEng* InputEng::getInstance() {

    if(m_pInstance == NULL)
        m_pInstance = new InputEng();

    return m_pInstance;

}


void InputEng::Update() {

    for (int i = 0; i < K_SIZE; i++)
        KeyUp[i] = KeyDown[i] = false;


    for (int i = 0; i < sf::Mouse::ButtonCount; ++i)
        MouseUp[i] = MouseDown[i] = false;


    sf::Event Event;
    while (App->pollEvent(Event)) {

        switch(Event.type) {
        //Window Closed
        case sf::Event::Closed:
            App->close();
            break;

            //KeyRelased
        case sf::Event::KeyReleased:
            for (int i = 0; i < K_SIZE; i++) {
                if (Event.key.code == KeyMap[i]) {
                    //std::cerr << "Key released: " << i << std::endl;
                    if(KeyState[i]) KeyUp[i] = true;
                    //KeyState[i] = false;
                }
            }
            break;

            //MouseButtonReleased
        case sf::Event::MouseButtonReleased:
            if (MouseState[(int)Event.mouseButton.button])
                MouseUp[(int)Event.mouseButton.button] = true;
            break;

            //MouseButtonPressed
        case sf::Event::MouseButtonPressed:
            if (!MouseState[(int)Event.mouseButton.button])
                MouseDown[(int)Event.mouseButton.button] = true;
            break;

            //KeyPressed
        case sf::Event::KeyPressed:
            for (int i = 0; i < K_SIZE; i++)
                if (Event.key.code == KeyMap[i])
                    if(!KeyState[i]) KeyDown[i] = true;
            break;

            //Resize
                        case sf::Event::Resized:
                                //Ignoring this because it causes trouble
            //App->setSize(Event.Size.width, Event.Size.height);
            break;

        default:
            //Nothing
            break;

        }
    }

    MousePos = sf::Mouse::getPosition(*App);

    for (int i = 0; i < sf::Mouse::ButtonCount; ++i)
        MouseState[i] = sf::Mouse::isButtonPressed((sf::Mouse::Button)i);

    for (int i = 0; i < K_SIZE; i++)
        KeyState[i] = sf::Keyboard::isKeyPressed(KeyMap[i]);

    //Chivato para saber estado de teclas
    /*
        std::cerr << "//";
        for (int i = 0; i < K_SIZE; i++)
                std::cerr << "Key 'i': " << i << " " << KeyState[i] << "  //";

        std::cerr << std::endl;
        */


}

bool InputEng::getKeyState(int key) {
    //std::cerr << "Con key- " << key <<"__KeyState de " << KeyMap[key] << " es: " << KeyState[key] << std::endl;
    return KeyState[key];
}

bool InputEng::getKeyUp(int key) {
    return KeyUp[key];
}

bool InputEng::getKeyDown(int key) {
    return KeyDown[key];
}

bool InputEng::getMouseDown(sf::Mouse::Button button) {
    return MouseDown[button];
}

bool InputEng::getMouseUp(sf::Mouse::Button button) {
    return MouseUp[button];
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
