#include "GameScreen.h"
#include <SFML/Audio.hpp>
#include <iostream>
#include <string>
#include <cctype>
// Maps a board char to its Unicode chess glyph.

static sf::String pieceSymbol(char p)
{
    switch (p)
    {
    case 'K': return sf::String(L"\u2654");
    case 'Q': return sf::String(L"\u2655");
    case 'R': return sf::String(L"\u2656");
    case 'B': return sf::String(L"\u2657");
    case 'N': return sf::String(L"\u2658");
    case 'P': return sf::String(L"\u2659");
    case 'k': return sf::String(L"\u265A");
    case 'q': return sf::String(L"\u265B");
    case 'r': return sf::String(L"\u265C");
    case 'b': return sf::String(L"\u265D");
    case 'n': return sf::String(L"\u265E");
    case 'p': return sf::String(L"\u265F");
    default:  return sf::String("");
    }
}
static std::string describeResult(Board::MoveResult result, bool moverWasWhite, int currentTurn)
{
    switch (result)
    {
    case Board::MoveResult::Checkmate:
        return moverWasWhite ? "Checkmate! White wins!  (ESC = menu)" : "Checkmate! Black wins!  (ESC = menu)";
    case Board::MoveResult::Check:
        return moverWasWhite ? "Black is in check!  Black's turn" : "White is in check!  White's turn";
    case Board::MoveResult::Invalid:
        return "Invalid move - try again";
    case Board::MoveResult::NeedsPromotion:
        return "Choose a piece to promote to";
    case Board::MoveResult::Success:
    default:
        return (currentTurn == 1) ? "White's turn  (ESC = back to menu)" : "Black's turn  (ESC = back to menu)";
    }
}

