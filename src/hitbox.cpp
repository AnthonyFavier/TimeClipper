#include "../include/hitbox.hpp"

template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}

Hitbox::Hitbox(sf::Vector2f center, sf::Vector2f half_size)
{
	m_center=center;
	m_half_size=half_size;
}

bool Hitbox::overlaps(Hitbox other, sf::Vector2f* overlap)
{
	bool ok=true;

	if(m_half_size.x==0 || m_half_size.y==0 || other.getHalfSize().x==0 || other.getHalfSize().y==0
	|| abs(m_center.x - other.m_center.x) > (m_half_size.x + other.getHalfSize().x)
	|| abs(m_center.y - other.m_center.y) > (m_half_size.y + other.getHalfSize().y))
		ok=false;

	*overlap = sf::Vector2f(sgn(m_center.x-other.m_center.x) * ((other.getHalfSize().x+m_half_size.x) - abs(m_center.x-other.m_center.x)),
				sgn(m_center.y-other.m_center.y) * ((other.getHalfSize().y+m_half_size.y) - abs(m_center.y-other.m_center.y)));

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
