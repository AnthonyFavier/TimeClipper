#ifndef DEF_QUADTREE
#define DEF_QUADTREE

#include <SFML/Graphics.hpp>
#include "movingObject.hpp"
#include "constants.hpp"
#include <math.h>
#include <algorithm>

const int cellWidth=WINDOW_WIDTH/3;
const int cellHeight=WINDOW_HEIGHT/3;
const int hCountGrid=(int)ceil(WINDOW_WIDTH/cellWidth);
const int vCountGrid=(int)ceil(WINDOW_HEIGHT/cellHeight);

class Quadtree
{
private:
	vector<MovingObject*> m_object_in_area[hCountGrid][vCountGrid];
public:
	Quadtree();
	void updateArea(MovingObject* obj);
	void addObjectToArea(sf::Vector2i areaIndex, MovingObject* obj);
	void removeObjectFromArea(sf::Vector2i areaIndex, int objIndexInArea, MovingObject* obj);
	void draw(sf::RenderWindow* window);
	void debug();
};

#endif
