#pragma once
#include <list>
#include <string>
#include <SFML/Graphics.hpp>

struct ArchiveIMPL;

class ArchiveSystem
{
	ArchiveIMPL* m_pImpl;
	static ArchiveSystem* m_ArchiveSystem;
	
	ArchiveSystem();
	ArchiveSystem(const ArchiveSystem&) = delete;
	ArchiveSystem& operator=(const ArchiveSystem&) = delete;

	void PlaceElements();
	void Move(const bool up);
public:
	sf::RenderWindow m_window;
	static ArchiveSystem* GetInstance();

	void Do();
	bool Add();
	void sort(const int& fieldPos);
	void erase(const int& i);
	void EditRecording(const int& i);
	void TakeBook();
};

