#include "../include/movingObject.hpp"
#include "../include/map.hpp"
#include <iostream>

using namespace std;

extern Map map;

sf::Vector2f interpolate(sf::Vector2f A, sf::Vector2f B, float t)
{
	sf::Vector2f I(B.x-A.x,B.y-A.y);

	return A + I*t;
}

CollisionData::CollisionData(MovingObject* other, sf::Vector2f overlaps, sf::Vector2f speed1, sf::Vector2f speed2, sf::Vector2f pos1, sf::Vector2f pos2, sf::Vector2f old_pos1, sf::Vector2f old_pos2)
{
	m_other=other;
	m_overlaps=overlaps;
	m_speed1=speed1;
	m_speed2=speed2;
	m_pos1=pos1;
	m_pos2=pos2;
	m_old_pos1=old_pos1;
	m_old_pos2=m_old_pos2;
}

/////////////////////////////

MovingObject::MovingObject(sf::Vector2f center, sf::Vector2f half_size, sf::Color color, string name) : m_hitbox(center, half_size)
{
	m_half_size=half_size;
	int W=m_half_size.x*2;
	int H=m_half_size.y*2;

	sf::Uint8* pixels = new sf::Uint8[H*W*4];
	for(int i=0; i<H*W*4; i+=4)
	{
		pixels[i]=sf::Uint8(255);
		pixels[i+1]=sf::Uint8(255);
		pixels[i+2]=sf::Uint8(255);
		pixels[i+3]=sf::Uint8(255);
	}

	m_texture.create(W, H);
	m_texture.update(pixels);
	m_sprite.setTexture(m_texture);
	m_sprite.setColor(color);
	m_sprite.setPosition(-m_half_size);

	m_name=name;

	this->setPosition(center);

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
	m_on_drop_tile=false;
	m_old_on_drop_tile=false;
	m_at_ceiling=false;
	m_old_at_ceiling=false;
}

void MovingObject::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	states.texture = &m_texture;
	target.draw(m_sprite, states);
}

string MovingObject::getName()
{
	return m_name;
}

void MovingObject::flipSpriteRight()
{	
	m_sprite.setScale(-1,1);
	m_sprite.setPosition(m_half_size.x, -m_half_size.y);
}

void MovingObject::flipSpriteLeft()
{
	m_sprite.setScale(1,1);
	m_sprite.setPosition(-m_half_size.x, -m_half_size.y);
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

	float ground_y=0, ceiling_y=0, right_wall_x=0, left_wall_x=0;
	
	// LEFT
	if(m_speed.x<=0 && this->hasLeftWall(&left_wall_x))
	{
		if(m_old_position.x - m_hitbox.getHalfSize().x + m_hitbox_offset.x >= left_wall_x)
		{
			this->setPosition(left_wall_x+m_hitbox.getHalfSize().x-m_hitbox_offset.x, this->getPosition().y);
			m_pushes_left_wall=true;
		}
		m_speed.x=std::max(m_speed.x,0.f);
	}
	else
		m_pushes_left_wall=false;

	// RIGHT
	if(m_speed.x>=0 && this->hasRightWall(&right_wall_x))
	{
		if(m_old_position.x + m_hitbox.getHalfSize().x + m_hitbox_offset.x <= right_wall_x)
		{
			this->setPosition(right_wall_x-m_hitbox.getHalfSize().x-m_hitbox_offset.x, this->getPosition().y);
			m_pushes_right_wall=true;
		}
		m_speed.x=std::min(m_speed.x,0.f);
	}
	else
		m_pushes_right_wall=false;

	// BOTTOM
	if(m_speed.y>=0 && this->hasGround(&ground_y))
	{
		this->setPosition(this->getPosition().x, ground_y - m_hitbox.getHalfSize().y - m_hitbox_offset.y);
		m_speed.y=0;
		m_on_ground=true;
	}
	else
		m_on_ground=false;
	
	// TOP
	if(m_speed.y<=0	&& this->hasCeiling(&ceiling_y))
	{
		this->setPosition(this->getPosition().x, ceiling_y + m_hitbox.getHalfSize().y - m_hitbox_offset.y);
		m_speed.y=0;
		m_at_ceiling=true;
	}
	else
		m_at_ceiling=false;

	m_hitbox.m_center=this->getPosition()+m_hitbox_offset;
}

