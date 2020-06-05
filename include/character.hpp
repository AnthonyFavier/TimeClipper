#ifndef DEF_CHARACTER
#define DEF_CHARACTER

#include "movingObject.hpp"
#include <algorithm>

#include <iostream>

using namespace std;

class Character : public MovingObject
{
public:
	Character(sf::Vector2f center, sf::Vector2f half_size);
	
	bool released(KeyInputChar key);
	bool keyState(KeyInputChar key);
	bool pressed(KeyInputChar key);
	
	enum CharacterState{Stand, Walk, Jump, GrabEdge};
	
	void updateC(sf::Time elapsed, bool inputs[NB_KEY_CHARACTER]);
	void update(sf::Time elapsed){};
	void updateInputs(bool inputs[NB_KEY_CHARACTER]);
	void updateOldInputs();

	void sizeUp();
	void sizeDown();

	void debug();
	
private:
	bool m_inputs[NB_KEY_CHARACTER];
	bool m_old_inputs[NB_KEY_CHARACTER];
	CharacterState m_current_state;
	float m_jump_speed;
	float m_walk_speed;
	int m_jump_count;
};

#endif

