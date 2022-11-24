//
// Created by bssayla on 23/11/22.
//

#ifndef CPP_ENGINE_H
#define CPP_ENGINE_H

#include "snakesection.h"
#include "apple.h"
#include "Wall.h"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <vector>
#include <deque>
#include <fstream>

using namespace sf;
using namespace std;

class Engine {
private:
    // custumising Window for the game
    Vector2f resolution;
    RenderWindow window;
    const unsigned int FPS = 60;
    static const Time TimePerFrame;
    // snake will be a table of snake sctions(lil rectangles)
    vector<SnakeSection> snake;
    int snakeDirection;
    int speed;
    deque<int> directionsQueue; // for the direction keys
    int sectionsToAdd; //after eating an apple we add a section to the snake
    //apple
    Apple apple;
    int appleEatenThisLevel;
    int appleEatenTotal;
    //walls
    vector<Wall> wallSections;
    Time timeSinceLastMove;

    //levels
    int currentGameLevel;
    int maxLevels;
    vector<String> levels;
    int currentGameState;
    int lastGameState;  //so we can pause game and remember the satats
    //texts
    Font mainFont;
    Text titleText;
    Text appleEatenText;
    Text currentLevelText;
    //Score
    int long score;
    Text scoreText;
    Text gameOverScore;

    //GameOver
    Text gameOverText;
    Text pressEnterText;


public:
    enum Directions{UP,DOWN,LEFT,RIGHT};
    enum GameState{ RUNNING, PAUSED, GAMEOVER };
    Engine();
    // user inputs to manage the engine
    void input();
    void addDirection(int direction);
    void update();
    void togglePause();
    void startTheGame();
    //texts
    static void setupText(Text *textitem,Font &font,const String &value,int size,Color color);


    // draw shapes in the window
    void draw();
    //init a snake
    void newSnake();
    //add a snake section to the snake after eating the fruit
    void addSnakeSection();
    //apple
    void moveApple();
    //levels
    void checkLevelFiles();
    void loadLevel(int levelNumber);
    void beginNextLevel();
    // The main loop will be in the run function / THE GAME
    void run();

};


#endif //CPP_ENGINE_H
