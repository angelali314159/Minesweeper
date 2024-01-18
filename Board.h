#pragma once
#include <SFML/System/Vector2.hpp>
#include "TextureManager.h"
#include <SFML/Window/Mouse.hpp>
#include "LeaderboardWindow.h"
#include <chrono>
#include <vector>
#include "Tile.h"
#include <random>
#include <cstdlib>
using namespace std;

class Board{
public:
    static void openBoard(int rowCount, int colCount, int numMine, string playerName) {
        //collection of variables for operation of game
        bool victory = false;
        vector<Tile *> boardTiles;
        int tileCounter = 0;
        vector<int> mineTiles;
        string type;
        sf::Vector2f tempLoc;
        bool debug = false;
        bool gameOver = false;
        int counter = numMine;
        bool revealMines = false;
        bool leaderBoard = false;
        bool victoryLeader = false;

        //generate random mine tiles
        int a = 0;
        srand((unsigned) time(NULL));
        bool duplicate;
        while (a < numMine) {
            int randomTemp = rand() % (rowCount * colCount);
            duplicate = false;
            for (int i = 0; i < mineTiles.size(); i++) {
                if (mineTiles.at(i) == randomTemp) {
                    duplicate = true;
                }
            }
            if (!duplicate) {
                a++;
                mineTiles.push_back(randomTemp);
            }
        }

        //vector for the tiles
        for (int x = 0; x < colCount; x++) {
            for (int y = 0; y < rowCount; y++) {
                type = "normal";
                //check through mine tiles to see if it should be a mine
                for (int i = 0; i < mineTiles.size(); i++) {
                    if (mineTiles.at(i) == tileCounter) {
                        type = "mine";
                    }
                }
                tempLoc = sf::Vector2f(x * 32, y * 32);
                Tile *temp = new Tile(tempLoc, type);
                boardTiles.push_back(temp);
                tileCounter++;
            }
        }

        //set adjacent tiles
        for (int i = 0; i < boardTiles.size(); i++) {
            boardTiles.at(i)->setAdjacent(boardTiles, rowCount, colCount);
        }

        //set up for the timer
        auto start_time = chrono::high_resolution_clock::now();
        auto pauseTime = chrono::high_resolution_clock::now();
        auto elapsed_paused_time = chrono::duration_cast<chrono::seconds>(
                chrono::high_resolution_clock::now() - pauseTime).count();
        bool paused = false;
        int minutes;
        int seconds;

        //set up for all buttons
        sf::Sprite sLeader;
        sLeader.setTexture(TextureManager::getTexture("leaderboard"));
        sLeader.setPosition((colCount * 32) - 176, 32 * (rowCount + 0.5));
        sLeader.setTextureRect(sf::IntRect(0, 0, 64, 64));

        sf::Sprite sFace;
        sFace.setTexture(TextureManager::getTexture("face_happy"));
        sFace.setPosition((colCount / 2 * 32 - 32), 32 * (rowCount + 0.5));
        sFace.setTextureRect(sf::IntRect(0, 0, 64, 64));

        sf::Sprite sDebug;
        sDebug.setTexture(TextureManager::getTexture("debug"));
        sDebug.setPosition(colCount * 32 - 304, 32 * (rowCount + 0.5));
        sDebug.setTextureRect(sf::IntRect(0, 0, 64, 64));

        sf::Sprite sPause;
        sPause.setTexture(TextureManager::getTexture("pause"));
        sPause.setPosition((colCount * 32 - 240), 32 * (rowCount + 0.5));
        sPause.setTextureRect(sf::IntRect(0, 0, 64, 64));

        sf::Sprite sPlay;
        sPlay.setTexture(TextureManager::getTexture("play"));
        sPlay.setPosition((colCount * 32 - 240), 32 * (rowCount + 0.5));
        sPlay.setTextureRect(sf::IntRect(0, 0, 64, 64));

        sf::Sprite sTileNum;
        sTileNum.setTextureRect(sf::IntRect(0, 0, 32, 32));

        sf::Sprite sFlag;
        sFlag.setTexture(TextureManager::getTexture("flag"));
        sFlag.setTextureRect(sf::IntRect(0, 0, 32, 32));

        sf::Sprite sTile;
        sTile.setTextureRect(sf::IntRect(0, 0, 32, 32));

        sf::Sprite sCounter;
        sCounter.setTexture(TextureManager::getTexture("digits"));

        sf::Sprite sTimer;
        sTimer.setTexture(TextureManager::getTexture("digits"));

        //************************************ window loop *************************
        sf::RenderWindow window(sf::VideoMode(colCount * 32, rowCount * 32 + 100), "Game Window", sf::Style::Close);
        while (window.isOpen()) {
            window.clear(sf::Color::White);

            //draw on all button sprites
            window.draw(sFace);
            window.draw(sDebug);
            window.draw(sLeader);
            window.draw(sPause);

            //****************** draw sprites for counter ******************************
            //ones place
            sCounter.setPosition(33 + 21, 32 * (rowCount + 0.5) + 16);
            sCounter.setTextureRect(sf::IntRect((abs(counter) % 10) * 21, 0, 21, 32));
            window.draw(sCounter);
            //tens place
            sCounter.setPosition(33, 32 * (rowCount + 0.5) + 16);
            sCounter.setTextureRect(
                    sf::IntRect((((abs(counter) % 100) - ((abs(counter) % 100) % 10)) / 10) * 21, 0, 21, 32));
            window.draw(sCounter);
            //hundreds place or negative sign
            if (counter < 0) {
                sCounter.setPosition(12, 32 * (rowCount + 0.5) + 16);
                sCounter.setTextureRect(sf::IntRect(21 * 10, 0, 21, 32));
                window.draw(sCounter);
            } else {
                sCounter.setPosition(12, 32 * (rowCount + 0.5) + 16);
                sCounter.setTextureRect(sf::IntRect(((counter - (counter % 100)) / 100) * 21, 0, 21, 32));
                window.draw(sCounter);
            }

            //******************************* draw tiles *********************************
            auto drawTiles = [&](vector<Tile *> boardTiles, sf::Sprite sTile, bool revealMines, bool gameOver,
                                 bool leaderBoard) {
                for (int i = 0; i < boardTiles.size(); i++) {
                    sTile.setPosition(boardTiles.at(i)->getLocation());
                    if (leaderBoard) {
                        sTile.setTexture(TextureManager::getTexture("tile_revealed"));
                        window.draw(sTile);
                    } else {
                        //not flipped tiles
                        if (!boardTiles.at(i)->getFlipped()) {
                            sTile.setTexture(TextureManager::getTexture("tile_hidden"));
                            window.draw(sTile);
                        }
                            //flipped tile
                        else if (boardTiles.at(i)->getFlipped()) {
                            sTile.setTexture(TextureManager::getTexture("tile_revealed"));
                            window.draw(sTile);
                            //is a mine
                            if (boardTiles.at(i)->getType() == "mine") {
                                sTile.setTexture(TextureManager::getTexture("mine"));
                                window.draw(sTile);
                            } else if (boardTiles.at(i)->getType() == "normal") {
                                if (boardTiles.at(i)->getAdMines() != 0) {
                                    sTile.setTexture(TextureManager::getTexture(
                                            "number_" + to_string(boardTiles.at(i)->getAdMines())));
                                    window.draw(sTile);
                                }
                            }
                        }
                        if (revealMines && gameOver) {
                            if (boardTiles.at(i)->getType() == "mine") {
                                sTile.setTexture(TextureManager::getTexture("tile_revealed"));
                                window.draw(sTile);
                            }
                        }
                        if (boardTiles.at(i)->getFlag()) {
                            sFlag.setPosition(boardTiles.at(i)->getLocation());
                            window.draw(sFlag);
                        }
                        if (revealMines) {
                            if (boardTiles.at(i)->getType() == "mine") {
                                sTile.setTexture(TextureManager::getTexture("mine"));
                                window.draw(sTile);
                            }
                        }
                    }
                }
            };

            drawTiles(boardTiles, sTile, revealMines, gameOver, leaderBoard);

            //****************** timer with play/pause ***************************
            auto game_duration = std::chrono::duration_cast<std::chrono::seconds>(
                    chrono::high_resolution_clock::now() - start_time);
            int total_time = game_duration.count(); // necessary to subtract elapsed time later because "game_duration.count()" is const

            if (!paused) {
                //enters if the game is NOT paused. This is the condition that keeps the timer from incrementing when paused.
                //cout << "not paused\n";
                total_time = total_time - elapsed_paused_time; //
                minutes = total_time / 60;
                seconds = total_time % 60;
            }

            if (paused) {
                window.draw(sPlay);
            }

            //******* drawing timer **********
            //minutes
            sTimer.setPosition(colCount * 32 - 97, 32 * (rowCount + 0.5) + 16);
            sTimer.setTextureRect(sf::IntRect((minutes - (minutes % 10) / 10) * 21, 0, 21, 32));
            window.draw(sTimer);
            sTimer.setPosition(colCount * 32 - 97 + 21, 32 * (rowCount + 0.5) + 16);
            sTimer.setTextureRect(sf::IntRect((minutes % 10) * 21, 0, 21, 32));
            window.draw(sTimer);
            //seconds
            sTimer.setPosition(colCount * 32 - 54, 32 * (rowCount + 0.5) + 16);
            sTimer.setTextureRect(sf::IntRect((seconds - (seconds % 10)) / 10 * 21, 0, 21, 32));
            window.draw(sTimer);
            sTimer.setPosition(colCount * 32 - 54 + 21, 32 * (rowCount + 0.5) + 16);
            sTimer.setTextureRect(sf::IntRect((seconds % 10) * 21, 0, 21, 32));
            window.draw(sTimer);

            if (leaderBoard) {
                drawTiles(boardTiles, sTile, revealMines, gameOver, leaderBoard);
                window.display();
                pauseTime = chrono::high_resolution_clock::now();
                LeaderboardWindow::openLeadWindow(rowCount, colCount, victory, playerName, minutes, seconds, victoryLeader);
                leaderBoard = false;
                auto unPausedTime = chrono::steady_clock::now();
                elapsed_paused_time += chrono::duration_cast<chrono::seconds>(unPausedTime - pauseTime).count();
            }

            //******************** event reactor **************************************
            sf::Event event;
            while (window.pollEvent(event)) {
                // "close requested" event: we close the window
                if (event.type == sf::Event::Closed) {
                    //paused = true;
                    window.close();
                }
                //clicking events
                if (event.type == sf::Event::MouseButtonPressed) {
                    sf::Mouse mouse;
                    sf::Vector2i mousePosition = mouse.getPosition(window);
                    //*********************** left clicks ***************************
                    if (mouse.isButtonPressed((sf::Mouse::Left))) {
                        //leaderboard button
                        if (sLeader.getGlobalBounds().contains(mousePosition.x, mousePosition.y)) {
                            leaderBoard = true;
                        }

                            //play and pause button
                        else if (sPause.getGlobalBounds().contains(mousePosition.x, mousePosition.y) && !gameOver &&
                                 !victory) {
                            paused = !paused;

                            if (paused) {
                                pauseTime = chrono::high_resolution_clock::now();

                            } else {
                                //unpaused
                                auto unPausedTime = chrono::steady_clock::now();
                                elapsed_paused_time += chrono::duration_cast<chrono::seconds>(unPausedTime -
                                                                                              pauseTime).count(); //Addition is necessary for when hitting the pause button more than onc
                            }
                        }

                            //debug button
                        else if (sDebug.getGlobalBounds().contains(mousePosition.x, mousePosition.y) && !gameOver && !victory &&!paused) {
                            debug = !debug;
                            if (debug) {
                                //go through all tiles and set them to flipped
                                revealMines = true;
                            } else if (!debug) {
                                revealMines = false;
                            }
                        }

                            //face button
                        else if (sFace.getGlobalBounds().contains(mousePosition.x, mousePosition.y)) {
                            //reset game
                            window.close();
                            openBoard(rowCount, colCount, numMine, playerName);
                        }

                            //tile clicks
                        else if ((mousePosition.y < rowCount * 32) && !gameOver && !victory && !paused) {
                            int tileNumber = rowCount * (mousePosition.x / 32) + (mousePosition.y / 32);
                            if (!boardTiles.at(tileNumber)->getFlag()) {
                                gameOver = boardTiles.at(tileNumber)->clicked(boardTiles.at(tileNumber));
                            }
                        }
                    }
                        //******************* right clicks *************************
                    else if (mouse.isButtonPressed((sf::Mouse::Right)) &&
                             ((mousePosition.y < rowCount * 32) && !gameOver && !victory) && !paused) {
                        //flag work
                        int tileNumber = rowCount * (mousePosition.x / 32) + (mousePosition.y / 32);
                        if (!boardTiles.at(tileNumber)->getFlipped()) {
                            boardTiles.at(tileNumber)->toggleFlag();
                            if (boardTiles.at(tileNumber)->getFlag()) {
                                counter--;
                            } else {
                                counter++;
                            }
                        }
                    }
                }
            }

            //************************* game over *****************************
            if (gameOver) {
                paused = true;
                revealMines = true;

                //change smiley face to dead face
                sFace.setTexture(TextureManager::getTexture("face_lose"));
                window.draw(sFace);
            }

            if (victory) {
                //draw face
                sFace.setTexture(TextureManager::getTexture("face_win"));
                window.draw(sFace);

                //put flags on all mines
                for (int i = 0; i < boardTiles.size(); i++) {
                    if (boardTiles.at(i)->getType() == "mine" && !boardTiles.at(i)->getFlag())
                        sFlag.setPosition(boardTiles.at(i)->getLocation());
                    window.draw(sFlag);
                }

            }

            //************************* check for victory ***********************************
            //if there are any not flipped tiles (minus mines), did not win
            victory = true;
            for (auto &boardTile: boardTiles) {
                if (!boardTile->getFlipped() && boardTile->getType() == "normal") {
                    victory = false;
                    break;
                }
            }

            if (victory && !victoryLeader) {
                paused = true;
                counter = 0;
                drawTiles(boardTiles, sTile, revealMines, gameOver, leaderBoard);
                window.display();
                LeaderboardWindow::openLeadWindow(rowCount, colCount, victory, playerName, minutes, seconds, victoryLeader);
                victoryLeader = true;
            }

            window.display();
        }
    }
};