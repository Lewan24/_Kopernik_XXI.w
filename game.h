////////////////////////////////////////////////////////////
//
// Copyright (C) 2018 Artur Lewandowski (aka. Uncle Bob) (LewanMordor@protonmail.com)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
// you must not claim that you wrote the original software.
// If you use this software in a product, an acknowledgment
// in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
// and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
////////////////////////////////////////////////////////////

#ifndef game_h
#define game_h

#include <SFML/Graphics.hpp>
#include <Windows.h>
#include <string>
//#include <SFML/Network.hpp>
//#include <SFML/Audio.hpp>
//#include <SFML/System.hpp>
//#include <SFML/Window.hpp>

using namespace std;

class Game
{
public:
	Game(void);  // konstruktor
	~Game(void);    // destruktor

	void runGame();
	void console();
	void ErrorMsg(string, string);

protected:
	enum GameState {MENU,GAMESTART,END,GAMEOVER};
	GameState state;

private:
	sf::Font font;
    string Title = "Kopernik i Plaska Ziemia";
	void menu();
	void startgame();
	void gameOver();
};

#endif // game_h
