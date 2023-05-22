#pragma once
#include <list>
#include <string>
#include <SFML/Graphics.hpp>

struct ArchiveIMPL;

class ArchiveSystem
{
	ArchiveIMPL* m_pImpl;
	
	ArchiveSystem();
	ArchiveSystem(const ArchiveSystem&) = delete;
	ArchiveSystem& operator=(const ArchiveSystem&) = delete;

	void SetBasicElements();
	void CheckClick();
	void CheckEnteringText(sf::Uint32 code);
	void CheckMousePos();
	void FixSize();
	void sort(const int& fieldPos);
	void PlaceElements();
	void Move(const bool up);
	void Draw();
	void Save();
	void Add();
	bool AddBook();
	void DeleteBook();
	bool Exit();
public:
	sf::RenderWindow m_window;
	static ArchiveSystem* GetInstance();

	void Do();
};

