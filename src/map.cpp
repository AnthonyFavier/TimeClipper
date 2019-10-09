#include "../include/map.hpp"

#include <iostream>
using namespace std;

Map::Map()
{
	m_position=sf::Vector2f(0,0);

	std::vector<Tile*> tiles;
	for(int xi=0; xi<8; xi++)
	{
		for(int yi=0; yi<8; yi++)
		{
			if(yi<7)
				tiles.push_back(new EmptyTile(sf::Vector2i(xi,yi)));
			else
				tiles.push_back(new BlockTile(sf::Vector2i(xi,yi)));
		}
		m_tiles.push_back(tiles);

		tiles.clear();
	}


	m_tiles[2][2]=new DropTile(sf::Vector2i(2,2));
	m_tiles[3][2]=new DropTile(sf::Vector2i(3,2));
	m_tiles[0][5]=new BlockTile(sf::Vector2i(0,5));
	m_tiles[1][5]=new BlockTile(sf::Vector2i(1,5));
	m_tiles[2][5]=new DropTile(sf::Vector2i(2,5));
	m_tiles[3][4]=new DropTile(sf::Vector2i(3,4));
	m_tiles[3][3]=new DropTile(sf::Vector2i(3,3));
	m_tiles[5][1]=new DropTile(sf::Vector2i(5,1));
	m_tiles[6][1]=new BlockTile(sf::Vector2i(6,1));
	m_tiles[7][1]=new BlockTile(sf::Vector2i(7,1));
	m_tiles[5][5]=new DropTile(sf::Vector2i(5,5));
	m_tiles[6][6]=new BlockTile(sf::Vector2i(6,6));
}

Map::~Map()
{
	for(int i=0; i<m_tiles.size(); i++)
	{
		for(int j=0; j<m_tiles[0].size(); j++)
			delete m_tiles[i][j];
	}
}

void Map::draw(sf::RenderWindow* window)
{
	for(int i=0; i<m_tiles.size(); i++)
	{
		for(int j=0; j<m_tiles[i].size(); j++)
		{
			m_tiles[i][j]->setOffsetMap(m_position);
			window->draw(*m_tiles[i][j]);
		}
	}
}

int Map::getHeight()
{
	int height=0;

	if(m_tiles.size()>0)
		height=m_tiles[0].size();

	return height;
}

int Map::getWidth()
{
	return m_tiles.size();
}

sf::Vector2f Map::getPosition()
{
	return m_position;
}

sf::Vector2i Map::getMapTileAtPoint(float x, float y)
{
	sf::Vector2i vec;

	vec.x=getMapTileXAtPoint(x);
	vec.y=getMapTileYAtPoint(y);

	return vec;
}

sf::Vector2i Map::getMapTileAtPoint(sf::Vector2f point)
{
	return this->getMapTileAtPoint(point.x, point.y);
}

int Map::getMapTileXAtPoint(float x)
{
	return (int)(std::floor((x-m_position.x)/(float)(TILE_SIZE_PIXEL)));
}

int Map::getMapTileYAtPoint(float y)
{
	return (int)(std::floor((y-m_position.y)/(float)(TILE_SIZE_PIXEL)));
}

sf::Vector2f Map::getMapTilePosition(sf::Vector2i tile)
{
	return getMapTilePosition(tile.x,tile.y);
}

sf::Vector2f Map::getMapTilePosition(int x, int y)
{
	sf::Vector2f vec;

	vec.x=(float)(x*TILE_SIZE_PIXEL + m_position.x);
	vec.y=(float)(y*TILE_SIZE_PIXEL + m_position.y);

	return vec;
}

bool Map::canGoThroughRightTile(int x, int y)
{
	bool ok=false;

	if(x>=0 && x<this->getWidth() && y>=0 && y<this->getHeight())
		ok=m_tiles[x][y]->canGoThroughRight();

	return ok;
}

bool Map::canGoThroughRightTile(sf::Vector2i tile_coord)
{
	return this->canGoThroughRightTile(tile_coord.x, tile_coord.y);
}

bool Map::canGoThroughLeftTile(int x, int y)
{
	bool ok=false;

	if(x>=0 && x<this->getWidth() && y>=0 && y<this->getHeight())
		ok=m_tiles[x][y]->canGoThroughLeft();

	return ok;
}

bool Map::canGoThroughLeftTile(sf::Vector2i tile_coord)
{
	return this->canGoThroughLeftTile(tile_coord.x, tile_coord.y);
}

bool Map::canGoThroughUpTile(int x, int y)
{
	bool ok=false;

	if(x>=0 && x<this->getWidth() && y>=0 && y<this->getHeight())
		ok=m_tiles[x][y]->canGoThroughUp();

	return ok;
}

bool Map::canGoThroughUpTile(sf::Vector2i tile_coord)
{
	return this->canGoThroughUpTile(tile_coord.x, tile_coord.y);
}

bool Map::canGoThroughDownTile(int x, int y)
{
	bool ok=false;

	if(x>=0 && x<this->getWidth() && y>=0 && y<this->getHeight())
		ok=m_tiles[x][y]->canGoThroughDown();

	return ok;
}

bool Map::canGoThroughDownTile(sf::Vector2i tile_coord)
{
	return this->canGoThroughDownTile(tile_coord.x, tile_coord.y);
}

bool Map::canDropDownThroughTile(int x, int y)
{
	bool ok=false;

	if(x>=0 && x<this->getWidth() && y>=0 && y<this->getHeight())
		ok=m_tiles[x][y]->canDropDownThrough();

	return ok;
}

bool Map::canDropDownThroughTile(sf::Vector2i tile_coord)
{
	return this->canDropDownThroughTile(tile_coord.x, tile_coord.y);
}