// BOTTOM
bool MovingObject::hasGround(float* ground_y)
{
	if(m_speed.y/FPS>0.8*TILE_SIZE_PIXEL) // 0.8 is a security coefficient
	{
		sf::Vector2f old_center=m_old_position + m_hitbox_offset;
		sf::Vector2f old_bottom_left = sf::Vector2f(std::floor(old_center.x - m_hitbox.getHalfSize().x),
							    std::ceil(old_center.y + m_hitbox.getHalfSize().y + 1));

		sf::Vector2f center=this->getPosition() + m_hitbox_offset;
		sf::Vector2f new_bottom_left = sf::Vector2f(std::floor(center.x - m_hitbox.getHalfSize().x),
							    std::ceil(center.y + m_hitbox.getHalfSize().y + 1));

		int end_y=::map.getMapTileYAtPoint(new_bottom_left.y);
		int beg_y=std::min(::map.getMapTileYAtPoint(old_bottom_left.y),end_y);
		int dist = std::max(abs(end_y-beg_y),1);

		sf::Vector2f bottom_left;
		sf::Vector2f bottom_right;

		for(int tile_index_y=beg_y; tile_index_y<=end_y; tile_index_y++)
		{
			bottom_left = interpolate(new_bottom_left,old_bottom_left,(float)abs(end_y-tile_index_y)/(dist+1));
			bottom_right = sf::Vector2f(bottom_left.x + 2*m_hitbox.getHalfSize().x - 2, bottom_left.y);

			if(this->checkGround(bottom_left, bottom_right, ground_y))
				return true;
		}

		return false;
	}

	else
	{
		sf::Vector2f center=this->getPosition() + m_hitbox_offset;

		sf::Vector2f bottom_left = sf::Vector2f(std::floor(center.x - m_hitbox.getHalfSize().x),
							std::ceil(center.y + m_hitbox.getHalfSize().y + 1));
		sf::Vector2f bottom_right = sf::Vector2f(std::ceil(bottom_left.x + 2*m_hitbox.getHalfSize().x - 1), std::ceil(bottom_left.y));

		return this->checkGround(bottom_left, bottom_right, ground_y);
	}
}
bool MovingObject::checkGround(sf::Vector2f bottom_left, sf::Vector2f bottom_right, float* ground_y)
{
	sf::Vector2i tile_coord;
	m_on_drop_tile=false;
	for(sf::Vector2f checked_tile = bottom_left; ; checked_tile.x += TILE_SIZE_PIXEL)
	{
		checked_tile.x=std::min(checked_tile.x,bottom_right.x);

		tile_coord = ::map.getMapTileAtPoint(checked_tile);

		*ground_y = (float) tile_coord.y*TILE_SIZE_PIXEL + ::map.getPosition().y;

		if(!::map.canGoThroughDownTile(tile_coord))
		{
			if(!::map.canDropDownThroughTile(tile_coord))
			{
				m_on_drop_tile=false;
				return true;
			}
			else if (::map.canDropDownThroughTile(tile_coord)
				&& abs(checked_tile.y-*ground_y) <= DROP_TILE_THRESHOLD + this->getPosition().y - m_old_position.y)
				m_on_drop_tile=true;
		}

		if(checked_tile.x>=bottom_right.x)
		{
			if(m_on_drop_tile)
				return true;
			break;
		}
	}

	return false;
}

