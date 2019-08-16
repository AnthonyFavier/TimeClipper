#ifndef DEF_AABB
#define DEF_AABB

#include <SFML/Graphics.hpp>
#include <cmath>

//class Entity : public sf::Drawable, public sf::Transformable
class AABB
{
public:
	AABB(sf::Vector2f center, sf::Vector2f halfSize);

	bool overlaps(AABB other);

	sf::Vector2f getCenter();
	sf::Vector2f getHalfSize();
	
private:
	//virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	
	//sf::VertexArray m_vertices;
	//sf::Texture m_texture;

	sf::Vector2f m_center;
	sf::Vector2f m_halfSize;
};

#endif
