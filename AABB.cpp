#include "AABB.hpp"

AABB::AABB(sf::Vector2f center, sf::Vector2f halfSize)
{
	m_center=center;
	m_halfSize=halfSize;

/*	m_vertices=sf::VertexArray(sf::Quads,4);

	m_vertices[0]=center+sf::Vector2f(-halfSize.x, halfSize.y);
	m_vertices[1]=center+sf::Vector2f(halfSize.x, halfSize.y);
	m_vertices[2]=center+sf::Vector2f(halfSize.x, -halfSize.y);
	m_vertices[3]=center+sf::Vector2f(-halfSize.x, -halfSize.y);*/
}

/*void AABB::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	states.texture = &m_texture;
	target.draw(m_vertices, states);
}*/

bool AABB::overlaps(AABB other)
{
	bool ok=true;

	if(abs(m_center.x - other.getCenter().x) > (m_halfSize.x + other.getHalfSize().x)
	|| abs(m_center.y - other.getCenter().y) > (m_halfSize.y + other.getHalfSize().y))
		ok=false;

	return ok;
}

sf::Vector2f AABB::getCenter()
{
	return m_center;
}

sf::Vector2f AABB::getHalfSize()
{
	return m_halfSize;
}
