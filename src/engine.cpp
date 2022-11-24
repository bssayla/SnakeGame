//
// Created by bssayla on 23/11/22.
//

#include "../headers/engine.h"
#include <iostream>

const Time Engine::TimePerFrame = seconds(1.f/60.f);

Engine::Engine() {
    resolution = Vector2f(800, 600);
    window.create(VideoMode(resolution.x, resolution.y), "Nibbles", Style::Default);
    window.setFramerateLimit(FPS);
    maxLevels = 0;
    checkLevelFiles();

    startTheGame();
    //title
    mainFont.loadFromFile("../assets/fonts/slant_regular.ttf");
    //Title
    setupText(&titleText,mainFont,"Snake Game",28,Color {46,184,219});
    FloatRect titleTextBounds = titleText.getGlobalBounds();
    titleText.setPosition(Vector2f ((resolution.x /2) - (titleTextBounds.width / 2),-9));
    //level
    setupText(&currentLevelText, mainFont,"Level 1",28,Color {46,184,219});
    currentLevelText.setPosition(Vector2f(15,-9));
    FloatRect currentLevelTextBounds = currentLevelText.getGlobalBounds();

    setupText(&appleEatenText, mainFont,"Apple 0",28,Color {46,184,219});
    appleEatenText.setPosition(Vector2f(
            currentLevelTextBounds.left + currentLevelTextBounds.width +20,
            -9));
    FloatRect appleEatenTextBounds = appleEatenText.getGlobalBounds();

    setupText(&scoreText,mainFont, "Score : "+to_string(score),28,Color {11,31,212});
    FloatRect scoreTextBounds = scoreText.getGlobalBounds();
    scoreText.setPosition(
            resolution.x -scoreTextBounds.width -15 ,
            -9
            );

    //gameover
    setupText(&gameOverText,mainFont,"Game Over",58,Color::Red);
    FloatRect gameOverTextBounds = gameOverText.getGlobalBounds();
    gameOverText.setPosition(
            (resolution.x /2) - (gameOverTextBounds.width / 2),
            100
            );
    gameOverText.setOutlineColor(Color::White);
    gameOverText.setOutlineThickness(2);

    setupText(&pressEnterText,mainFont,"press ENTER to re-start",28,Color::Red);
    FloatRect pressEnterTextBounds = pressEnterText.getGlobalBounds();
    pressEnterText.setPosition(
            (resolution.x /2) - (pressEnterTextBounds.width / 2),
            200
    );
    pressEnterText.setOutlineColor(Color::White);
    pressEnterText.setOutlineThickness(2);


    //score
}

void Engine::startTheGame() {
    score =0;
    speed = 2;
    snakeDirection = Directions::RIGHT;
    timeSinceLastMove = Time::Zero;
    sectionsToAdd = 0;
    currentGameLevel =1;
    appleEatenThisLevel =0;
    appleEatenTotal = 0;
    directionsQueue.clear();


    wallSections.clear();
    loadLevel(currentGameLevel);

    currentGameState = GameState::RUNNING;
    lastGameState = currentGameState;
    currentLevelText.setString("Level" + to_string(currentGameLevel));
    appleEatenText.setString("Apple" + to_string(appleEatenTotal));
    FloatRect currentLevelTextBounds = currentLevelText.getGlobalBounds();
    appleEatenText.setPosition(Vector2f(
            currentLevelTextBounds.left + currentLevelTextBounds.width +20,
            -9));

    scoreText.setString("Score: " + to_string(score));
    FloatRect scoreTextBounds = scoreText.getGlobalBounds();
    scoreText.setPosition(
            resolution.x -scoreTextBounds.width -15 ,
            -9
    );

    newSnake();
    moveApple();
}

void Engine::beginNextLevel() {

    currentGameLevel ++;
    wallSections.clear();
    directionsQueue.clear();
    speed = 2+ currentGameLevel;
    snakeDirection = Directions::RIGHT;
    sectionsToAdd = 0;
    appleEatenThisLevel =0;


    loadLevel(currentGameLevel);
    newSnake();
    moveApple();
    currentLevelText.setString("Level" + to_string(currentGameLevel));


}

