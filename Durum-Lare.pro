TEMPLATE = app
CONFIG += console
CONFIG -= qt

LIBS += -lsfml-system -lsfml-window -lsfml-graphics -lsfml-audio

SOURCES += src/main.cpp \
    src/game.cpp \
    src/input_engine.cpp \
    src/graphics_engine.cpp \
    src/scene.cpp \
    src/game_scene.cpp \
    src/game_reg.cpp \
    src/map.cpp \
    src/person.cpp \
    src/object.cpp \
    src/generator.cpp \
    src/city.cpp \
    src/player.cpp \
    src/police.cpp \
    src/npc.cpp \
    src/animation.cpp \
    src/character.cpp \
    src/transition.cpp \
<<<<<<< HEAD
    src/item.cpp \
    src/hud.cpp
=======
    src/particleemitter.cpp \
    src/particle.cpp
>>>>>>> 5530fbb018159954fa2055abe2b34f5010240114

HEADERS += headers/game.h \
    headers/game_states.h \
    headers/input.h \
    headers/input_engine.h \
    headers/graphics_engine.h \
    headers/scene.h \
    headers/defines.h \
    headers/game_scene.h \
    headers/event.h \
    headers/game_reg.h \
    headers/utils.h \
    headers/person.h \
    headers/object.h \
    headers/generator.h \
    headers/map.h \
    headers/city.h \
    headers/player.h \
    headers/police.h \
    headers/npc.h \
    headers/animation.h \
    headers/character.h \
    headers/transition.h \
<<<<<<< HEAD
    headers/item.h \
    headers/hud.h
=======
    headers/particleemitter.h \
    headers/particle.h
>>>>>>> 5530fbb018159954fa2055abe2b34f5010240114

INCLUDEPATH += src \
               headers
