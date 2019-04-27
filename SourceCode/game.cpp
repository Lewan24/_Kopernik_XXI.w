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
#include <cstring>
#include <SFML/Audio.hpp>
#include <cstdio>

#include "game.h"

using namespace sf;
using namespace std;

short wysokosc = 600, szerokosc = 800;

sf::Vector2i screenDimensions(szerokosc,wysokosc);
sf::RenderWindow window(sf::VideoMode(screenDimensions.x, screenDimensions.y), L"Kopernik i Płaska Ziemia",Style::Close);

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

// TODO: Wyniki moga byc liczone od sekund przebytych w grze, od ilosci przeskokow pomiedzy prawa a lewa krawedzia
// gry, ilosc wysokich skokow np. po spacji, poziom trudnosci im ciezszy tym wiecej punktow sie dostaje
// szybkosc dojscia do konca gry, ilosc zebranych ciekawostek o koperniku
// TODO: Stworzyc przed ostatnia cutscenka tablice z koncowym wynikiem itd;

string intToStr(int n)
{
     string tmp, ret;
     if(n < 0) {
      ret = "-";
      n = -n;
     }
     do {
      tmp += n % 10 + 48;
      n -= n % 10;
     }
     while(n /= 10);
     for(int i = tmp.size()-1; i >= 0; i--)
      ret += tmp[i];
     return ret;
}

Game::Game(void)
{
	state = END;

	if(!font.loadFromFile("Resources/Fonts/main.otf")) {
        ErrorMsg("Hmm... Chyba brakuje czcionki! Sprawdz 'Resources/Fonts/main.otf'","ERROR");
        return;
    }
    cout << "Loading Background Textures..." << endl;

    for(int i = 0; i < this->howmany; i++){
        jakisstring[i] = std::string("Resources/Game/frames/frame")+intToStr(i+1)+".png";

        if (!bgTexture[i].loadFromFile(jakisstring[i]))
            ErrorMsg("Hmm... Brakuje textury! Sprawdz czy masz wszystkie w 'Resources/Game/frames/'","ERROR");

        cout << std::string("Frame ") + intToStr(i+1) +"/"+intToStr(howmany) + " Loaded" << endl;
    }
    cout << "Background Textures Loaded" << endl;

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

void Game::runGame(){
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

            /* TODO:  opis gry
            opis Kopernika i jego osiagniec // w dodatkach bedzie jakas dluzsza wersja troche o nim opowiadajaca
            */
            default: break;
		}
	}
}

