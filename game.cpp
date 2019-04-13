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

Game::Game(void)
{
	state = END;

	if(!font.loadFromFile("Resources/Fonts/ziemia.otf")) {
        ErrorMsg("Hmm... Chyba brakuje czcionki! Sprawdz 'Resources/Fonts/ziemia.otf'","ERROR");
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
{
    window.setFramerateLimit(60);

	while(state != END)
	{
		switch (state)
		{
		case MENU:
			menu();
			break;
		case GAMESTART:
		    startgame();
			break;
        case GAMEOVER:
            gameOver();
            break;
			/* TODO: Zrobic Dodatki
			wprowadzic opcje,
			mozliwosc zmienienia pooziomu trudnosci
			mozliwosc wylaczenia zderzen z tabliczkami
			mozliwosc zmiany predkosci i wysokosci skoku
			wraz z zmniejszeniem trudnosci trzeba powiekszyc ilosc zyc
			tutorial?
			opis gry
			podziekowania
			credits
			opis Kopernika i jego osiagniec // w dodatkach bedzie jakas dluzsza wersja troche o nim opowiadajaca
			*/
		}
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
	const int ile = 3;

	Text tekst[ile];

	string str[] = {"Rozpocznij","Dodatki","Zamknij"};
	for(int i=0;i<ile;i++)
	{
		tekst[i].setFont(font);
		tekst[i].setCharacterSize(60);              // Main Menu, texts and buttons

		tekst[i].setString(str[i]);
		tekst[i].setPosition(szerokosc/2-tekst[i].getGlobalBounds().width/2,250+i*120);
	}
///////////////////////////////////////////////////////////////////////////////////
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
			else if(tekst[0].getGlobalBounds().contains(mouse) &&
				event.type == Event::MouseButtonReleased && event.key.code == Mouse::Left)
			{
				state = GAMESTART;
			}
		}
		for(int i=0;i<ile;i++)
			if(tekst[i].getGlobalBounds().contains(mouse))
				tekst[i].setColor(sf::Color::Cyan); // when you will move your mouse on button
			else tekst[i].setColor(sf::Color::White);

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

struct point
{ int x,y;};

void Game::startgame()
{
    // TODO: Stworzyc 3 cutscenek:
    // TODO: Cutscenka 1: na poczatku przygody, krotkie wprowadzenie, i zapowiedz gry
    // TODO: cutscena 2: w okolo polowie rozgrywki, przedstawic jakies badanie kopernika, moze opowiedziec krotkie jakas ciekawostke
    // TODO: cutscena 3: na koncu rozgrywki, podsumowanie, podziekowania, opis dokonan kopernika, napisy koncowe
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

    sBackground.setPosition(200,-4572);

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

    short iloscZyc = 4;
    //short *wsk = &iloscZyc; // TODO: zrobic konsole z komenda dodawania zyc

    sf::Font font2;
    if (!font2.loadFromFile("Resources/Fonts/lives.ttf"))
        ErrorMsg("Hmm... Chyba brakuje czcionki! Sprawdz 'Resources/Fonts/lives.ttf'","ERROR");

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

    /*  TODO: Tabliczki
     *  prezent z ciekawostka o koperniku
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

        dy+=0.2;
        y+=dy;

        //std::cout << "dy: " << dy << "      Player x: " << x << ", y: " << y << endl;

        if (y>600)  {
            //dy=-10;
            if (iloscZyc <= 0)
                state = GAMEOVER;   //state = MENU;   //TODO: Zrobic efekt Game over etc
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

	title.setPosition(szerokosc/3-title.getGlobalBounds().width/2,20);
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
		tekst[i].setCharacterSize(60);              // Main Menu, texts and buttons

		tekst[i].setString(str[i]);
		tekst[i].setPosition(szerokosc/2-tekst[i].getGlobalBounds().width/2,250+i*120);
	}
///////////////////////////////////////////////////////////////////////////////////
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
		}
		for(int i=0;i<ile;i++)
			if(tekst[i].getGlobalBounds().contains(mouse))
				tekst[i].setColor(sf::Color::Cyan); // when you will move your mouse on button
			else tekst[i].setColor(sf::Color::White);

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

using namespace std;

void help()
{
    cout << "=================================================" << endl;
    cout << "                  Komendy: " << endl << endl;

    cout << "menu - przejscie do menu gry" << endl;
    cout << "startgame - przejscie do glownej gry" << endl;
    cout << "end - zamyka aplikacje" << endl;
    cout << "exit - wyjscie z konsoli" << endl;
    cout << "gameover - przejscie do okna gameover" << endl;
    cout << "help - wyswietlenie wszystkich komend" << endl;
    cout << "=================================================" << endl;
}

void Game::console()
{
    string command;

    komenda:
    cout << endl;
    cout << "Aktualnie: ";

    if (state == GAMESTART)
        cout << "GRA" << endl;
    else if (state == MENU)
        cout << "MENU" << endl;
    else if (state == GAMEOVER)
        cout << "GAMEOVER" << endl;
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
    //else if (command == "addlive")
        //*lives += 2;
    else if (command == "gameover")
        state = GAMEOVER;
    else if (command == "exit")
        return;
    else if (command == "help")
        help();
    else{
        cout << "Zla komenda sprobuj jeszcze raz...";
        goto komenda;
    }
    goto komenda;
}
