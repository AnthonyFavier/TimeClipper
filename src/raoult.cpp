#include "../include/raoult.hpp"

Raoult::Raoult(sf::Vector2f center, sf::Vector2f half_size, string name, bool isKinematic) : MovingObject(center, half_size, sf::Color(255,255,255), name, isKinematic)
{
	m_current_state=Stand;

	dir=1;
	this->flipSpriteRight();

	m_texture.loadFromFile("rsc/sprites/raoult.png");
	m_sprite.setTexture(m_texture);

	m_move_speed=60;

	if(!m_buffer.loadFromFile("rsc/sounds/scream.waw"))
		exit(-1);
	m_sound.setVolume(30);
	m_sound.setBuffer(m_buffer);
}

void Raoult::update(sf::Time elapsed)
{
	switch(m_current_state)
	{
		case Stand:
			m_speed=sf::Vector2f(0,0);
			if(!m_pushes_bottom_tile)
				m_current_state=Fall;
			break;

		case Walk:
			if(m_pushes_right_tile)
			{
				dir=-1;
				this->flipSpriteLeft();
				m_sound.play();
			}
			else if(m_pushes_left_tile)
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
