#include "../include/physicalObject.hpp"

PhysicalObject::PhysicalObject(sf::Vector2f center, sf::Vector2f half_size, sf::Color color, string name) : MovingObject(center, half_size, color, name)
{
	m_current_state=Move;
	m_move_speed=100;
}

void PhysicalObject::update(sf::Time elapsed)
{
	static int dir=1;
	switch(m_current_state)
	{
		case Stand:
			m_speed=sf::Vector2f(0,0);
			if(!m_on_ground)
				m_current_state=Fall;
			break;

		case Move:
			if(m_pushes_right_wall || m_pushes_left_wall)
				dir=-dir;
			m_speed=sf::Vector2f(dir*m_move_speed,0);
			if(!m_on_ground)
				m_current_state=Fall;
			break;

		case Fall:
			if(m_on_ground)
			{
				m_speed.y=0;
				dir=-dir;
				m_current_state=Move;
			}
			else
			{
				m_speed.y+=GRAVITY*elapsed.asSeconds();
				m_speed.y=std::min(m_speed.y, (float)MAX_FALLING_SPEED);
			}
			break;

		default:
			break;
	}

	this->updatePhysics(elapsed);
}
