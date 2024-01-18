#pragma once
#include <string>
#include <vector>
#include <SFML/Graphics.hpp>
using namespace std;

class LeaderboardWindow {
    static void setText(sf::Text &text, float x, float y) {
        sf::FloatRect textRect = text.getLocalBounds();
        text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
        text.setPosition(sf::Vector2f(x, y));
    }

public:
    static void
    openLeadWindow(int rowCount, int colCount, bool victory, string playerName, int playerMinutes, int playerSeconds, bool victoryLeader) {
        int newLeader = -1;
        sf::Font font;
        if (!font.loadFromFile("files/font.ttf")) {
            cout << "Error loading font" << endl; //error
        }

        //read csv file and store in arrays
        ifstream file;
        file.open("files/leaderboard.txt");
        string readLine;
        vector<int> minutes;
        vector<int> seconds;
        vector<string> timeString;
        vector<string> names;
        while (getline(file, readLine)) {
            timeString.push_back(readLine.substr(0, readLine.find(',')));
            minutes.push_back(stoi(readLine.substr(0, readLine.find(':'))));
            seconds.push_back(stoi(readLine.substr(readLine.find(':') + 1)));
            names.push_back(readLine.substr(readLine.find(',') + 1));
        }
        file.close();

        if (victory && !victoryLeader) {
            string playerTimeString;
            if (playerMinutes < 10 && playerSeconds < 10) {
                playerTimeString = "0" + to_string(playerMinutes) + ":0" + to_string(playerSeconds);
            } else if (playerMinutes < 10) {
                playerTimeString = "0" + to_string(playerMinutes) + ":" + to_string(playerSeconds);
            } else if (playerSeconds < 10) {
                playerTimeString = to_string(playerMinutes) + ":0" + to_string(playerSeconds);
            } else {
                playerTimeString = to_string(playerMinutes) + ":" + to_string(playerSeconds);
            }

            //iterate through vectors and compare to player score
            for (int i = 0; i < names.size(); i++) {
                if (playerMinutes * 60 + playerSeconds < minutes.at(i) * 60 + seconds.at(i)) {
                    names.insert(names.begin() + i, playerName);
                    names.pop_back();
                    timeString.pop_back();
                    timeString.insert(timeString.begin() + i, playerTimeString);
                    newLeader = i;
                    break;
                }
            }

            //write into file
            ofstream outFile;
            outFile.open("files/leaderboard.txt");
            string writeTemp;
            //iterate through arrays
            for (int i = 0; i < 5; i++) {
                writeTemp.append(timeString.at(i));
                writeTemp.append("," + names.at(i));
                outFile << writeTemp << endl;
                writeTemp.clear();
            }
            outFile.close();
        }

        //open window
        int width = colCount * 16;
        int height = rowCount * 16 + 50;
        sf::RenderWindow lwindow(sf::VideoMode(width, height), "Leaderboard Window", sf::Style::Close);
        while (lwindow.isOpen()) {

            lwindow.clear(sf::Color::Blue);

            sf::Event event;
            while (lwindow.pollEvent(event)) {
                // "close requested" event: we close the window
                if (event.type == sf::Event::Closed)
                    lwindow.close();
            }

            //display "leaderboard"
            sf::Text text;
            text.setFont(font);
            text.setString("LEADERBOARD");
            text.setCharacterSize(20);
            text.setFillColor(sf::Color::White);
            text.setStyle(sf::Text::Bold | sf::Text::Underlined);
            setText(text, width / 2, height / 2 - 120);
            lwindow.draw(text);

            //display contents of leaderboard
            text.setCharacterSize(18);
            text.setStyle(sf::Text::Bold);
            string leaderString;

            //iterate through vectors
            for (int i = 0; i < names.size(); i++) {
                if (newLeader != -1 && i == newLeader && names.at(i) == playerName)
                {
                    leaderString += std::to_string(i + 1) + '.' + "\t" + timeString.at(i) + "\t" + names.at(i) + "*\n\n";
                }
                else
                {
                    leaderString += std::to_string(i + 1) + '.' + "\t" + timeString.at(i) + "\t" + names.at(i) + "\n\n";
                }

            }
            text.setString(leaderString);
            setText(text, width / 2, height / 2 + 20);
            lwindow.draw(text);

            lwindow.display();
        }
    }
};