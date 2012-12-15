TEMPLATE = app
CONFIG += console
CONFIG -= qt

LIBS += -lsfml-system -lsfml-window -lsfml-graphics

SOURCES += src/main.cpp \
    src/game.cpp \
    src/input_engine.cpp \
    src/graphics_engine.cpp \
    src/scene.cpp \
    src/game_scene.cpp \
    src/game_reg.cpp \
    src/map.cpp

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
    src/map.h

INCLUDEPATH += src \
               headers

