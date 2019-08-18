#include "map.hpp"

Map::Map()
{
	std::vector<Tile*> tiles;

	tiles.push_back(new BlockTile(sf::Vector2i(0,0)));
	tiles.push_back(new EmptyTile(sf::Vector2i(1,0)));
	m_tiles.push_back(tiles);

	tiles[0]=new EmptyTile(sf::Vector2i(0,1));
	tiles[1]=new BlockTile(sf::Vector2i(1,1));
	m_tiles.push_back(tiles);

	tiles[0]=new BlockTile(sf::Vector2i(0,2));
	tiles[1]=new EmptyTile(sf::Vector2i(1,2));
	m_tiles.push_back(tiles);

	tiles[0]=new BlockTile(sf::Vector2i(0,3));
	tiles[1]=new EmptyTile(sf::Vector2i(1,3));
	m_tiles.push_back(tiles);

	tiles[0]=new BlockTile(sf::Vector2i(5,3));
	tiles[1]=new EmptyTile(sf::Vector2i(6,3));
	m_tiles.push_back(tiles);
}

void Map::draw(sf::RenderWindow* window)
{
	for(int i=0; i<m_tiles.size(); i++)
	{
		for(int j=0; j<m_tiles[i].size(); j++)
		{
			window->draw(*m_tiles[i][j]);
		}
	}
}
