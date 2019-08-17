#include <SFML/Graphics.hpp>
#include "constants.hpp"
#include "AABB.hpp"
#include "movingObject.hpp"
#include "character.hpp"

using namespace std;

int main(int argc, char** argv)
{
	sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Game");
	window.setFramerateLimit(FPS);
	
	Character character(sf::Vector2f(100.f, 0.f), sf::Vector2f(20.f, 20.f));

	bool inputs[NB_KEY_CHARACTER];
	for(int i=0; i<NB_KEY_CHARACTER; i++)
		inputs[i]=false;

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
						
						case sf::Keyboard::Right:
							inputs[(int)GoRight]=true;
							break;
						
						case sf::Keyboard::Left:
							inputs[(int)GoLeft]=true;
							break;
						
						case sf::Keyboard::Space:
							inputs[(int)GoJump]=true;
							break;
						
						case sf::Keyboard::Down:
							inputs[(int)GoDown]=true;
							break;
					}
					break;

				case sf::Event::KeyReleased:
					switch(event.key.code)
					{
						case sf::Keyboard::Escape:
							window.close();
							break;
						
						case sf::Keyboard::Right:
							inputs[(int)GoRight]=false;
							break;
						
						case sf::Keyboard::Left:
							inputs[(int)GoLeft]=false;
							break;
						
						case sf::Keyboard::Space:
							inputs[(int)GoJump]=false;
							break;
						
						case sf::Keyboard::Down:
							inputs[(int)GoDown]=false;
							break;
					}
					break;
			}
		}
		
		// update it
		sf::Time elapsed = clock.restart();
		character.update(elapsed, inputs);
		character.debug();
		
		// draw it
		window.clear();
		window.draw(character);
		window.display();
	}
	
	return 0;
}

