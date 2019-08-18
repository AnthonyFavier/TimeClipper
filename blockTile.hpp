#ifndef DEF_BLOCKTILE
#define DEF_BLOCKTILE

#include "tile.hpp"

class BlockTile : public Tile
{
public:
	BlockTile(sf::Vector2i pos) : m_vertices(sf::Quads,4), Tile(pos)
	{
		m_can_go_through_right=false;
		m_can_go_through_left=false;
		m_can_go_through_up=false;
		m_can_go_through_down=false;

		m_vertices[0]=sf::Vector2f(0.f,0.f);
		m_vertices[1]=sf::Vector2f(m_size_pixel,0.f);
		m_vertices[2]=sf::Vector2f(m_size_pixel, m_size_pixel);
		m_vertices[3]=sf::Vector2f(0.f,m_size_pixel);

		m_vertices[0].color=sf::Color(0,255,0);
		m_vertices[1].color=sf::Color(0,255,0);
		m_vertices[2].color=sf::Color(0,255,0);
		m_vertices[3].color=sf::Color(0,255,0);
	};
private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		states.transform *= getTransform();
		states.texture = &m_texture;
		target.draw(m_vertices, states);
	};
	sf::VertexArray m_vertices;
	sf::Texture m_texture;
};

#endif
