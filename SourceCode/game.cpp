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

#include <fstream>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <time.h>
#include <string>
#include <windows.h>

#include "game.h"

using namespace sf;

short wysokosc = 600, szerokosc = 800;

sf::Vector2i screenDimensions(szerokosc,wysokosc);
sf::RenderWindow window(sf::VideoMode(screenDimensions.x, screenDimensions.y), "Kopernik i Plaska Ziemia",Style::Close);

/*
    window.setMouseCursorVisible(false);

	sf::View fixed = window.getView();
	sf::Texture cursorTexture;
	if(!cursorTexture.loadFromFile("Resources/Textures/cursor.png"))
    {
        MessageBox(NULL,"Cursor not found! Check: 'Resources/Textures/cursor.png'","ERROR",NULL);
		state = END;
    }
	sf::Sprite cursor(cursorTexture);

    cursor.setPosition(static_cast<sf::Vector2f>(sf::Mouse::getPosition(window))); // przed 'window.clear();'
    window.setView(fixed);
    window.draw(cursor);
*/
//TODO: Znalezc jakies ciemne nie razace w oczy tla do menu etc / lub zostawic czarno bialy design

// TODO: Wyniki moga byc liczone od sekund przebytych w grze, od ilosci przeskokow pomiedzy prawa a lewa krawedzia
// gry, ilosc wysokich skokow np. po spacji, poziom trudnosci im ciezszy tym wiecej punktow sie dostaje
// szybkosc dojscia do konca gry, ilosc zebranych ciekawostek o koperniku
// TODO: Stworzyc przed ostatnia cutscenka tablice z koncowym wynikiem itd;

Game::Game(void)
{
	state = END;

	if(!font.loadFromFile("Resources/Fonts/main.otf")) {
        ErrorMsg("Hmm... Chyba brakuje czcionki! Sprawdz 'Resources/Fonts/main.otf'","ERROR");
        return;
    }
    if (!font2.loadFromFile("Resources/Fonts/optional.ttf")){
        ErrorMsg("Hmm... Chyba brakuje czcionki! Sprawdz 'Resources/Fonts/optional.ttf'","ERROR");
        return;
    }
    if (!font3.loadFromFile("Resources/Fonts/text.ttf")){
        ErrorMsg("Hmm... Chyba brakuje czcionki! Sprawdz 'Resources/Fonts/text.ttf'","ERROR");
        return;
    }

	state = MENU;
}


Game::~Game(void)
{
}

void Game::ErrorMsg(string error, string title)
{
    window.close();
    MessageBoxA(NULL, error.c_str(), title.c_str(), MB_ICONERROR | MB_OK);
    state = END;
}

void Game::runGame()
{   // TODO: Dodac sciezke dzwiekowa
    window.setFramerateLimit(60);

	while(state != END)
	{
		switch (state)
		{
            case MENU:
                menu();
                break;

            case MENUOPTIONS:
                options();
                break;

            case GAMESTART:
                startgame();
                break;

            case GAMEOVER:
                gameOver();
                break;

            case CUT1:
                cut1();
                break;

            case CUT2:
                cut2();
                break;

            case CUT3:
                cut3();
                break;

            case CUT4:
                cut4();
                break;

            case GREETINGS:
                greetings();
                break;

            case HINTS:
                hints();
                break;

            /* TODO: Zrobic Dodatki
            mozliwosc wylaczenia zderzen z tabliczkami
            tutorial?
            opis gry
            opis Kopernika i jego osiagniec // w dodatkach bedzie jakas dluzsza wersja troche o nim opowiadajaca
            */
		}
	}
}

void Game::greetings()
{
    // state = GREETINGS

    Text title(Title,font,40);
	title.setStyle(Text::Bold);

	title.setPosition(szerokosc/2-title.getGlobalBounds().width/2,20);
//////////////////////////////////////////////////////////////////////////////
    window.setMouseCursorVisible(false);

	sf::View fixed = window.getView();
	sf::Texture cursorTexture;
	if(!cursorTexture.loadFromFile("Resources/Textures/cursor.png"))// Custon Cursor
		ErrorMsg("Cursor not found! Check: 'Resources/Textures/cursor.png'","ERROR");

	sf::Sprite cursor(cursorTexture);
/////////////////////////////////////////////////////////////////////////////

    sf::Text skip(Skip,font,20);
    skip.setPosition(szerokosc-skip.getGlobalBounds().width-15,wysokosc-skip.getGlobalBounds().height-20);

    Texture t1;
    if (!t1.loadFromFile("Resources/Textures/greetings.png"))
        ErrorMsg("Hmm... Chyba brakuje textury! Sprawdz 'Resources/Textures/greetings.png'","ERROR");

    sf::Sprite Background(t1);

    Background.setPosition(200, 600);

    sf::Clock zegar;
    sf::Time czas;

    zegar.restart();

	while(state == GREETINGS)
	{
		Vector2f mouse(Mouse::getPosition(window));
		Event event;

		while(window.pollEvent(event))
		{
			//Wciœniêcie ESC lub przycisk X
			if(event.type == Event::Closed)
				state = END;

			//klikniêcie SPACE
			else if(event.type == Event::KeyPressed && event.key.code == Keyboard::Space)
			{
				state = MENU;
			}
			if(event.type == Event::KeyPressed && event.key.code == Keyboard::Slash)
                console();
		}

        //std::cout << Background.getPosition().y << std::endl;

        if (Background.getPosition().y < -1750)
            state = MENU;

        cursor.setPosition(static_cast<sf::Vector2f>(sf::Mouse::getPosition(window))); // for custom cursor

        if (czas.asMilliseconds() > 10){
            Background.move(0,-1);
            zegar.restart();
        }
        czas=zegar.getElapsedTime();

		window.clear();

        window.draw(Background);

		window.draw(title);
		window.draw(skip);

        window.setView(fixed);
        window.draw(cursor);
		window.display();
	}
}

