#include "../include/character.hpp"
#include "../include/logManager.hpp"

extern LogManager logM;

Character::Character(sf::Vector2f center, bool* inputs) : 
	MovingObject(center, sf::Vector2f(14,25), sf::Vector2f(14,24.5), sf::Color(255,255,255), "bob", false)
{
	m_current_state=Stand;
	m_jump_speed=CHAR_JUMP_SPEED;
	m_jump_count=0;
	m_walk_speed=CHAR_WALK_SPEED;
	m_inputs=inputs;	
	for(int i=0; i<NB_KEY_CHARACTER; i++)
		m_old_inputs[i]=false;

	m_texture.loadFromFile("rsc/sprites/newboi.png");
	m_sprite.setTexture(m_texture);
}

bool Character::released(KeyInputChar key)
{
	return (!m_inputs[(int)key] && m_old_inputs[(int)key]);
}

bool Character::keyState(KeyInputChar key)
{
	return (m_inputs[(int)key]);
}

bool Character::pressed(KeyInputChar key)
{
	return (m_inputs[(int)key] && !m_old_inputs[(int)key]);
}

void Character::update(sf::Time elapsed)
{
	/*if(pressed(SizeUp))
		this->sizeUp();
	if(pressed(SizeDown))
		this->sizeDown();*/

	switch(m_current_state)
	{
		case Stand:
			m_speed=sf::Vector2f(0,0);

			if(!m_pushes_bottom_tile)
				m_current_state=Jump;
			else if(pressed(GoJump))
			{
				m_speed.y=m_jump_speed;
				m_current_state=Jump;
			}
			else if(keyState(GoRight) != keyState(GoLeft))
				m_current_state=Walk;
			if(keyState(GoDown))
			{
				if(m_on_drop_tile)
					this->move(sf::Vector2f(0,DROP_TILE_THRESHOLD));
			}
			break;
		
		case Walk:
			if(keyState(GoRight) == keyState(GoLeft))
			{
				m_current_state=Stand;
				m_speed=sf::Vector2f(0,0);
			}
			else if(keyState(GoRight))
			{
				this->flipSpriteRight();
				m_speed.x=m_walk_speed;
			}
			else if(keyState(GoLeft))
			{
				this->flipSpriteLeft();
				m_speed.x=-m_walk_speed;
			}
			
			if(keyState(GoDown))
			{
				if(m_on_drop_tile)
					this->move(sf::Vector2f(0,DROP_TILE_THRESHOLD));
			}
			else if(pressed(GoJump))
			{
				m_speed.y=m_jump_speed;
				m_current_state=Jump;
			}
			else if(!m_pushes_bottom_tile)
			{
				m_current_state=Jump;
				m_jump_count=CHAR_JUMP_AMOUNT;
			}

			break;
		
		case Jump:
			if(m_pushes_bottom_tile)
			{
				m_speed.y=0;
				m_jump_count=0;
				if(keyState(GoRight) == keyState(GoLeft))
					m_current_state=Stand;
				else
					m_current_state=Walk;
				break;
			}
			else
			{
				m_speed.y+=GRAVITY*elapsed.asSeconds();
				m_speed.y=std::min(m_speed.y, (float)MAX_FALLING_SPEED);
			}

			if(keyState(GoRight) == keyState(GoLeft))
				m_speed.x=0;
			else if(keyState(GoRight))
			{
				this->flipSpriteRight();
				m_speed.x=m_walk_speed;
			}
			else if(keyState(GoLeft))
			{
				this->flipSpriteLeft();
				m_speed.x=-m_walk_speed;
			}
			
			if(!keyState(GoJump))
				m_jump_count=CHAR_JUMP_AMOUNT;
			else
			{
				if(m_jump_count<CHAR_JUMP_AMOUNT)
					m_speed.y=m_jump_speed;
				m_jump_count+=elapsed.asSeconds();
			}
			break;

		default:
			break;
	}

	this->updateOldInputs();
}

void Character::updateOldInputs()
{
	for(int i=0; i<NB_KEY_CHARACTER; i++)
		m_old_inputs[i]=m_inputs[i];
}

