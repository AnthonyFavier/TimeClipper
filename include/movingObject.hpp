#ifndef DEF_MOVINGOBJECT
#define DEF_MOVINGOBJECT

#include <SFML/Graphics.hpp>
#include "constants.hpp"
#include "hitbox.hpp"
#include "map.hpp"

sf::Vector2f interpolate(sf::Vector2f A, sf::Vector2f B, float t);

class MovingObject: public sf::Drawable, public sf::Transformable
{
public:
	MovingObject(sf::Vector2f center, sf::Vector2f hafl_size, sf::Color color);

	void updatePhysics(sf::Time elapsed);

	void flipSpriteRight();
	void flipSpriteLeft();

	bool hasGround(float* ground_y);
	bool checkGround(sf::Vector2f bottom_left, sf::Vector2f bottom_right, float* ground_y);
	
	bool hasCeiling(float* ceiling_y);
	bool checkCeiling(sf::Vector2f top_left, sf::Vector2f top_right, float* ceiling_y);

	bool hasRightWall(float* right_wall_x);
	bool checkRightWall(sf::Vector2f right_top, sf::Vector2f right_bottom, float* right_wall_x);

	bool hasLeftWall(float* left_wall_x);
	bool checkLeftWall(sf::Vector2f left_top, sf::Vector2f left_bottom, float* left_wall_x);

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

	bool m_on_drop_tile;
	bool m_old_on_drop_tile;

	bool m_at_ceiling;
	bool m_old_at_ceiling;

	sf::Texture m_texture;
	sf::Sprite m_sprite;

private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

#endif