void Game::greetings()
{
    // state = GREETINGS

    Text title(L"Kopernik i Płaska Ziemia",font,40);
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

    sf::Text skip(L"Space, aby pominąć",font,20);
    skip.setPosition(szerokosc-skip.getGlobalBounds().width-15,wysokosc-skip.getGlobalBounds().height-20);

    Texture t1;
    if (!t1.loadFromFile("Resources/Textures/greetings.png"))
        ErrorMsg("Hmm... Chyba brakuje textury! Sprawdz 'Resources/Textures/greetings.png'","ERROR");

    sf::Sprite Background(t1);

    Background.setPosition(200, 600);

    sf::Clock zegar;
    sf::Time czas;

    zegar.restart();


    ///////////////////
    // Muzyka


    sf::Music endp;

    if(!endp.openFromFile("Resources/Game/Music/EndCredits.ogg"))
        ErrorMsg("Hmm.. chyba brakuje dzwieku! Sprawdz: Resources/Game/Music/EndCredits.ogg", "ERROR");

    endp.setVolume(15.f);
    endp.setLoop(false);

    endp.play();

	while(state == GREETINGS)
	{
		Vector2f mouse(Mouse::getPosition(window));
		Event event;

		while(window.pollEvent(event))
		{
			//Wciœniêcie ESC lub przycisk X
			if(event.type == Event::Closed){
                endp.stop();
                state = END;
			}

			//klikniêcie SPACE
			else if(event.type == Event::KeyPressed && event.key.code == Keyboard::Space)
			{
			    endp.stop();
				state = MENU;
			}
			if(event.type == Event::KeyPressed && event.key.code == Keyboard::Slash)
                console();
		}

        //std::cout << Background.getPosition().y << std::endl;

        if (Background.getPosition().y < -1400){
            endp.stop();
            state = MENU;
        }

        cursor.setPosition(static_cast<sf::Vector2f>(sf::Mouse::getPosition(window))); // for custom cursor

        if (czas.asMilliseconds() > 10){
            Background.move(0,-3);
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
    this->backgroundY = -4080;
    this->przej = true;
    this->playmusic1 = true;
    this->zycia = true;
    this->platforma = true;
    for (int i = 0; i < this->ile_linii; i++)
        this->ciekawostki[i] = false;

    Text title(L"Kopernik i Płaska Ziemia",font,80);
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
    const int ile = 6;

    Text tekst[ile];

    sf::String str[] = {"Rozpocznij","Dodatki","Zamknij","Porady",L"Dźwięk: ", "Wł."};
    for(int i=0;i<ile;i++)
    {
        tekst[i].setFont(font);
        tekst[i].setCharacterSize(70);              // Main Menu, texts and buttons

        tekst[i].setString(str[i]);
        tekst[i].setPosition(szerokosc/2-tekst[i].getGlobalBounds().width/2,250+i*75);
    }
///////////////////////////////////////////////////////////////////////////////////
    tekst[3].setPosition(5,wysokosc-tekst[3].getGlobalBounds().height-30);
    tekst[4].setPosition(szerokosc-tekst[4].getGlobalBounds().width-120,wysokosc-tekst[4].getGlobalBounds().height-36);
    tekst[5].setPosition(szerokosc-tekst[5].getGlobalBounds().width,wysokosc-tekst[5].getGlobalBounds().height-35);
    tekst[5].setCharacterSize(60);

    while(state == MENU)
    {
        if (this->dzwiek == true)
            tekst[5].setString(L"Wł.");
        else tekst[5].setString(L"Wył.");

        Vector2f mouse(Mouse::getPosition(window));
        Event event;

        while(window.pollEvent(event))
        {
            //Wciœniêcie ESC lub przycisk X
            if(event.type == Event::Closed || (event.type == Event::KeyPressed &&
                event.key.code == Keyboard::Escape))
                state = END;

            //klikniêcie EXIT
            else if(tekst[2].getGlobalBounds().contains(mouse) &&
                event.type == Event::MouseButtonReleased && event.mouseButton.button == Mouse::Left)
            {
                state = END;
            }
            else if(tekst[1].getGlobalBounds().contains(mouse) &&
                event.type == Event::MouseButtonReleased && event.mouseButton.button == Mouse::Left)
            {
                state = MENUOPTIONS;
            }
            else if(tekst[0].getGlobalBounds().contains(mouse) &&
                event.type == Event::MouseButtonReleased && event.mouseButton.button == Mouse::Left)
            {
                state = CUT1;
            }
            else if(tekst[3].getGlobalBounds().contains(mouse) &&
                event.type == Event::MouseButtonReleased && event.mouseButton.button == Mouse::Left)
            {
                state = HINTS;
            }
            else if(tekst[4].getGlobalBounds().contains(mouse) &&
                event.type == Event::MouseButtonReleased && event.mouseButton.button == Mouse::Left)
            {
                if (this->dzwiek == true)
                    this->dzwiek = false;
                else this->dzwiek = true;
            }
            if(event.type == Event::KeyPressed && event.key.code == Keyboard::Slash)
                console();
        }
        for(int i=0;i<ile-1;i++)
            if(tekst[i].getGlobalBounds().contains(mouse))
                tekst[i].setFillColor(Color::Cyan); // when you will move your mouse on button
            else tekst[i].setFillColor(Color::White);

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
    // state = HINTS

	sf::Text title(L"Kopernik i Płaska Ziemia",font,30);
	title.setStyle(Text::Bold);
    title.setPosition(5,565);
//////////////////////////////////////////////////////////////////////////////
    window.setMouseCursorVisible(false);

	sf::View fixed = window.getView();
	sf::Texture cursorTexture;
	if(!cursorTexture.loadFromFile("Resources/Textures/cursor.png"))// Custon Cursor
		ErrorMsg("Cursor not found! Check: 'Resources/Textures/cursor.png'","ERROR");

	sf::Sprite cursor(cursorTexture);
//////////////////////////////////////////////////////////////////////////////
    const int menu = 2;

	sf::Text wartoscimenu[menu];

	string strmenu[] = {"Kontynuuj",""};
	for(int i=0;i<menu;i++)
	{
		wartoscimenu[i].setFont(font);
		wartoscimenu[i].setCharacterSize(40);

		wartoscimenu[i].setString(strmenu[i]);
		wartoscimenu[i].setPosition(szerokosc-200,450+i*40);
		wartoscimenu[i].setStyle(Text::Bold);
	}

    string linia;
    int ile=0;
    short ilosc_linii = 14; // ilosc linii - 1
    sf::String str[ilosc_linii+1];

    fstream plik;
    plik.open("Resources/Game/hints.data", ios::in);

    if(plik.good()==false)
        ErrorMsg("File not found, Check: 'Resources/Game/cutscenes/hints.hint'","ERROR");

    std::basic_string < sf::Uint32 > tmp[ilosc_linii+1];
    //cout << "Jestem tutaj!" << endl;
    while (getline(plik, linia))
    {
        //str[ile] = linia;
        sf::Utf8::toUtf32( linia.begin(), linia.end(), std::back_inserter( tmp[ile] ) );
        str[ile] = tmp[ile];
        ile++;
        //cout << "Jestem tutaj!" << endl;
    }
    //cout << "Jestem tutaj!" << endl;
    plik.close();

	sf::Text tekst[ile];

	for(int i=0;i<ile;i++)
	{
		tekst[i].setFont(font);
		tekst[i].setCharacterSize(36);              // Tekst cutscenki z pliku txt

		tekst[i].setString(str[i]);
		tekst[i].setPosition(5,i*35);
		tekst[i].setStyle(Text::Bold);
	}

	sf::Clock zegar;
    sf::Time czas;

    zegar.restart();

    int cos = 0;

    //sf::Text skip(Skip,font,20);
    //skip.setPosition(szerokosc-skip.getGlobalBounds().width-3,600);

	while(state == HINTS)
	{
		Vector2f mouse(Mouse::getPosition(window));
		Event event;

		while(window.pollEvent(event))
		{
			//Wciœniêcie ESC lub przycisk X
			if(event.type == Event::Closed)
                state = END;
			if(wartoscimenu[0].getGlobalBounds().contains(mouse) &&
				event.type == Event::MouseButtonReleased && event.mouseButton.button == Mouse::Left)
			{
				state = MENU;
			}
            if(event.type == Event::KeyPressed && event.key.code == Keyboard::Slash)
                console();
		}
        for(int i=0;i<menu;i++)
			if(wartoscimenu[i].getGlobalBounds().contains(mouse))
				wartoscimenu[i].setFillColor(Color::Cyan); // when you will move your mouse on button
			else wartoscimenu[i].setFillColor(Color::White);

        cursor.setPosition(static_cast<sf::Vector2f>(sf::Mouse::getPosition(window))); // for custom cursor

        //std::cout << cos << endl;

        if (czas.asMilliseconds() > 50){
            cos++;
            if (cos > ilosc_linii)
                cos = cos-1;

            zegar.restart();
        }
        czas=zegar.getElapsedTime();

		window.clear();

		window.draw(title);
		//window.draw(skip);

        for(int i=0;i<menu;i++)
			window.draw(wartoscimenu[i]);

        for (int i = 0; i < cos+1; i++)
            window.draw(tekst[i]);

        window.setView(fixed);
        window.draw(cursor);
		window.display();
	}
}


void Game::options()
{
    // state = MENUOPTIONS

    Text title(L"Kopernik i Płaska Ziemia",font,40);
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

	sf::String str[] = {L"Poziom Trudności: ",L"Podziękowania","<--", "Zresetuj Opcje"};

	for(int i=0;i<ile;i++)
	{
		tekst[i].setFont(font);
		tekst[i].setCharacterSize(50);              // Main Menu, texts and buttons

		tekst[i].setString(str[i]);
		tekst[i].setPosition(szerokosc/4-tekst[i].getGlobalBounds().width/2,250+i*60);
		tekst[i].setStyle(Text::Bold);
	}
///////////////////////////////////////////////////////////////////////////////////
    const short trudnosci = 5;
    sf::Text poziomy[trudnosci];

    string strudnosc[trudnosci] = {"Easy", "Normal", "Hard", "Serious", "Mental"};
    sf::String sopisTrudnosci[trudnosci] = {L"Poziom dla Turystów",L"Przeciętna trudność",L"Tylko dla Weteranów",
                                        L"Jesteś poważny?",L"Nie mam pytań"};

    sf::Text opisTrudnosci[trudnosci];

    for (int i = 0; i < trudnosci; i++){
        poziomy[i].setFont(font);
        poziomy[i].setPosition(szerokosc/2-poziomy[i].getGlobalBounds().width/2,250);
        poziomy[i].setCharacterSize(50);
        poziomy[i].setString(strudnosc[i]);

        opisTrudnosci[i].setFont(font);
        opisTrudnosci[i].setPosition(szerokosc/2-opisTrudnosci[i].getGlobalBounds().width/2+30,poziomy[i].getPosition().y-50);
        opisTrudnosci[i].setCharacterSize(40);
        opisTrudnosci[i].setString(sopisTrudnosci[i]);

        poziomy[i].setStyle(Text::Bold);
        opisTrudnosci[i].setStyle(Text::Bold);
    }

    tekst[3].setPosition(szerokosc-tekst[3].getGlobalBounds().width-5,wysokosc-tekst[3].getGlobalBounds().height-30);

    string zyciastr[6] = {"0","1","2","3","4","5"};

    sf::Text ilezyc;
    ilezyc.setFont(font);
    ilezyc.setCharacterSize(45);
    ilezyc.setPosition(50, 150);
    ilezyc.setStyle(Text::Bold);

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
				event.type == Event::MouseButtonReleased && event.mouseButton.button == Mouse::Left)
			{
				state = MENU;
			}
			else if(tekst[1].getGlobalBounds().contains(mouse) &&
				event.type == Event::MouseButtonReleased && event.mouseButton.button == Mouse::Left)
			{
				state = GREETINGS;
			}
			else if(tekst[0].getGlobalBounds().contains(mouse) &&
				event.type == Event::MouseButtonReleased && event.mouseButton.button == Mouse::Left)
			{
				this->trudnosc++;
			}
			else if(tekst[3].getGlobalBounds().contains(mouse) &&
				event.type == Event::MouseButtonReleased && event.mouseButton.button == Mouse::Left)
			{
				optionsReset();
			}
			if(event.type == Event::KeyPressed && event.key.code == Keyboard::Slash)
                console();
		}
		for(int i=0;i<ile;i++)
			if(tekst[i].getGlobalBounds().contains(mouse))
				tekst[i].setFillColor(Color::Cyan); // when you will move your mouse on button
			else tekst[i].setFillColor(Color::White);

        if (this->trudnosc >= trudnosci)
            this->trudnosc = 0;

        ilezyc.setString(L"Ilość żyć na start: " + zyciastr[iloscZyc]);

        cursor.setPosition(static_cast<sf::Vector2f>(sf::Mouse::getPosition(window))); // for custom cursor

		window.clear();

		window.draw(title);

		for(int i=0;i<ile;i++)
			window.draw(tekst[i]);

        window.draw(poziomy[trudnosc]);
        window.draw(opisTrudnosci[trudnosc]);
        window.draw(ilezyc);

        window.setView(fixed);
        window.draw(cursor);
		window.display();
	}
}

