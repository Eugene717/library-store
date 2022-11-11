#include "ArchiveSystem.h"
#include <iterator>

struct Recording
{
	std::string m_name;
	std::string m_author;
	std::string m_genre;
	int m_countOfPages;
	int m_count;

	int m_countOfTaked;
	bool m_openList;   //not shown

	std::list<std::pair<std::string, int>>* m_people;
		
	//std::string m_nameOfTaker;
	//int m_timeToReturn;
};

//Recording::Recording(const std::string& name, const std::string& author, const std::string& genre, const int& countOfPages, const int& count) : m_name(name), m_author(author), m_genre(genre), m_countOfPages(countOfPages), m_count(count)
//{
//	m_countOfTaked = 0;
//	//m_nameOfTaker = "";
//	//m_timeToReturn = 0;
//	m_people = new std::list<std::pair<std::string, int>>;
//	m_people = nullptr;
//}

Recording& ArchiveSystem::operator[](const int i)
{
	auto l_iter = m_list.begin();
	std::advance(l_iter, i);

	return *l_iter;
}

void ArchiveSystem::sort(const int& fieldPos)
{
	//m_list.sort([]() {});
}

void ArchiveSystem::push()
{

}

void ArchiveSystem::erase(const int& i)
{
	auto l_iter = m_list.begin();
	std::advance(l_iter, i);


}

void ArchiveSystem::EditRecording(const int& i)
{
	auto l_iter = m_list.begin();
	std::advance(l_iter, i);


}

void ArchiveSystem::TakeBook()
{

}

void ArchiveSystem::Draw() const
{

}