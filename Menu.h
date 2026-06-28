
#pragma once
#include <SFML/Graphics.hpp>

enum class MenuResult { Play, Exit };

class Menu {
public:

    static MenuResult show(sf::RenderWindow& window, int& boardColorTheme);
};