// This is the initial snake created at the start of each level
void Engine::newSnake() {
    snake.clear();
    snake.emplace_back(Vector2f(100,100));
    snake.emplace_back(Vector2f(80,100));
    snake.emplace_back(Vector2f(60,100));
}

void Engine::addSnakeSection() {
    // Just add the section at the same position as the last section of the snake, it will follow normally
    Vector2f newSectionPosition = snake[snake.size() - 1].getPosition();
    snake.emplace_back(newSectionPosition);
}


void Engine::moveApple() {
    //find a place to place the apple
    //make sure it not be spawning in a wall or in the snake body

    Vector2f appleResolusion(Vector2f(( resolution.x/20) - 2,(resolution.y/20) - 2));
    Vector2f newAppleLocation;
    bool badLocaton= false;

    srand(time(nullptr));
    //loop until we find a good location

    do{
        badLocaton = false;
        // make some random locations for the apple
        newAppleLocation.y = (float) (1 + rand() / ((RAND_MAX + 1u)/ (int)appleResolusion.y)) *20;
        newAppleLocation.x = (float) (1 + rand() / ((RAND_MAX + 1u)/ (int)appleResolusion.x)) *20;

        //check if it's in the snake
        for(auto & s:snake ){
            if(s.getShape().getGlobalBounds()
            .intersects(Rect<float>(newAppleLocation.x,newAppleLocation.y,20,20))){
                badLocaton = true;
                break;
            }
        }
        //check if the apple in wall
        for(auto & w:wallSections){
            if(w.getShape().getGlobalBounds().intersects(Rect<float>(newAppleLocation.x,newAppleLocation.y,20,20))){
                badLocaton = true;
                break;
            }
        }

    }while (badLocaton);
    apple.setPosition(newAppleLocation);

}


void Engine::togglePause() {
    if(currentGameState == GameState::RUNNING){
        lastGameState = currentGameState;
        currentGameState = GameState::PAUSED;
    }else if(currentGameState == GameState::PAUSED){
        currentGameState = lastGameState;
    }



}
//check level files and if you can open them all
//then add them to levels vector
void Engine::checkLevelFiles() {
    ifstream levelManifest("../assets/Levels/levels.txt");
    ifstream testFile;
    for(string manifestLine ;getline(levelManifest,manifestLine);){
        //testing the files
        testFile.open("../assets/Levels/" + manifestLine);
        if(testFile.is_open()){
            //the file is working
            levels.emplace_back("../assets/Levels/" + manifestLine);
            testFile.close();
            maxLevels++;
        }
    }

}
//load the levels from the files and check the 'x' to add a wall
void Engine::loadLevel(int levelNumber) {
    string levelFile = levels[levelNumber -1];
    ifstream level(levelFile);
    string line;
    if(level.is_open()){
        for (int y = 0; y < 30; y++) {
            getline(level, line);
            for (int x = 0; x < 40; x++) {
                if (line[x] == 'x') {
                    wallSections.emplace_back(Wall(Vector2f(x * 20, y * 20), Vector2f(20, 20)));
                }
            }
        }
    }
    level.close();


}
void Engine::setupText(sf::Text *textItem, sf::Font &font, const sf::String &value, int size, sf::Color color) {
    textItem->setFont(font);
    textItem->setString(value);
    textItem->setCharacterSize(size);
    textItem->setFillColor(color);
}

void Engine::run() {
    Clock clock;
    // Main loop - Runs until the window is closed
    while (window.isOpen()) {
        Time dt = clock.restart(); //delta time
        //the  time to know the  last time we made a move
        timeSinceLastMove += dt;


        if((currentGameState == GameState::GAMEOVER) || (currentGameState == GameState::PAUSED)){
            //check for an input to unpause the game
            input();

            //
            if(currentGameState == GameState::GAMEOVER){
                draw();
            }
            sleep(milliseconds(2));
            continue;
        }


        input();
        update();
        draw();
    }
}