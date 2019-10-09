#ifndef DEF_DROPTILE
#define DEF_DROPTILE

#include <iostream>
#include "tile.hpp"

class DropTile : public Tile
{
public:
	DropTile(sf::Vector2i pos) : Tile(pos)
	{
		m_can_go_through_right=true;
		m_can_go_through_left=true;
		m_can_go_through_up=true;
		m_can_go_through_down=false;
		m_can_drop_down_through=true;

		m_texture.loadFromFile(m_texture_filename["DropTile"]);
		m_sprite.setTexture(m_texture);
	};
private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		states.transform *= getTransform();
		states.texture = &m_texture;
		target.draw(m_sprite, states);
	}
};

#endif

