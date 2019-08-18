#ifndef DEF_HitBox
#define DEF_HitBox

#include <SFML/Graphics.hpp>
#include <cmath>

class Hitbox
{
public:
	Hitbox(sf::Vector2f center, sf::Vector2f halfSize);

	bool overlaps(Hitbox other);

	sf::Vector2f m_center;
	sf::Vector2f getHalfSize();
	
private:
	sf::Vector2f m_half_size;
};

#endif
