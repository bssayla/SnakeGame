//
// Created by bssayla on 23/11/22.
//

#include "../headers/engine.h"

void Engine::input() {
    Event event{};

    while (window.pollEvent(event)) {
        // Window closed
        if (event.type == Event::Closed) {
            window.close();
        }

        // Handle Keyboard Input
        if (event.type == Event::KeyPressed) {
            // Quit
            if (event.key.code == Keyboard::Escape ) {
                window.close();
            }

            //pause
            if(event.key.code == Keyboard::Escape){
                 togglePause();
            }

            //re-start the Gmae
            if(currentGameState == GameState::GAMEOVER ){
                if(event.key.code == Keyboard::Enter){
                    startTheGame();
                }
            }

            // Directions handling
            if(event.key.code == Keyboard::Up){
                addDirection(Directions::UP);
            } else if(event.key.code == Keyboard::Down){
                addDirection(Directions::DOWN);

            } else if(event.key.code == Keyboard::Right){
                addDirection(Directions::RIGHT);

            }else if(event.key.code == Keyboard::Left){
                addDirection(Directions::LEFT ) ;

            }
        }
    }
}
void Engine::addDirection(int newDirection) {
    if(directionsQueue.empty()){
        //add in the back of the table
        directionsQueue.emplace_back(newDirection);
    } else {
        if (directionsQueue.back() != newDirection ){
            //adding a new direction to the queue just if the last direction wasn't same
            directionsQueue.emplace_back(newDirection);
        }
    }
}
