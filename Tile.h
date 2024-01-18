#pragma once
#include "TextureManager.h"
#include <SFML/Graphics.hpp>
#include <string>
#include <SFML/System/Vector2.hpp>

using namespace std;

class Tile{
private:
    vector<Tile*> adjacentTiles;
    sf::Vector2f location;
    string type;
    bool flipped;
    int adMines;
    bool flag;

public:
    Tile(sf::Vector2f location, string type)
    {
        this->location = location;
        this->type = type;
        flipped = false;
        adMines = 0;
        flag = false;
    }

    bool getFlag()
    {
        return flag;
    }

    void toggleFlag()
    {
        flag = !flag;
    }

    bool clicked(Tile* tile)
    {
        if (!tile->getFlipped() && !tile->getFlag())
        {
            flip();
            if (tile->getType() == "mine")
            {
                return true;
            }
            else
            {
                if (tile->getAdMines() == 0) {
                    flipAdjacent(tile);
                }
            }
        }
        return false;
    }

    int getAdMines()
    {
        return adMines;
    }

    vector<Tile*> getAdTiles()
    {
        return adjacentTiles;
    }

    bool getFlipped()
    {
        return flipped;
    }

    sf::Vector2f getLocation()
    {
        return location;
    }

    string getType()
    {
        return type;
    }

    void flip()
    {
        flipped = true;
    }

    void hide()
    {
        flipped = false;
    }

    void setAdjacent(vector<Tile*> boardTiles, int rowCount, int colCount)
    {
        int tileNumber = rowCount * (location.x / 32) + (location.y / 32);
        //not bottom row
        if ((tileNumber - (rowCount - 1)) % rowCount != 0 || tileNumber == 0) {
            //tile below
            adjacentTiles.push_back(boardTiles.at( tileNumber + 1));
        }
        //not top row
        if (tileNumber != 0 && tileNumber % (rowCount) != 0){
            //tile on top
            adjacentTiles.push_back(boardTiles.at(tileNumber - 1));
        }
        //not left column
        if (tileNumber > rowCount){
            //tile on left
            adjacentTiles.push_back(boardTiles.at(tileNumber - rowCount));
        }
        //not right column
        if(tileNumber < (rowCount * colCount - rowCount)) {
            //tile on right
            adjacentTiles.push_back(boardTiles.at(tileNumber + rowCount));
        }
        //not top row or right column
        if ((tileNumber != 0 && tileNumber % (rowCount) != 0) && (tileNumber < (rowCount * colCount - rowCount))){
            //upper right
            adjacentTiles.push_back(boardTiles.at(tileNumber - 1 + rowCount));
        }
        //not top row or left column
        if ((tileNumber != 0 && tileNumber % (rowCount) != 0) && (tileNumber > rowCount)) {
            //upper left
            adjacentTiles.push_back(boardTiles.at(tileNumber - 1 - rowCount));
        }
        //not bottom row or right column
        if (((tileNumber - (rowCount - 1)) % rowCount != 0 || tileNumber == 0) && (tileNumber < (rowCount * colCount - rowCount)))
        {
            //bottom right
            adjacentTiles.push_back(boardTiles.at(tileNumber + 1 + rowCount));
        }
        //not bottom row or left column
        if (((tileNumber - (rowCount - 1)) % rowCount != 0 || tileNumber == 0) && (tileNumber > rowCount))
        {
            //bottom left
            adjacentTiles.push_back(boardTiles.at(tileNumber + 1 - rowCount));
        }
        for (auto & adjacentTile : adjacentTiles)
        {
            if (adjacentTile->type == "mine")
            {
                adMines++;
            }
        }
    }

    void flipAdjacent(Tile* tile)
    {
        for (int i = 0; i < tile->getAdTiles().size(); i++)
        {
            if (tile->getAdTiles().at(i)->type != "mine")
            {
                tile->getAdTiles().at(i)->clicked(adjacentTiles.at(i));
            }
        }
    }
};