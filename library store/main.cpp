#include "ArchiveSystem.h"

int main()
{
	sf::Event event;

	ArchiveSystem* archive = ArchiveSystem::GetInstance();

	while(archive->m_window.isOpen())
	{
		while (archive->m_window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				archive->m_window.close();
		}

		archive->Do();
	}

	return 0;
}