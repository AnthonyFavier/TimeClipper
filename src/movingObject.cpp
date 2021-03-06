#include "../include/movingObject.hpp"
#include "../include/map.hpp"
#include "../include/logManager.hpp"
#include <iostream>

using namespace std;

extern Map map;
extern LogManager logM;

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
	m_old_pos2=old_pos2;
}

/////////////////////////////

MovingObject::MovingObject(sf::Vector2f center, sf::Vector2f half_size_sprite, sf::Vector2f half_size_hitbox, sf::Color color, string name, bool is_kinematic) : m_hitbox(center, half_size_hitbox)
{
	m_half_size=half_size_sprite;
	int W=m_half_size.x*2;
	int H=m_half_size.y*2;

	if(name.length()>=2 && name[0]=='M' && name[1]=='O')
	{
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
	}

	m_sprite.setColor(color);
	m_sprite.setPosition(-m_half_size);

	m_is_kinematic=is_kinematic;

	m_name=name;

	this->setPosition(center);

	m_old_position=this->getPosition();
	m_speed=sf::Vector2f(0,0);
	m_old_speed=m_speed;

	m_hitbox_offset=sf::Vector2f(0,0);

	m_on_drop_tile=false;
	m_old_on_drop_tile=false;

	m_pushes_right=false;
	m_pushes_left=false;
	m_pushes_bottom=false;
	m_pushes_top=false;
	m_pushed_right=false;
	m_pushed_left=false;
	m_pushed_bottom=false;
	m_pushed_top=false;

	m_pushes_right_obj=false;
	m_pushes_left_obj=false;
	m_pushes_bottom_obj=false;
	m_pushes_top_obj=false;
	m_pushed_right_obj=false;
	m_pushed_left_obj=false;
	m_pushed_bottom_obj=false;
	m_pushed_top_obj=false;

	m_pushes_right_tile=false;
	m_pushes_left_tile=false;
	m_pushes_bottom_tile=false;
	m_pushes_top_tile=false;
	m_pushed_right_tile=false;
	m_pushed_left_tile=false;
	m_pushed_bottom_tile=false;
	m_pushed_top_tile=false;

	m_can_continue=true;
}

void MovingObject::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	states.texture = &m_texture;
	target.draw(m_sprite, states);
}

void MovingObject::flipSpriteRight()
{	
	m_sprite.setScale(-1,1);
	m_sprite.setPosition(m_half_size.x, -m_half_size.y);
}

void MovingObject::flipSpriteLeft()
{
	m_sprite.setScale(1,1);
	m_sprite.setPosition(-m_half_size);
}

void MovingObject::updatePhysics(sf::Time elapsed)
{
	m_old_position=this->getPosition();
	m_old_speed=m_speed;

	m_pushed_bottom_tile=m_pushes_bottom_tile;
	m_pushed_right_tile=m_pushes_right_tile;
	m_pushed_left_tile=m_pushes_left_tile;
	m_pushed_top_tile=m_pushes_top_tile;

	if(m_pushes_right_tile || (m_pushes_right_obj && !m_is_kinematic))
		m_speed.x = std::min(m_speed.x,0.f);
	if(m_pushes_left_tile || (m_pushes_left_obj && !m_is_kinematic))
		m_speed.x = std::max(m_speed.x,0.f);
	if(m_pushes_top_tile || (m_pushes_top_obj && !m_is_kinematic))
		m_speed.y = std::max(m_speed.y,0.f);
	if(m_pushes_bottom_tile || (m_pushes_bottom_obj && !m_is_kinematic))
		m_speed.y = std::min(m_speed.y,0.f);
	this->move(m_speed*elapsed.asSeconds());
}