void Game::menu()
{
	Text title(Title,font,70);
	title.setStyle(Text::Bold);

	title.setPosition(szerokosc/2-title.getGlobalBounds().width/2,20);
//////////////////////////////////////////////////////////////////////////////
    window.setMouseCursorVisible(false);

	sf::View fixed = window.getView();
	sf::Texture cursorTexture;
	if(!cursorTexture.loadFromFile("Resources/Textures/cursor.png"))// Custon Cursor
		ErrorMsg("Cursor not found! Check: 'Resources/Textures/cursor.png'","ERROR");

	sf::Sprite cursor(cursorTexture);
/////////////////////////////////////////////////////////////////////////////
	const int ile = 4;

	Text tekst[ile];

	string str[] = {"Rozpocznij","Dodatki","Zamknij","Porady"};
	for(int i=0;i<ile;i++)
	{
		tekst[i].setFont(font);
		tekst[i].setCharacterSize(60);              // Main Menu, texts and buttons

		tekst[i].setString(str[i]);
		tekst[i].setPosition(szerokosc/2-tekst[i].getGlobalBounds().width/2,250+i*120);
	}
///////////////////////////////////////////////////////////////////////////////////
    tekst[3].setPosition(5,wysokosc-tekst[3].getGlobalBounds().height-15);

	while(state == MENU)
	{
		Vector2f mouse(Mouse::getPosition(window));
		Event event;

		while(window.pollEvent(event))
		{
			//Wciœniêcie ESC lub przycisk X
			if(event.type == Event::Closed || event.type == Event::KeyPressed &&
				event.key.code == Keyboard::Escape)
				state = END;

			//klikniêcie EXIT
			else if(tekst[2].getGlobalBounds().contains(mouse) &&
				event.type == Event::MouseButtonReleased && event.key.code == Mouse::Left)
			{
				state = END;
			}
			else if(tekst[1].getGlobalBounds().contains(mouse) &&
				event.type == Event::MouseButtonReleased && event.key.code == Mouse::Left)
			{
				state = MENUOPTIONS;
			}
			else if(tekst[0].getGlobalBounds().contains(mouse) &&
				event.type == Event::MouseButtonReleased && event.key.code == Mouse::Left)
			{
				state = CUT1;
			}
			else if(tekst[3].getGlobalBounds().contains(mouse) &&
				event.type == Event::MouseButtonReleased && event.key.code == Mouse::Left)
			{
				state = HINTS;
			}
			if(event.type == Event::KeyPressed && event.key.code == Keyboard::Slash)
                console();
		}
		for(int i=0;i<ile;i++)
			if(tekst[i].getGlobalBounds().contains(mouse))
				tekst[i].setColor(Color::Cyan); // when you will move your mouse on button
			else tekst[i].setColor(Color::White);

        cursor.setPosition(static_cast<sf::Vector2f>(sf::Mouse::getPosition(window))); // for custom cursor

		window.clear();

		window.draw(title);
		for(int i=0;i<ile;i++)
			window.draw(tekst[i]);

        window.setView(fixed);
        window.draw(cursor);
		window.display();
	}
}

void Game::hints()
{
    // TODO: 2-4 zdjecia z poradami do gry, moze jakies screeny z gry, najlepiej zrobione w PhotoShop'ie
    // state = HINTS
	Text title(Title,font,40);
	title.setStyle(Text::Bold);

	title.setPosition(szerokosc/2-title.getGlobalBounds().width/2,20);
//////////////////////////////////////////////////////////////////////////////
    window.setMouseCursorVisible(false);

	sf::View fixed = window.getView();
	sf::Texture cursorTexture;
	if(!cursorTexture.loadFromFile("Resources/Textures/cursor.png"))// Custon Cursor
		ErrorMsg("Cursor not found! Check: 'Resources/Textures/cursor.png'","ERROR");

	sf::Sprite cursor(cursorTexture);
/////////////////////////////////////////////////////////////////////////////
	const int ile = 2;

	Text tekst[ile];

	string str[] = {"Kontynuuj","Wyjdz"};
	for(int i=0;i<ile;i++)
	{
		tekst[i].setFont(font);
		tekst[i].setCharacterSize(40);              // Main Menu, texts and buttons

		tekst[i].setString(str[i]);
		tekst[i].setPosition(szerokosc-tekst[i].getGlobalBounds().width-15,500+i*40);
	}
///////////////////////////////////////////////////////////////////////////////////

	while(state == HINTS)
	{
		Vector2f mouse(Mouse::getPosition(window));
		Event event;

		while(window.pollEvent(event))
		{
			//Wciœniêcie ESC lub przycisk X
			if(event.type == Event::Closed || event.type == Event::KeyPressed &&
				event.key.code == Keyboard::Escape)
				state = END;

			//klikniêcie EXIT
            else if(tekst[1].getGlobalBounds().contains(mouse) &&
				event.type == Event::MouseButtonReleased && event.key.code == Mouse::Left)
			{
				state = END;
			}
			else if(tekst[0].getGlobalBounds().contains(mouse) &&
				event.type == Event::MouseButtonReleased && event.key.code == Mouse::Left)
			{
				state = MENU;
			}

			if(event.type == Event::KeyPressed && event.key.code == Keyboard::Slash)
                console();
		}

		for(int i=0;i<ile;i++)
			if(tekst[i].getGlobalBounds().contains(mouse))
				tekst[i].setColor(Color::Cyan); // when you will move your mouse on button
			else tekst[i].setColor(Color::White);

        cursor.setPosition(static_cast<sf::Vector2f>(sf::Mouse::getPosition(window))); // for custom cursor

		window.clear();

		window.draw(title);

		for(int i=0;i<ile;i++)
			window.draw(tekst[i]);

        window.setView(fixed);
        window.draw(cursor);
		window.display();
	}
}


