#include "Menu.h"
#include <SFML/Audio.hpp>
#include <iostream>
#include <string>
#include <algorithm>

static const int NUM_THEMES = 4;
static const char* themeNames[NUM_THEMES] = { "Classic", "Blue", "Green", "Gray" };
struct Button
{
    sf::RectangleShape shadow;
    sf::RectangleShape body;
    sf::RectangleShape accent;
    sf::Text label;
    sf::Color baseColor;
    sf::Color accentColor;

    void setup(float x, float y, float w, float h,
        const std::string& text, sf::Font& font,
        sf::Color base, sf::Color accentCol)
    {
        baseColor = base;
        accentColor = accentCol;

        shadow.setSize(sf::Vector2f(w, h));
        shadow.setPosition(x + 5.f, y + 6.f);
        shadow.setFillColor(sf::Color(0, 0, 0, 90));

        body.setSize(sf::Vector2f(w, h));
        body.setPosition(x, y);
        body.setFillColor(base);
        body.setOutlineThickness(1.5f);
        body.setOutlineColor(sf::Color(255, 255, 255, 50));

        accent.setSize(sf::Vector2f(6.f, h));
        accent.setPosition(x, y);
        accent.setFillColor(accentCol);

        label.setFont(font);
        label.setString(text);
        label.setCharacterSize(26);
        label.setStyle(sf::Text::Bold);
        label.setFillColor(sf::Color::White);
        centerLabel();
    }

    void centerLabel()
    {
        sf::FloatRect b = label.getLocalBounds();
        sf::FloatRect bodyRect = body.getGlobalBounds();
        label.setPosition(
            bodyRect.left + (bodyRect.width - b.width) / 2.f - b.left,
            bodyRect.top + (bodyRect.height - b.height) / 2.f - b.top - 3.f
        );
    }

    void setText(const std::string& text)
    {
        label.setString(text);
        centerLabel();
    }

    bool contains(sf::Vector2f p) const
    {
        return body.getGlobalBounds().contains(p);
    }

    void updateHover(sf::Vector2f mouse)
    {
        bool hover = contains(mouse);

        // Lighten on hover
        sf::Color c = baseColor;
        if (hover)
        {
            c.r = (sf::Uint8)std::min(255, c.r + 28);
            c.g = (sf::Uint8)std::min(255, c.g + 28);
            c.b = (sf::Uint8)std::min(255, c.b + 28);
        }
        body.setFillColor(c);
        body.setOutlineColor(hover ? sf::Color(255, 255, 255, 130) : sf::Color(255, 255, 255, 50));

   
        sf::Vector2f basePos = body.getPosition();
        float grow = hover ? 3.f : 0.f;
        sf::Vector2f size = body.getSize(); // size kept constant we just shift shadow instead

        shadow.setPosition(basePos.x + 5.f + grow * 0.5f, basePos.y + 6.f + grow * 0.5f);
        accent.setPosition(basePos.x, basePos.y);
    }

    void draw(sf::RenderWindow& window)
    {
        window.draw(shadow);
        window.draw(body);
        window.draw(accent);
        window.draw(label);
    }
};



