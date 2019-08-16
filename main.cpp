#include <SFML/Graphics.hpp>
#include "AABB.hpp"
#include "movingObject.hpp"

int main(int argc, char** argv)
{
	sf::RenderWindow window(sf::VideoMode(800, 800), "Game");
	window.setFramerateLimit(60);
	
	AABB ent(sf::Vector2f(200.f, 200.f), sf::Vector2f(50.f, 100.f));
	
	// create a clock to track the elapsed time
	sf::Clock clock;
	
	// run the main loop
	while(window.isOpen())
	{
		// handle events
		sf::Event event;
		while(window.pollEvent(event))
		{
			switch(event.type)
			{
				case sf::Event::Closed:
					window.close();
					break;
				
				case sf::Event::KeyPressed:
					switch(event.key.code)
					{
						case sf::Keyboard::Escape:
							window.close();
							break;
						
						/*case sf::Keyboard::Right:
							ent.setPosition(ent.getPosition().x+20, ent.getPosition().y);
							break;
						
						case sf::Keyboard::Left:
							ent.setPosition(ent.getPosition().x-20, ent.getPosition().y);
							break;
						
						case sf::Keyboard::Up:
							ent.setPosition(ent.getPosition().x, ent.getPosition().y-20);
							break;
						
						case sf::Keyboard::Down:
							ent.setPosition(ent.getPosition().x, ent.getPosition().y+20);
							break;*/
					}
					break;
			}
		}
		
		// update it
		sf::Time elapsed = clock.restart();
		
		// draw it
		window.clear();
		//window.draw(ent);
		window.display();
	}
	
	return 0;
}

