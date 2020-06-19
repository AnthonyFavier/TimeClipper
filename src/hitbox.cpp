#include "../include/hitbox.hpp"

template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}

Hitbox::Hitbox(sf::Vector2f center, sf::Vector2f half_size) : m_shape(sf::Vector2f(half_size.x*2-2, half_size.y*2-2))
{
	m_center=center;
	m_half_size=half_size;
	m_shape.setPosition(m_center-m_half_size+sf::Vector2f(1,1));
	m_shape.setFillColor(sf::Color(0,0,0,0));
	m_shape.setOutlineThickness(1);
	m_shape.setOutlineColor(sf::Color(255,0,0));
}

void Hitbox::updateCenter(sf::Vector2f center)
{
	m_center=center;
	m_shape.setPosition(m_center-m_half_size+sf::Vector2f(1,1));
}

void Hitbox::draw(sf::RenderWindow* window)
{
	window->draw(m_shape);
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


