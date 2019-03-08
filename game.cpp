#include <fstream>
#include <SFML/Graphics.hpp>
#include <iostream>

#include "game.h"

sf::Vector2i screenDimensions(1280,720);
sf::RenderWindow window(sf::VideoMode(screenDimensions.x, screenDimensions.y), "Jeszcze nie wiem",Style::Fullscreen);

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
    window.setFramerateLimit(60);

	while(state != END)
	{
		switch (state)
		{
		case MENU:
			menu();
			break;
		case GAME:
		    mainGame();
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
			else if(tekst[0].getGlobalBounds().contains(mouse) &&
				event.type == Event::MouseButtonReleased && event.key.code == Mouse::Left)
			{
				state = GAME;
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

void Game::mainGame()
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

    sf::Texture playerTexture;
    if (!playerTexture.loadFromFile("Resources/Game/player.png"))
        ErrorMsg("Nie znaleziono, lub nie udalo sie wczytac tekstury postaci. Sprawdz czy posiadasz: 'Resources/Game/player.png'","ERROR");

    fstream file;
    string file_name = "Resources/Game/Levels/Level_1.LEVEL";
    file.open(file_name.c_str(), ios::in);

    Animation walkingAnimationDown;
    walkingAnimationDown.setSpriteSheet(playerTexture);
    walkingAnimationDown.addFrame(sf::IntRect(32, 0, 32, 32));
    walkingAnimationDown.addFrame(sf::IntRect(64, 0, 32, 32));
    walkingAnimationDown.addFrame(sf::IntRect(32, 0, 32, 32));
    walkingAnimationDown.addFrame(sf::IntRect( 0, 0, 32, 32));

    Animation walkingAnimationLeft;
    walkingAnimationLeft.setSpriteSheet(playerTexture);
    walkingAnimationLeft.addFrame(sf::IntRect(32, 32, 32, 32));
    walkingAnimationLeft.addFrame(sf::IntRect(64, 32, 32, 32));
    walkingAnimationLeft.addFrame(sf::IntRect(32, 32, 32, 32));
    walkingAnimationLeft.addFrame(sf::IntRect( 0, 32, 32, 32));

    Animation walkingAnimationRight;
    walkingAnimationRight.setSpriteSheet(playerTexture);
    walkingAnimationRight.addFrame(sf::IntRect(32, 64, 32, 32));
    walkingAnimationRight.addFrame(sf::IntRect(64, 64, 32, 32));
    walkingAnimationRight.addFrame(sf::IntRect(32, 64, 32, 32));
    walkingAnimationRight.addFrame(sf::IntRect( 0, 64, 32, 32));

    Animation walkingAnimationUp;
    walkingAnimationUp.setSpriteSheet(playerTexture);
    walkingAnimationUp.addFrame(sf::IntRect(32, 96, 32, 32));
    walkingAnimationUp.addFrame(sf::IntRect(64, 96, 32, 32));
    walkingAnimationUp.addFrame(sf::IntRect(32, 96, 32, 32));
    walkingAnimationUp.addFrame(sf::IntRect( 0, 96, 32, 32));

    Animation* currentAnimation = &walkingAnimationDown;

    AnimatedSprite animatedSprite(sf::seconds(0.2), true, false);
    animatedSprite.setPosition(sf::Vector2f(screenDimensions / 2));

    sf::Clock frameClock;

    float speed = 80.f;
    float speedtemp = speed;
    float run = 200.f;
    bool noKeyWasPressed = true;

    Gracz player(Gracz(sf::Vector2f(25,25)));

    Object blok; // Generacja ustawien domyslnych objektu
    blok.editObject(sf::Vector2f(100,100),sf::Vector2f(100,50),sf::Color::Blue,sf::Color::Green,0); // edycja stworzonego wczesniej objektu

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

        while(state == GAME)
        {
            player.rect.setPosition(animatedSprite.getPosition().x+4, animatedSprite.getPosition().y+4);

            Vector2f mouse(Mouse::getPosition());
            Event event;

            while(window.pollEvent(event))
            {
                //Wciœniêcie ESC lub przycisk X
                if(event.type == Event::Closed || event.type == Event::KeyPressed &&
                    event.key.code == Keyboard::Escape)
                    state = MENU;
            }

            sf::Time frameTime = frameClock.restart();

            // if a key was pressed set the correct animation and move correctly
            sf::Vector2f movement(0.f, 0.f);

            player.Collision(blok, movement, speed);

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
                speed = run;
            else    speed = speedtemp;

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            {
                currentAnimation = &walkingAnimationUp;
                noKeyWasPressed = false;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            {
                currentAnimation = &walkingAnimationDown;
                noKeyWasPressed = false;
            }
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            {
                currentAnimation = &walkingAnimationLeft;
                noKeyWasPressed = false;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            {
                currentAnimation = &walkingAnimationRight;
                noKeyWasPressed = false;
            }
            animatedSprite.play(*currentAnimation);
            animatedSprite.move(movement * frameTime.asSeconds());

            // if no key was pressed stop the animation
            if (noKeyWasPressed)
            {
                animatedSprite.stop();
            }
            noKeyWasPressed = true;

            // update AnimatedSprite
            animatedSprite.update(frameTime);
            blok.Update();
            player.Update();

            cursor.setPosition(static_cast<sf::Vector2f>(sf::Mouse::getPosition(window)));

            window.clear(sf::Color(100,100,100,255));

            window.draw(map);
            window.draw(title);

            window.draw(blok.rect);
            //window.draw(player.rect);
            window.draw(animatedSprite);

            window.setView(fixed);
            window.draw(cursor);

            window.display();
        }
        file.close();
    }
    else
        ErrorMsg("Map not found! Check: 'Resources/Game/Levels/Level_1.LEVEL'","ERROR");
}

