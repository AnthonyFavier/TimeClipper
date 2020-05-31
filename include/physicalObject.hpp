#ifndef DEF_PHYOBJ
#define DEF_PHYOBJ

#include "movingObject.hpp"

class PhysicalObject : public MovingObject
{
public:
	PhysicalObject(sf::Vector2f center, sf::Vector2f half_size, sf::Color color, string name);

	enum ObjectState{Stand, Move, Fall};

	void update(sf::Time elapsed);

private:
	ObjectState m_current_state;
	float m_move_speed;
};

#endif
