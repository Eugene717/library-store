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
	void CheckClick(int recording);
	void PlaceElements();
	void Move(const bool up);
	void Draw();
	void SoldOrBuyBook(const bool sold, const int i);
	void Save();
	bool Exit();
public:
	sf::RenderWindow m_window;
	static ArchiveSystem* GetInstance();

	void Do();
	bool Add();
	void Edit(const int& i);
	void sort(const int& fieldPos);
};

