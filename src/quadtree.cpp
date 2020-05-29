#include "../include/quadtree.hpp"

Quadtree::Quadtree(Map* map)
{
	m_map=map;
}

void Quadtree::updateArea(MovingObject* obj)
{
	sf::Vector2i top_left = m_map->getMapTileAtPoint(obj->m_hitbox.m_center + sf::Vector2f(-obj->m_hitbox.getHalfSize().x, obj->m_hitbox.getHalfSize().y));
	sf::Vector2i top_right = m_map->getMapTileAtPoint(obj->m_hitbox.m_center + obj->m_hitbox.getHalfSize());
	sf::Vector2i bottom_left = m_map->getMapTileAtPoint(obj->m_hitbox.m_center - obj->m_hitbox.getHalfSize());
	sf::Vector2i bottom_right;

	top_left.x /= cellWidth;
	top_left.y /= cellHeight;

	top_right.x /= cellWidth;
	top_right.y /= cellHeight;

	bottom_left.x /= cellWidth;
	bottom_left.y /= cellHeight;

	bottom_right.x = top_right.x;
	bottom_right.y = bottom_left.y;

	// on suppose que l'objet n'est pas en dehors de la map
	// sinon test sup pour l'ignorer
	// on suppose que obj plus petit qu'une cell
	// sinon faut faire loop et ajouter toutes ares inter
	
	vector<sf::Vector2i> overlappingAreas;

	if(top_left.x == top_right.x && top_left.y == bottom_left.y)
	{
		overlappingAreas.push_back(top_left);
	}
	else if(top_left.x == top_right.x)
	{
		overlappingAreas.push_back(top_left);
		overlappingAreas.push_back(bottom_left);
	}
	else if(top_left.y == bottom_left.y)
	{
		overlappingAreas.push_back(top_left);
		overlappingAreas.push_back(top_right);
	}
	else
	{
		overlappingAreas.push_back(top_left);
		overlappingAreas.push_back(top_right);
		overlappingAreas.push_back(bottom_left);
		overlappingAreas.push_back(bottom_left);
	}

	vector<sf::Vector2i> areas = obj->m_areas;
	vector<int> ids = obj->m_ids_in_areas;
	vector<sf::Vector2i>::iterator it;

	// loop through previous areas
	for(unsigned int i=0; i<areas.size(); i++)
	{
		it=find(areas.begin(), areas.end(), areas[i]);
		if(it==areas.end())
		{
			removeObjectFromArea(areas[i], ids[i], obj);
			areas.erase(areas.begin()+i);
			ids.erase(ids.begin()+i);
			--i;
		}
	}

	// loop through new areas
	for(unsigned int i=0; i<overlappingAreas.size(); i++)
	{
		sf::Vector2i area = overlappingAreas[i];
		it=find(areas.begin(), areas.end(), area);
		if(it==areas.end())
			addObjectToArea(area, obj);
	}

	overlappingAreas.clear();
}

void Quadtree::addObjectToArea(sf::Vector2i areaIndex, MovingObject* obj)
{
	vector<MovingObject*> area = m_object_in_area[areaIndex.x][areaIndex.y];

	obj->m_areas.push_back(areaIndex);
	obj->m_ids_in_areas.push_back(area.size());

	area.push_back(obj);
}

void Quadtree::removeObjectFromArea(sf::Vector2i areaIndex, int objIndexInArea, MovingObject* obj)
{
	vector<MovingObject*> area = m_object_in_area[areaIndex.x][areaIndex.y];

	// swap last item with the one we are removing
	MovingObject* tmp = area.back();
	area[area.size()-1] = obj;
	area[objIndexInArea] = tmp;

	vector<int> tmp_ids = tmp->m_ids_in_areas;
	vector<sf::Vector2i> tmp_areas = tmp->m_areas;

	for(unsigned int i=0; i<tmp_areas.size(); i++)
	{
		if(tmp_areas[i] == areaIndex)
		{
			tmp_ids[i] = objIndexInArea;
			break;
		}
	}

	// remove last item
	area.pop_back();
}
