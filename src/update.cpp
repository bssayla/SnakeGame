//
// Created by bssayla on 23/11/22.
//
#include "../headers/engine.h"

//this func is for updating the snake positions
void Engine::update() {
    if (timeSinceLastMove.asSeconds() >= seconds( 1.f / float(speed)).asSeconds()){
        Vector2f currentSnakePosition = snake[0].getPosition();
        Vector2f lastSakePosition = currentSnakePosition;

        //checking if there are any inputs
        if(!directionsQueue.empty()) {
            //making sure that we don't reverse the snake movement
            switch (snakeDirection) {
                case Directions::UP :
                    if (directionsQueue.front() != Directions::DOWN) {
                        snakeDirection = directionsQueue.front();
                    }
                    break;
                case Directions::DOWN :
                    if (directionsQueue.front() != Directions::UP) {
                        snakeDirection = directionsQueue.front();
                    }
                    break;
                case Directions::LEFT :
                    if (directionsQueue.front() != Directions::RIGHT) {
                        snakeDirection = directionsQueue.front();
                    }
                    break;
                case Directions::RIGHT :
                    if (directionsQueue.front() != Directions::LEFT) {
                        snakeDirection = directionsQueue.front();
                    }
                    break;
            }
            directionsQueue.pop_front();
        }




            //grow the snake
            if (sectionsToAdd){
                addSnakeSection();
                sectionsToAdd--;
            }



            // updating the position of the head
            switch (snakeDirection) {
                case Directions::RIGHT:
                    if(currentSnakePosition.x == resolution.x){
                        snake[0].setPosition(Vector2f(0,currentSnakePosition.y));

                    }else{
                        snake[0].setPosition(Vector2f(currentSnakePosition.x +20,currentSnakePosition.y));

                    }

                    break;
                case Directions::LEFT:
                    if(currentSnakePosition.x == 0){
                        snake[0].setPosition(Vector2f(resolution.x,currentSnakePosition.y));

                    } else{
                        snake[0].setPosition(Vector2f(currentSnakePosition.x -20,currentSnakePosition.y));

                    }
                    break;
                case Directions::UP:
                    if(currentSnakePosition.y == 0){
                        snake[0].setPosition(Vector2f(currentSnakePosition.x,resolution.y));

                    } else{
                        snake[0].setPosition(Vector2f(currentSnakePosition.x ,currentSnakePosition.y -20));

                    }
                    break;
                case Directions::DOWN:
                    if(currentSnakePosition.y == resolution.y){
                        snake[0].setPosition(Vector2f(currentSnakePosition.x,0));

                    } else{
                        snake[0].setPosition(Vector2f(currentSnakePosition.x ,currentSnakePosition.y +20));

                    }
                    break;

            }
            //updating the position of the rest of the body
            for(int s =1; s<snake.size() ;s++){
                currentSnakePosition = snake[s].getPosition();
                snake[s].setPosition(lastSakePosition);
                lastSakePosition = currentSnakePosition;
            }
            //
            for(auto & s: snake){
                s.update();
            }
            //collision between the snake and the apple
            if(snake[0].getShape().getGlobalBounds()
            .intersects(apple.getsprite().getGlobalBounds())){
                //update score
                score += snake.size() * (appleEatenTotal +1);
                scoreText.setString("Score: " + to_string(score));
                FloatRect scoreTextBounds = scoreText.getGlobalBounds();
                scoreText.setPosition(
                        resolution.x -scoreTextBounds.width -15 ,
                        -9
                );
                //Score
                appleEatenThisLevel++;
                appleEatenTotal++;
                appleEatenText.setString("Apple "+ to_string(appleEatenTotal));
                FloatRect currentLevelTextBounds = currentLevelText.getGlobalBounds();
                appleEatenText.setPosition(Vector2f(
                        currentLevelTextBounds.left + currentLevelTextBounds.width +20,
                        -9));
                bool beginningNewLevel = false;
                if (appleEatenThisLevel >= 10) {
                    // Begin the next level if there are more,
                    // otherwise, just stay on the last level and keep getting harder until we die.
                    if (currentGameLevel  < maxLevels) {
                        beginningNewLevel = true;
                        beginNextLevel();
                    }
                }
                if(!beginningNewLevel){
                    //when we hit the apple add section to the snake
                    sectionsToAdd += 4;
                    //spawn a new apple
                    moveApple();
                    //increase speed
                    speed++;
                }


            }
            //snake body collision
            for(int s = 1;s<snake.size();s++) {
                if (snake[0].getShape().getGlobalBounds().intersects(snake[s].getShape().getGlobalBounds())){
                    // Game Over
                    currentGameState = GameState::GAMEOVER;
                }
            }
            //collision between the snake and the wall
            for(auto & w: wallSections){
                if(snake[0].getShape().getGlobalBounds().intersects(w.getShape().getGlobalBounds())){
                    currentGameState = GameState::GAMEOVER;
                }
            }

            //last move timer
            timeSinceLastMove = Time::Zero;

    }

}
