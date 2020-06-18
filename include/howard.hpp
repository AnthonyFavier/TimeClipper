#ifndef DEF_HOWARD
#define DEF_HOWARD

#include "movingObject.hpp"
#include "animationHandler.hpp"

class Howard : public MovingObject
{
public:
	Howard(sf::Vector2f center, sf::Vector2f half_size, string name, bool is_kinematic);

	enum ObjectState{Stand, Walk, Fall};

	void update(sf::Time elapsed);

private:
	ObjectState m_current_state;
	float m_move_speed;
	int dir;
	AnimationHandler m_anim;
};

#endif
