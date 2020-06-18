#ifndef DEF_PHYOBJ
#define DEF_PHYOBJ

#include "movingObject.hpp"

class PhysicalObject : public MovingObject
{
public:
	PhysicalObject(sf::Vector2f center, sf::Vector2f half_size, sf::Color color, string name, bool is_kinematic);

	enum ObjectState{Stand, MoveRight, MoveLeft, Fall};

	void update(sf::Time elapsed);

private:
	ObjectState m_current_state;
	float m_move_speed;
	int dir;
	sf::SoundBuffer m_buffer;
	sf::Sound m_sound;
};

#endif
