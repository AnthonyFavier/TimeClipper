#ifndef DEF_ANIMATIONHANDLER
#define DEF_ANIMATIONHANDLER

#include <SFML/Graphics.hpp>
#include <iostream>

using namespace std;

class AnimationHandler
{
public:
	AnimationHandler(int nb_w, int nb_h, int nb_tot, sf::Vector2i size_sprite, sf::Time duree, string filename, sf::Sprite* sprite);
	void update(sf::Time elapsed);

private:
	int m_nb_w;
	int m_nb_h;
	int m_nb_tot;
	sf::Vector2i m_size_sprite;
	sf::Time m_duree_sprite;
	sf::Texture m_sheet;
	sf::Sprite* m_sprite;
	sf::Time m_acc;
	sf::Vector2i m_index_sheet;
};

#endif

