#ifndef DEF_TILE
#define DEF_TILE

#include <SFML/Graphics.hpp>
#include <map>
#include <iostream>
#include <string>
#include "../constants.hpp"

using namespace std;

class Tile : public sf::Drawable, public sf::Transformable
{
public:
	Tile(sf::Vector2i pos)
	{
		sf::Texture texture;
		texture.loadFromFile("rsc/tiles/tile1.png");
		m_textures["DropTile"]=texture;

		texture.loadFromFile("rsc/tiles/tile4.png");
		m_textures["BlockTile"]= texture;

		texture.loadFromFile("rsc/tiles/tile2.png");
		m_textures["EmptyTile"]= texture;

		m_can_go_through_right=false;
		m_can_go_through_left=false;
		m_can_go_through_up=false;
		m_can_go_through_down=false;
		m_can_drop_down_through=false;
		m_pos=pos;
		this->setPosition(m_pos.x*TILE_SIZE_PIXEL, m_pos.y*TILE_SIZE_PIXEL);
	};

	void setOffsetMap(sf::Vector2f offset)
	{
		this->setPosition(offset.x+m_pos.x*TILE_SIZE_PIXEL, offset.y+m_pos.y*TILE_SIZE_PIXEL);
	};

	bool canGoThroughRight(){return m_can_go_through_right;};
	bool canGoThroughLeft(){return m_can_go_through_left;};
	bool canGoThroughUp(){return m_can_go_through_up;};
	bool canGoThroughDown(){return m_can_go_through_down;};
	bool canDropDownThrough(){return m_can_drop_down_through;};
	
protected:
	sf::Vector2i m_pos;
	bool m_can_go_through_right;
	bool m_can_go_through_left;
	bool m_can_go_through_up;
	bool m_can_go_through_down;
	bool m_can_drop_down_through;

	sf::Sprite m_sprite;
	map<string, sf::Texture> m_textures;
	sf::Texture m_texture;
	
private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const=0;
};

#endif

