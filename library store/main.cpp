#include "ArchiveSystem.h"

int main()
{
	setlocale(LC_ALL, "Russian");
	sf::Event event;

	ArchiveSystem* archive = ArchiveSystem::GetInstance();

	while(archive->m_window.isOpen())
	{
		archive->Do();
	}

	return 0;
}