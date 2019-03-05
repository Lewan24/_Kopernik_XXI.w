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
using namespace sf;

class Game
{
public:
	Game(void);  // konstruktor
	~Game(void);    // destruktor

	void runGame();
	void ErrorMsg(string, string);

protected:
	enum GameState {MENU,GAME,GAMEOVER,END};
	GameState state;

private:
	Font font;
    const string Title = "Jeszcze nie wiem";
	void menu();
};

class Gracz
{
    public:

    sf::RectangleShape rect;
    float bottom, left, right, top;

    Gracz(sf::Vector2f position, sf::Vector2f size, sf::Color c1, sf::Color c2, int thi)
    {
        rect.setPosition(position);
        rect.setSize(size);
        rect.setFillColor(c1);
        rect.setOutlineColor(c2);
        rect.setOutlineThickness(thi);
    }

    void Update()
    {
        bottom = rect.getPosition().y + rect.getSize().y;
        left = rect.getPosition().x;
        right = rect.getPosition().x + rect.getSize().x;
        top = rect.getPosition().y;
    }

    bool Collision(Gracz p)
    {
        if (right < p.left || left > p.right ||
            top > p.bottom || bottom < p.top)
        {
            return false;
        }
        return true;
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