void Game::options()
{
    // state = MENUOPTIONS

    Text title(Title,font,40);
	title.setStyle(Text::Bold);

	title.setPosition(szerokosc/4-title.getGlobalBounds().width/2,20);
//////////////////////////////////////////////////////////////////////////////
    window.setMouseCursorVisible(false);

	sf::View fixed = window.getView();
	sf::Texture cursorTexture;
	if(!cursorTexture.loadFromFile("Resources/Textures/cursor.png"))// Custon Cursor
		ErrorMsg("Cursor not found! Check: 'Resources/Textures/cursor.png'","ERROR");

	sf::Sprite cursor(cursorTexture);
/////////////////////////////////////////////////////////////////////////////
	const int ile = 4;

	Text tekst[ile];

	string str[] = {"Poziom Trudnosci: ","Podziekowania","<--", "Options Reset"}; // TODO: potem dodac mozliwosc przyspieszenia gry, ale np po przejsciu gry
                                                                                // na jakims poziomie np hard :)
	for(int i=0;i<ile;i++)
	{
		tekst[i].setFont(font);
		tekst[i].setCharacterSize(50);              // Main Menu, texts and buttons

		tekst[i].setString(str[i]);
		tekst[i].setPosition(szerokosc/4-tekst[i].getGlobalBounds().width/2,250+i*60);
	}
///////////////////////////////////////////////////////////////////////////////////
    const short trudnosci = 5;
    sf::Text poziomy[trudnosci];

    string strudnosc[trudnosci] = {"Easy", "Normal", "Hard", "Serious", "Mental"};

    // TODO: Opcje maja zapisywac sie i odczytywac z pliku configuracyjnego w plikach gry

    for (int i = 0; i < trudnosci; i++){
        poziomy[i].setFont(font);
        poziomy[i].setPosition(szerokosc/2-poziomy[i].getGlobalBounds().width/2,250);
        poziomy[i].setCharacterSize(50);
        poziomy[i].setString(strudnosc[i]);
    }

    tekst[3].setPosition(szerokosc-tekst[3].getGlobalBounds().width-5,wysokosc-tekst[3].getGlobalBounds().height-5);

    string zyciastr[6] = {"0","1","2","3","4","5"};

    sf::Text ilezyc;
    ilezyc.setFont(font2);
    ilezyc.setCharacterSize(30);
    ilezyc.setString("Ilosc zyc na starcie: " + zyciastr[iloscZyc]);
    ilezyc.setPosition(15, 200);

	while(state == MENUOPTIONS)
	{
	    gameUpdate(this->zycia);

		Vector2f mouse(Mouse::getPosition(window));
		Event event;

		while(window.pollEvent(event))
		{
			//Wciœniêcie ESC lub przycisk X
			if(event.type == Event::Closed)
				state = END;

            if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape)
                state = MENU;
			//klikniêcie EXIT
			else if(tekst[2].getGlobalBounds().contains(mouse) &&
				event.type == Event::MouseButtonReleased && event.key.code == Mouse::Left)
			{
				state = MENU;
			}
			else if(tekst[1].getGlobalBounds().contains(mouse) &&
				event.type == Event::MouseButtonReleased && event.key.code == Mouse::Left)
			{
				state = GREETINGS;
			}
			else if(tekst[0].getGlobalBounds().contains(mouse) &&
				event.type == Event::MouseButtonReleased && event.key.code == Mouse::Left)
			{
				this->trudnosc++;
			}
			else if(tekst[3].getGlobalBounds().contains(mouse) &&
				event.type == Event::MouseButtonReleased && event.key.code == Mouse::Left)
			{
				optionsReset();
			}
			if(event.type == Event::KeyPressed && event.key.code == Keyboard::Slash)
                console();
		}
		for(int i=0;i<ile;i++)
			if(tekst[i].getGlobalBounds().contains(mouse))
				tekst[i].setColor(Color::Cyan); // when you will move your mouse on button
			else tekst[i].setColor(Color::White);

        if (this->trudnosc >= trudnosci)
            this->trudnosc = 0;

        ilezyc.setString("Ilosc zyc na starcie: " + zyciastr[iloscZyc]);

        cursor.setPosition(static_cast<sf::Vector2f>(sf::Mouse::getPosition(window))); // for custom cursor

		window.clear();

		window.draw(title);

		for(int i=0;i<ile;i++)
			window.draw(tekst[i]);

        window.draw(poziomy[trudnosc]);
        window.draw(ilezyc);

        window.setView(fixed);
        window.draw(cursor);
		window.display();
	}
}

