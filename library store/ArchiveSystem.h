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
public:
	sf::RenderWindow m_window;
	static ArchiveSystem* GetInstance();

	auto& operator[](const int);
	void Do();
	void sort(const int& fieldPos);
	void push();
	void erase(const int& i);
	void EditRecording(const int& i);
	void TakeBook();
};

