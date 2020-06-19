#include "../include/quadtree.hpp"

#include <iostream>
using namespace std;

Quadtree::Quadtree()
{
}

void Quadtree::updateArea(MovingObject* obj)
{
	sf::Vector2f tmp = obj->m_hitbox.m_center - obj->m_hitbox.getHalfSize();
	sf::Vector2i top_left((int)tmp.x, (int)tmp.y);
	tmp = obj->m_hitbox.m_center + sf::Vector2f(obj->m_hitbox.getHalfSize().x,-obj->m_hitbox.getHalfSize().y);
	sf::Vector2i top_right((int)tmp.x, (int)tmp.y);
	tmp = obj->m_hitbox.m_center + sf::Vector2f(-obj->m_hitbox.getHalfSize().x,obj->m_hitbox.getHalfSize().y);
	sf::Vector2i bottom_left((int)tmp.x, (int)tmp.y);
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
	
	vector<sf::Vector2i> overlappingAreas;

	// whole object in one cell
	if(top_left.x == top_right.x && top_left.y == bottom_left.y)
		overlappingAreas.push_back(top_left);
	// object in several vertical cells
	else if(top_left.x == top_right.x)
	{
		sf::Vector2i cell;
		cell.x=top_left.x;
		for(cell.y=top_left.y; cell.y<=bottom_left.y; cell.y++)
			overlappingAreas.push_back(cell);
	}
	// object in several horizontal cells
	else if(top_left.y == bottom_left.y)
	{
		sf::Vector2i cell;
		cell.y=top_left.y;
		for(cell.x=top_left.x; cell.x<=top_right.x; cell.x++)
			overlappingAreas.push_back(cell);
	}
	// object in several horizontal and vertical cells
	else
	{
		sf::Vector2i cell;
		for(cell.x=top_left.x; cell.x<=top_right.x; cell.x++)
		{
			for(cell.y=top_left.y; cell.y<=bottom_left.y; cell.y++)
				overlappingAreas.push_back(cell);
		}
	}

	vector<sf::Vector2i>::iterator it;

	// loop through previous areas
	for(unsigned int i=0; i<obj->m_areas.size(); i++)
	{
		it=find(overlappingAreas.begin(), overlappingAreas.end(), obj->m_areas[i]);
		if(it==overlappingAreas.end())
		{
			removeObjectFromArea(obj->m_areas[i], obj->m_ids_in_areas[i], obj);
			obj->m_areas.erase(obj->m_areas.begin()+i);
			obj->m_ids_in_areas.erase(obj->m_ids_in_areas.begin()+i);
			--i;
		}
	}

	// loop through new areas
	for(unsigned int i=0; i<overlappingAreas.size(); i++)
	{
		it=find(obj->m_areas.begin(), obj->m_areas.end(), overlappingAreas[i]);
		if(it==obj->m_areas.end())
			addObjectToArea(overlappingAreas[i], obj);
	}

	overlappingAreas.clear();
}

void Quadtree::addObjectToArea(sf::Vector2i areaIndex, MovingObject* obj)
{
	obj->m_areas.push_back(areaIndex);
	obj->m_ids_in_areas.push_back(m_object_in_area[areaIndex.x][areaIndex.y].size());

	m_object_in_area[areaIndex.x][areaIndex.y].push_back(obj);
}

void Quadtree::removeObjectFromArea(sf::Vector2i areaIndex, int objIndexInArea, MovingObject* obj)
{
	// swap last item with the one we are removing
	MovingObject* tmp = m_object_in_area[areaIndex.x][areaIndex.y].back();
	*m_object_in_area[areaIndex.x][areaIndex.y].end() = obj;
	m_object_in_area[areaIndex.x][areaIndex.y][objIndexInArea] = tmp;

	for(unsigned int i=0; i<tmp->m_areas.size(); i++)
	{
		if(tmp->m_areas[i] == areaIndex)
		{
			tmp->m_ids_in_areas[i] = objIndexInArea; 
			break;
		}
	}

	// remove last item
	m_object_in_area[areaIndex.x][areaIndex.y].pop_back();
}

void Quadtree::draw(sf::RenderWindow* window)
{
	sf::RectangleShape line(sf::Vector2f(WINDOW_WIDTH, 2));

	for(int i=0; i<hCountGrid-1; i++)
	{
		line.setPosition(0,cellHeight*(i+1));	
		window->draw(line);
	}
	line.rotate(90);
	for(int i=0; i<vCountGrid-1; i++)
	{
		line.setPosition(cellWidth*(i+1),0);	
		window->draw(line);
	}
}

void Quadtree::debug()
{
	cout << "Quadtree:" << endl;
	for(int i=0; i<hCountGrid; i++)
	{
		for(int j=0; j<vCountGrid; j++)
			cout << m_object_in_area[j][i].size() << " ";
		cout << endl;
	}
}
