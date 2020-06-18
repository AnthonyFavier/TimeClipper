#ifndef DEF_BIGCHUNGUS
#define DEF_BIGCHUNGUS

#include "movingObject.hpp"

class BigChungus : public MovingObject
{
public:
	BigChungus(sf::Vector2f center, sf::Vector2f half_size, string name, bool is_kinematic);

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
