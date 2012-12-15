TEMPLATE = app
CONFIG += console
CONFIG -= qt

SOURCES += src/main.cpp \
    src/game.cpp \
    src/input_engine.cpp \
    src/map.cpp

HEADERS += headers/game.h \
    headers/game_states.h \
    headers/input.h \
    headers/input_engine.h \
    src/map.h