// TOP
bool MovingObject::hasCeiling(float* ceiling_y)
{
	if(-m_speed.y/FPS>0.8*TILE_SIZE_PIXEL) // 0.8 is a security coefficient
	{
		sf::Vector2f old_center=m_old_position + m_hitbox_offset;
		sf::Vector2f old_top_left = sf::Vector2f(std::floor(old_center.x - m_hitbox.getHalfSize().x),
							 std::floor(old_center.y - m_hitbox.getHalfSize().y - 1));

		sf::Vector2f center=this->getPosition() + m_hitbox_offset;
		sf::Vector2f new_top_left = sf::Vector2f(std::floor(center.x - m_hitbox.getHalfSize().x + 1),
							 std::floor(center.y - m_hitbox.getHalfSize().y - 1));

		int end_y=::map.getMapTileYAtPoint(new_top_left.y);
		int beg_y=std::max(::map.getMapTileYAtPoint(old_top_left.y),end_y);
		int dist = std::max(abs(end_y-beg_y),1);

		sf::Vector2f top_left;
		sf::Vector2f top_right;

		for(int tile_index_y=beg_y; tile_index_y>=end_y; tile_index_y--)
		{
			top_left = interpolate(new_top_left,old_top_left,(float)abs(end_y-tile_index_y)/(dist+1));
			top_right = sf::Vector2f(top_left.x + 2*m_hitbox.getHalfSize().x - 2, top_left.y);

			if(this->checkCeiling(top_left, top_right, ceiling_y))
				return true;
		}

		return false;
	}

	else
	{
		sf::Vector2f center=this->getPosition() + m_hitbox_offset;

		sf::Vector2f top_left = sf::Vector2f(std::floor(center.x - m_hitbox.getHalfSize().x),
						     std::floor(center.y - m_hitbox.getHalfSize().y - 1));
		sf::Vector2f top_right = sf::Vector2f(std::ceil(top_left.x + 2*m_hitbox.getHalfSize().x - 1), std::floor(top_left.y));

		return this->checkCeiling(top_left, top_right, ceiling_y);
	}
}
bool MovingObject::checkCeiling(sf::Vector2f top_left, sf::Vector2f top_right, float* ceiling_y)
{
	sf::Vector2i tile_coord;
	for(sf::Vector2f checked_tile = top_left; ; checked_tile.x += TILE_SIZE_PIXEL)
	{
		checked_tile.x=std::min(checked_tile.x,top_right.x);

		tile_coord = ::map.getMapTileAtPoint(checked_tile);

		*ceiling_y = (float) (tile_coord.y+1)*TILE_SIZE_PIXEL + ::map.getPosition().y;

		if(!::map.canGoThroughUpTile(tile_coord))
			return true;

		if(checked_tile.x>=top_right.x)
			break;
	}

	return false;
}

// RIGHT
bool MovingObject::hasRightWall(float* right_wall_x)
{
	if(m_speed.x/FPS>0.8*TILE_SIZE_PIXEL) // 0.8 is a security coefficient
	{
		sf::Vector2f old_center=m_old_position + m_hitbox_offset;
		sf::Vector2f old_right_top = sf::Vector2f(std::ceil(old_center.x + m_hitbox.getHalfSize().x + 1),
							  std::floor(old_center.y - m_hitbox.getHalfSize().y));

		sf::Vector2f center=this->getPosition() + m_hitbox_offset;
		sf::Vector2f new_right_top = sf::Vector2f(std::ceil(center.x + m_hitbox.getHalfSize().x + 1),
							  std::floor(center.y - m_hitbox.getHalfSize().y));

		int end_x=::map.getMapTileXAtPoint(new_right_top.x);
		int beg_x=std::min(::map.getMapTileXAtPoint(old_right_top.x),end_x);
		int dist = std::max(abs(end_x-beg_x),1);

		sf::Vector2f right_top;
		sf::Vector2f right_bottom;

		for(int tile_index_x=beg_x; tile_index_x<=end_x; tile_index_x++)
		{
			right_top = interpolate(new_right_top,old_right_top,(float)abs(end_x-tile_index_x)/(dist+1));
			right_bottom = sf::Vector2f(right_top.x, right_top.y + 2*m_hitbox.getHalfSize().y-1);

			if(this->checkRightWall(right_top, right_bottom, right_wall_x))
				return true;
		}

		return false;
	}

	else
	{
		sf::Vector2f center=this->getPosition() + m_hitbox_offset;

		sf::Vector2f right_top = sf::Vector2f(std::ceil(center.x + m_hitbox.getHalfSize().x + 1),
						      std::floor(center.y - m_hitbox.getHalfSize().y));
		sf::Vector2f right_bottom = sf::Vector2f(std::ceil(right_top.x), std::ceil(right_top.y + 2*m_hitbox.getHalfSize().y-1));

		return this->checkRightWall(right_top, right_bottom, right_wall_x);
	}
}
bool MovingObject::checkRightWall(sf::Vector2f right_top, sf::Vector2f right_bottom, float* right_wall_x)
{
	sf::Vector2i tile_coord;
	for(sf::Vector2f checked_tile = right_top; ; checked_tile.y += TILE_SIZE_PIXEL)
	{
		checked_tile.y=std::min(checked_tile.y,right_bottom.y);

		tile_coord = ::map.getMapTileAtPoint(checked_tile);

		*right_wall_x = (float) tile_coord.x*TILE_SIZE_PIXEL + ::map.getPosition().x;

		if(!::map.canGoThroughRightTile(tile_coord))
			return true;

		if(checked_tile.y>=right_bottom.y)
			break;
	}

	return false;
}

