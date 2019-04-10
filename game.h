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
	void ErrorMsg(string, string);

protected:
	enum GameState {MENU,GAMESTART,END};
	GameState state;

private:
	sf::Font font;
    const string Title = "Kopernik i Plaska Ziemia";
	void menu();
	void startgame();
};

#endif // game_h
