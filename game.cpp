#include "game.h"
#include <fstream>

RenderWindow window(VideoMode(1280,720),"Jeszcze nie wiem",Style::Fullscreen);

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

	if(!font.loadFromFile("Resources/Fonts/kop.otf")) { //TODO:Znalezc jakas fajna czcionke
        ErrorMsg("Hmm... Chyba brakuje czcionki! Sprawdz 'Resources/Fonts/kop.otf'","ERROR");
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
	while(state != END)
	{
		switch (state)
		{
		case MENU:
			menu();
			break;
		case GAME:
			break;
		}
	}
}

void Game::menu()
{
	Text title(Title,font,80);
	title.setStyle(Text::Bold);

	title.setPosition(1280/2-title.getGlobalBounds().width/2,20);
//////////////////////////////////////////////////////////////////////////////
    window.setMouseCursorVisible(false);

	sf::View fixed = window.getView();
	sf::Texture cursorTexture;
	if(!cursorTexture.loadFromFile("Resources/Textures/cursor.png"))// Custon Cursor//TODO: zmienic error boxa
		ErrorMsg("Cursor not found! Check: 'Resources/Textures/cursor.png'","ERROR");

	sf::Sprite cursor(cursorTexture);
/////////////////////////////////////////////////////////////////////////////
	const int ile = 3;

	Text tekst[ile];

	string str[] = {"Rozpocznij","Dodatki","Zamknij"};
	for(int i=0;i<ile;i++)
	{
		tekst[i].setFont(font);
		tekst[i].setCharacterSize(65);              // Main Menu, texts and buttons

		tekst[i].setString(str[i]);
		tekst[i].setPosition(1280/2-tekst[i].getGlobalBounds().width/2,250+i*120);
	}
///////////////////////////////////////////////////////////////////////////////////
	while(state == MENU)
	{
		Vector2f mouse(Mouse::getPosition());
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

/*void Game::l1()
{
    Text title(Title,font,50);
	title.setStyle(Text::Bold);
    title.setOutlineColor(sf::Color::Blue);
    title.setOutlineThickness(2);
	title.setPosition(1280/2-title.getGlobalBounds().width/2,20);

    window.setMouseCursorVisible(false);

	sf::View fixed = window.getView();
	sf::Texture cursorTexture;
	if(!cursorTexture.loadFromFile("Resources/Textures/cursor.png"))
        ErrorMsg("Cursor not found! Check: 'Resources/Textures/cursor.png'","ERROR");

	sf::Sprite cursor(cursorTexture);

	sf::Texture bg_tex;
    if (!bg_tex.loadFromFile("Resources/Textures/floor.png"))
		ErrorMsg("Some Resources not found! Check: 'Resources/Textures/floor.png'","ERROR");

    sf::Sprite background(bg_tex);

    sf::Texture leverOFFtexture, leverONtexture;
    if (!leverOFFtexture.loadFromFile("Resources/Textures/leverOFF.png"))
        ErrorMsg("Some Resources not found! Check: 'Resources/Textures/leverOFF.png'","ERROR");

    if (!leverONtexture.loadFromFile("Resources/Textures/leverON.png"))
        ErrorMsg("Some Resources not found! Check: 'Resources/Textures/leverON.png'","ERROR");

    sf::Sprite leverOFF(leverOFFtexture), leverOFF2(leverOFFtexture);
    sf::Sprite leverON(leverONtexture), leverON2(leverONtexture);

    leverOFF.setPosition(50,50);
    leverON.setPosition(50,50);

    leverOFF2.setPosition(250,150);
    leverON2.setPosition(250,150);

    sf::Texture tex_door_closed, tex_door_opened;
    if (!tex_door_closed.loadFromFile("Resources/Game/door_closed.png"))
        ErrorMsg("Some Resources not found! Check: 'Resources/Game/door_closed.png'","ERROR");
    if (!tex_door_opened.loadFromFile("Resources/Game/door_opened.png"))
        ErrorMsg("Some Resources not found! Check: 'Resources/Game/door_opened.png'","ERROR");

    sf::Sprite door_op(tex_door_opened), door_cl(tex_door_closed);

    door_op.setPosition(320,160);
    door_cl.setPosition(320,160);

    bool leverstage1 = 0;
    bool leverstage2 = 0;
    bool first_door = 0;

    //auto pos = sf::Mouse::getPosition(window);
    //TODO: Odtwarzac dzwiek dxwigni

    fstream file;
    string file_name = "Resources/Game/Levels/Level_1.LEVEL";
    file.open(file_name.c_str(), ios::in);

    if(file.good())
    {
        int i = 0;
        string line;
        int level[576];

        while (getline(file, line, ' '))
        {
            level[i] = atoi(line.c_str());
            i++;
        }

        TileMap map;
        if (!map.load("Resources/Game/Tileset_1.png", sf::Vector2u(40, 40), level, 32, 18))
            ErrorMsg("Map not found! Check: 'Resources/Game/Tileset_1.png'","ERROR");

        while(state == L1)
        {
            Vector2f mouse(Mouse::getPosition());
            Event event;

            while(window.pollEvent(event))
            {
                //Wciœniêcie ESC lub przycisk X
                if(event.type == Event::Closed || event.type == Event::KeyPressed &&
                    event.key.code == Keyboard::Escape)
                    state = SELECT;
                if(leverOFF2.getGlobalBounds().contains(sf::Mouse::getPosition(window).x,sf::Mouse::getPosition(window).y && event.type == sf::Event::MouseButtonPressed && event.key.code == sf::Mouse::Left)
                   || leverON2.getGlobalBounds().contains(sf::Mouse::getPosition(window).x,sf::Mouse::getPosition(window).y) && event.type == sf::Event::MouseButtonPressed && event.key.code == sf::Mouse::Left)
                {
                    if (leverstage2 == 0)
                        leverstage2 = 1;
                    else
                        leverstage2 = 0;
                }
                if(leverOFF.getGlobalBounds().contains(sf::Mouse::getPosition(window).x,sf::Mouse::getPosition(window).y && event.type == sf::Event::MouseButtonPressed && event.key.code == sf::Mouse::Left)
                   || leverON.getGlobalBounds().contains(sf::Mouse::getPosition(window).x,sf::Mouse::getPosition(window).y) && event.type == sf::Event::MouseButtonPressed && event.key.code == sf::Mouse::Left)
                {
                    if (leverstage1 == 0)
                        leverstage1 = 1;
                    else
                        leverstage1 = 0;
                }
            }

            cursor.setPosition(static_cast<sf::Vector2f>(sf::Mouse::getPosition(window)));

            window.clear();

            window.draw(background);
            window.draw(map);
            window.draw(title);

            if (leverstage1 == 0)
            {
                window.draw(leverOFF);
                first_door = 0;
            }
            else if (leverstage1 == 1)
                window.draw(leverON);

            if (leverstage1 == 1)
            {
                if (leverstage2 == 0)
                {
                    window.draw(leverOFF2);
                    first_door = 0;
                }
                else if (leverstage2 == 1)
                {
                    window.draw(leverON2);
                    first_door = 1;
                }
            }

            if (first_door == 1)
                window.draw(door_op);
            else
                window.draw(door_cl);

            window.setView(fixed);
            window.draw(cursor);
            window.display();
	}
	file.close();
    }
    else
        ErrorMsg("Map not found! Check: 'Resources/Game/Levels/Level_1.LEVEL'","ERROR");
}
*/