MenuResult Menu::show(sf::RenderWindow& window, int& boardColorTheme)
{
    sf::Font uiFont;
    if (!uiFont.loadFromFile("arial.ttf"))
        std::cerr << "Warning: could not load arial.ttf\n";

    float winW = (float)window.getSize().x;
    float winH = (float)window.getSize().y;
    sf::Texture bgTexture;
    bool hasBgImage = bgTexture.loadFromFile("images/background7.jpg"); 
    sf::Sprite bgSprite;
    if (hasBgImage)
    {
        bgSprite.setTexture(bgTexture);
        sf::Vector2u texSize = bgTexture.getSize();
        bgSprite.setScale(winW / (float)texSize.x, winH / (float)texSize.y);
    }
    sf::VertexArray bg(sf::TriangleStrip, 4);
    sf::Color topColor(32, 40, 58);
    sf::Color bottomColor(58, 68, 92);
    bg[0].position = sf::Vector2f(0, 0);       bg[0].color = topColor;
    bg[1].position = sf::Vector2f(winW, 0);    bg[1].color = topColor;
    bg[2].position = sf::Vector2f(0, winH);    bg[2].color = bottomColor;
    bg[3].position = sf::Vector2f(winW, winH); bg[3].color = bottomColor;
    sf::RectangleShape overlay(sf::Vector2f(winW, winH));
    overlay.setFillColor(sf::Color(15, 18, 25, 140)); // semi-transparent dark veil

    sf::Text title("Chess Game", uiFont, 64);
    title.setStyle(sf::Text::Bold | sf::Text::Italic);
    title.setFillColor(sf::Color::White);
    title.setOutlineColor(sf::Color(20, 20, 30));
    title.setOutlineThickness(2.f);
    sf::FloatRect tb = title.getLocalBounds();
    title.setPosition(winW / 2.f - tb.width / 2.f - tb.left, 110.f);
    float btnW = 340.f, btnH = 66.f, gap = 26.f;
    float startY = 470.f;
    float btnX = winW / 2.f - btnW / 2.f;

    Button playButton, colorButton, exitButton;
    playButton.setup(btnX, startY, btnW, btnH, "Play",
        uiFont, sf::Color(40, 130, 100), sf::Color(80, 220, 170));
    colorButton.setup(btnX, startY + (btnH + gap), btnW, btnH, "Board Color: " + std::string(themeNames[boardColorTheme]),
        uiFont, sf::Color(55, 70, 95), sf::Color(120, 160, 220));
    exitButton.setup(btnX, startY + 2 * (btnH + gap), btnW, btnH, "Exit",
        uiFont, sf::Color(120, 55, 55), sf::Color(230, 110, 110));

    sf::SoundBuffer clickBuffer;
    bool hasClickSound = clickBuffer.loadFromFile("sounds/click.wav");
    if (!hasClickSound)
        std::cerr << "Warning: could not load sounds/click.wav\n";

    sf::Sound clickSound;
    if (hasClickSound)
    {
        clickSound.setBuffer(clickBuffer);
        clickSound.setVolume(20.f); // 0-100, lower this for a quieter click
    }
    auto playClickAndWait = [&]()
        {
            if (hasClickSound)
            {
                clickSound.play();
                sf::sleep(sf::milliseconds(150));
            }
        };

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
                return MenuResult::Exit;
            }

            if (event.type == sf::Event::MouseButtonPressed &&
                event.mouseButton.button == sf::Mouse::Left)
            {
                sf::Vector2f mousePos((float)event.mouseButton.x, (float)event.mouseButton.y);

                if (playButton.contains(mousePos))
                {
                    playClickAndWait();
                    return MenuResult::Play;
                }

                if (colorButton.contains(mousePos))
                {
                    playClickAndWait();
                    boardColorTheme = (boardColorTheme + 1) % NUM_THEMES;
                    colorButton.setText("Board Color: " + std::string(themeNames[boardColorTheme]));
                }

                if (exitButton.contains(mousePos))
                {
                    playClickAndWait();
                    window.close();
                    return MenuResult::Exit;
                }
            }
        }

        sf::Vector2i mp = sf::Mouse::getPosition(window);
        sf::Vector2f mpf((float)mp.x, (float)mp.y);

        playButton.updateHover(mpf);
        colorButton.updateHover(mpf);
        exitButton.updateHover(mpf);

        window.clear();

        if (hasBgImage)
            window.draw(bgSprite);
        else
            window.draw(bg);

        window.draw(overlay);

        window.draw(title);

        playButton.draw(window);
        colorButton.draw(window);
        exitButton.draw(window);

        window.display();
    }
    return MenuResult::Exit;
}