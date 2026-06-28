
#include <SFML/Graphics.hpp>
#include "Board.h"
#include "Menu.h"
#include "GameScreen.h"

int main()
{
  
    sf::RenderWindow window(sf::VideoMode(800, 840), "Chess",
        sf::Style::Titlebar | sf::Style::Close);

    int boardColorTheme = 0; 

    while (window.isOpen())
    {
        MenuResult choice = Menu::show(window, boardColorTheme);

        if (choice == MenuResult::Exit)
        {
            if (window.isOpen()) window.close();
            break;
        }

        if (choice == MenuResult::Play)
        {
            Board board;
            board.allocate();

            GameScreen::run(window, board, boardColorTheme);
     
        }
    }

    return 0;
}