void Game::cut1(){
    //state = CUT1

    sf::Text title(Title,font,20);
	title.setStyle(Text::Bold);


    title.setPosition(5,575);
    //title.setPosition(200,575);
	//title.setPosition(screenDimensions.x/2-title.getGlobalBounds().width/2,575);
	//skip.setPosition(szerokosc-skip.getGlobalBounds().width-3,550);
//////////////////////////////////////////////////////////////////////////////
    window.setMouseCursorVisible(false);

	sf::View fixed = window.getView();
	sf::Texture cursorTexture;
	if(!cursorTexture.loadFromFile("Resources/Textures/cursor.png"))// Custon Cursor
		ErrorMsg("Cursor not found! Check: 'Resources/Textures/cursor.png'","ERROR");

	sf::Sprite cursor(cursorTexture);
//////////////////////////////////////////////////////////////////////////////
    string linia;
    int ile=0;
    short ilosc_linii = 13; // ilosc linii - 1
    string str[ilosc_linii];

    fstream plik;
    plik.open("Resources/Game/cutscenes/cut1.cutscene", ios::in);

    if(plik.good()==false)
        ErrorMsg("File not found, Check: 'Resources/Game/cutscenes/cut1.cutscene'","ERROR");

    while (getline(plik, linia))
    {
        str[ile] = linia;
        ile++;
    }
    plik.close();

	sf::Text tekst[ile];

	for(int i=0;i<ile;i++)
	{
		tekst[i].setFont(font3);
		tekst[i].setCharacterSize(24);              // Main Menu, texts and buttons

		tekst[i].setString(str[i]);
		tekst[i].setPosition(szerokosc/2-tekst[i].getGlobalBounds().width/2,i*35);
	}

	sf::Clock zegar;
    sf::Time czas;

    zegar.restart();

    int cos = 0;

    sf::Text skip(Skip,font,20);
    skip.setPosition(szerokosc-skip.getGlobalBounds().width-3,550);

	while(state == CUT1)
	{
		Vector2f mouse(Mouse::getPosition(window));
		Event event;

		while(window.pollEvent(event))
		{
			//Wciœniêcie ESC lub przycisk X
			if(event.type == Event::Closed)
                state = END;
            if(event.type == Event::KeyPressed && event.key.code == Keyboard::Space)
				state = GAMESTART;
            if(event.type == Event::KeyPressed && event.key.code == Keyboard::Escape)
				state = MENU;
            if(event.type == Event::KeyPressed && event.key.code == Keyboard::Slash)
                console();
		}

        cursor.setPosition(static_cast<sf::Vector2f>(sf::Mouse::getPosition(window))); // for custom cursor

        //std::cout << cos << endl;

        if (czas.asSeconds() > 1.5){
            cos++;
            if (cos > ilosc_linii)
                cos = cos-1;

            zegar.restart();
        }
        czas=zegar.getElapsedTime();

		window.clear();

		window.draw(title);
		window.draw(skip);

        for (int i = 0; i < cos+1; i++)
            window.draw(tekst[i]);

        window.setView(fixed);
        window.draw(cursor);
		window.display();
	}
}

void Game::cut4(){
    // state = CUT4

    sf::Text title(Title,font,20);
	title.setStyle(Text::Bold);
    title.setPosition(5,575);
//////////////////////////////////////////////////////////////////////////////
    window.setMouseCursorVisible(false);

	sf::View fixed = window.getView();
	sf::Texture cursorTexture;
	if(!cursorTexture.loadFromFile("Resources/Textures/cursor.png"))// Custon Cursor
		ErrorMsg("Cursor not found! Check: 'Resources/Textures/cursor.png'","ERROR");

	sf::Sprite cursor(cursorTexture);
//////////////////////////////////////////////////////////////////////////////
    string linia;
    int ile=0;
    short ilosc_linii = 6; // ilosc linii - 1
    string str[ilosc_linii];

    fstream plik;
    plik.open("Resources/Game/cutscenes/cut4.cutscene", ios::in);

    if(plik.good()==false)
        ErrorMsg("File not found, Check: 'Resources/Game/cutscenes/cut4.cutscene'","ERROR");

    while (getline(plik, linia))
    {
        str[ile] = linia;
        ile++;
    }
    plik.close();

	sf::Text tekst[ile];

	for(int i=0;i<ile;i++)
	{
		tekst[i].setFont(font3);
		tekst[i].setCharacterSize(24);              // Tekst cutscenki z pliku txt

		tekst[i].setString(str[i]);
		tekst[i].setPosition(szerokosc/2-tekst[i].getGlobalBounds().width/2,i*35);
	}

	sf::Clock zegar;
    sf::Time czas;

    zegar.restart();

    int cos = 0;

    sf::Text skip(Skip,font,20);
    skip.setPosition(szerokosc-skip.getGlobalBounds().width-3,550);

	while(state == CUT4)
	{
		Vector2f mouse(Mouse::getPosition(window));
		Event event;

		while(window.pollEvent(event))
		{
			//Wciœniêcie ESC lub przycisk X
			if(event.type == Event::Closed)
                state = END;
            if(event.type == Event::KeyPressed && event.key.code == Keyboard::Space){
                state = GREETINGS;
            }
            if(event.type == Event::KeyPressed && event.key.code == Keyboard::Slash)
                console();
		}

        cursor.setPosition(static_cast<sf::Vector2f>(sf::Mouse::getPosition(window))); // for custom cursor

        //std::cout << cos << endl;

        if (czas.asSeconds() > 1.5){
            cos++;
            if (cos > ilosc_linii)
                cos = cos-1;

            zegar.restart();
        }
        czas=zegar.getElapsedTime();

		window.clear();

		window.draw(title);
		window.draw(skip);

        for (int i = 0; i < cos+1; i++)
            window.draw(tekst[i]);

        window.setView(fixed);
        window.draw(cursor);
		window.display();
	}
}