bool MovingObject::collisionTiles()
{
	logM << m_name << " - Debut collisiontTiles" << endl;
	sf::Vector2f pos_avant=this->getPosition();

	float ground_y=0, ceiling_y=0, right_wall_x=0, left_wall_x=0;

	// LEFT
	if((m_speed.x<=0 || this->getPosition().x-m_old_position.x<=0)  && this->hasLeftWall(&left_wall_x))
	{
		logM << "LEFT" << endl;
		logM << "left_wall_x=" << left_wall_x << endl;
		logM << "old_p=" << m_old_position.x << " , " << m_old_position.y << endl;
		logM << "old_p.x-hibox.halfSize.X+offset.X=" << m_old_position.x - m_hitbox.getHalfSize().x + m_hitbox_offset.x << endl;
		if(m_old_position.x - m_hitbox.getHalfSize().x + m_hitbox_offset.x >= left_wall_x)
		{
			this->setPosition(left_wall_x+m_hitbox.getHalfSize().x-m_hitbox_offset.x, this->getPosition().y);
			m_pushes_left_tile=true;
			logM << "new pose= " << left_wall_x+m_hitbox.getHalfSize().x-m_hitbox_offset.x << "," << this->getPosition().y << endl;
		}
		m_speed.x=std::max(m_speed.x,0.f);
	}
	else
		m_pushes_left_tile=false;

	// RIGHT
	if((m_speed.x>=0 || this->getPosition().x-m_old_position.x>=0) && this->hasRightWall(&right_wall_x))
	{
		logM << "RIGHT";
		if(m_old_position.x + m_hitbox.getHalfSize().x + m_hitbox_offset.x <= right_wall_x)
		{
			this->setPosition(right_wall_x-m_hitbox.getHalfSize().x-m_hitbox_offset.x, this->getPosition().y);
			logM << "new pose= " << right_wall_x-m_hitbox.getHalfSize().x-m_hitbox_offset.x << "," << this->getPosition().y << endl;
			m_pushes_right_tile=true;
		}
		m_speed.x=std::min(m_speed.x,0.f);
		logM << endl;
	}
	else
		m_pushes_right_tile=false;

	// TOP
	if((m_speed.y<=0 || this->getPosition().y-m_old_position.y<=0) && this->hasCeiling(&ceiling_y))
	{
		logM << "TOP" << endl;
		this->setPosition(this->getPosition().x, ceiling_y + m_hitbox.getHalfSize().y - m_hitbox_offset.y);
		logM << "new pose= " << this->getPosition().x << "," << ceiling_y + m_hitbox.getHalfSize().y - m_hitbox_offset.y << endl;
		m_speed.y=0;
		m_pushes_top_tile=true;
	}
	else
		m_pushes_top_tile=false;

	// BOTTOM
	if((m_speed.y>=0 || this->getPosition().y-m_old_position.y>=0) && this->hasGround(&ground_y))
	{
		logM << "BOT" << endl;
		this->setPosition(this->getPosition().x, ground_y - m_hitbox.getHalfSize().y - m_hitbox_offset.y);
		logM << "new pose= " << this->getPosition().x << "," << ground_y - m_hitbox.getHalfSize().y - m_hitbox_offset.y << endl;
		m_speed.y=0;
		m_pushes_bottom_tile=true;
	}
	else
	{
		m_pushes_bottom_tile=false;
		m_on_drop_tile=false;
	}

	m_hitbox.updateCenter(this->getPosition()+m_hitbox_offset);

	logM << "fin collisionTiles" << endl;

	if(this->getPosition()!=pos_avant)
		return true;
	return false;
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

void MovingObject::updatePhysicsResponse()
{
	m_pushed_right_obj=m_pushes_right_obj;
	m_pushed_left_obj=m_pushes_left_obj;
	m_pushed_bottom_obj=m_pushes_bottom_obj;
	m_pushed_top_obj=m_pushes_top_obj;

	m_pushes_left_obj=false;
	m_pushes_right_obj=false;
	m_pushes_bottom_obj=false;
	m_pushes_top_obj=false;

	m_can_continue=true;

	sf::Vector2f offset_sum(0,0);

	CollisionData* data=NULL;
	sf::Vector2f overlap;
	for(unsigned int i=0; i<m_all_colliding_objects.size(); i++)
	{
		data=&m_all_colliding_objects[i];
		overlap=data->m_overlaps - offset_sum;

		logM << "my name=" << m_name << endl;
		logM << "data:";
		logM << "\tother=" << data->m_other->getName() << endl;
		logM << "\tm_overlaps=" << overlap.x << "," << overlap.y << endl;
		logM << "\tm_speed1=" << data->m_speed1.x << "," << data->m_speed1.y << endl;
		logM << "\tm_speed2=" << data->m_speed2.x << "," << data->m_speed2.y << endl;
		logM << "\tm_pos1=" << data->m_pos1.x << "," << data->m_pos1.y << endl;
		logM << "\tm_pos2=" << data->m_pos2.x << "," << data->m_pos2.y << endl;
		logM << "\tm_old_pos1=" << data->m_old_pos1.x << "," << data->m_old_pos1.y << endl;
		logM << "\tm_old_pos2=" << data->m_old_pos2.x << "," << data->m_old_pos2.y << endl;

		if(overlap.x == 0)
		{
			logM << "\tcas1" << endl;
			if(data->m_other->m_hitbox.m_center.x > m_hitbox.m_center.x)
			{
				m_pushes_right_obj=true;
				if(!m_is_kinematic)
					m_speed.x=std::min(m_speed.x,0.f);
			}
			else
			{
				m_pushes_left_obj=true;
				if(!m_is_kinematic)
					m_speed.x=std::max(m_speed.x,0.f);
			}
		}
		else if(overlap.y == 0)
		{
			logM << "\tcas2" << endl;
			if(data->m_other->m_hitbox.m_center.y > m_hitbox.m_center.y)
			{
				m_pushes_bottom_obj=true;
				if(!m_is_kinematic)
					m_speed.y=std::min(m_speed.y,0.f);
			}
			else
			{
				m_pushes_top_obj=true;
				if(!m_is_kinematic)
					m_speed.y=std::max(m_speed.y,0.f);
			}
		}
		else
		{
			logM << "\tcas3" << endl;
			// Calcul des vitesses //
			sf::Vector2f abs_speed1 = sf::Vector2f(std::abs(data->m_pos1.x - data->m_old_pos1.x),std::abs(data->m_pos1.y - data->m_old_pos1.y));
			sf::Vector2f abs_speed2 = sf::Vector2f(std::abs(data->m_pos2.x - data->m_old_pos2.x),std::abs(data->m_pos2.y - data->m_old_pos2.y));
			sf::Vector2f speed_sum = abs_speed1 + abs_speed2;

			// Cherche ou est other par rapport à this
			bool overlapped_last_frame_x = std::abs(data->m_old_pos1.x - data->m_old_pos2.x) 
				< m_hitbox.getHalfSize().x+data->m_other->m_hitbox.getHalfSize().x;
			bool overlapped_last_frame_y = std::abs(data->m_old_pos1.y - data->m_old_pos2.y) 
				< m_hitbox.getHalfSize().y+data->m_other->m_hitbox.getHalfSize().y;
			bool left(false),right(false),top(false),bottom(false);
			if((!overlapped_last_frame_x && overlapped_last_frame_y)
			|| (!overlapped_last_frame_x && !overlapped_last_frame_y 
				&& std::abs(overlap.x) <= std::abs(overlap.y)))
			{
				if(overlap.x < 0)
					right=true; // other is on the right of this
				else
					left=true; // other is on the left of this
			}
			else
			{	
				if(overlap.y < 0)
					bottom=true; // other is at the bottom of this
				else
					top=true; // other is on top of this
			}
			logM << "\toverlapped_last_frame_x=" << overlapped_last_frame_x << endl;
			logM << "\toverlapped_last_frame_y=" << overlapped_last_frame_y << endl;
			if(right)
				logM << "\tright" << endl;
			else if(left)
				logM << "\tleft" << endl;
			else if(top)
				logM << "\ttop" << endl;
			else if(bottom)
				logM << "\tbottom" << endl;

			// Calcul du ratio //
			sf::Vector2f speed_ratio;
			bool pushes_left=m_pushes_left_tile||m_pushes_left_obj;
			bool pushes_right=m_pushes_right_tile||m_pushes_right_obj;
			bool pushes_top=m_pushes_top_tile||m_pushes_top_obj;
			bool pushes_bottom=m_pushes_bottom_tile||m_pushes_bottom_obj;
			bool other_pushes_left=data->m_other->getPushesLeftTile()||data->m_other->getPushesLeftObj();
			bool other_pushes_right=data->m_other->getPushesRightTile()||data->m_other->getPushesRightObj();
			bool other_pushes_top=data->m_other->getPushesTopTile()||data->m_other->getPushesTopObj();
			bool other_pushes_bottom=data->m_other->getPushesBottomTile()||data->m_other->getPushesBottomObj();
			// this stuck
			if((right && pushes_left) || (left && pushes_right) || (top && pushes_bottom) || (bottom && pushes_top))
			{
				logM << "\tthis stuck" << endl;
				speed_ratio.x=0;
				speed_ratio.y=0;
				m_can_continue=false;
			}
			// other stuck
			else if((right && other_pushes_right) || (left && other_pushes_left) || (top && other_pushes_top) || (bottom && other_pushes_bottom))
			{
				logM << "\tother stuck" << endl;
				speed_ratio.x=1;
				speed_ratio.y=1;
				m_can_continue=false;
			}
			// this kine
			else if(m_is_kinematic)
			{
				// other kine
				if(data->m_other->getIsKinematic())
				{
					// ratio partagé
					if(speed_sum.x==0 && speed_sum.y==0)
					{
						speed_ratio.x=0.5;
						speed_ratio.y=0.5;
					}
					else if(speed_sum.x==0)
					{
						speed_ratio.x=0.5;
						speed_ratio.y=abs_speed1.y/speed_sum.y;
					}
					else if(speed_sum.y==0)
					{
						speed_ratio.x=abs_speed1.x/speed_sum.x;
						speed_ratio.y=0.5;
					}
					else
					{
						speed_ratio.x=abs_speed1.x/speed_sum.x;
						speed_ratio.y=abs_speed1.y/speed_sum.y;
					}
				}
				// other !kine
				else
				{
					speed_ratio.x=0;
					speed_ratio.y=0;
				}
			}
			// this !kine
			else
			{
				// other kine
				if(data->m_other->getIsKinematic())
				{
					speed_ratio.x=1;
					speed_ratio.y=1;
				}
				// other !kine
				else
				{
					// ratio partagé
					if(speed_sum.x==0 && speed_sum.y==0)
					{
						speed_ratio.x=0.5;
						speed_ratio.y=0.5;
					}
					else if(speed_sum.x==0)
					{
						speed_ratio.x=0.5;
						speed_ratio.y=abs_speed1.y/speed_sum.y;
					}
					else if(speed_sum.y==0)
					{
						speed_ratio.x=abs_speed1.x/speed_sum.x;
						speed_ratio.y=0.5;
					}
					else
					{
						speed_ratio.x=abs_speed1.x/speed_sum.x;
						speed_ratio.y=abs_speed1.y/speed_sum.y;
					}
				}
			}
			logM << "\tspeed_ratio = " << speed_ratio.x << "," << speed_ratio.y << endl;

			// Move //
			sf::Vector2f offset_to_apply(overlap.x*speed_ratio.x, overlap.y*speed_ratio.y);
			if((!overlapped_last_frame_x && overlapped_last_frame_y)
			|| (!overlapped_last_frame_x && !overlapped_last_frame_y 
				&& std::abs(overlap.x) <= std::abs(overlap.y)))
			{
				this->move(sf::Vector2f(offset_to_apply.x,0));
				offset_sum.x += offset_to_apply.x;
				offset_to_apply.y=0;

				if(overlap.x < 0)
				{
					m_pushes_right_obj=true;
					if(speed_ratio.x!=0 || speed_ratio.y!=0 || !m_is_kinematic)
						m_speed.x=std::min(m_speed.x,0.f);
				}
				else
				{
					m_pushes_left_obj=true;
					if(speed_ratio.x!=0 || speed_ratio.y!=0 || !m_is_kinematic)
						m_speed.x=std::max(m_speed.x,0.f);
				}
			}
			else
			{	
				this->move(sf::Vector2f(0,offset_to_apply.y));
				offset_sum.y += offset_to_apply.y;
				offset_to_apply.x=0;

				if(overlap.y < 0)
				{
					m_pushes_bottom_obj=true;
					if(speed_ratio.x!=0 || speed_ratio.y!=0 || !m_is_kinematic)
						m_speed.x=std::min(m_speed.y,0.f);
				}
				else
				{
					m_pushes_top_obj=true;
					if(speed_ratio.x!=0 || speed_ratio.y!=0 || !m_is_kinematic)
						m_speed.x=std::max(m_speed.y,0.f);
				}
			}
			logM << "\tmoved by " << offset_to_apply.x << "," << offset_to_apply.y << endl;
		}
	}
}

void MovingObject::updatePhysicsP2()
{
	this->updatePhysicsResponse();

	m_pushes_right = m_pushes_right_obj || m_pushes_right_tile;
	m_pushes_left = m_pushes_left_obj || m_pushes_left_tile;
	m_pushes_bottom = m_pushes_bottom_obj || m_pushes_bottom_tile;
	m_pushes_top = m_pushes_top_obj || m_pushes_top_tile;

	m_hitbox.m_center=this->getPosition()+m_hitbox_offset; // sans offset ????
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
