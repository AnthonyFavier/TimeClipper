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
private:
	std::vector<std::vector<Tile*>> m_tiles;
};

#endif
