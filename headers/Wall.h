//
// Created by bssayla on 23/11/22.
//

#ifndef CPP_WALL_H
#define CPP_WALL_H
#include <SFML/Graphics.hpp>

using namespace sf;

class Wall {
private:
    RectangleShape wallShape;


public:
    Wall(Vector2f position,Vector2f size);
    RectangleShape getShape();
};


#endif //CPP_WALL_H
