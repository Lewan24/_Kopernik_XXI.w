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
			// TODO: Zrobic Dodatki
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
     //  TODO: Zwiekszyc pole gry do wiekszego rozmiaru
     //  TODO: Dodaj widownie rzucajaca tabliczki (przeszkoda)
     /*  TODO: Zaladowac tlo gry zmieniajace sie w czasie wraz z iloscia przeskoczonych
           przeszkod...*/
     /*	TODO: Zmienic tekstury blokow na wlasne
            Zmienic teksture gracza z doodle na kopernika etc.
     */

    window.setMouseCursorVisible(false);

	sf::View fixed = window.getView();
	sf::Texture cursorTexture;
	if(!cursorTexture.loadFromFile("Resources/Textures/cursor.png"))
        ErrorMsg("Cursor not found! Check: 'Resources/Textures/cursor.png'","ERROR");

	sf::Sprite cursor(cursorTexture);

    srand(time(0));

    Texture t1,t2,t3,t4;
    t1.loadFromFile("Resources/Textures/background.png");
    t2.loadFromFile("Resources/Textures/platform.png");
    t3.loadFromFile("Resources/Textures/doodle.png");
    t4.loadFromFile("Resources/Textures/doodle2.png");

    sf::Sprite sBackground(t1), sPlat(t2);
    sf::Sprite sPers(t3);

    sBackground.setPosition(200,0);

    point plat[20];

    for (int i=0;i<10;i++)
    {
       plat[i].x=rand()%400+200;
       plat[i].y=rand()%600;
    }

    int x=szerokosc/2, y=wysokosc/2, h=wysokosc/2;
    float dx=0,dy=0;


    while(state == GAMESTART)
    {
        Vector2f mouse(Mouse::getPosition());

        Event event;

        while(window.pollEvent(event))
        {
            //Wciœniêcie ESC lub przycisk X
            if(event.type == Event::Closed || event.type == Event::KeyPressed &&
                event.key.code == Keyboard::Escape)
                state = MENU;
        }

        dy+=0.2;
        y+=dy;

        if (y>600)  {
            dy=-10;
            state = MENU;
            //TODO: Zrobic efekt Game over etc
        }

        //if (dy == 0.2) sBackground.move(0,5);

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

        for (int i=0;i<10;i++)
        if ((x+50>plat[i].x) && (x+20<plat[i].x+68)
        && (y+70>plat[i].y) && (y+70<plat[i].y+14) && (dy>0))  dy=-10;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)){
            x-=3;
            sPers.setTexture(t3);

            if (sPers.getPosition().x < (150)){
                //x = x + sPers.getScale().x + szerokosc;
                x = x + sPers.getScale().x + 400;
            }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)){
            x+=3;
            sPers.setTexture(t4);

            if (sPers.getPosition().x > 580){
                //x = x + sPers.getScale().x + szerokosc;
                x = 200;
            }
        }

        sPers.setPosition(x,y);

        cursor.setPosition(static_cast<sf::Vector2f>(sf::Mouse::getPosition(window)));

        window.clear();

        window.draw(sBackground);

        for (int i=0;i<10;i++)
        {
            sPlat.setPosition(plat[i].x,plat[i].y);
            window.draw(sPlat);
        }

        window.draw(sPers);

        window.setView(fixed);
        window.draw(cursor);

        window.display();
    }
}