void Game::cut3(){
    // state = CUT3

    sf::Text title(Title,font,20);
	title.setStyle(Text::Bold);
    title.setPosition(5,575);
//////////////////////////////////////////////////////////////////////////////
    window.setMouseCursorVisible(false);

	sf::View fixed = window.getView();
	sf::Texture cursorTexture;
	if(!cursorTexture.loadFromFile("Resources/Textures/cursor.png"))// Custon Cursor
		ErrorMsg("Cursor not found! Check: 'Resources/Textures/cursor.png'","ERROR");

	sf::Sprite cursor(cursorTexture);
//////////////////////////////////////////////////////////////////////////////
    string linia;
    int ile=0;
    short ilosc_linii = 13; // ilosc linii - 1
    string str[ilosc_linii];

    fstream plik;
    plik.open("Resources/Game/cutscenes/cut3.cutscene", ios::in);

    if(plik.good()==false)
        ErrorMsg("File not found, Check: 'Resources/Game/cutscenes/cut3.cutscene'","ERROR");

    while (getline(plik, linia))
    {
        str[ile] = linia;
        ile++;
    }
    plik.close();

	sf::Text tekst[ile];

	for(int i=0;i<ile;i++)
	{
		tekst[i].setFont(font3);
		tekst[i].setCharacterSize(24);              // Tekst cutscenki z pliku txt

		tekst[i].setString(str[i]);
		tekst[i].setPosition(szerokosc/2-tekst[i].getGlobalBounds().width/2,i*35);
	}

	sf::Clock zegar;
    sf::Time czas;

    zegar.restart();

    int cos = 0;

    sf::Text skip(Skip,font,20);
    skip.setPosition(szerokosc-skip.getGlobalBounds().width-3,550);

	while(state == CUT3)
	{
		Vector2f mouse(Mouse::getPosition(window));
		Event event;

		while(window.pollEvent(event))
		{
			//Wciœniêcie ESC lub przycisk X
			if(event.type == Event::Closed)
                state = END;
            if(event.type == Event::KeyPressed && event.key.code == Keyboard::Space){
                state = CUT4;
            }
            if(event.type == Event::KeyPressed && event.key.code == Keyboard::Slash)
                console();
		}

        cursor.setPosition(static_cast<sf::Vector2f>(sf::Mouse::getPosition(window))); // for custom cursor

        //std::cout << cos << endl;

        if (czas.asSeconds() > 1.5){
            cos++;
            if (cos > ilosc_linii)
                cos = cos-1;

            zegar.restart();
        }
        czas=zegar.getElapsedTime();

		window.clear();

		window.draw(title);
		window.draw(skip);

        for (int i = 0; i < cos+1; i++)
            window.draw(tekst[i]);

        window.setView(fixed);
        window.draw(cursor);
		window.display();
	}
}

