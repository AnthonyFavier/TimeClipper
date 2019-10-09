#include <SFML/Graphics.hpp>
#include "../include/constants.hpp"
#include "../include/character.hpp"
#include "../include/map.hpp"

#include <iostream>
using namespace std;

Map map;

// Check sensors size
// Check high speed collision

int main(int argc, char** argv)
{
	sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Game");
	window.setFramerateLimit(FPS);

	//Character(center, halfsize)
	Character character(sf::Vector2f(100, 60), sf::Vector2f(14, 25));

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

						case sf::Keyboard::A:
							inputs[(int)SizeDown]=true;
							break;

						case sf::Keyboard::Z:
							inputs[(int)SizeUp]=true;
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

						case sf::Keyboard::A:
							inputs[(int)SizeDown]=false;
							break;

						case sf::Keyboard::Z:
							inputs[(int)SizeUp]=false;
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
		::map.draw(&window);
		window.draw(character);
		window.display();
	}

	return 0;
}
