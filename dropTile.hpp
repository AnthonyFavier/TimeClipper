#ifndef DEF_DROPTILE
#define DEF_DROPTILE

#include "tile.hpp"

class DropTile : public Tile
{
public:
	DropTile(sf::Vector2i pos) : m_vertices(sf::Quads,4), Tile(pos)
	{
		m_can_go_through_right=true;
		m_can_go_through_left=true;
		m_can_go_through_up=true;
		m_can_go_through_down=false;
		m_can_drop_down_through=true;

		m_vertices[0]=sf::Vector2f(0,0);
		m_vertices[1]=sf::Vector2f(TILE_SIZE_PIXEL,0);
		m_vertices[2]=sf::Vector2f(TILE_SIZE_PIXEL, TILE_SIZE_PIXEL);
		m_vertices[3]=sf::Vector2f(0,TILE_SIZE_PIXEL);

		m_vertices[0].color=sf::Color(255,0,255);
		m_vertices[1].color=sf::Color(0,100,100);
		m_vertices[2].color=sf::Color(255,0,255);
		m_vertices[3].color=sf::Color(0,100,100);
	};
private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		states.transform *= getTransform();
		states.texture = &m_texture;
		target.draw(m_vertices, states);
	}
	sf::VertexArray m_vertices;
	sf::Texture m_texture;
};

#endif