void Game::cut2(){
    // state = CUT2

    sf::Text title(Title,font,20);
	title.setStyle(Text::Bold);
    title.setPosition(5,575);
//////////////////////////////////////////////////////////////////////////////
    window.setMouseCursorVisible(false);

	sf::View fixed = window.getView();
	sf::Texture cursorTexture;
	if(!cursorTexture.loadFromFile("Resources/Textures/cursor.png"))// Custon Cursor
		ErrorMsg("Cursor not found! Check: 'Resources/Textures/cursor.png'","ERROR");

	sf::Sprite cursor(cursorTexture);
//////////////////////////////////////////////////////////////////////////////
    string linia;
    int ile=0;
    short ilosc_linii = 12; // ilosc linii - 1
    string str[ilosc_linii];

    fstream plik;
    plik.open("Resources/Game/cutscenes/cut2.cutscene", ios::in);

    if(plik.good()==false)
        ErrorMsg("File not found, Check: 'Resources/Game/cutscenes/cut2.cutscene'","ERROR");

    while (getline(plik, linia))
    {
        str[ile] = linia;
        ile++;
    }
    plik.close();

	sf::Text tekst[ile];

	for(int i=0;i<ile;i++)
	{
		tekst[i].setFont(font3);
		tekst[i].setCharacterSize(24);              // Tekst cutscenki z pliku txt

		tekst[i].setString(str[i]);
		tekst[i].setPosition(szerokosc/2-tekst[i].getGlobalBounds().width/2,i*35);
	}

	sf::Clock zegar;
    sf::Time czas;

    zegar.restart();

    int cos = 0;

    sf::Text skip(Skip,font,20);
    skip.setPosition(szerokosc-skip.getGlobalBounds().width-3,550);

	while(state == CUT2)
	{
		Vector2f mouse(Mouse::getPosition(window));
		Event event;

		while(window.pollEvent(event))
		{
			//Wciœniêcie ESC lub przycisk X
			if(event.type == Event::Closed)
                state = END;
            if(event.type == Event::KeyPressed && event.key.code == Keyboard::Space){
                this->iloscZyc += 1;
                this->backgroundY = -2000;
                this->zycia = false;
                state = GAMESTART;
            }
            if(event.type == Event::KeyPressed && event.key.code == Keyboard::Slash)
                console();
		}

        cursor.setPosition(static_cast<sf::Vector2f>(sf::Mouse::getPosition(window))); // for custom cursor

        //std::cout << cos << endl;

        if (czas.asSeconds() > 1.5){
            cos++;
            if (cos > ilosc_linii)
                cos = cos-1;

            zegar.restart();
        }
        czas=zegar.getElapsedTime();

		window.clear();

		window.draw(title);
		window.draw(skip);

        for (int i = 0; i < cos+1; i++)
            window.draw(tekst[i]);

        window.setView(fixed);
        window.draw(cursor);
		window.display();
	}
}

struct point
{ int x,y;};

