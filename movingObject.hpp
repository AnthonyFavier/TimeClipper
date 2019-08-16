#include <SFML/Graphics.hpp>
#include "AABB.hpp"

class MovingObject: public sf::Drawable, public sf::Transformable
{
public:
	MovingObject();

private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	sf::VertexArray m_vertices;
	sf::Texture m_texture;
};
