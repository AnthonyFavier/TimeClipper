#include "../include/orang.hpp"

Orang::Orang(sf::Vector2f center, string name, bool isKinematic) : 
	MovingObject(center, sf::Vector2f(25,22), sf::Vector2f(25,22), sf::Color(255,255,255), name, isKinematic)
{
	m_current_state=Stand;

	dir=1;
	this->flipSpriteRight();

	m_texture.loadFromFile("rsc/sprites/orang.png");
	m_sprite.setTexture(m_texture);

	m_move_speed=140;

	if(!m_buffer.loadFromFile("rsc/sounds/yes.waw"))
		exit(-1);
	m_sound.setBuffer(m_buffer);
}

void Orang::update(sf::Time elapsed)
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
			if(m_pushes_left)
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
}