void Game::startgame()
{
    gameUpdate(this->zycia);

     //  TODO: Dodaj widownie rzucajaca tabliczki (przeszkoda)

    window.setMouseCursorVisible(false);

	sf::View fixed = window.getView();
	sf::Texture cursorTexture;
	if(!cursorTexture.loadFromFile("Resources/Textures/cursor.png"))
        ErrorMsg("Cursor not found! Check: 'Resources/Textures/cursor.png'","ERROR");

	sf::Sprite cursor(cursorTexture);

    srand(time(0));

    Texture t1,t2,t3,t4;
    if (!t1.loadFromFile("Resources/Textures/background.png"))
        ErrorMsg("Hmm... Chyba brakuje textury! Sprawdz 'Resources/Textures/background.png'","ERROR");
    if (!t2.loadFromFile("Resources/Textures/platform.png"))
        ErrorMsg("Hmm... Chyba brakuje textury! Sprawdz 'Resources/Textures/platform.png'","ERROR");
    if (!t3.loadFromFile("Resources/Textures/doodle.png"))
        ErrorMsg("Hmm... Chyba brakuje textury! Sprawdz 'Resources/Textures/doodle.png'","ERROR");
    if (!t4.loadFromFile("Resources/Textures/doodle2.png"))
        ErrorMsg("Hmm... Chyba brakuje textury! Sprawdz 'Resources/Textures/doodle2.png'","ERROR");

    sf::Sprite sBackground(t1), sPlat(t2);
    sf::Sprite sPers(t3);

    sBackground.setPosition(200, backgroundY);

    sf::Texture audience1, audience2;

    if (!audience1.loadFromFile("Resources/Textures/audience.png"))
        ErrorMsg("Hmm... Chyba brakuje textury! Sprawdz 'Resources/Textures/audience.png'","ERROR");
    if (!audience2.loadFromFile("Resources/Textures/audience2.png"))
        ErrorMsg("Hmm... Chyba brakuje textury! Sprawdz 'Resources/Textures/audience2.png'","ERROR");

    sf::Sprite audienceleft, audienceright;

    audienceleft.setTexture(audience1);
    audienceright.setTexture(audience2);
    audienceleft.setPosition(0,0);
    audienceright.setPosition(600,0);

    point plat[20];

    for (int i=0;i<10;i++)
    {
       plat[i].x=rand()%400+200;
       plat[i].y=rand()%600;
    }

    int x=szerokosc/2, y=10, h=wysokosc/2;
    float dy=0;

    //bool isSpacePressed;

    const short ile = iloscZyc+1;
    //sf::Text zycia0("Lives: 0",font,30), zycia1("Lives: 1",font,30), zycia2("Lives: 2",font,30), zycia3("Lives: 3",font,30);
    sf::Text zycia[ile];


    const short ile2 = 11; // ustalam wieklosc tablicy; zawsze ma byc o 1 wieksza od max liczby w tablicy
    string zyciastr[ile2] = {"0","1","2","3","4","5","6","7","8","9","10"}; // max 10 zyc
    //string str[] = {"Lives: 0", "Lives: 1", "Lives: 2", "Lives: 3"};
    string str[ile2];

    for(int i = 0; i < ile2; i++){ // dla kazdego stringa przypisuje odpowiednia wartosc
        str[i] = "Lives: " + zyciastr[i];
    }

    for (int i = 0; i < ile; i++){ // dla kazdego stringa ustawiam kolor, wielkosc itd
        zycia[i].setFont(font2);
        zycia[i].setPosition(210,0);
        zycia[i].setCharacterSize(20);
        zycia[i].setString(str[i]);
        zycia[i].setColor(sf::Color::Red);
    }

    /*  TODO: prezent z ciekawostka o koperniku
     *  wyniki koncowe
     *  licznik wyniku
     *  tlo do gameover
     *  napis gameover + moze jakis efekt napisu
     *      napis ma byc nad opcjami do wyboru / zmniejszyc rozmiar opcji
     */

    while(state == GAMESTART)
    {
        //isSpacePressed = false;

        Vector2f mouse(Mouse::getPosition());

        Event event;

        while(window.pollEvent(event))
        {
            //Wciœniêcie ESC lub przycisk X
            if(event.type == Event::Closed || event.type == Event::KeyPressed &&
                event.key.code == Keyboard::Escape)
                state = MENU;
            if(event.type == Event::KeyPressed && event.key.code == Keyboard::Slash)
                console();

        }

        if (sBackground.getPosition().y < -2005 && sBackground.getPosition().y > -2008){
            state = CUT2;
        }

        if (sBackground.getPosition().y < 110 && sBackground.getPosition().y > 100){
            state = CUT3;
        }

        // debugowanie; sprawdzenie gdzie znajduje sie tlo
        //std::cout << "Pozycja y mapy: " << sBackground.getPosition().y << std::endl;

        dy+=0.2;
        y+=dy;

        //std::cout << "dy: " << dy << "      Player x: " << x << ", y: " << y << endl;

        if (y>600)  {
            //dy=-10;
            if (iloscZyc <= 0)
                state = GAMEOVER;
            else{
                iloscZyc -= 1;
                dy=-15;
            }
        }

        if (dy > -10 && dy < 0) sBackground.move(0,1); // plynne przechodzenie mapy

        if (y<h)
            for (int i=0;i<10;i++)
            {
                y=h;
                plat[i].y = plat[i].y-dy;
                if (plat[i].y>580)
                {
                    plat[i].y=0;
                    plat[i].x=rand()%400+200;
                }
            }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A)){
            x-=3;
            sPers.setTexture(t3);

            if (sPers.getPosition().x < (150)){
                //x = x + sPers.getScale().x + szerokosc;
                x = x + sPers.getScale().x + 400;
            }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D)){
            x+=3;
            sPers.setTexture(t4);

            if (sPers.getPosition().x > 580){
                //x = x + sPers.getScale().x + szerokosc;
                x = 200;
            }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)){
            dy+=1;

            for (int i=0;i<10;i++)
                if ((x+50>plat[i].x) && (x+20<plat[i].x+68)
                && (y+70>plat[i].y) && (y+70<plat[i].y+14) && (dy>0))  dy=-15;

            if (dy > 0 && dy < 1) sBackground.move(0,-5);

            //isSpacePressed = true;
        }
        else{
            for (int i=0;i<10;i++)
                if ((x+50>plat[i].x) && (x+20<plat[i].x+68)
                && (y+70>plat[i].y) && (y+70<plat[i].y+14) && (dy>0))   dy=-10;
        }

        sPers.setPosition(x,y);

        cursor.setPosition(static_cast<sf::Vector2f>(sf::Mouse::getPosition(window)));

        window.clear();

        window.draw(sBackground);

        for (int i=0;i<10;i++)
        {
            sPlat.setPosition(plat[i].x,plat[i].y);
            window.draw(sPlat);
        }window.draw(audienceleft);

        window.draw(sPers);

        window.draw(zycia[iloscZyc]);

        window.draw(audienceleft);
        window.draw(audienceright);

        window.setView(fixed);
        window.draw(cursor);

        window.display();
    }
}

