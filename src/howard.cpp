#include "../include/howard.hpp"

Howard::Howard(sf::Vector2f center, string name, bool isKinematic) : 
	MovingObject(center, sf::Vector2f(100,155), sf::Vector2f(50,155), sf::Color(255,255,255), name, isKinematic),
	m_anim(3,3,8,sf::Vector2i(200,310),sf::seconds(0.4412),"howard_sheet",&m_sprite)
{
	m_current_state=Stand;

	dir=-1;
	m_move_speed=30;

	if(!m_music.openFromFile("rsc/sounds/howard.waw"))
		exit(-1);
	m_music.setLoop(true);
	m_music.play();
}

void Howard::update(sf::Time elapsed)
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
				dir=-1;
			else if(m_pushes_left_tile)
				dir=1;
			m_speed=sf::Vector2f(dir*m_move_speed,0);

			if(!m_pushes_bottom_tile)
				m_current_state=Fall;
			break;

		case Fall:
			if(m_pushes_bottom_tile)
			{
				m_speed.y=0;	
				m_current_state=Stand;
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

	this->m_anim.update(elapsed);
}
