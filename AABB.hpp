#ifndef DEF_AABB
#define DEF_AABB

#include <SFML/Graphics.hpp>
#include <cmath>

class AABB
{
public:
	AABB(sf::Vector2f center, sf::Vector2f halfSize);

	bool overlaps(AABB other);

	sf::Vector2f m_center;
	sf::Vector2f getHalfSize();
	
private:
	sf::Vector2f m_half_size;
};

#endif
