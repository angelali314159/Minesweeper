#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <SFML/Graphics.hpp>
#include "WelcomeWindow.h"

using namespace std;

int main() {
    //read configure file
    int rowCount, colCount, numMine;
    ifstream file;
    file.open("files/board_config.cfg");

    string readLine;
    getline(file, readLine);
    colCount = stoi(readLine);
    getline(file, readLine);
    rowCount = stoi(readLine);
    getline(file, readLine);
    numMine = stoi(readLine);
    file.close();

    //load font
    sf::Font font;
    if (!font.loadFromFile("files/font.ttf"))
    {
        cout << "Error loading font" << endl; //error
    }

    //open Welcome Window
    WelcomeWindow::openWelcomeWindow(rowCount, colCount, font, numMine);

    return 0;
}