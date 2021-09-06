#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "../include/constants.hpp"
#include "../include/map.hpp"
#include "../include/character.hpp"
#include "../include/physicalObject.hpp"
#include "../include/raoult.hpp"
#include "../include/orang.hpp"
#include "../include/bigChungus.hpp"
#include "../include/howard.hpp"
#include "../include/logManager.hpp"

#include <iostream>
using namespace std;

// project global var
// used in MovingObject
Map map;

LogManager logM;

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

void fixedUpdate(sf::Time elapsed, vector<MovingObject*> objects)
{
	// Faire custom update puis systematiquement physics ?
	// au lieu de updatePhysic dans le custom

	for(unsigned int i=0; i<objects.size(); i++)
	{
		objects[i]->update(elapsed);
		objects[i]->updatePhysics(elapsed);
		objects[i]->collisionTiles();
		::map.quadtreeUpdateArea(objects[i]);
		objects[i]->clearCollision();
	}

	::map.checkCollisions();
	for(unsigned int i=0; i<objects.size(); i++)
		objects[i]->updatePhysicsP2();

	bool modif=false;
	for(unsigned int i=0; i<objects.size(); i++)
	{
		if(objects[i]->collisionTiles())
			modif=true;
	}
	if(modif)
	{
		for(unsigned int i=0; i<objects.size(); i++)
			objects[i]->updatePhysicsP2();
	}
}

int main(int argc, char** argv)
{
	sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Game");
	window.setFramerateLimit(FPS);

	logM.disable();

	bool inputs[NB_KEY_CHARACTER];
	for(int i=0; i<NB_KEY_CHARACTER; i++)
		inputs[i]=false;

	vector<MovingObject*> objects;

	//Character(center, halfsize) //
	objects.push_back(new Character(sf::Vector2f(120, 60), inputs));

	// Objects (center, name, isKinematic) //
		objects.push_back(new Orang(sf::Vector2f(300,7), "orang", false));
		objects.push_back(new Raoult(sf::Vector2f(50,90), "raoult", false));
		objects.push_back(new BigChungus(sf::Vector2f(550,300), "big_chungus", false));
		// objects.push_back(new BigChungus(sf::Vector2f(450,300), "big_chungus2", true));
		objects.push_back(new Howard(sf::Vector2f(680,150), "howard", false));

	// create a clock to track the elapsed time //
	sf::Clock clock;

	// run the main loop //
	while(window.isOpen())
	{
		logM << endl << "=> Debut <=" << endl;

		// handle events //
		handleEvents(&window, inputs);

		// update it //
		fixedUpdate(clock.restart(), objects);

		// debug log //
		//::map.quadtreeDebug2();
		reinterpret_cast<Character*>(objects[0])->debug2();
		logM << "big= " << objects[1]->getPosition().x << " , " << objects[1]->getPosition().y << endl;
		logM << endl;

		// draw it //
		window.clear();
		::map.draw(&window);
		//::map.drawQuadtree(&window); //debug
		for(unsigned int i=0; i<objects.size(); i++)
		{
			window.draw(*objects[i]);
			objects[i]->m_hitbox.draw(&window); //debug
		}
		window.display();
	}

	for(unsigned int i=0; i<objects.size(); i++)
		delete objects[i];

	return 0;
}
