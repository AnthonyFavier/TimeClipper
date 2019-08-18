#include "movingObject.hpp"

MovingObject::MovingObject(sf::Vector2f center, sf::Vector2f half_size, sf::Color color) : m_vertices(sf::Quads,4), m_AABB(center, half_size)
{
	m_half_size=half_size;

	this->setPosition(center);

	m_vertices[0]=sf::Vector2f(-m_half_size.x, -m_half_size.y);
	m_vertices[1]=sf::Vector2f(m_half_size.x, -m_half_size.y);
	m_vertices[2]=sf::Vector2f(m_half_size.x, m_half_size.y);
	m_vertices[3]=sf::Vector2f(-m_half_size.x, m_half_size.y);

	m_vertices[0].color =color;
	m_vertices[1].color =color;
	m_vertices[2].color =color;
	m_vertices[3].color =color;

	m_old_position=this->getPosition();
	m_speed=sf::Vector2f(0.f,0.f);
	m_old_speed=m_speed;

	m_AABB_offset=sf::Vector2f(0.f,0.f);

	m_pushes_right_wall=false;
	m_old_right_wall=false;
	m_pushes_left_wall=false;
	m_old_left_wall=false;
	m_on_ground=true;
	m_old_on_ground=true;
	m_at_ceiling=false;
	m_old_at_ceiling=false;
}

void MovingObject::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	states.texture = &m_texture;
	target.draw(m_vertices, states);
}

void MovingObject::updatePhysics(sf::Time elapsed)
{
	m_old_position=this->getPosition();
	m_old_speed=m_speed;

	m_old_on_ground=m_on_ground;
	m_old_right_wall=m_pushes_right_wall;
	m_old_left_wall=m_pushes_left_wall;
	m_old_at_ceiling=m_at_ceiling;

	this->move(m_speed*elapsed.asSeconds());

	if(this->getPosition().y+m_half_size.y>=WINDOW_HEIGHT)
	{
		this->setPosition(sf::Vector2f(this->getPosition().x,WINDOW_HEIGHT-m_half_size.y));
		m_on_ground=true;
	}
	else
		m_on_ground=false;

	m_AABB.m_center=this->getPosition()+m_AABB_offset;
}
