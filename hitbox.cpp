#include "hitbox.hpp"

Hitbox::Hitbox(sf::Vector2f center, sf::Vector2f half_size)
{
	m_center=center;
	m_half_size=half_size;
}

bool Hitbox::overlaps(Hitbox other)
{
	bool ok=true;

	if(abs(m_center.x - other.m_center.x) > (m_half_size.x + other.getHalfSize().x)
	|| abs(m_center.y - other.m_center.y) > (m_half_size.y + other.getHalfSize().y))
		ok=false;

	return ok;
}

sf::Vector2f Hitbox::getHalfSize()
{
	return m_half_size;
}

void Hitbox::setHalfSize(sf::Vector2f hs)
{
	m_half_size=hs;
}