void GameScreen::run(sf::RenderWindow& window, Board& board, int boardColorTheme)
{
    const int squareSize = (int)window.getSize().x / 8;
    const float boardPixelSize = (float)(squareSize * 8);

    // 4 board color themes: { light, dark }
    static const sf::Color lightColors[4] = {
        sf::Color(240, 217, 181), // classic cream
        sf::Color(222, 235, 250), // light blue
        sf::Color(220, 237, 193), // light green
        sf::Color(235, 235, 235)  // light gray
    };
    static const sf::Color darkColors[4] = {
        sf::Color(181, 136, 99),  // classic brown
        sf::Color(120, 150, 200), // blue
        sf::Color(110, 160, 110), // green
        sf::Color(120, 120, 120)  // gray
    };

    sf::Font pieceFont;
    if (!pieceFont.loadFromFile("seguisym.ttf"))
        std::cerr << "Warning: could not load seguisym.ttf (place it next to the .exe)\n";

    sf::Font uiFont;
    if (!uiFont.loadFromFile("arial.ttf"))
        std::cerr << "Warning: could not load arial.ttf (place it next to the .exe)\n";

    // ---- sound effects -----------------------------------------------
  
    // Put 3 short clips in a "sounds" folder next to the .exe.
    sf::SoundBuffer moveBuffer, promoteBuffer, winBuffer;
    bool hasMoveSound = moveBuffer.loadFromFile("sounds/card-place.ogg");
    bool hasPromoteSound = promoteBuffer.loadFromFile("sounds/promote_sound.wav");
    bool hasWinSound = winBuffer.loadFromFile("sounds/win.wav"); 

    if (!hasMoveSound)    std::cerr << "Warning: could not load sounds/card-place.ogg\n";
    if (!hasPromoteSound) std::cerr << "Warning: could not load sounds/promote sound.wav\n";
    if (!hasWinSound)     std::cerr << "Warning: could not load sounds/win.wav\n";

    sf::Sound moveSound, promoteSound, winSound;
    if (hasMoveSound) { moveSound.setBuffer(moveBuffer);       moveSound.setVolume(20.f); }    // 0-100, quiet tap
    if (hasPromoteSound) { promoteSound.setBuffer(promoteBuffer); promoteSound.setVolume(75.f); }  // a bit more noticeable
    if (hasWinSound) { winSound.setBuffer(winBuffer);         winSound.setVolume(30.f); }      // loudest - it's the big moment

    int selectedRow = -1, selectedCol = -1;
    std::string statusMessage = "White's turn  (ESC = back to menu)";

    // ---- pawn promotion picker state ----
    bool awaitingPromotion = false;
    bool promotionIsWhite = true;
    const char promoLetters[4] = { 'Q', 'R', 'B', 'N' };

    const float optionSize = 90.f;
    const float optionGap = 14.f;
    const float optionsTotalWidth = 4 * optionSize + 3 * optionGap;
    const float optionsStartX = (window.getSize().x - optionsTotalWidth) / 2.f;
    const float optionsY = boardPixelSize / 2.f - optionSize / 2.f;

    sf::FloatRect promoRects[4];
    for (int i = 0; i < 4; i++)
    {
        promoRects[i] = sf::FloatRect(optionsStartX + i * (optionSize + optionGap), optionsY, optionSize, optionSize);
    }

    // ---- big checkmate banner state ----
    bool showGameOverBanner = false;
    std::string winnerText = "";

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
                return;
            }

            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
            {
                return; // back to menu
            }

            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
            {
                sf::Vector2f mousePos((float)event.mouseButton.x, (float)event.mouseButton.y);

                if (awaitingPromotion)
                {
                    // Only the 4 promotion options are clickable right now
                    for (int i = 0; i < 4; i++)
                    {
                        if (promoRects[i].contains(mousePos))
                        {
                            bool moverWasWhite = promotionIsWhite;
                            Board::MoveResult result = board.finishPromotion(promoLetters[i]);
                            awaitingPromotion = false;

                            if (hasPromoteSound) promoteSound.play();

                            if (result == Board::MoveResult::Checkmate)
                            {
                                showGameOverBanner = true;
                                winnerText = moverWasWhite ? "White Wins!" : "Black Wins!";
                                if (hasWinSound) winSound.play();
                            }

                            statusMessage = describeResult(result, moverWasWhite, board.turn);
                            break;
                        }
                    }
                }
                else if (!board.isGameOver())
                {
                    int col = event.mouseButton.x / squareSize;
                    int row = event.mouseButton.y / squareSize;

                    if (row >= 0 && row < 8 && col >= 0 && col < 8)
                    {
                        if (selectedRow == -1)
                        {
                            char piece = board.board[row][col];
                            if (piece != '-')
                            {
                                bool isWhitePiece = isupper(piece);
                                bool isWhitesTurn = (board.turn == 1);
                                if (isWhitePiece == isWhitesTurn)
                                {
                                    selectedRow = row;
                                    selectedCol = col;
                                }
                            }
                        }
                        else
                        {
                            if (row == selectedRow && col == selectedCol)
                            {
                                selectedRow = selectedCol = -1;
                            }
                            else
                            {
                                char clickedPiece = board.board[row][col];
                                bool clickedSameColor = false;
                                if (clickedPiece != '-')
                                {
                                    bool isWhitePiece = isupper(clickedPiece);
                                    bool isWhitesTurn = (board.turn == 1);
                                    clickedSameColor = (isWhitePiece == isWhitesTurn);
                                }

                                if (clickedSameColor)
                                {
                                    selectedRow = row;
                                    selectedCol = col;
                                }
                                else
                                {
                                    bool moverWasWhite = (board.turn == 1);
                                    Board::MoveResult result = board.tryMove(selectedRow, selectedCol, row, col);
                                    selectedRow = selectedCol = -1;

                                
                                    if (result != Board::MoveResult::Invalid && hasMoveSound)
                                        moveSound.play();

                                    if (result == Board::MoveResult::NeedsPromotion)
                                    {
                                        awaitingPromotion = true;
                                        promotionIsWhite = moverWasWhite;
                                    }
                                    else if (result == Board::MoveResult::Checkmate)
                                    {
                                        showGameOverBanner = true;
                                        winnerText = moverWasWhite ? "White Wins!" : "Black Wins!";
                                        if (hasWinSound) winSound.play();
                                    }

                                    statusMessage = describeResult(result, moverWasWhite, board.turn);
                                }
                            }
                        }
                    }
                }
            }
        }

        window.clear();

        // ---- draw board squares ----
        for (int r = 0; r < 8; r++)
        {
            for (int c = 0; c < 8; c++)
            {
                sf::RectangleShape square(sf::Vector2f((float)squareSize, (float)squareSize));
                square.setPosition((float)(c * squareSize), (float)(r * squareSize));

                bool isLight = ((r + c) % 2 == 0);
                square.setFillColor(isLight ? lightColors[boardColorTheme] : darkColors[boardColorTheme]);

                if (r == selectedRow && c == selectedCol)
                    square.setFillColor(sf::Color(255, 255, 0, 180));

                window.draw(square);
            }
        }

     
        {
            sf::Text label("", uiFont, 16);
            label.setStyle(sf::Text::Bold);

            for (int c = 0; c < 8; c++)
            {
                bool isLightSquare = ((7 + c) % 2 == 0); // bottom row is r = 7
                label.setString(std::string(1, char('a' + c)));
                label.setFillColor(isLightSquare ? darkColors[boardColorTheme] : lightColors[boardColorTheme]);
                sf::FloatRect lb = label.getLocalBounds();
                label.setPosition(c * squareSize + squareSize - lb.width - 6.f - lb.left,
                    7 * squareSize + squareSize - lb.height - 6.f - lb.top);
                window.draw(label);
            }

            for (int r = 0; r < 8; r++)
            {
                bool isLightSquare = (r % 2 == 0); // left column is c = 0
                label.setString(std::to_string(8 - r));
                label.setFillColor(isLightSquare ? darkColors[boardColorTheme] : lightColors[boardColorTheme]);
                sf::FloatRect lb = label.getLocalBounds();
                label.setPosition(6.f - lb.left, r * squareSize + 4.f - lb.top);
                window.draw(label);
            }
        }

        // ---- draw pieces ----
        for (int r = 0; r < 8; r++)
        {
            for (int c = 0; c < 8; c++)
            {
                char p = board.board[r][c];
                if (p == '-') continue;

                sf::Text text(pieceSymbol(p), pieceFont, (unsigned int)(squareSize * 70 / 100));
                text.setFillColor(isupper(p) ? sf::Color::White : sf::Color::Black);
                if (isupper(p))
                {
                    text.setOutlineColor(sf::Color::Black);
                    text.setOutlineThickness(1.5f);
                }

                sf::FloatRect bounds = text.getLocalBounds();
                text.setPosition(
                    c * squareSize + (squareSize - bounds.width) / 2.f - bounds.left,
                    r * squareSize + (squareSize - bounds.height) / 2.f - bounds.top
                );

                window.draw(text);
            }
        }

        // ---- pawn promotion picker overlay ----
        if (awaitingPromotion)
        {
            sf::RectangleShape dim(sf::Vector2f(boardPixelSize, boardPixelSize));
            dim.setPosition(0, 0);
            dim.setFillColor(sf::Color(0, 0, 0, 170));
            window.draw(dim);

            sf::Text prompt("Promote pawn to:", uiFont, 22);
            prompt.setFillColor(sf::Color::White);
            sf::FloatRect promptBounds = prompt.getLocalBounds();
            prompt.setPosition((window.getSize().x - promptBounds.width) / 2.f - promptBounds.left,
                optionsY - 50.f);
            window.draw(prompt);

            sf::Vector2i mp = sf::Mouse::getPosition(window);
            sf::Vector2f mpf((float)mp.x, (float)mp.y);

            for (int i = 0; i < 4; i++)
            {
                sf::RectangleShape optionBox(sf::Vector2f(optionSize, optionSize));
                optionBox.setPosition(promoRects[i].left, promoRects[i].top);
                bool hovered = promoRects[i].contains(mpf);
                optionBox.setFillColor(hovered ? sf::Color(235, 235, 245) : sf::Color(255, 255, 255));
                optionBox.setOutlineThickness(2.f);
                optionBox.setOutlineColor(sf::Color(40, 40, 40));
                window.draw(optionBox);

                char displayPiece = promotionIsWhite ? promoLetters[i] : (char)tolower(promoLetters[i]);
                sf::Text pieceText(pieceSymbol(displayPiece), pieceFont, (unsigned int)(optionSize * 0.65f));
                pieceText.setFillColor(sf::Color::Black);
                sf::FloatRect pb = pieceText.getLocalBounds();
                pieceText.setPosition(
                    promoRects[i].left + (optionSize - pb.width) / 2.f - pb.left,
                    promoRects[i].top + (optionSize - pb.height) / 2.f - pb.top
                );
                window.draw(pieceText);
            }
        }

        // ---- big checkmate banner ----
        if (showGameOverBanner)
        {
            sf::RectangleShape dim(sf::Vector2f(boardPixelSize, boardPixelSize));
            dim.setPosition(0, 0);
            dim.setFillColor(sf::Color(0, 0, 0, 190));
            window.draw(dim);

            float centerY = boardPixelSize / 2.f;

            sf::Text big("CHECKMATE", uiFont, 56);
            big.setStyle(sf::Text::Bold);
            big.setFillColor(sf::Color(255, 210, 60));
            big.setOutlineColor(sf::Color::Black);
            big.setOutlineThickness(3.f);
            sf::FloatRect bb = big.getLocalBounds();
            big.setPosition((window.getSize().x - bb.width) / 2.f - bb.left, centerY - 110.f);
            window.draw(big);

            sf::Text winner(winnerText, uiFont, 40);
            winner.setStyle(sf::Text::Bold);
            winner.setFillColor(sf::Color::White);
            winner.setOutlineColor(sf::Color::Black);
            winner.setOutlineThickness(2.f);
            sf::FloatRect wb = winner.getLocalBounds();
            winner.setPosition((window.getSize().x - wb.width) / 2.f - wb.left, centerY - 30.f);
            window.draw(winner);

            sf::Text hint("Press ESC to return to menu", uiFont, 20);
            hint.setFillColor(sf::Color(220, 220, 220));
            sf::FloatRect hb = hint.getLocalBounds();
            hint.setPosition((window.getSize().x - hb.width) / 2.f - hb.left, centerY + 50.f);
            window.draw(hint);
        }
        sf::RectangleShape statusBar(sf::Vector2f((float)window.getSize().x, 32.f));
        statusBar.setPosition(0, boardPixelSize);
        statusBar.setFillColor(sf::Color(20, 20, 20));
        window.draw(statusBar);

        sf::Text status(statusMessage, uiFont, 18);
        status.setFillColor(sf::Color::White);
        status.setPosition(10.f, boardPixelSize + 6.f);
        window.draw(status);
        window.display();
    }
}