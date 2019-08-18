#include "movingObject.hpp"

extern Map map;

MovingObject::MovingObject(sf::Vector2f center, sf::Vector2f half_size, sf::Color color) : m_vertices(sf::Quads,4), m_hitbox(center, half_size)
{
	m_half_size=half_size;

	this->setPosition(center);

	m_vertices[0]=sf::Vector2f(-m_half_size.x, -m_half_size.y);
	m_vertices[1]=sf::Vector2f(m_half_size.x, -m_half_size.y);
	m_vertices[2]=sf::Vector2f(m_half_size.x, m_half_size.y);
	m_vertices[3]=sf::Vector2f(-m_half_size.x, m_half_size.y);

	m_vertices[0].color =color;
	m_vertices[1].color =color;
	m_vertices[2].color =color;
	m_vertices[3].color =color;

	m_old_position=this->getPosition();
	m_speed=sf::Vector2f(0,0);
	m_old_speed=m_speed;

	m_hitbox_offset=sf::Vector2f(0,0);

	m_pushes_right_wall=false;
	m_old_right_wall=false;
	m_pushes_left_wall=false;
	m_old_left_wall=false;
	m_on_ground=true;
	m_old_on_ground=true;
	m_at_ceiling=false;
	m_old_at_ceiling=false;
}

void MovingObject::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	states.texture = &m_texture;
	target.draw(m_vertices, states);
}

void MovingObject::updatePhysics(sf::Time elapsed)
{
	m_old_position=this->getPosition();
	m_old_speed=m_speed;

	m_old_on_ground=m_on_ground;
	m_old_right_wall=m_pushes_right_wall;
	m_old_left_wall=m_pushes_left_wall;
	m_old_at_ceiling=m_at_ceiling;

	this->move(m_speed*elapsed.asSeconds());

	/*if(this->getPosition().y+m_half_size.y>=WINDOW_HEIGHT)
	{
		this->setPosition(sf::Vector2f(this->getPosition().x,WINDOW_HEIGHT-m_half_size.y));
		m_on_ground=true;
	}
	else
		m_on_ground=false;*/

	float ground_y=0;
	bool hasGround = this->hasGround(&ground_y);
	if(m_speed.y>=0 && hasGround)
	{
		this->setPosition(this->getPosition().x,ground_y - m_hitbox.getHalfSize().y - m_hitbox_offset.y);
		m_speed.y=0;
		m_on_ground=true;
	}
	else
		m_on_ground=false;

	m_hitbox.m_center=this->getPosition()+m_hitbox_offset;
}

bool MovingObject::hasGround(float* ground_y)
{
	sf::Vector2f bottom_left = sf::Vector2f(m_hitbox.m_center.x + m_hitbox_offset.x - m_hitbox.getHalfSize().x + 1, 
						m_hitbox.m_center.y + m_hitbox_offset.y + m_hitbox.getHalfSize().y + 1);
	sf::Vector2f bottom_right = sf::Vector2f(bottom_left.x + 2*m_hitbox.getHalfSize().x - 2, bottom_left.y);

	sf::Vector2i tile_coord;

	for(sf::Vector2f checkedTile = bottom_left; ; checkedTile.x += TILE_SIZE_PIXEL)
	{
		checkedTile.x=std::min(checkedTile.x,bottom_right.x);

		tile_coord = ::map.getMapTileAtPoint(checkedTile);

		*ground_y = (float) tile_coord.y*TILE_SIZE_PIXEL + ::map.getPosition().y;

		if(!::map.canGoThroughDownTile(tile_coord))
			return true;

		if(checkedTile.x>=bottom_right.x)
			break;
	}

	return false;
}
