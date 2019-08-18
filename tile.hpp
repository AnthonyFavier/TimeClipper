#ifndef DEF_TILE
#define DEF_TILE

#include <SFML/Graphics.hpp>

class Tile : public sf::Drawable, public sf::Transformable
{
public:
	Tile(sf::Vector2i pos)
	{
		m_size_pixel=16;
		m_pos=pos;
		this->setPosition(m_pos.x*m_size_pixel, m_pos.y*m_size_pixel);
	};
	bool canGoThroughRight(){return m_can_go_through_right;};
	bool canGoThroughLeft(){return m_can_go_through_left;};
	bool canGoThroughUp(){return m_can_go_through_up;};
	bool canGoThroughDown(){return m_can_go_through_down;};
	
protected:
	int m_size_pixel;
	sf::Vector2i m_pos;
	bool m_can_go_through_right;
	bool m_can_go_through_left;
	bool m_can_go_through_up;
	bool m_can_go_through_down;
	
private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const=0;
};

#endif

