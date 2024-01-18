#include <SFML/Graphics.hpp>
#include "Board.h"
using namespace std;

class WelcomeWindow{
private:
    //shifts origin to center
    static void setText(sf::Text &text, float x, float y){
        sf::FloatRect textRect = text.getLocalBounds();
        text.setOrigin(textRect.left + textRect.width/2.0f, textRect.top + textRect.height/2.0f);
        text.setPosition(sf::Vector2f(x, y));
    }

public:

    static void openWelcomeWindow(int rowCount, int colCount, sf::Font const& font, int numMine){

        int width = colCount * 32;
        int height = rowCount * 32 + 100;

        //create Welcome Window
        sf::RenderWindow window(sf::VideoMode(width, height), "Welcome Window", sf::Style::Close);

        //set up for username
        sf::Text username;
        username.setFont(font);
        username.setFillColor(sf::Color::Yellow);
        username.setStyle(sf::Text::Bold);
        username.setCharacterSize(18);
        username.setString("|");
        setText(username, width / 2, height / 2 - 45);
        vector<char> input;
        string inputString;

        while (window.isOpen())
        {
            window.clear(sf::Color::Blue);

            //all of this text goes between window.clear and window.display

            //any events that go on
            sf::Event event;
            while(window.pollEvent(event))
            {
                //closes window
                if(event.type == sf::Event::Closed)
                {
                    window.close();
                }

                //text input for username
                if (event.type == sf::Event::TextEntered)
                {
                    char inputChar = event.text.unicode;
                    if (isalpha(inputChar)) {
                        if (input.empty()) {
                            inputString.clear();
                            input.push_back(toupper(inputChar));
                        } else if (input.size() != 10) {
                            input.push_back(tolower(inputChar));
                        }
                    }
                    for (char c : input)
                    {
                        inputString.push_back(c);
                    }
                    inputString.push_back('|');
                    username.setString(inputString);
                    setText(username, width / 2, height / 2 - 45);
                    inputString.clear();
                }

                //key pressed events
                if (event.type == sf::Event::KeyPressed)
                {
                    //backspace function
                    if (event.key.code == sf::Keyboard::BackSpace)
                    {
                        input.pop_back();
                        for (char c : input)
                        {
                            inputString.push_back(c);
                        }
                        inputString.push_back('|');
                        username.setString(inputString);
                        setText(username, width / 2, height / 2 - 45);
                        inputString.clear();
                    }

                    //enter function
                    if (event.key.code == sf::Keyboard::Enter)
                    {
                        if (!input.empty())
                        {
                            input.pop_back(); //removes the |
                            string playerName;
                            for (char c : input)
                            {
                                playerName.push_back(c);
                            }
                            window.close();
                            Board::openBoard(rowCount, colCount, numMine, playerName);
                        }
                    }
                }
            }
            window.draw(username);

            //write welcome message on window
            sf::Text text;
            text.setFont(font);
            text.setString("WELCOME TO MINESWEEPER!");
            text.setCharacterSize(24);
            text.setFillColor(sf::Color::White);
            text.setStyle(sf::Text::Bold | sf::Text::Underlined);
            setText(text, width/2, height / 2 - 150);
            window.draw(text);

            //write "enter your name"
            text.setString("Enter your name:");
            text.setStyle(sf::Text::Bold);
            setText(text, width / 2, height / 2 - 75);
            window.draw(text);

            window.display();
        }
    }
};