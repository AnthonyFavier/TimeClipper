#include <SFML/Graphics.hpp>
#include "../include/constants.hpp"
#include "../include/character.hpp"
#include "../include/physicalObject.hpp"
#include "../include/map.hpp"

#include <iostream>
using namespace std;

// project global var
// used in MovingObject
Map map;

// Check sensors size
// Check high speed collision

void handleEvents(sf::RenderWindow* window, bool inputs[])
{
	sf::Event event;
	while(window->pollEvent(event))
	{
		switch(event.type)
		{
			case sf::Event::Closed:
				window->close();
				break;

			case sf::Event::KeyPressed:
				switch(event.key.code)
				{
					case sf::Keyboard::Escape:
						window->close();
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

					default:
						break;
				}
				break;

			case sf::Event::KeyReleased:
				switch(event.key.code)
				{
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

					default:
						break;
				}
				break;

			default:
				break;
		}
	}

}

void fixedUpdate(sf::Time elapsed, bool inputs[], Character* character, vector<MovingObject*> objects)
{	
	// Faire custom update puis systematiquement physics ?
	// au lieu de updatePhysic dans le custom
	character->updateC(elapsed, inputs);
	::map.quadtreeUpdateArea(character);
	character->clearCollision();

	for(unsigned int i=0; i<objects.size(); i++)
	{
		objects[i]->update(elapsed);
		::map.quadtreeUpdateArea(objects[i]);
		objects[i]->clearCollision();
	}

	::map.checkCollisions();

	character->updatePhysicsResponse();
	for(unsigned int i=0; i<objects.size(); i++)
		objects[i]->updatePhysicsP2();
}

int main(int argc, char** argv)
{
	sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Game");
	window.setFramerateLimit(FPS);

	//Character(center, halfsize) //
	Character character(sf::Vector2f(120, 60), sf::Vector2f(14, 25));

	// PhysicalObject //
	vector<MovingObject*> objects;
	//objects.push_back(new PhysicalObject(sf::Vector2f(300,7), sf::Vector2f(25,22), sf::Color(255,255,255), "orange", false));
	//objects.push_back(new PhysicalObject(sf::Vector2f(50,90), sf::Vector2f(45,41), sf::Color(255,255,255), "raoult", false));
	objects.push_back(new PhysicalObject(sf::Vector2f(410,200), sf::Vector2f(25,49), sf::Color(255,255,255), "big_chungus", true));

	// create a clock to track the elapsed time //
	sf::Clock clock;

	bool inputs[NB_KEY_CHARACTER];
	for(int i=0; i<NB_KEY_CHARACTER; i++)
		inputs[i]=false;

	// run the main loop //
	while(window.isOpen())
	{
		// handle events //
		handleEvents(&window, inputs);

		// update it //
		fixedUpdate(clock.restart(), inputs, &character, objects);

		// debug //
		//::map.quadtreeDebug();
		character.debug();
		cout << endl;

		// draw it //
		window.clear();

		::map.draw(&window);
		//::map.drawQuadtree(&window);
		window.draw(character);
		for(unsigned int i=0; i<objects.size(); i++)
			window.draw(*objects[i]);

		window.display();
	}

	for(unsigned int i=0; i<objects.size(); i++)
		delete objects[i];

	return 0;
}