void Character::sizeDown()
{
	m_half_size.x-=5;
	m_half_size.y-=5;

	if(m_half_size.x <= 0 || m_half_size.y <= 0)
	{
		m_half_size.x=5;
		m_half_size.y=5;
	}

	/*m_vertices[0]=sf::Vector2f(-m_half_size.x, -m_half_size.y);
	  m_vertices[1]=sf::Vector2f(m_half_size.x, -m_half_size.y);
	  m_vertices[2]=sf::Vector2f(m_half_size.x, m_half_size.y);
	  m_vertices[3]=sf::Vector2f(-m_half_size.x, m_half_size.y);*/

	m_hitbox.setHalfSize(m_half_size);
}

void Character::sizeUp()
{
	m_half_size.x+=5;
	m_half_size.y+=5;

	/*m_vertices[0]=sf::Vector2f(-m_half_size.x, -m_half_size.y);
	  m_vertices[1]=sf::Vector2f(m_half_size.x, -m_half_size.y);
	  m_vertices[2]=sf::Vector2f(m_half_size.x, m_half_size.y);
	  m_vertices[3]=sf::Vector2f(-m_half_size.x, m_half_size.y);*/

	m_hitbox.setHalfSize(m_half_size);
}

void Character::debug()
{
	cout << endl;
	cout << "position x=" << this->getPosition().x << " y=" << this->getPosition().y << endl;
	cout << "speed x=" << m_speed.x << " y=" << m_speed.y << endl;
	cout << "m_half_size x= " << m_half_size.x << " y=" << m_half_size.y << endl;
	cout << "current_state (Stand, Walk, Jump, GrabEdge)=" << m_current_state << endl;
	cout << "inputs ";
	for(int i=0; i<NB_KEY_CHARACTER; i++)
		cout << i << "-" << m_inputs[i] << " ";
	cout << endl;
	cout << "m_jump_count=" << m_jump_count << endl;
	cout << "Pushes (o-t-a):" << endl;
	cout << "\t right\t" << m_pushes_right_obj << "-" << m_pushes_right_tile << "-" << m_pushes_right << endl;
	cout << "\t left\t" << m_pushes_left_obj << "-" << m_pushes_left_tile << "-" << m_pushes_left << endl;
	cout << "\t top\t" << m_pushes_top_obj << "-" << m_pushes_top_tile << "-" << m_pushes_top << endl;
	cout << "\t bottom\t" << m_pushes_bottom_obj << "-" << m_pushes_bottom_tile << "-" << m_pushes_bottom << endl;
	cout << "Collision : ";
	for(unsigned int i=0; i<m_all_colliding_objects.size(); i++)
		cout << m_all_colliding_objects[i].m_other->getName() << " ";
	cout << endl;
}

void Character::debug2()
{
	logM << endl;
	logM << "position x=" << this->getPosition().x << " y=" << this->getPosition().y << endl;
	logM << "speed x=" << m_speed.x << " y=" << m_speed.y << endl;
	logM << "m_half_size x= " << m_half_size.x << " y=" << m_half_size.y << endl;
	logM << "current_state (Stand, Walk, Jump, GrabEdge)=" << m_current_state << endl;
	logM << "inputs ";
	for(int i=0; i<NB_KEY_CHARACTER; i++)
		logM << i << "-" << m_inputs[i] << " ";
	logM << endl;
	logM << "m_jump_count=" << m_jump_count << endl;
	logM << "Pushes (o-t-a):" << endl;
	logM << "\t right\t" << m_pushes_right_obj << "-" << m_pushes_right_tile << "-" << m_pushes_right << endl;
	logM << "\t left\t" << m_pushes_left_obj << "-" << m_pushes_left_tile << "-" << m_pushes_left << endl;
	logM << "\t top\t" << m_pushes_top_obj << "-" << m_pushes_top_tile << "-" << m_pushes_top << endl;
	logM << "\t bottom\t" << m_pushes_bottom_obj << "-" << m_pushes_bottom_tile << "-" << m_pushes_bottom << endl;
	logM << "Collision : ";
	for(unsigned int i=0; i<m_all_colliding_objects.size(); i++)
		logM << m_all_colliding_objects[i].m_other->getName() << " ";
	logM << endl;
}
