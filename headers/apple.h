//
// Created by bssayla on 23/11/22.
//

#ifndef CPP_APPLE_H
#define CPP_APPLE_H

#include <SFML/Graphics.hpp>
using namespace sf;

class Apple {
private:
    RectangleShape sprite;

public:
    Apple();
    void setPosition(Vector2f newPosition);
    RectangleShape getsprite();
};


#endif //CPP_APPLE_H
