#include "../include/physicalObject.hpp"

PhysicalObject::PhysicalObject(sf::Vector2f center, sf::Vector2f half_size, sf::Color color, string name, bool isKinematic) : MovingObject(center, half_size, color, name, isKinematic)
{
	m_current_state=Stand;
	m_move_speed=100;

	dir=1;

	if(name=="orange" || name=="raoult" || name=="big_chungus")
	{
		m_texture.loadFromFile("rsc/"+name+".png");
		m_sprite.setTexture(m_texture);
	}

	if(name=="raoult")
		m_move_speed=60;
	else if(name=="orange")
		m_move_speed=140;

	if(name=="raoult")
	{
		if(!m_buffer.loadFromFile("rsc/scream.waw"))
			exit(-1);
		m_sound.setVolume(30);
	}
	else if(name=="orange")
	{
		if(!m_buffer.loadFromFile("rsc/yes.waw"))
			exit(-1);
	}
	else if(name=="big_chungus")
	{
		if(!m_buffer.loadFromFile("rsc/OOF.waw"))
			exit(-1);
	}
	m_sound.setBuffer(m_buffer);
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

		case MoveRight:
			dir=1;
			this->flipSpriteRight();
			m_speed=sf::Vector2f(dir*m_move_speed,0);
			if(m_pushes_right_tile || (m_pushes_right_obj&&m_name!="raoult"))
			{
				dir=-1;
				this->flipSpriteLeft();
				m_speed=sf::Vector2f(dir*m_move_speed,0);
				m_sound.play();
				m_current_state=MoveLeft;
			}
			if(!m_pushes_bottom_tile)
				m_current_state=Fall;
			break;

		case MoveLeft:
			dir=-1;
			this->flipSpriteLeft();
			m_speed=sf::Vector2f(dir*m_move_speed,0);
			if(m_pushes_left_tile || (m_pushes_left_obj&&m_name!="raoult"))
			{
				dir=1;
				this->flipSpriteRight();
				m_speed=sf::Vector2f(dir*m_move_speed,0);
				m_sound.play();
				m_current_state=MoveRight;
			}
			if(!m_pushes_bottom_tile)
				m_current_state=Fall;
			break;

		case Fall:
			if(m_pushes_bottom_tile)
			{
				m_speed.y=0;	
				m_sound.play();

				if(dir==1)
				{
					m_current_state=MoveLeft;
					this->flipSpriteLeft();
				}
				else
				{
					m_current_state=MoveRight;
					this->flipSpriteRight();
				}
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
