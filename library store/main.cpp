#include "ArchiveSystem.h"

int main()
{
	ArchiveSystem* archive = ArchiveSystem::GetInstance();

	while(archive->m_window.isOpen())
	{
		archive->Do();
	}

	return 0;
}