#include "ArchiveSystem.h"
#include <iterator>
#include <fstream>

struct Recording :public sf::Drawable
{
	std::string m_name;
	std::string m_author;
	std::string m_genre;
	int m_countOfPages;
	int m_count;
	int m_countOfTaked;

	bool m_openList;   //not shown

	std::list<std::pair<std::string, int>>* m_people;

	Recording(const std::string& name, const std::string& author, const std::string& genre, const int& countOfPages, const int& count);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	//std::string m_nameOfTaker;
	//int m_timeToReturn;
private:
	sf::RectangleShape m_nameSh;
	sf::RectangleShape m_authorSh;
	sf::RectangleShape m_genreSh;
	sf::RectangleShape m_countOfPagesSh;
	sf::RectangleShape m_countSh;
	sf::RectangleShape m_countOfTakedSh;
};

struct ArchiveIMPL
{
	sf::Font m_font;
	std::list<Recording> m_list;
};

Recording::Recording(const std::string& name, const std::string& author, const std::string& genre, const int& countOfPages, const int& count) : m_name(name), m_author(author), m_genre(genre), m_countOfPages(countOfPages), m_count(count)
{
	m_countOfTaked = 0;
	//m_nameOfTaker = "";
	//m_timeToReturn = 0;
	m_people = new std::list<std::pair<std::string, int>>;
	m_people = nullptr;
}

void Recording::draw(sf::RenderTarget& target, sf::RenderStates states) const
{

}


ArchiveSystem* ArchiveSystem::m_ArchiveSystem = nullptr;

ArchiveSystem* ArchiveSystem::GetInstance()
{
	if (m_ArchiveSystem == nullptr)
		m_ArchiveSystem = new ArchiveSystem();
	return m_ArchiveSystem;
}

ArchiveSystem::ArchiveSystem()
{
	m_pImpl = new ArchiveIMPL();

	m_window.create(sf::VideoMode(1280, 720), "Library");

	m_pImpl->m_font.loadFromFile("resourses/arial.ttf");

	std::fstream fin("resourses/recordings.dat", std::ios::in | std::ios::out | std::ios::app);
	if (fin.is_open());


}

auto& ArchiveSystem::operator[](const int i)
{
	auto l_iter = m_pImpl->m_list.begin();
	std::advance(l_iter, i);

	return *l_iter;
}

void ArchiveSystem::sort(const int& fieldPos)
{
	/*m_list.sort([](const Recording& a,const Recording& b)
	{ 

	});*/
}

void ArchiveSystem::push()
{

}

void ArchiveSystem::erase(const int& i)
{
	auto l_iter = m_pImpl->m_list.begin();
	std::advance(l_iter, i);


}

void ArchiveSystem::EditRecording(const int& i)
{
	auto l_iter = m_pImpl->m_list.begin();
	std::advance(l_iter, i);


}

void ArchiveSystem::TakeBook()
{

}

void ArchiveSystem::Do()
{
	sf::Text names("Name", m_pImpl->m_font);
	names.setFillColor(sf::Color::Black);
	names.setPosition(150, 55);
	sf::Text authors("Author", m_pImpl->m_font);
	authors.setFillColor(sf::Color::Black);
	authors.setPosition(460, 55);
	sf::Text genres("Genre", m_pImpl->m_font);
	genres.setFillColor(sf::Color::Black);
	genres.setPosition(710, 55);
	sf::Text pages("Pages", m_pImpl->m_font);
	pages.setFillColor(sf::Color::Black);
	pages.setPosition(855, 55);
	sf::Text taken("Taken", m_pImpl->m_font);
	taken.setFillColor(sf::Color::Black);
	taken.setPosition(1000, 55);

	sf::RectangleShape bottom;
	bottom.setSize(sf::Vector2f(1280, 40));
	bottom.setOrigin(640, 20);
	bottom.setPosition(640, 700);
	bottom.setFillColor(sf::Color(230,230,230));
	bottom.setOutlineThickness(3);
	bottom.setOutlineColor(sf::Color(220,220,220));

	sf::RectangleShape menu;
	menu.setSize(sf::Vector2f(1180, 700));
	menu.setOrigin(590, 250);
	menu.setPosition(640, 300);
	menu.setOutlineThickness(5);
	menu.setOutlineColor(sf::Color::Black);

	sf::RectangleShape names_shape;
	names_shape.setSize(sf::Vector2f(300, 50));
	names_shape.setPosition(50, 50);
	names_shape.setOutlineThickness(3);
	names_shape.setOutlineColor(sf::Color::Black);
	sf::RectangleShape authors_shape;
	authors_shape.setSize(sf::Vector2f(300, 50));
	authors_shape.setPosition(350, 50);
	authors_shape.setOutlineThickness(3);
	authors_shape.setOutlineColor(sf::Color::Black);
	sf::RectangleShape genres_shape;
	genres_shape.setSize(sf::Vector2f(200, 50));
	genres_shape.setPosition(650, 50);
	genres_shape.setOutlineThickness(3);
	genres_shape.setOutlineColor(sf::Color::Black);
	sf::RectangleShape pages_shape;
	pages_shape.setSize(sf::Vector2f(100, 50));
	pages_shape.setPosition(850, 50);
	pages_shape.setOutlineThickness(3);
	pages_shape.setOutlineColor(sf::Color::Black);
	sf::RectangleShape taken_shape;
	taken_shape.setSize(sf::Vector2f(180, 50));
	taken_shape.setPosition(950, 50);
	taken_shape.setOutlineThickness(3);
	taken_shape.setOutlineColor(sf::Color::Black);
	sf::RectangleShape action;
	action.setSize(sf::Vector2f(100, 50));
	action.setPosition(1130, 50);
	action.setOutlineThickness(3);
	action.setOutlineColor(sf::Color::Black);

	sf::Event event;
	while (m_window.isOpen())
	{
		while (m_window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				m_window.close();
		}

		m_window.clear(sf::Color::White);

		m_window.draw(menu);
		m_window.draw(bottom);
		m_window.draw(names_shape);
		m_window.draw(authors_shape);
		m_window.draw(genres_shape);
		m_window.draw(pages_shape);
		m_window.draw(taken_shape);
		m_window.draw(action);

		m_window.draw(names);
		m_window.draw(authors);
		m_window.draw(genres);
		m_window.draw(pages);
		m_window.draw(taken);

		m_window.display();
	}
}