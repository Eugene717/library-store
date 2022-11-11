#pragma once
#include <list>
#include <string>

struct Recording;

class ArchiveSystem
{
	std::list<Recording> m_list;
public:
	void Draw() const;
	void sort(const int& fieldPos);
	Recording& operator[](const int);
	void push();
	void erase(const int& i);
	void EditRecording(const int& i);
	void TakeBook();
};

