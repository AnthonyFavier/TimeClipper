#ifndef DEF_EMPTYTILE
#define DEF_EMPTYTILE

#include "tile.hpp"

class EmptyTile : public Tile
{
public:
	EmptyTile(sf::Vector2i pos) : Tile(pos)
	{
		m_can_go_through_right=true;
		m_can_go_through_left=true;
		m_can_go_through_up=true;
		m_can_go_through_down=true;
		m_can_drop_down_through=false;

		m_texture.loadFromFile(m_texture_filename["EmptyTile"]);
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

