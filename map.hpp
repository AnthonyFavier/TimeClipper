#ifndef DEF_MAP
#define DEF_MAP

#include <SFML/Graphics.hpp>
#include <vector>
#include "tile.hpp"
#include "blockTile.hpp"
#include "emptyTile.hpp"

class Map
{
public:
	Map();
	void draw(sf::RenderWindow* window);

	int getWidth();
	int getHeight();

	sf::Vector2f getPosition();

	sf::Vector2i getMapTileAtPoint(sf::Vector2f point);
	sf::Vector2i getMapTileAtPoint(float x, float y);
	int getMapTileXAtPoint(float x);
	int getMapTileYAtPoint(float y);

	sf::Vector2f getMapTilePosition(sf::Vector2i tile);
	sf::Vector2f getMapTilePosition(int x, int y);

	bool canGoThroughRightTile(int x, int y);
	bool canGoThroughRightTile(sf::Vector2i tile_coord);
	bool canGoThroughLeftTile(int x, int y);
	bool canGoThroughLeftTile(sf::Vector2i tile_coord);
	bool canGoThroughUpTile(int x, int y);
	bool canGoThroughUpTile(sf::Vector2i tile_coord);
	bool canGoThroughDownTile(int x, int y);
	bool canGoThroughDownTile(sf::Vector2i tile_coord);

private:
	std::vector<std::vector<Tile*>> m_tiles; 
	sf::Vector2f m_position;
};

#endif
