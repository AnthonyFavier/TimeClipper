#include "../include/animationHandler.hpp"

AnimationHandler::AnimationHandler(int nb_w, int nb_h, int nb_tot, sf::Vector2i size_sprite, sf::Time duree, string filename, sf::Sprite* sprite)
{
	m_nb_w=nb_w;
	m_nb_h=nb_h;
	m_nb_tot=nb_tot;
	m_size_sprite=size_sprite;
	m_duree_sprite=duree;
	m_sprite=sprite;

	m_sheet.loadFromFile("rsc/sprites/"+filename+".png");
	m_sprite->setTexture(m_sheet);
	m_sprite->setTextureRect(sf::IntRect(sf::Vector2i(0,0), m_size_sprite));
}

void AnimationHandler::update(sf::Time elapsed)
{
	m_acc+=elapsed;

	if(m_acc>m_duree_sprite)
	{
		m_acc-=m_duree_sprite;

		// si on est à la deriere ligne
		if(m_index_sheet.y==m_nb_h -1)
		{
			if(m_index_sheet.x < m_nb_tot - m_nb_w*(m_nb_h - 1) -1)
				m_index_sheet.x++;
			else
			{
				m_index_sheet.x=0;
				m_index_sheet.y=0;
			}
		}
		else
		{
			if(m_index_sheet.x<m_nb_w -1)
				m_index_sheet.x++;
			else
			{
				m_index_sheet.x=0;
				m_index_sheet.y++;
			}
		}

		m_sprite->setTextureRect(sf::IntRect(sf::Vector2i(m_index_sheet.x*m_size_sprite.x,m_index_sheet.y*m_size_sprite.y), m_size_sprite));
	}
}
