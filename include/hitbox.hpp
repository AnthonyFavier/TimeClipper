#ifndef DEF_HitBox
#define DEF_HitBox

#include <SFML/Graphics.hpp>
#include <cmath>

class Hitbox
{
public:
	Hitbox(sf::Vector2f center, sf::Vector2f halfSize);

	bool overlaps(Hitbox other, sf::Vector2f* overlap);

	sf::Vector2f m_center;
	sf::Vector2f getHalfSize();
	void setHalfSize(sf::Vector2f hs);
	void updateCenter(sf::Vector2f center);

	void draw(sf::RenderWindow* window);
	
private:
	sf::Vector2f m_half_size;
	sf::RectangleShape m_shape;
};

#endif
