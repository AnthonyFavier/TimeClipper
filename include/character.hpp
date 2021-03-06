#ifndef DEF_CHARACTER
#define DEF_CHARACTER

#include "movingObject.hpp"
#include <algorithm>

#include <iostream>

using namespace std;

class Character : public MovingObject
{
public:
	Character(sf::Vector2f center, bool* inputs);
	
	bool released(KeyInputChar key);
	bool keyState(KeyInputChar key);
	bool pressed(KeyInputChar key);
	
	enum CharacterState{Stand, Walk, Jump, GrabEdge};
	
	void update(sf::Time elapsed);
	void updateOldInputs();

	void sizeUp();
	void sizeDown();

	void debug();
	void debug2();
	
private:
	bool* m_inputs;
	bool m_old_inputs[NB_KEY_CHARACTER];
	CharacterState m_current_state;
	float m_jump_speed;
	float m_walk_speed;
	float m_jump_count;
};

#endif

