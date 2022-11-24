//
// Created by bssayla on 23/11/22.
//

#include "../headers/engine.h"

void Engine::draw() {
    window.clear(Color::Black);
    //draw an apple
    window.draw(apple.getsprite());
    //draw walls
    for(auto & w:wallSections){
        window.draw(w.getShape());
    }


    // Draw snake sections
    for (auto & s : snake) {
        window.draw(s.getShape());
    }

    //gameOver Screen
    if(currentGameState == GameState::GAMEOVER){
        window.draw(gameOverText);
        window.draw(pressEnterText);
        scoreText.setPosition(
                (resolution.x /2) - (scoreText.getGlobalBounds().width / 2),
                300
        );
        window.draw(scoreText);
    }

    //draw texts

    window.draw(titleText);
    window.draw(currentLevelText);
    window.draw(appleEatenText);
    window.draw(scoreText);

    window.display();
}