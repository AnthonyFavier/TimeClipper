#ifndef DEF_BLOCKTILE
#define DEF_BLOCKTILE

#include "tile.hpp"

class BlockTile : public Tile
{
public:
	BlockTile(sf::Vector2i pos) : Tile(pos)
	{
		m_can_go_through_right=false;
		m_can_go_through_left=false;
		m_can_go_through_up=false;
		m_can_go_through_down=false;
		m_can_drop_down_through=false;

		m_texture.loadFromFile(m_texture_filename["BlockTile"]);
		m_sprite.setTexture(m_texture);
	};
private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		states.transform *= getTransform();
		states.texture = &m_texture;
		target.draw(m_sprite, states);
	};
};

#endif
