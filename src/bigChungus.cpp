#include "../include/bigChungus.hpp"

BigChungus::BigChungus(sf::Vector2f center, string name, bool isKinematic) : 
	MovingObject(center, sf::Vector2f(25,49), sf::Vector2f(25,49), sf::Color(255,255,255), name, isKinematic)
{
	m_current_state=Stand;

	dir=1;
	this->flipSpriteRight();

	m_texture.loadFromFile("rsc/sprites/big_chungus.png");
	m_sprite.setTexture(m_texture);

	m_move_speed=100;

	if(!m_buffer.loadFromFile("rsc/sounds/OOF.waw"))
		exit(-1);
	m_sound.setBuffer(m_buffer);
}

void BigChungus::update(sf::Time elapsed)
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
				m_sound.play();
			}	
			else if(m_pushes_left)
			{
				dir=1;
				this->flipSpriteRight();
				m_sound.play();
			}
			m_speed=sf::Vector2f(dir*m_move_speed,0);

			if(!m_pushes_bottom_tile)
				m_current_state=Fall;
			break;

		case Fall:
			if(m_pushes_bottom_tile)
			{
				m_speed.y=0;	
				m_sound.play();

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

	this->updatePhysics(elapsed);
}
