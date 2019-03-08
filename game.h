#ifndef game_h
#define game_h

#include <SFML/Graphics.hpp>
#include <Windows.h>
#include <string>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/System/Vector2.hpp>

#include "Animation.hpp"
//#include <SFML/Network.hpp>
//#include <SFML/Audio.hpp>
//#include <SFML/System.hpp>
//#include <SFML/Window.hpp>

using namespace std;
using namespace sf;

class Game
{
public:
	Game(void);  // konstruktor
	~Game(void);    // destruktor

	void runGame();
	void ErrorMsg(string, string);

protected:
	enum GameState {MENU,GAME,END};
	GameState state;

private:
	Font font;
    const string Title = "Jeszcze nie wiem";
	void menu();
	void mainGame();
};

class AnimatedSprite : public sf::Drawable, public sf::Transformable
{
public:
    //explicit AnimatedSprite(sf::Time frameTime = sf::seconds(0.2f), bool paused = false, bool looped = true);
    explicit AnimatedSprite(sf::Time frameTime = sf::seconds(0.2f), bool paused = false, bool looped = true);

    void update(sf::Time deltaTime);
    void setAnimation(const Animation& animation);
    void setFrameTime(sf::Time time);
    void play();
    void play(const Animation& animation);
    void pause();
    void stop();
    void setLooped(bool looped);
    void setColor(const sf::Color& color);
    const Animation* getAnimation() const;
    sf::FloatRect getLocalBounds() const;
    sf::FloatRect getGlobalBounds() const;
    bool isLooped() const;
    bool isPlaying() const;
    sf::Time getFrameTime() const;
    void setFrame(std::size_t newFrame, bool resetTime = true);
private:
    const Animation* m_animation;
    sf::Time m_frameTime;
    sf::Time m_currentTime;
    std::size_t m_currentFrame;
    bool m_isPaused;
    bool m_isLooped;
    const sf::Texture* m_texture;
    sf::Vertex m_vertices[4];

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

class Object
{
    friend class Gracz;
    public:

    sf::RectangleShape rect;
    float bottom, left, right, top;

    // konstruktor
    Object()
    {
        rect.setPosition(sf::Vector2f(200,200));
        rect.setSize(sf::Vector2f(50,50));
        rect.setFillColor(sf::Color::Red);
        rect.setOutlineColor(sf::Color::White);
        rect.setOutlineThickness(2);
    }

    // funckcja edycji obiektu
    void editObject(sf::Vector2f position, sf::Vector2f size, sf::Color fillColor, sf::Color outLineColor, int thickness)
    {
        rect.setPosition(sf::Vector2f(position));
        rect.setSize(size);
        rect.setFillColor(fillColor);
        rect.setOutlineColor(outLineColor);
        rect.setOutlineThickness(thickness);
    }

    void Update()
    {
        bottom = rect.getPosition().y + rect.getSize().y;
        left = rect.getPosition().x;
        right = rect.getPosition().x + rect.getSize().x;
        top = rect.getPosition().y;
    }
};

class Gracz
{
    public:
    //bool kolizja = false;

    sf::RectangleShape rect;
    float bottom, left, right, top;

    Gracz(sf::Vector2f size)
    {
        rect.setSize(size);
    }

    void Collision(Object p, sf::Vector2f &movement, float &speed)
    {
        float temp = 500.f;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            if (rect.getGlobalBounds().intersects(p.rect.getGlobalBounds()))
                movement.y += speed + temp;
            else
                movement.y -= speed;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            if (rect.getGlobalBounds().intersects(p.rect.getGlobalBounds()))
                movement.y -= speed + temp;
            else
                movement.y += speed;

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            if (rect.getGlobalBounds().intersects(p.rect.getGlobalBounds()))
                movement.x += speed + temp;
            else
                movement.x -= speed;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            if (rect.getGlobalBounds().intersects(p.rect.getGlobalBounds()))
                movement.x -= speed + temp;
            else
                movement.x += speed;
    }

    /*bool isCoolided()   {
        if (kolizja == true)
            return true;
        else
            return false;
    }*/

    void Update()
    {
        bottom = rect.getPosition().y + rect.getSize().y;
        left = rect.getPosition().x;
        right = rect.getPosition().x + rect.getSize().x;
        top = rect.getPosition().y;
    }
};

class TileMap : public sf::Drawable, public sf::Transformable
{
public:

    bool load(const std::string& tileset, sf::Vector2u tileSize, const int* tiles, unsigned int width, unsigned int height)
    {
        // load the tileset texture
        if (!m_tileset.loadFromFile(tileset))
            return false;

        // resize the vertex array to fit the level size
        m_vertices.setPrimitiveType(sf::Quads);
        m_vertices.resize(width * height * 4);

        // populate the vertex array, with one quad per tile
        for (unsigned int i = 0; i < width; ++i)
            for (unsigned int j = 0; j < height; ++j)
            {
                // get the current tile number
                int tileNumber = tiles[i + j * width];

                // find its position in the tileset texture
                int tu = tileNumber % (m_tileset.getSize().x / tileSize.x);
                int tv = tileNumber / (m_tileset.getSize().x / tileSize.x);

                // get a pointer to the current tile's quad
                sf::Vertex* quad = &m_vertices[(i + j * width) * 4];

                // define its 4 corners
                quad[0].position = sf::Vector2f(i * tileSize.x, j * tileSize.y);
                quad[1].position = sf::Vector2f((i + 1) * tileSize.x, j * tileSize.y);
                quad[2].position = sf::Vector2f((i + 1) * tileSize.x, (j + 1) * tileSize.y);
                quad[3].position = sf::Vector2f(i * tileSize.x, (j + 1) * tileSize.y);

                // define its 4 texture coordinates
                quad[0].texCoords = sf::Vector2f(tu * tileSize.x, tv * tileSize.y);
                quad[1].texCoords = sf::Vector2f((tu + 1) * tileSize.x, tv * tileSize.y);
                quad[2].texCoords = sf::Vector2f((tu + 1) * tileSize.x, (tv + 1) * tileSize.y);
                quad[3].texCoords = sf::Vector2f(tu * tileSize.x, (tv + 1) * tileSize.y);
            }

        return true;
    }

private:

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        // apply the transform
        states.transform *= getTransform();

        // apply the tileset texture
        states.texture = &m_tileset;

        // draw the vertex array
        target.draw(m_vertices, states);
    }

    sf::VertexArray m_vertices;
    sf::Texture m_tileset;
};

#endif // game_h
