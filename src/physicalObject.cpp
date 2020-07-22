#include "../include/physicalObject.hpp"

PhysicalObject::PhysicalObject(sf::Vector2f center, sf::Vector2f half_size, sf::Color color, string name, bool isKinematic) : 
	MovingObject(center, half_size, half_size, color, name, isKinematic)
{
	m_current_state=Stand;
	m_move_speed=100;

	dir=-1;
}

void PhysicalObject::update(sf::Time elapsed)
{
	switch(m_current_state)
	{
		case Stand:
			m_speed=sf::Vector2f(0,0);
			if(!m_pushes_bottom_tile)
				m_current_state=Fall;
			break;

		case Walk:
			if(m_pushes_right)
			{
				dir=-1;
				this->flipSpriteLeft();
			}
			else if(m_pushes_left)
			{
				dir=1;
				this->flipSpriteRight();
			}
			m_speed=sf::Vector2f(dir*m_move_speed,0);

			if(!m_pushes_bottom_tile)
				m_current_state=Fall;
			break;

		case Fall:
			if(m_pushes_bottom_tile)
			{
				m_speed.y=0;	

				if(dir==1)
					this->flipSpriteLeft();
				else if(dir==-1)
					this->flipSpriteRight();
				dir=-dir;
				m_current_state=Walk;
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
}
