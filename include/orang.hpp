#ifndef DEF_ORANG
#define DEF_ORANG

#include "movingObject.hpp"

class Orang : public MovingObject
{
public:
	Orang(sf::Vector2f center, string name, bool is_kinematic);

	enum ObjectState{Stand, Walk, Fall};

	void update(sf::Time elapsed);

private:
	ObjectState m_current_state;
	float m_move_speed;
	int dir;
	sf::SoundBuffer m_buffer;
	sf::Sound m_sound;
};

#endif
