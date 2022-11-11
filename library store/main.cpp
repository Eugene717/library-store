#include <SFML/Graphics.hpp>
#include <fstream>
#include "ArchiveSystem.h"

int main()
{
	sf::RenderWindow window(sf::VideoMode(1280, 720), "Library");
	sf::Event event;

	sf::Font font;
	font.loadFromFile("resourses/arial.ttf");

	std::fstream fin("resourses/recordings.dat", std::ios::in | std::ios::out | std::ios::app);
	if (fin.is_open());

	while(window.isOpen())
	{
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}
		window.clear(sf::Color::White);

		window.display();
	}

	return 0;
}