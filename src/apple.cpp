//
// Created by bssayla on 23/11/22.
//

#include "../headers/apple.h"
Apple::Apple()
{
    Vector2f startingPosition(400,300);
    sprite.setSize(Vector2f(20,20));
    sprite.setFillColor(Color::Red);
    sprite.setPosition(startingPosition);
}

void Apple::setPosition(sf::Vector2f newPosition) {
    sprite.setPosition(newPosition);
}

RectangleShape Apple::getsprite() {
    return sprite;
}

