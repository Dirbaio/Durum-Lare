#ifndef INPUTENGINE_H
#define INPUTENGINE_H

#include <SFML/Graphics.hpp>
#include "defines.h"

class InputEng
{
public:
        enum Key
        {
                PLAYER_UP, PLAYER_DOWN, PLAYER_LEFT, PLAYER_RIGHT,

                K_SIZE
        };

        static InputEng* getInstance();
        InputEng();
        ~InputEng();

        void Update();

        bool getKeyState(int key);
        bool getKeyUp(int key);
        bool getKeyDown(int key);

        bool isKeyDown(int key);

        bool getMouseDown(sf::Mouse::Button button);
        bool getMouseUp(sf::Mouse::Button button);
        sf::Vector2f getMousePos();
                sf::Vector2f getGlobalMousePos();
                void setGlobalMousePos(const sf::Vector2f& pos);

        float getFrameTime();


private:
        static InputEng *m_pInstance;

        bool KeyState[K_SIZE];
        bool KeyUp[K_SIZE];
        bool KeyDown[K_SIZE];
        sf::Keyboard::Key KeyMap[K_SIZE];

        bool MouseUp[sf::Mouse::ButtonCount];
        bool MouseDown[sf::Mouse::ButtonCount];
        bool MouseState[sf::Mouse::ButtonCount];

        sf::Vector2i MousePos; //Window coords
                sf::Vector2f globalMousePos; //World coords
};

#endif // INPUTENGINE_H
