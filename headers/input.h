#ifndef INPUT_H
#define INPUT_H

#include "settings.h"


class Input
{
public:
    static Input* pGetInstance();
    bool init();
    ~Input();

    void update();
    bool exit();

    float get_global_time();
    int get_real_time();

    bool get_key_state(unsigned char key); // te indica si una tecla esta apretada o no
    bool get_key_down (unsigned char key); // te indica si una tecla se acaba de apretar
    bool get_key_up   (unsigned char key); // te indica si una tecla se acaba de levantar
    void clear_keys(); // limpia los eventos

private:
    static Input *m_pInstance;
    Settings* m_pSettings;

    Input();

    bool KeyState[K_SIZE];
    bool KeyUp   [K_SIZE];
    bool KeyDown [K_SIZE];

    float m_GlobalTime;
    bool  DoExit;
};


#endif // INPUT_H