void Game::gameOver()
{

    // TODO: Koncowy wynik (wyswietlic uzytkownikowi i dopisac do pliku txt)
    // state == GAMEOVER
    Text title(Title,font,40);
	title.setStyle(Text::Bold);

	title.setPosition(szerokosc/2-title.getGlobalBounds().width/2,20);
//////////////////////////////////////////////////////////////////////////////
    window.setMouseCursorVisible(false);

	sf::View fixed = window.getView();
	sf::Texture cursorTexture;
	if(!cursorTexture.loadFromFile("Resources/Textures/cursor.png"))// Custon Cursor
		ErrorMsg("Cursor not found! Check: 'Resources/Textures/cursor.png'","ERROR");

	sf::Sprite cursor(cursorTexture);
/////////////////////////////////////////////////////////////////////////////
	const int ile = 3;

	Text tekst[ile];

	string str[] = {"Kontynuuj","Wyniki","Zamknij"};
	for(int i=0;i<ile;i++)
	{
		tekst[i].setFont(font);
		tekst[i].setCharacterSize(40);              // Main Menu, texts and buttons

		tekst[i].setString(str[i]);
		tekst[i].setPosition(szerokosc/2-tekst[i].getGlobalBounds().width/2,400+i*50);
	}
///////////////////////////////////////////////////////////////////////////////////

    sf::Text gameover("<!Game Over!>",font,90);
    gameover.setPosition(szerokosc/2-gameover.getGlobalBounds().width/2,wysokosc/3-gameover.getGlobalBounds().height/2);

    sf::Text tryagain("Ale mozesz sprobowac jeszcze raz...",font,30);
    tryagain.setPosition(szerokosc/2-gameover.getGlobalBounds().width/3-10,tekst[0].getPosition().y-gameover.getGlobalBounds().height/2);

	while(state == GAMEOVER)
	{
		Vector2f mouse(Mouse::getPosition(window));
		Event event;

		while(window.pollEvent(event))
		{
			//Wciœniêcie ESC lub przycisk X
			if(event.type == Event::Closed)
                state = END;
            if(event.type == Event::KeyPressed && event.key.code == Keyboard::Escape)
				state = MENU;

			//klikniêcie EXIT
			else if(tekst[2].getGlobalBounds().contains(mouse) &&
				event.type == Event::MouseButtonReleased && event.key.code == Mouse::Left)
			{
				state = END;
			}
			else if(tekst[0].getGlobalBounds().contains(mouse) &&
				event.type == Event::MouseButtonReleased && event.key.code == Mouse::Left)
			{
				state = MENU;
			}
			if(event.type == Event::KeyPressed && event.key.code == Keyboard::Slash)
                console();
		}
		for(int i=0;i<ile;i++)
			if(tekst[i].getGlobalBounds().contains(mouse))
				tekst[i].setColor(sf::Color::Cyan); // when you will move your mouse on button
			else tekst[i].setColor(sf::Color::White);

        cursor.setPosition(static_cast<sf::Vector2f>(sf::Mouse::getPosition(window))); // for custom cursor

		window.clear();

		window.draw(title);
        window.draw(gameover);
        window.draw(tryagain);

		for(int i=0;i<ile;i++)
			window.draw(tekst[i]);

        window.setView(fixed);
        window.draw(cursor);
		window.display();
	}
}

void Game::help()
{
    cout << "=================================================" << endl;
    cout << "                  Komendy: " << endl << endl;

    cout << "menu - przejscie do menu gry" << endl;
    cout << "startgame - przejscie do glownej gry" << endl;
    cout << "end - zamyka aplikacje" << endl;
    cout << "exit - wyjscie z konsoli" << endl;
    cout << "gameover - przejscie do okna gameover" << endl;
    cout << "help - wyswietlenie wszystkich komend" << endl;
    cout << "cutscene1 - przejscie do cutsceny nr 1" << endl;
    cout << "cutscene2 - przejscie do cutsceny nr 2" << endl;
    cout << "cutscene3 - przejscie do cutsceny nr 3" << endl;
    cout << "cutscene4 - przejscie do cutsceny nr 4" << endl;
    cout << "options - przejscie do menu opcji" << endl;
    cout << "greetings - przejscie do Podziekowan" << endl;
    cout << "=================================================" << endl;
    // cout << "" << endl;
}

void Game::console()
{
    string command;
    int com2 = 0;

    komenda:
    cout << endl;
    cout << "Aktualnie: ";

    if (state == GAMESTART)
        cout << "Gra" << endl;
    else if (state == MENU)
        cout << "Menu" << endl;
    else if (state == GAMEOVER)
        cout << "GameOver" << endl;
    else if (state == CUT1)
        cout << "Cutscena nr 1" << endl;
    else if (state == CUT2)
        cout << "Cutscena nr 2" << endl;
    else if (state == MENUOPTIONS)
        cout << "Menu Opcji" << endl;
    else if (state == CUT3)
        cout << "Cutscena nr 3" << endl;
    else if (state == CUT4)
        cout << "Cutscena nr 4" << endl;
    else if (state == GREETINGS)
        cout << "Podziekowania" << endl;
    else
        cout << "Pozycja Nie Znana" << endl;

    cout << ">";
    cin >> command;

    if (command == "menu")
        state = MENU;
    else if (command == "startgame")
        state = GAMESTART;
    else if (command == "end")
        state = END;
    else if (command == "gameover")
        state = GAMEOVER;
    else if (command == "exit")
        return;
    else if (command == "help")
        help();
    else if (command == "cutscene1")
        state = CUT1;
    else if (command == "cutscene2")
        state = CUT2;
    else if (command == "options")
        state = MENUOPTIONS;
    else if (command == "cutscene3")
        state = CUT3;
    else if (command == "cutscene4")
        state = CUT4;
    else if (command == "greetings")
        state = GREETINGS;
    else{
        cout << "Zla komenda sprobuj jeszcze raz...";
        goto komenda;
    }
    goto komenda;
}

void Game::gameUpdate(bool a)  {
        if (a == true){
            if (this->trudnosc == 0)    this->iloscZyc = 4;
            else if (this->trudnosc == 1)   this->iloscZyc = 3;
            else if (this->trudnosc == 2)   this->iloscZyc = 2;
            else if (this->trudnosc == 3)   this->iloscZyc = 1;
            else    this->iloscZyc = 0;
        }
    }

void Game::optionsReset(){
    // defaultowe opcje gry
    this->trudnosc = 0;
    this->iloscZyc = 4;
    this->zycia = false;
}