void Game::cut1(){
    //state = CUT1

    sf::Text title(L"Kopernik i Płaska Ziemia",font,20);
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
    sf::String str[ilosc_linii+1];

    fstream plik;
    plik.open("Resources/Game/cutscenes/cut1.cutscene", ios::in);

    if(plik.good()==false)
        ErrorMsg("File not found, Check: 'Resources/Game/cutscenes/cut1.cutscene'","ERROR");

    std::basic_string < sf::Uint32 > tmp[ilosc_linii+1];
    while (getline(plik, linia))
    {
        //str[ile] = linia;
        sf::Utf8::toUtf32( linia.begin(), linia.end(), std::back_inserter( tmp[ile] ) );
        str[ile] = tmp[ile];
        ile++;
    }
    plik.close();

	sf::Text tekst[ile];

	for(int i=0;i<ile;i++)
	{
		tekst[i].setFont(font);
		tekst[i].setCharacterSize(38);              // Main Menu, texts and buttons

		tekst[i].setString(str[i]);
		tekst[i].setPosition(szerokosc/2-tekst[i].getGlobalBounds().width/2,i*35);
		tekst[i].setStyle(Text::Bold);
	}

	sf::Clock zegar;
    sf::Time czas;

    zegar.restart();

    int cos = 0;

    sf::Text skip(L"Space, aby pominąć",font,20);
    skip.setPosition(szerokosc-skip.getGlobalBounds().width-3,550);

    //////////////////////////////////
    // Mamrotanie :D


    sf::Music mumbling1, mumbling2;

    mumbling1.openFromFile("Resources/Game/Music/mumbling.ogg");
    mumbling2.openFromFile("Resources/Game/Music/mumbling2.wav");

    mumbling1.setLoop(false);
    mumbling2.setLoop(false);


    int ktorymumbling = rand()%2;

    if(this->dzwiek == true){
        mumbling1.setVolume(50.f);
        mumbling2.setVolume(50.f);

        if (ktorymumbling == 0)
            mumbling1.play();
        else mumbling2.play();
    }

	while(state == CUT1)
	{
		Vector2f mouse(Mouse::getPosition(window));
		Event event;

		while(window.pollEvent(event))
		{
			//Wciœniêcie ESC lub przycisk X
			if(event.type == Event::Closed){
                mumbling1.stop();
                mumbling2.stop();
                state = END;
			}

            if(event.type == Event::KeyPressed && event.key.code == Keyboard::Space){
                mumbling1.stop();
                mumbling2.stop();
                state = GAMESTART;
            }

            if(event.type == Event::KeyPressed && event.key.code == Keyboard::Escape){
                mumbling1.stop();
                mumbling2.stop();
                state = MENU;
            }

            if(event.type == Event::KeyPressed && event.key.code == Keyboard::Slash)
                console();
		}

        cursor.setPosition(static_cast<sf::Vector2f>(sf::Mouse::getPosition(window))); // for custom cursor

        //std::cout << cos << endl;

        if (czas.asSeconds() > 0.25){
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

    sf::Text title(L"Kopernik i Płaska Ziemia",font,20);
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
    short ilosc_linii = 15; // ilosc linii - 1
    sf::String str[ilosc_linii+1];

    fstream plik;
    plik.open("Resources/Game/cutscenes/cut4.cutscene", ios::in);

    if(plik.good()==false)
        ErrorMsg("File not found, Check: 'Resources/Game/cutscenes/cut4.cutscene'","ERROR");

    std::basic_string < sf::Uint32 > tmp[ilosc_linii+1];
    while (getline(plik, linia))
    {
        //str[ile] = linia;
        sf::Utf8::toUtf32( linia.begin(), linia.end(), std::back_inserter( tmp[ile] ) );
        str[ile] = tmp[ile];
        ile++;
    }
    plik.close();

	sf::Text tekst[ile];

	for(int i=0;i<ile;i++)
	{
		tekst[i].setFont(font);
		tekst[i].setCharacterSize(38);              // Tekst cutscenki z pliku txt

		tekst[i].setString(str[i]);
		tekst[i].setPosition(szerokosc/2-tekst[i].getGlobalBounds().width/2,i*35);
		tekst[i].setStyle(Text::Bold);
	}

	sf::Clock zegar;
    sf::Time czas;

    zegar.restart();

    int cos = 0;

    sf::Text skip(L"Space, aby pominąć",font,20);
    skip.setPosition(szerokosc-skip.getGlobalBounds().width-3,550);

    //////////////////////////////////
    // Mamrotanie :D


    sf::Music mumbling1, mumbling2;

    mumbling1.openFromFile("Resources/Game/Music/mumbling.ogg");
    mumbling2.openFromFile("Resources/Game/Music/mumbling2.wav");

    mumbling1.setLoop(false);
    mumbling2.setLoop(false);


    int ktorymumbling = rand()%2;

    if(this->dzwiek == true){
        mumbling1.setVolume(50.f);
        mumbling2.setVolume(50.f);

        if (ktorymumbling == 0)
            mumbling1.play();
        else mumbling2.play();
    }

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

        if (czas.asSeconds() > 0.25){
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

    sf::Text title(L"Kopernik i Płaska Ziemia",font,20);
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
    short ilosc_linii = 11; // ilosc linii - 1
    sf::String str[ilosc_linii+1];

    fstream plik;
    plik.open("Resources/Game/cutscenes/cut3.cutscene", ios::in);

    if(plik.good()==false)
        ErrorMsg("File not found, Check: 'Resources/Game/cutscenes/cut3.cutscene'","ERROR");

    std::basic_string < sf::Uint32 > tmp[ilosc_linii+1];
    while (getline(plik, linia))
    {
        //str[ile] = linia;
        sf::Utf8::toUtf32( linia.begin(), linia.end(), std::back_inserter( tmp[ile] ) );
        str[ile] = tmp[ile];
        ile++;
    }
    plik.close();

	sf::Text tekst[ile];

	for(int i=0;i<ile;i++)
	{
		tekst[i].setFont(font);
		tekst[i].setCharacterSize(38);              // Tekst cutscenki z pliku txt

		tekst[i].setString(str[i]);
		tekst[i].setPosition(szerokosc/2-tekst[i].getGlobalBounds().width/2,i*35);
		tekst[i].setStyle(Text::Bold);
	}

	sf::Clock zegar;
    sf::Time czas;

    zegar.restart();

    int cos = 0;

    sf::Text skip(L"Space, aby pominąć",font,20);
    skip.setPosition(szerokosc-skip.getGlobalBounds().width-3,550);

    //////////////////////////////////
    // Mamrotanie :D


    sf::Music mumbling1, mumbling2;

    mumbling1.openFromFile("Resources/Game/Music/mumbling.ogg");
    mumbling2.openFromFile("Resources/Game/Music/mumbling2.wav");

    mumbling1.setLoop(false);
    mumbling2.setLoop(false);


    int ktorymumbling = rand()%2;

    if(this->dzwiek == true){
        mumbling1.setVolume(50.f);
        mumbling2.setVolume(50.f);

        if (ktorymumbling == 0)
            mumbling1.play();
        else mumbling2.play();
    }

	while(state == CUT3)
	{
		Vector2f mouse(Mouse::getPosition(window));
		Event event;

		while(window.pollEvent(event))
		{
			//Wciœniêcie ESC lub przycisk X
			if(event.type == Event::Closed){
                mumbling1.stop();
                mumbling2.stop();
                state = END;
			}

            if(event.type == Event::KeyPressed && event.key.code == Keyboard::Space){
                mumbling1.stop();
                mumbling2.stop();
                state = CUT4;
            }
            if(event.type == Event::KeyPressed && event.key.code == Keyboard::Slash)
                console();
		}

        cursor.setPosition(static_cast<sf::Vector2f>(sf::Mouse::getPosition(window))); // for custom cursor

        //std::cout << cos << endl;

        if (czas.asSeconds() > 0.3){
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

    this->platforma = true;

    sf::Text title(L"Kopernik i Płaska Ziemia",font,20);
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
    short ilosc_linii = 9; // ilosc linii - 1
    sf::String str[ilosc_linii+1];

    fstream plik;
    plik.open("Resources/Game/cutscenes/cut2.cutscene", ios::in);

    if(plik.good()==false)
        ErrorMsg("File not found, Check: 'Resources/Game/cutscenes/cut2.cutscene'","ERROR");

    std::basic_string < sf::Uint32 > tmp[ilosc_linii+1];
    while (getline(plik, linia))
    {
        //str[ile] = linia;
        sf::Utf8::toUtf32( linia.begin(), linia.end(), std::back_inserter( tmp[ile] ) );
        str[ile] = tmp[ile];
        ile++;
    }
    plik.close();

	sf::Text tekst[ile];

	for(int i=0;i<ile;i++)
	{
		tekst[i].setFont(font);
		tekst[i].setCharacterSize(38);              // Tekst cutscenki z pliku txt

		tekst[i].setString(str[i]);
		tekst[i].setPosition(szerokosc/2-tekst[i].getGlobalBounds().width/2,i*35);
		tekst[i].setStyle(Text::Bold);
	}

	sf::Clock zegar;
    sf::Time czas;

    zegar.restart();

    int cos = 0;

    sf::Text skip(L"Space, aby pominąć",font,20);
    skip.setPosition(szerokosc-skip.getGlobalBounds().width-3,550);

    //////////////////////////////////
    // Mamrotanie :D


    sf::Music mumbling1, mumbling2;

    mumbling1.openFromFile("Resources/Game/Music/mumbling.ogg");
    mumbling2.openFromFile("Resources/Game/Music/mumbling2.wav");

    mumbling1.setLoop(false);
    mumbling2.setLoop(false);


    int ktorymumbling = rand()%2;

    if(this->dzwiek == true){
        mumbling1.setVolume(50.f);
        mumbling2.setVolume(50.f);

        if (ktorymumbling == 0)
            mumbling1.play();
        else mumbling2.play();
    }

	while(state == CUT2)
	{
		Vector2f mouse(Mouse::getPosition(window));
		Event event;

		while(window.pollEvent(event))
		{
			//Wciœniêcie ESC lub przycisk X
			if(event.type == Event::Closed){
                mumbling1.stop();
                mumbling2.stop();
                state = END;
			}

            if(event.type == Event::KeyPressed && event.key.code == Keyboard::Space){
                this->iloscZyc += 1;
                this->backgroundY = -2150;
                this->zycia = false;
                mumbling1.stop();
                mumbling2.stop();
                state = GAMESTART;
            }
            if(event.type == Event::KeyPressed && event.key.code == Keyboard::Slash)
                console();
		}

        cursor.setPosition(static_cast<sf::Vector2f>(sf::Mouse::getPosition(window))); // for custom cursor

        //std::cout << cos << endl;

        if (czas.asSeconds() > 0.3){
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
    //TODO: Lektor do ciekawostek
    //TODO: Nowe Ciekawostki dodac

    cout << "Updating Game..." << endl;

    gameUpdate(zycia);

    cout << "Game Updated" << endl;

    cout << "Loading Cursor Theme..." << endl;

    window.setMouseCursorVisible(false);

	sf::View fixed = window.getView();
	sf::Texture cursorTexture;
	if(!cursorTexture.loadFromFile("Resources/Textures/cursor.png"))
        ErrorMsg("Cursor not found! Check: 'Resources/Textures/cursor.png'","ERROR");

	sf::Sprite cursor(cursorTexture);
    cout << "Cursor Loaded" << endl;
    srand(time(0));

    cout << "Loading Basic Textures..." << endl;

    Texture platform1,platform2,platform3,t3,t4,doodle3,doodle4;
    /*Texture t1;
    if (!t1.loadFromFile("Resources/Textures/background.png"))
        ErrorMsg("Hmm... Chyba brakuje textury! Sprawdz 'Resources/Textures/background.png'","ERROR");*/
    if (!platform1.loadFromFile("Resources/Textures/platform1.png"))
        ErrorMsg("Hmm... Chyba brakuje textury! Sprawdz 'Resources/Textures/platform1.png'","ERROR");
    if (!platform2.loadFromFile("Resources/Textures/platform2.png"))
        ErrorMsg("Hmm... Chyba brakuje textury! Sprawdz 'Resources/Textures/platform2.png'","ERROR");
    if (!platform3.loadFromFile("Resources/Textures/platform3.png"))
        ErrorMsg("Hmm... Chyba brakuje textury! Sprawdz 'Resources/Textures/platform3.png'","ERROR");
    if (!t3.loadFromFile("Resources/Textures/doodle.png"))
        ErrorMsg("Hmm... Chyba brakuje textury! Sprawdz 'Resources/Textures/doodle.png'","ERROR");
    if (!t4.loadFromFile("Resources/Textures/doodle2.png"))
        ErrorMsg("Hmm... Chyba brakuje textury! Sprawdz 'Resources/Textures/doodle2.png'","ERROR");
    if (!doodle3.loadFromFile("Resources/Textures/doodle3.png"))
        ErrorMsg("Hmm... Chyba brakuje textury! Sprawdz 'Resources/Textures/doodle3.png'","ERROR");
    if (!doodle4.loadFromFile("Resources/Textures/doodle4.png"))
        ErrorMsg("Hmm... Chyba brakuje textury! Sprawdz 'Resources/Textures/doodle4.png'","ERROR");

    cout << "Basic Textures Loaded" << endl;

    int ktoratextura = 0;
    sf::Sprite sBackground(bgTexture[0]);

    //sf::Sprite sBackground(t1);

    sf::Sprite sPlat(platform1);
    sf::Sprite sPers(t3);

    sBackground.setPosition(200, backgroundY);

    cout << "Loading Audience Textures..." << endl;

    sf::Texture audience1, audience2;

    if (!audience1.loadFromFile("Resources/Textures/audience.png"))
        ErrorMsg("Hmm... Chyba brakuje textury! Sprawdz 'Resources/Textures/audience.png'","ERROR");
    if (!audience2.loadFromFile("Resources/Textures/audience2.png"))
        ErrorMsg("Hmm... Chyba brakuje textury! Sprawdz 'Resources/Textures/audience2.png'","ERROR");

    cout << "Audience Textures Loaded" << endl;

    sf::Sprite audienceleft, audienceright;

    audienceleft.setTexture(audience1);
    audienceright.setTexture(audience2);
    audienceleft.setPosition(0,0);
    audienceright.setPosition(600,0);

    point plat[20];

    for (int i=0;i<10;i++)
    {
       plat[i].x=rand()%340+200;
       plat[i].y=rand()%590;
    }

    int x=szerokosc/2, y=10, h=wysokosc/2;
    float dy=0;

    //bool isSpacePressed;

    const short ile = iloscZyc+1;
    //sf::Text zycia0("Lives: 0",font,30), zycia1("Lives: 1",font,30), zycia2("Lives: 2",font,30), zycia3("Lives: 3",font,30);
    sf::Text zycia[ile];

    cout << "Loading Lives..." << endl;

    const short ile2 = 11; // ustalam wieklosc tablicy; zawsze ma byc o 1 wieksza od max liczby w tablicy
    string zyciastr[ile2] = {"0","1","2","3","4","5","6","7","8","9","10"}; // max 10 zyc
    //string str[] = {"Lives: 0", "Lives: 1", "Lives: 2", "Lives: 3"};
    string str[ile2];

    for(int i = 0; i < ile2; i++){ // dla kazdego stringa przypisuje odpowiednia wartosc
        str[i] = "Zycia: " + zyciastr[i];
    }

    for (int i = 0; i < ile; i++){ // dla kazdego stringa ustawiam kolor, wielkosc itd
        zycia[i].setFont(font);
        zycia[i].setPosition(210,0);
        zycia[i].setCharacterSize(25);
        zycia[i].setString(str[i]);
        zycia[i].setFillColor(sf::Color::Red);
        zycia[i].setStyle(Text::Bold);
    }

    cout << "Lives Loaded" << endl;
    cout << "Loading Gift Textures..." << endl;

    sf::Texture giftTexture;
    if(!giftTexture.loadFromFile("Resources/Textures/gift.png"))
        ErrorMsg("hmm... Brakuje Textury! Sprawdz: Resources/Textures/gift.png","ERROR");

    cout << "Gift Texture Loaded" << endl;

    sf::Sprite gift;
    gift.setTexture(giftTexture);
    //gift.setPosition(300,0);

    bool isGift = false;

    cout << "Loading Clock/Time..." << endl;

    sf::Clock zegar;
    sf::Time czas;

    zegar.restart();

    cout << "Main Clock/Time Loaded" << endl;

    string liniapliku;
    int ktoralinia=0;
    //short ile_linii = 14; // ilosc linii - 1
    sf::String napisy[ile_linii+1];

    cout << "Opening File" << endl;

    fstream plik2;
    plik2.open("Resources/Game/ciekawostki.data", ios::in);

    cout << "File Opened" << endl;

    if(plik2.good()==false)
        ErrorMsg("File not found, Check: 'Resources/Game/ciekawostki.data'","ERROR");

    cout << "File Loaded" << endl;

    std::basic_string < sf::Uint32 > tmp[ile_linii+1];

    cout << "Loading Texts..." << endl;

    while (getline(plik2, liniapliku))
    {
        //str[ile] = linia;
        sf::Utf8::toUtf32( liniapliku.begin(), liniapliku.end(), std::back_inserter( tmp[ktoralinia] ) );
        napisy[ktoralinia] = tmp[ktoralinia];
        ktoralinia++;
    }
    cout << "Texts Loaded" << endl;

    plik2.close();

    cout << "File Closed" << endl;
    cout << "Loading Curiosities..." << endl;

    //cout << "Tu jestem!" << endl;
	sf::Text ciekawostka[ktoralinia];

	for(int i=0;i<ktoralinia;i++)
	{
		ciekawostka[i].setFont(font);
		ciekawostka[i].setCharacterSize(34);              // Tekst cutscenki z pliku txt

		ciekawostka[i].setString(napisy[i]);
		ciekawostka[i].setPosition(szerokosc/2-ciekawostka[i].getGlobalBounds().width/2,200);
		ciekawostka[i].setFillColor(Color::White);
		ciekawostka[i].setStyle(Text::Bold);
	}

    cout << "Curiosities Loaded" << endl;

    sf::RectangleShape tloNapisow;
    tloNapisow.setFillColor(Color::Black);
    tloNapisow.setPosition(0,ciekawostka[1].getPosition().y+4);
    tloNapisow.setSize(sf::Vector2f(800, 43));

    int whichGift = rand()%ile_linii;

    int giftX = rand()%350+200;
    float giftY = 0;
    bool drawciek = false;
    bool isGift2 = false;

    ///////////////////////////////
    // Muzyka
    cout << "Loading Music..." << endl;
    cout << "Setting Volume..." << endl;

    float volume = 30.f;

    cout << "Volume set" << endl;

    sf::Music soundtrack, soundtrack2;
    if(!soundtrack.openFromFile("Resources/Game/Music/GameSoundTrack2.ogg"))
        ErrorMsg("hmm... Chyba brakuje dzwiekow, Sprawdz: Resources/Game/Music/GameSoundTrack2.ogg","ERROR");
    if(!soundtrack2.openFromFile("Resources/Game/Music/GameSoundTrack3.ogg"))
        ErrorMsg("hmm... Chyba brakuje dzwiekow, Sprawdz: Resources/Game/Music/GameSoundTrack3.ogg","ERROR");

    cout << "Music Loaded" << endl;

    soundtrack.setLoop(false);
    soundtrack2.setLoop(false);

    soundtrack.setVolume(volume/2);
    soundtrack2.setVolume(volume/2);

    if (playmusic1 == true)
        soundtrack.play();
    else    soundtrack2.play();


    ////////////////////////
    // Dzwieki

    cout << "Loading sounds..." << endl;

    sf::SoundBuffer sfall1, sfall2, sfall3;
    sf::SoundBuffer shop;
    sf::SoundBuffer sprzejscie;
    if(!sfall1.loadFromFile("Resources/Game/Music/fall.wav"))
        ErrorMsg("hmm... Chyba brakuje dzwieku! Sprawdz: Resources/Game/Music/fall.wav", "ERROR");
    if(!sfall2.loadFromFile("Resources/Game/Music/fall2.wav"))
       ErrorMsg("hmm... Chyba brakuje dzwieku! Sprawdz: Resources/Game/Music/fall2.wav", "ERROR");
    if(!sfall3.loadFromFile("Resources/Game/Music/fall3.wav"))
       ErrorMsg("hmm... Chyba brakuje dzwieku! Sprawdz: Resources/Game/Music/fall3.wav", "ERROR");
    if(!shop.loadFromFile("Resources/Game/Music/hop.wav"))
       ErrorMsg("hmm... Chyba brakuje dzwieku! Sprawdz: Resources/Game/Music/hop.wav", "ERROR");
    if(!sprzejscie.loadFromFile("Resources/Game/Music/przejscie.wav"))
       ErrorMsg("hmm... Chyba brakuje dzwieku! Sprawdz: Resources/Game/Music/przejscie.wav", "ERROR");

    cout << "Sounds Loaded" << endl;

    sf::Sound fall1, fall2, fall3;
    sf::Sound przejscie;
    sf::Sound hop; // -10 - -9.8
    fall1.setBuffer(sfall1);
    fall2.setBuffer(sfall2);
    fall3.setBuffer(sfall3);

    hop.setBuffer(shop);
    przejscie.setBuffer(sprzejscie);

    fall1.setLoop(false);
    fall2.setLoop(false);
    fall3.setLoop(false);

    hop.setLoop(false);
    przejscie.setLoop(false);

    fall1.setVolume(volume);
    fall2.setVolume(volume);
    fall3.setVolume(volume);

    hop.setVolume(volume-10.f);
    przejscie.setVolume(volume);

    int ktoryfall = 0;

    //////////////////
    // Wielka platforma na poczatku sceny zeby nie spasc na glupi ryj

    sf::RectangleShape FirstPlatform;
    if (this->backgroundY >= -2150)
        FirstPlatform.setFillColor(Color::Blue);
    else    FirstPlatform.setFillColor(Color(87,65,47)); // Kolor brazowy

    FirstPlatform.setPosition(100,500);
    FirstPlatform.setSize(sf::Vector2f(600,10));

    //sf::Time BackGroundTime = clock.getElapsedTime();

    /////////////////////////////
    // Do ciekawostek

    //bool ciekawostki[ile_linii] = {false};

    while(state == GAMESTART)
    {
        if (ktoratextura >= howmany) // ilosc obrazkow
            ktoratextura = 0; //zerowanie // powrot do pierwszej klatki

        //sBackground.setTexture(bgTexture[ktoratextura]);

        if(this->dzwiek == true)
            volume = 30.f;
        else    volume = 0.f;

        fall1.setVolume(volume);
        fall2.setVolume(volume);
        fall3.setVolume(volume);

        if(volume <= 0.f && volume < 10)   hop.setVolume(volume);
        else if (volume >= 10.f)  hop.setVolume(volume-10.f);

        przejscie.setVolume(volume);
        soundtrack.setVolume(volume/2);
        soundtrack2.setVolume(volume/2);

        if (this->platforma == true && (FirstPlatform.getPosition().y <= (sPers.getPosition().y+sPers.getGlobalBounds().height))){
            this->platforma = false;
            dy = -15;
        }


        //cout << "dy: " << dy << endl;
        gift.setPosition(giftX,giftY);

        //cout << whichGift << endl;
        //isSpacePressed = false;

        Vector2f mouse(Mouse::getPosition());

        Event event;

        while(window.pollEvent(event))
        {
            //Wciœniêcie ESC lub przycisk X
            if(event.type == Event::Closed){
                soundtrack.stop();
                soundtrack2.stop();
                state = END;
            }
            if(event.type == Event::KeyPressed && event.key.code == Keyboard::Escape){
                soundtrack.stop();
                soundtrack2.stop();
                state = MENU;
            }
            if(event.type == Event::KeyPressed && event.key.code == Keyboard::Slash){
                console();
                sBackground.setPosition(200,this->backgroundY);
            }
            if(event.type == Event::KeyPressed && event.key.code == Keyboard::M){
                if (this->dzwiek == true)
                    this->dzwiek = false;
                else this->dzwiek = true;
            }
        }

        if (sBackground.getPosition().y < -2200 && sBackground.getPosition().y > -2205){
            soundtrack.stop();
            this->playmusic1 = false;
            state = CUT2;
        }
        else if (sBackground.getPosition().y > 200){
            soundtrack2.stop();
            state = CUT3;
        }

        if (sBackground.getPosition().y > -2800 && sBackground.getPosition().y < -851)
            sPlat.setTexture(platform2);
        if (sBackground.getPosition().y > -850)
            sPlat.setTexture(platform3);

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

                ktoryfall = rand()%3;

                if(this->dzwiek == true){
                    if (ktoryfall == 0)
                        fall1.play();
                    else if (ktoryfall == 1)
                        fall2.play();
                    else    fall3.play();
                }
            }
        }

        if (dy > -10 && dy < 0) sBackground.move(0,1); // plynne przechodzenie mapy
        //if (dy > -10 && dy < 0) gift.move(0,2);

        if (y<h)
            for (int i=0;i<10;i++)
            {
                y=h;
                plat[i].y = plat[i].y-dy;

                giftY = giftY+0.7;

                if (plat[i].y>580)
                {
                    plat[i].y=0;
                    plat[i].x=rand()%400+200;
                }
                if(isGift == true && gift.getPosition().y > 580){
                    giftY = 0;
                }
            }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A)){
            x-=5;

            if (sBackground.getPosition().y > -850)
                sPers.setTexture(doodle3);
            else sPers.setTexture(t3);

            if (sPers.getPosition().x < (150)){
                //x = x + sPers.getScale().x + szerokosc;
                x = x + sPers.getScale().x + 400;

                if(this->przej == true){
                    if(this->dzwiek == true)
                        przejscie.play();

                    this->przej = false;
                }
            }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D)){
            x+=5;

            if (sBackground.getPosition().y > -850)
                sPers.setTexture(doodle4);
            else sPers.setTexture(t4);

            if (sPers.getPosition().x > 580){
                //x = x + sPers.getScale().x + szerokosc;
                x = 200;

                if(this->przej == true){
                    if(this->dzwiek == true)
                        przejscie.play();

                    this->przej = false;
                }

            }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)){
            dy+=1;
// -10 - -9.8
            for (int i=0;i<10;i++)
                if ((x+50>plat[i].x) && (x+20<plat[i].x+68)
                && (y+70>plat[i].y) && (y+70<plat[i].y+14) && (dy>0)){
                    dy=-15;
                    if(this->dzwiek == true)    hop.play();
                }

            if (dy > 0 && dy < 1) sBackground.move(0,-5);

            //isSpacePressed = true;
        }
        else{
            for (int i=0;i<10;i++)
                if ((x+50>plat[i].x) && (x+20<plat[i].x+68)
                && (y+70>plat[i].y) && (y+70<plat[i].y+14) && (dy>0)){
                    dy=-10;
                    if(this->dzwiek == true)    hop.play();
                }
        }

        sPers.setPosition(x,y);

        cursor.setPosition(static_cast<sf::Vector2f>(sf::Mouse::getPosition(window)));

        if (isGift == true && sPers.getGlobalBounds().contains(gift.getPosition().x,gift.getPosition().y)){ // czy gracz dotyka prezentu?
            //drawciek = true; // i pokaz ciekawostke
            isGift = false; // jesli tak to przestan pokazywac prezent
            drawciek = true;
            zegar.restart();
        }

        if (this->platforma == true && czas.asSeconds() > 2){
            this->platforma = false;
        }

        if (czas.asMilliseconds() > 50 && czas.asMilliseconds() < 55 || czas.asMilliseconds() > 100 && czas.asMilliseconds() < 105 ||
            czas.asMilliseconds() > 150 && czas.asMilliseconds() < 155 || czas.asMilliseconds() > 200 && czas.asMilliseconds() < 205 ||
            czas.asMilliseconds() > 250 && czas.asMilliseconds() < 255 || czas.asMilliseconds() > 300 && czas.asMilliseconds() < 305 ||
            czas.asMilliseconds() > 350 && czas.asMilliseconds() < 355 || czas.asMilliseconds() > 400 && czas.asMilliseconds() < 405 ||
            czas.asMilliseconds() > 450 && czas.asMilliseconds() < 455 || czas.asMilliseconds() > 500 && czas.asMilliseconds() < 505 ||
            czas.asMilliseconds() > 550 && czas.asMilliseconds() < 555 || czas.asMilliseconds() > 600 && czas.asMilliseconds() < 605 ||
            czas.asMilliseconds() > 650 && czas.asMilliseconds() < 655 || czas.asMilliseconds() > 700 && czas.asMilliseconds() < 705 ||
            czas.asMilliseconds() > 750 && czas.asMilliseconds() < 705 || czas.asMilliseconds() > 800 && czas.asMilliseconds() < 805 ||
            czas.asMilliseconds() > 850 && czas.asMilliseconds() < 855 || czas.asMilliseconds() > 900 && czas.asMilliseconds() < 905 ||
            czas.asMilliseconds() > 950 && czas.asMilliseconds() < 955 || czas.asMilliseconds() > 1000 && czas.asMilliseconds() < 1005 ||
            czas.asMilliseconds() > 1050 && czas.asMilliseconds() < 1055 || czas.asMilliseconds() > 1100 && czas.asMilliseconds() < 1105 ||
            czas.asMilliseconds() > 1150 && czas.asMilliseconds() < 1155 || czas.asMilliseconds() > 1200 && czas.asMilliseconds() < 1205 ||
            czas.asMilliseconds() > 1250 && czas.asMilliseconds() < 1255 || czas.asMilliseconds() > 1300 && czas.asMilliseconds() < 1305 ||
            czas.asMilliseconds() > 1350 && czas.asMilliseconds() < 1355 || czas.asMilliseconds() > 1400 && czas.asMilliseconds() < 1405 ||
            czas.asMilliseconds() > 1450 && czas.asMilliseconds() < 1455 || czas.asMilliseconds() > 1500 && czas.asMilliseconds() < 1505 ||
            czas.asMilliseconds() > 1550 && czas.asMilliseconds() < 1555 || czas.asMilliseconds() > 1600 && czas.asMilliseconds() < 1605 ||
            czas.asMilliseconds() > 1650 && czas.asMilliseconds() < 1655 || czas.asMilliseconds() > 1700 && czas.asMilliseconds() < 1705 ||
            czas.asMilliseconds() > 1750 && czas.asMilliseconds() < 1755 || czas.asMilliseconds() > 1800 && czas.asMilliseconds() < 1805 ||
            czas.asMilliseconds() > 1850 && czas.asMilliseconds() < 1855 || czas.asMilliseconds() > 1900 && czas.asMilliseconds() < 1905 ||
            czas.asMilliseconds() > 1950 && czas.asMilliseconds() < 1955 || czas.asMilliseconds() > 2000 && czas.asMilliseconds() < 2005 ||
            czas.asMilliseconds() > 2050 && czas.asMilliseconds() < 2055 || czas.asMilliseconds() > 2100 && czas.asMilliseconds() < 2105 ||
            czas.asMilliseconds() > 2150 && czas.asMilliseconds() < 2155 || czas.asMilliseconds() > 2200 && czas.asMilliseconds() < 2205 ||
            czas.asMilliseconds() > 2250 && czas.asMilliseconds() < 2255 || czas.asMilliseconds() > 2300 && czas.asMilliseconds() < 2305 ||
            czas.asMilliseconds() > 2350 && czas.asMilliseconds() < 2355 || czas.asMilliseconds() > 2400 && czas.asMilliseconds() < 2400 ||
            czas.asMilliseconds() > 2450 && czas.asMilliseconds() < 2455 || czas.asMilliseconds() > 2500 && czas.asMilliseconds() < 2505 ||
            czas.asMilliseconds() > 2550 && czas.asMilliseconds() < 2555 || czas.asMilliseconds() > 2600 && czas.asMilliseconds() < 2605 ||
            czas.asMilliseconds() > 2650 && czas.asMilliseconds() < 2655 || czas.asMilliseconds() > 2700 && czas.asMilliseconds() < 2705 ||
            czas.asMilliseconds() > 2750 && czas.asMilliseconds() < 2755 || czas.asMilliseconds() > 2800 && czas.asMilliseconds() < 2805 ||
            czas.asMilliseconds() > 2850 && czas.asMilliseconds() < 2855 || czas.asMilliseconds() > 2900 && czas.asMilliseconds() < 2905 ||
            czas.asMilliseconds() > 2950 && czas.asMilliseconds() < 2955 || czas.asMilliseconds() > 3000 && czas.asMilliseconds() < 3005 ||
            czas.asMilliseconds() > 3050 && czas.asMilliseconds() < 3055 || czas.asMilliseconds() > 3100 && czas.asMilliseconds() < 3105 ||
            czas.asMilliseconds() > 3150 && czas.asMilliseconds() < 3155 || czas.asMilliseconds() > 3200 && czas.asMilliseconds() < 3205 ||
            czas.asMilliseconds() > 3250 && czas.asMilliseconds() < 3255 || czas.asMilliseconds() > 3300 && czas.asMilliseconds() < 3305 ||
            czas.asMilliseconds() > 3350 && czas.asMilliseconds() < 3355 || czas.asMilliseconds() > 3400 && czas.asMilliseconds() < 3405 ||
            czas.asMilliseconds() > 3450 && czas.asMilliseconds() < 3455 || czas.asMilliseconds() > 3500 && czas.asMilliseconds() < 3505 ||
            czas.asMilliseconds() > 3550 && czas.asMilliseconds() < 3555 || czas.asMilliseconds() > 3600 && czas.asMilliseconds() < 3605 ||
            czas.asMilliseconds() > 3650 && czas.asMilliseconds() < 3655 || czas.asMilliseconds() > 3700 && czas.asMilliseconds() < 3705 ||
            czas.asMilliseconds() > 3750 && czas.asMilliseconds() < 3755 || czas.asMilliseconds() > 3800 && czas.asMilliseconds() < 3805 ||
            czas.asMilliseconds() > 3850 && czas.asMilliseconds() < 3855 || czas.asMilliseconds() > 3900 && czas.asMilliseconds() < 3905 ||
            czas.asMilliseconds() > 3950 && czas.asMilliseconds() < 3955 || czas.asMilliseconds() > 4000 && czas.asMilliseconds() < 4005){ // zmiana klatki co 50 milisekund
            sBackground.setTexture(bgTexture[ktoratextura]);
            ktoratextura++;
        }
        czas=zegar.getElapsedTime();


        if (isGift == false){ // gdy prezentu nie ma na mapie
            if (drawciek == true){ // jezeli wyswietlona ciekawostka
                if (czas.asSeconds() > 4){ // po trzech sekundach

                    prezent:
                        whichGift = rand()%ile_linii+1; // losuje ktora ciekawoste wyswietlic

                        if (ciekawostki[whichGift] == false)
                            ciekawostki[whichGift] = true;
                        else    goto prezent;

                    //cout << whichGift << endl;
                    isGift2 = true; // gift jest na mapie
                    drawciek = false; // przestan wyswietlac ciekawostke
                    zegar.restart();
                }
                czas=zegar.getElapsedTime();
            }
            else if (isGift2 == true){ // gdy mam ciekawostki nie rysowac
                if (czas.asSeconds() > 4){ // co cztery sekundy
                    giftX = rand()%350+200; // losuje polozenie nastepnego prezentu
                    isGift = true; // prezent jest na mapie
                    isGift2 = false;
                    drawciek = false;
                    zegar.restart();
                }
                czas=zegar.getElapsedTime();
            }
            else{
                if (drawciek == false){ // gdy mam ciekawostki nie rysowac
                    if (czas.asSeconds() > 4){ // co cztery sekundy
                        giftX = rand()%350+200; // losuje polozenie nastepnego prezentu
                        isGift = true; // prezent jest na mapie
                        zegar.restart();
                    }
                    czas=zegar.getElapsedTime();
                }
            }
        }

        window.clear();

        window.draw(sBackground);

        for (int i=0;i<10;i++)
        {
            sPlat.setPosition(plat[i].x,plat[i].y);
            window.draw(sPlat);
        }

        window.draw(zycia[iloscZyc]);

        if (this->platforma == true){
            window.draw(FirstPlatform);
        }

        window.draw(sPers);

        window.draw(audienceleft);
        window.draw(audienceright);

        if (isGift == true)
            window.draw(gift);

        if (drawciek == true){
            window.draw(tloNapisow);
            window.draw(ciekawostka[whichGift]);
        }

        window.setView(fixed);
        window.draw(cursor);

        window.display();
    }
}

void Game::gameOver()
{
    // state == GAMEOVER
    Text title(L"Kopernik i Płaska Ziemia",font,40);
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
		tekst[i].setStyle(Text::Bold);
	}
///////////////////////////////////////////////////////////////////////////////////

    sf::Text gameover("<!Game Over!>",font,90);
    gameover.setPosition(szerokosc/2-gameover.getGlobalBounds().width/2,wysokosc/3-gameover.getGlobalBounds().height/2);

    sf::Text tryagain(L"Ale możesz spróbować jeszcze raz...",font,30);
    tryagain.setPosition(szerokosc/2-gameover.getGlobalBounds().width/3-10,tekst[0].getPosition().y-gameover.getGlobalBounds().height/2);

    gameover.setStyle(Text::Bold);
    tryagain.setStyle(Text::Bold);

    ////////////////////////
    // Dzwieki


    sf::SoundBuffer sover1, sover2;
    if(!sover1.loadFromFile("Resources/Game/Music/gameover1.wav"))
        ErrorMsg("hmm... Chyba brakuje dzwieku! Sprawdz Resources/Game/Music/gameover1.wav","ERROR");
    if(!sover2.loadFromFile("Resources/Game/Music/gameover2.wav"))
        ErrorMsg("hmm... Chyba brakuje dzwieku! Sprawdz Resources/Game/Music/gameover2.wav","ERROR");

    sf::Sound over1, over2;
    over1.setBuffer(sover1);
    over2.setBuffer(sover2);

    over1.setLoop(false);
    over2.setLoop(false);

    over1.setVolume(40.f);
    over2.setVolume(40.f);

    srand(time(NULL));

    int ktoryover = rand()%2;

    if (ktoryover == 0)
        over1.play();
    else over2.play();

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
				event.type == Event::MouseButtonReleased && event.mouseButton.button == Mouse::Left)
			{
				state = END;
			}
			else if(tekst[0].getGlobalBounds().contains(mouse) &&
				event.type == Event::MouseButtonReleased && event.mouseButton.button == Mouse::Left)
			{
				state = MENU;
			}
			if(event.type == Event::KeyPressed && event.key.code == Keyboard::Slash)
                console();
		}
		for(int i=0;i<ile;i++)
			if(tekst[i].getGlobalBounds().contains(mouse))
				tekst[i].setFillColor(sf::Color::Cyan); // when you will move your mouse on button
			else tekst[i].setFillColor(sf::Color::White);

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
    cout << "gotoLast - przejscie do koncowki gry" << endl;
    cout << "=================================================" << endl;
    // cout << "" << endl;
}

void Game::console()
{
    string command;

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
    else if (command == "gotoLast")
        this->backgroundY = -300;
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
    //this->zycia = false;
}
