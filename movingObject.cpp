#include "movingObject.hpp"
#include <iostream>

using namespace std;

extern Map map;

sf::Vector2f interpolate(sf::Vector2f A, sf::Vector2f B, float t)
{
	sf::Vector2f I(B.x-A.x,B.y-A.y);

	return A + I*t;
}

sf::Vector2f roundVector(sf::Vector2f v)
{
	return sf::Vector2f(std::round(v.x), std::round(v.y));
}

/////////////////////////////

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
	m_on_drop_tile=false;
	m_old_on_drop_tile=false;
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
	cout << "m_speed x=" << m_speed.x << " y=" << m_speed.y << endl;
	cout << "m_position x=" << this->getPosition().x << " y=" << this->getPosition().y << endl;

	float ground_y=0, ceiling_y=0, right_wall_x=0, left_wall_x=0;
	
	// LEFT
	if(m_speed.x<=0
	&& this->hasLeftWall(&left_wall_x))
	{
		if(m_old_position.x - m_hitbox.getHalfSize().x + m_hitbox_offset.x >= left_wall_x)
		{
			cout << "LEFT" << endl;
			this->setPosition(left_wall_x+m_hitbox.getHalfSize().x-m_hitbox_offset.x, this->getPosition().y);
			m_pushes_left_wall=true;
		}
		m_speed.x=std::max(m_speed.x,0.f);
	}
	else
		m_pushes_left_wall=false;

	// RIGHT
	if(m_speed.x>=0
	&& this->hasRightWall(&right_wall_x))
	{
		if(m_old_position.x + m_hitbox.getHalfSize().x + m_hitbox_offset.x <= right_wall_x)
		{
			cout << "RIGHT" << endl;
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
		cout << "BOTTOM" << endl;
		this->setPosition(this->getPosition().x, ground_y - m_hitbox.getHalfSize().y - m_hitbox_offset.y);
		m_speed.y=0;
		m_on_ground=true;
	}
	else
		m_on_ground=false;
	
	// TOP
	if(m_speed.y<=0	&& this->hasCeiling(&ceiling_y))
	{
		cout << "TOP" << endl;
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
		cout << "=> High speed bot <=" << endl;
		sf::Vector2f old_center=m_old_position + m_hitbox_offset;
		sf::Vector2f old_bottom_left = roundVector(sf::Vector2f(old_center.x - m_hitbox.getHalfSize().x, 
							    old_center.y + m_hitbox.getHalfSize().y + 1));
		sf::Vector2f old_bottom_right = roundVector(sf::Vector2f(old_bottom_left.x + 2*m_hitbox.getHalfSize().x - 1, old_bottom_left.y));

		sf::Vector2f center=this->getPosition() + m_hitbox_offset;
		sf::Vector2f new_bottom_left = roundVector(sf::Vector2f(center.x - m_hitbox.getHalfSize().x, 
							    center.y + m_hitbox.getHalfSize().y + 1));
		sf::Vector2f new_bottom_right = roundVector(sf::Vector2f(new_bottom_left.x + 2*m_hitbox.getHalfSize().x - 1, new_bottom_left.y));

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

		sf::Vector2f bottom_left = roundVector(sf::Vector2f(center.x - m_hitbox.getHalfSize().x, 
							center.y + m_hitbox.getHalfSize().y + 1));
		sf::Vector2f bottom_right = roundVector(sf::Vector2f(bottom_left.x + 2*m_hitbox.getHalfSize().x - 1, bottom_left.y));

		cout << "bottom_left x=" << bottom_left.x << " y=" << bottom_left.y << endl;
		cout << "bottom_right x=" << bottom_right.x << " y=" << bottom_right.y << endl;

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
		cout << "ground_tile_coord x=" << tile_coord.x << " y=" << tile_coord.y << endl;

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
		cout << "=> High speed top <=" << endl;
		sf::Vector2f old_center=m_old_position + m_hitbox_offset;
		sf::Vector2f old_top_left = roundVector(sf::Vector2f(old_center.x - m_hitbox.getHalfSize().x + 1, 
							    old_center.y - m_hitbox.getHalfSize().y - 1));
		sf::Vector2f old_top_right = roundVector(sf::Vector2f(old_top_left.x + 2*m_hitbox.getHalfSize().x - 2, old_top_left.y));

		sf::Vector2f center=this->getPosition() + m_hitbox_offset;
		sf::Vector2f new_top_left = roundVector(sf::Vector2f(center.x - m_hitbox.getHalfSize().x + 1, 
							 center.y - m_hitbox.getHalfSize().y - 1));
		sf::Vector2f new_top_right = roundVector(sf::Vector2f(new_top_left.x + 2*m_hitbox.getHalfSize().x - 2, new_top_left.y));

		int end_y=::map.getMapTileYAtPoint(new_top_left.y);
		int beg_y=std::max(::map.getMapTileYAtPoint(old_top_left.y),end_y);
		int dist = std::max(abs(end_y-beg_y),1);

		int tile_index_x;
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

		sf::Vector2f top_left = roundVector(sf::Vector2f(center.x - m_hitbox.getHalfSize().x + 1, 
						     center.y - m_hitbox.getHalfSize().y - 1));
		sf::Vector2f top_right = roundVector(sf::Vector2f(top_left.x + 2*m_hitbox.getHalfSize().x - 2, top_left.y));

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
		cout << "ceiling_tile_coord x=" << tile_coord.x << " y=" << tile_coord.y << endl;

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
		cout << "=> High speed right <=" << endl;
		sf::Vector2f old_center=m_old_position + m_hitbox_offset;
		sf::Vector2f old_right_top = roundVector(sf::Vector2f(old_center.x + m_hitbox.getHalfSize().x + 1, 
							  old_center.y - m_hitbox.getHalfSize().y));
		sf::Vector2f old_right_bottom = roundVector(sf::Vector2f(old_right_top.x, old_right_top.y + 2*m_hitbox.getHalfSize().y-1));

		sf::Vector2f center=this->getPosition() + m_hitbox_offset;
		sf::Vector2f new_right_top = roundVector(sf::Vector2f(center.x + m_hitbox.getHalfSize().x + 1,
							  center.y - m_hitbox.getHalfSize().y));
		sf::Vector2f new_right_bottom = roundVector(sf::Vector2f(new_right_top.x, new_right_top.y + 2*m_hitbox.getHalfSize().y-1));

		int end_x=::map.getMapTileXAtPoint(new_right_top.x);
		int beg_x=std::min(::map.getMapTileXAtPoint(old_right_top.x),end_x);
		int dist = std::max(abs(end_x-beg_x),1);

		int tile_index_y;
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

		sf::Vector2f right_top = roundVector(sf::Vector2f(center.x + m_hitbox.getHalfSize().x + 1, 
						      center.y - m_hitbox.getHalfSize().y));
		sf::Vector2f right_bottom = roundVector(sf::Vector2f(right_top.x, right_top.y + 2*m_hitbox.getHalfSize().y-1));

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
		cout << "right_tile_coord x=" << tile_coord.x << " y=" << tile_coord.y << endl;

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
		cout << "=> High speed left <=" << endl;
		sf::Vector2f old_center=m_old_position + m_hitbox_offset;
		sf::Vector2f old_left_top = roundVector(sf::Vector2f(old_center.x - m_hitbox.getHalfSize().x - 1, 
							 old_center.y - m_hitbox.getHalfSize().y));
		sf::Vector2f old_left_bottom = roundVector(sf::Vector2f(old_left_top.x, old_left_top.y + 2*m_hitbox.getHalfSize().y-1));

		sf::Vector2f center=this->getPosition() + m_hitbox_offset;
		sf::Vector2f new_left_top = roundVector(sf::Vector2f(center.x - m_hitbox.getHalfSize().x - 1,
							 center.y - m_hitbox.getHalfSize().y));
		sf::Vector2f new_left_bottom = roundVector(sf::Vector2f(new_left_top.x, new_left_top.y + 2*m_hitbox.getHalfSize().y-1));

		int end_x=::map.getMapTileXAtPoint(new_left_top.x);
		int beg_x=std::max(::map.getMapTileXAtPoint(old_left_top.x),end_x);
		int dist = std::max(abs(end_x-beg_x),1);

		int tile_index_y;
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

		sf::Vector2f left_top = roundVector(sf::Vector2f(center.x - m_hitbox.getHalfSize().x - 1, 
						     center.y - m_hitbox.getHalfSize().y));
		sf::Vector2f left_bottom = roundVector(sf::Vector2f(left_top.x, left_top.y + 2*m_hitbox.getHalfSize().y-1));

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
		cout << "left_tile_coord x=" << tile_coord.x << " y=" << tile_coord.y << endl;

		*left_wall_x = (float) (tile_coord.x+1)*TILE_SIZE_PIXEL + ::map.getPosition().x;

		if(!::map.canGoThroughLeftTile(tile_coord))
			return true;

		if(checked_tile.y>=left_bottom.y)
			break;
	}

	return false;
}


