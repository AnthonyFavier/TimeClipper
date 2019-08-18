#ifndef DEF_MOVINGOBJECT
#define DEF_MOVINGOBJECT

#include <SFML/Graphics.hpp>
#include "constants.hpp"
#include "hitbox.hpp"
#include "map.hpp"

class MovingObject: public sf::Drawable, public sf::Transformable
{
public:
	MovingObject(sf::Vector2f center, sf::Vector2f hafl_size, sf::Color color);

	void updatePhysics(sf::Time elapsed);

	bool hasGround(float* ground_y);

protected:
	sf::Vector2f m_old_position;
	sf::Vector2f m_speed;
	sf::Vector2f m_old_speed;

	sf::Vector2f m_half_size;

	Hitbox m_hitbox;
	sf::Vector2f m_hitbox_offset;

	bool m_pushes_right_wall;
	bool m_old_right_wall;

	bool m_pushes_left_wall;
	bool m_old_left_wall;

	bool m_on_ground;
	bool m_old_on_ground;

	bool m_at_ceiling;
	bool m_old_at_ceiling;

private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	sf::VertexArray m_vertices;
	sf::Texture m_texture;
};

#endif
