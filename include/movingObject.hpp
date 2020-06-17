#ifndef DEF_MOVINGOBJECT
#define DEF_MOVINGOBJECT

#include <SFML/Graphics.hpp>
#include "constants.hpp"
#include "hitbox.hpp"
#include <vector>

using namespace std;

class Map;

class CollisionData;

sf::Vector2f interpolate(sf::Vector2f A, sf::Vector2f B, float t);

class MovingObject: public sf::Drawable, public sf::Transformable
{
public:
	MovingObject(sf::Vector2f center, sf::Vector2f half_size, sf::Color color, string name, bool is_kinematic);

	void updatePhysics(sf::Time elapsed);
	void updatePhysicsResponse();
	void updatePhysicsP2();

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

	bool hasCollisionDataFor(MovingObject* other);

	void clearCollision();
	void setAlpha(int a);
	virtual void update(sf::Time elapsed)=0;


	// Getters
	sf::Vector2f getSpeed(){return m_speed;};
	sf::Vector2f getOldPos(){return m_old_position;};
	bool getPushRightWall(){return m_pushes_right_wall;};
	bool getPushLeftWall(){return m_pushes_left_wall;};
	bool getAtCeiling(){return m_at_ceiling;};
	bool getOnGround(){return m_on_ground;};
	bool getOnDropTile(){return m_on_drop_tile;};
	bool getIsKinematic(){return m_is_kinematic;};
	string getName(){return m_name;};

	// Variables
	Hitbox m_hitbox;
	vector<sf::Vector2i> m_areas;
	vector<int> m_ids_in_areas;

	vector<CollisionData> m_all_colliding_objects;

protected:
	sf::Vector2f m_old_position;
	sf::Vector2f m_speed;
	sf::Vector2f m_old_speed;

	sf::Vector2f m_half_size;

	sf::Vector2f m_hitbox_offset;

	sf::Texture m_texture;
	sf::Sprite m_sprite;

	string m_name;

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

	bool m_pushes_right;
	bool m_pushes_left;
	bool m_pushes_bottom;
	bool m_pushes_top;
	bool m_pushed_right;
	bool m_pushed_left;
	bool m_pushed_bottom;
	bool m_pushed_top;

	bool m_pushes_right_obj;
	bool m_pushes_left_obj;
	bool m_pushes_bottom_obj;
	bool m_pushes_top_obj;
	bool m_pushed_right_obj;
	bool m_pushed_left_obj;
	bool m_pushed_bottom_obj;
	bool m_pushed_top_obj;

	bool m_pushes_right_tile;
	bool m_pushes_left_tile;
	bool m_pushes_bottom_tile;
	bool m_pushes_top_tile;
	bool m_pushed_right_tile;
	bool m_pushed_left_tile;
	bool m_pushed_bottom_tile;
	bool m_pushed_top_tile;

	bool m_is_kinematic;

private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

//////////////////////////////

class CollisionData
{
public:
	CollisionData(MovingObject* other, sf::Vector2f overlaps, sf::Vector2f speed1, sf::Vector2f speed2, sf::Vector2f pos1, sf::Vector2f pos2, sf::Vector2f old_pos1, sf::Vector2f old_pos2);
	MovingObject* m_other;
	sf::Vector2f m_overlaps;
	sf::Vector2f m_speed1, m_speed2;
	sf::Vector2f m_pos1, m_pos2, m_old_pos1, m_old_pos2;
};


#endif