// LEFT
bool MovingObject::hasLeftWall(float* left_wall_x)
{
	if(-m_speed.x/FPS>0.8*TILE_SIZE_PIXEL) // 0.8 is a security coefficient
	{
		sf::Vector2f old_center=m_old_position + m_hitbox_offset;
		sf::Vector2f old_left_top = sf::Vector2f(std::floor(old_center.x - m_hitbox.getHalfSize().x - 1),
							 std::floor(old_center.y - m_hitbox.getHalfSize().y));

		sf::Vector2f center=this->getPosition() + m_hitbox_offset;
		sf::Vector2f new_left_top = sf::Vector2f(std::floor(center.x - m_hitbox.getHalfSize().x - 1),
							 std::floor(center.y - m_hitbox.getHalfSize().y));

		int end_x=::map.getMapTileXAtPoint(new_left_top.x);
		int beg_x=std::max(::map.getMapTileXAtPoint(old_left_top.x),end_x);
		int dist = std::max(abs(end_x-beg_x),1);

		sf::Vector2f left_top;
		sf::Vector2f left_bottom;

		for(int tile_index_x=beg_x; tile_index_x>=end_x; tile_index_x--)
		{
			left_top = interpolate(new_left_top,old_left_top,(float)abs(end_x-tile_index_x)/(dist+1));
			left_bottom = sf::Vector2f(left_top.x, left_top.y + 2*m_hitbox.getHalfSize().y-1);

			if(this->checkLeftWall(left_top, left_bottom, left_wall_x))
				return true;
		}

		return false;
	}

	else
	{
		sf::Vector2f center=this->getPosition() + m_hitbox_offset;

		sf::Vector2f left_top = sf::Vector2f(std::floor(center.x - m_hitbox.getHalfSize().x - 1),
						     std::floor(center.y - m_hitbox.getHalfSize().y));
		sf::Vector2f left_bottom = sf::Vector2f(std::floor(left_top.x), std::ceil(left_top.y + 2*m_hitbox.getHalfSize().y-1));

		return this->checkLeftWall(left_top, left_bottom, left_wall_x);
	}
}
bool MovingObject::checkLeftWall(sf::Vector2f left_top, sf::Vector2f left_bottom, float* left_wall_x)
{
	sf::Vector2i tile_coord;
	for(sf::Vector2f checked_tile = left_top; ; checked_tile.y += TILE_SIZE_PIXEL)
	{
		checked_tile.y=std::min(checked_tile.y,left_bottom.y);

		tile_coord = ::map.getMapTileAtPoint(checked_tile);

		*left_wall_x = (float) (tile_coord.x+1)*TILE_SIZE_PIXEL + ::map.getPosition().x;

		if(!::map.canGoThroughLeftTile(tile_coord))
			return true;

		if(checked_tile.y>=left_bottom.y)
			break;
	}

	return false;
}

sf::Vector2f MovingObject::getSpeed()
{
	return m_speed;
}

sf::Vector2f MovingObject::getOldPos()
{
	return m_old_position;
}

bool MovingObject::hasCollisionDataFor(MovingObject* other)
{
	for(unsigned int i=0; i<m_all_colliding_objects.size(); i++)
	{
		if(m_all_colliding_objects[i].m_other == other)
			return true;
	}

	return false;
}

void MovingObject::clearCollision()
{
	m_all_colliding_objects.clear();
	this->setAlpha(255);
}

void MovingObject::setAlpha(int a)
{
	sf::Color color=m_sprite.getColor();
	color.a=a;
	m_sprite.setColor(color);
}
