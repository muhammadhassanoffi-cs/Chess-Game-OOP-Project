
#pragma once
#include <SFML/Graphics.hpp>
#include "Board.h"

class GameScreen {
public:

    static void run(sf::RenderWindow& window, Board& board, int boardColorTheme);
};
