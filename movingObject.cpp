#include "movingObject.hpp"
#include <iostream>

using namespace std;

extern Map map;

sf::Vector2f interpolate(sf::Vector2f A, sf::Vector2f B, float t)
{
	sf::Vector2f I(B.x-A.x,B.y-A.y);

	return A + I*t;
}

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
	if(m_speed.x/FPS>0.8*TILE_SIZE_PIXEL || m_speed.y/FPS>0.8*TILE_SIZE_PIXEL) // 0.8 is a security coefficient
	{
		sf::Vector2f center=this->getPosition() + m_hitbox_offset;
		sf::Vector2f old_center=m_old_position + m_hitbox_offset;

		sf::Vector2f old_bottom_left = sf::Vector2f(old_center.x - m_hitbox.getHalfSize().x + 1, 
							    old_center.y + m_hitbox.getHalfSize().y + 1);
		sf::Vector2f old_bottom_right = sf::Vector2f(old_bottom_left.x + 2*m_hitbox.getHalfSize().x - 2, old_bottom_left.y);
		sf::Vector2f new_bottom_left = sf::Vector2f(center.x - m_hitbox.getHalfSize().x + 1, 
							    center.y + m_hitbox.getHalfSize().y + 1);
		sf::Vector2f new_bottom_right = sf::Vector2f(new_bottom_left.x + 2*m_hitbox.getHalfSize().x - 2, new_bottom_left.y);

		int end_y=::map.getMapTileYAtPoint(new_bottom_left.y);
		int beg_y=::std::min(::map.getMapTileYAtPoint(old_bottom_left.y)-1,end_y);
		int dist = std::max(abs(end_y-beg_y),1);

		int tile_index_x;
		sf::Vector2f bottom_left;
		sf::Vector2f bottom_right;

		for(int tile_index_y=beg_y; tile_index_y<=end_y; tile_index_y++)
		{
			bottom_left = interpolate(new_bottom_left,old_bottom_left,(float)abs(end_y-tile_index_y)/dist);
			bottom_right = sf::Vector2f(bottom_left.x + 2*m_hitbox.getHalfSize().x - 2, bottom_left.y);

			sf::Vector2i tile_coord;
			for(sf::Vector2f checkedTile = bottom_left; ; checkedTile.x += TILE_SIZE_PIXEL)
			{
				checkedTile.x=std::min(checkedTile.x,bottom_right.x);

				tile_coord = ::map.getMapTileAtPoint(checkedTile);
				cout << "tile_coord x=" << tile_coord.x << " y=" << tile_coord.y << endl;

				*ground_y = (float) tile_coord.y*TILE_SIZE_PIXEL + ::map.getPosition().y;

				if(!::map.canGoThroughDownTile(tile_coord))
					return true;

				if(checkedTile.x>=bottom_right.x)
					break;
			}

		}

		return false;
	}

	else
	{
		sf::Vector2f center=this->getPosition() + m_hitbox_offset;

		sf::Vector2f bottom_left = sf::Vector2f(center.x - m_hitbox.getHalfSize().x + 1, 
							center.y + m_hitbox.getHalfSize().y + 1);
		sf::Vector2f bottom_right = sf::Vector2f(bottom_left.x + 2*m_hitbox.getHalfSize().x - 2, bottom_left.y);

		sf::Vector2i tile_coord;
		for(sf::Vector2f checkedTile = bottom_left; ; checkedTile.x += TILE_SIZE_PIXEL)
		{
			checkedTile.x=std::min(checkedTile.x,bottom_right.x);

			tile_coord = ::map.getMapTileAtPoint(checkedTile);
			cout << "tile_coord x=" << tile_coord.x << " y=" << tile_coord.y << endl;

			*ground_y = (float) tile_coord.y*TILE_SIZE_PIXEL + ::map.getPosition().y;

			if(!::map.canGoThroughDownTile(tile_coord))
				return true;

			if(checkedTile.x>=bottom_right.x)
				break;
		}

		return false;
	}
}

