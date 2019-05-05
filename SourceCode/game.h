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
#include <cstring>
#include <iostream>

//#include <SFML/Network.hpp>
//#include <SFML/System.hpp>
//#include <SFML/Window.hpp>

using namespace std;

class Game
{
public:
	Game(void);  // konstruktor
	~Game(void);    // destruktor

    short ile_linii = 21;

    int howmany = 23; // ilosc klatek
    sf::Texture bgTexture[23];
    sf::String jakisstring[23];

	void runGame();
	void gameUpdate(bool);
	void help();
	void console();
	void optionsReset();
	void ErrorMsg(string, string);

protected:
	enum GameState {MENU,GAMESTART,END,GAMEOVER,
                    CUT1,CUT2,CUT3,CUT4,
                    MENUOPTIONS,HINTS,GREETINGS,
                    WYNIKI,ANIMACJA,LOADING,INFO,
                    KOP1, KOP2, KOP3};
	GameState state;

private:
    int iloscprezentow = 0;
    bool przegrana = false;
    float punkty = 0;
    float mnoznikpkt = 1;
    //bool loading = true;

    bool ciekawostki[21] = {false}; // ilosc linii w ciekawostkach - 1

    bool dzwiek = true;
    bool przej = true;
    bool playmusic1 = true;
    bool zycia = true;
    bool platforma = true;

    int backgroundY = -4080;
    short iloscZyc = 4;
    short trudnosc = 0;
	sf::Font font;

    //string Wyjscie = "Escape, aby wrocic";
	void menu();
	void greetings();
	void hints();
	void options();
	void cut1(); // cut scena nr 1
	void cut2();
	void cut3();
	void cut4();
	void gameOver();
	void startgame();
	void animacja();
	void wyniki();
	void loading();
	void kop_info();
	void kop1();
	void kop2();
	void kop3();
};

#endif // game_h
