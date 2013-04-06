TEMPLATE = app
CONFIG += console
CONFIG -= qt

QMAKE_CXXFLAGS += -pg -Wextra -Wconversion -Wuninitialized -Wmissing-include-dirs -Wshadow -g
QMAKE_LDFLAGS += -pg -g

LIBS += -lsfml-system -lsfml-window -lsfml-graphics -lsfml-audio -lsfml-network

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
    src/item.cpp \
    src/hud.cpp \
    src/item_factory.cpp \
    src/menu_scene.cpp \
    src/score_scene.cpp \
    src/shop.cpp



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
    headers/item.h \
    headers/hud.h \
    headers/item_factory.h \
    headers/menu_scene.h \
    headers/score_scene.h \
    headers/shop.h

INCLUDEPATH += src \
               headers
