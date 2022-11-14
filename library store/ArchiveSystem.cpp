#include "ArchiveSystem.h"
#include <iterator>
#include <fstream>
#include "Recording.h"

struct ArchiveIMPL
{
	sf::Font m_font;
	std::list<Recording> m_list;
	int m_sorted;
};

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

	m_window.create(sf::VideoMode(1280, 720), "Library", sf::Style::Titlebar);

	m_pImpl->m_font.loadFromFile("resourses/arial.ttf");

	m_pImpl->m_sorted = -1;

	std::fstream fin("resourses/recordings.dat", std::ios::in | std::ios::out | std::ios::app);

	std::string name, author, genre;
	int countPag, count, countTaked;

	while (!fin.eof())
	{
		name = author = genre = ""; countPag, count, countTaked = 0;
		fin >> name >> author >> genre >> countPag >> count >> countTaked;
		if (name != "")
			m_pImpl->m_list.push_back(Recording(name, author, genre, countPag, count, m_pImpl->m_font, countTaked));
	}
	PlaceElements();
}

void ArchiveSystem::PlaceElements()
{
	int n = 0;
	for (auto& i : m_pImpl->m_list)
	{
		i.Place(50, 100 + n * 35);
		n++;		
	}
}

void ArchiveSystem::Move(const bool up)
{
	if (up)
		if (m_pImpl->m_list.begin()->GetY() >= 100)
			return;
	if (!up)
		if (m_pImpl->m_list.begin()->GetY() <= 110 || (--m_pImpl->m_list.end())->GetY() <= 600)
			return;

	for (auto& i : m_pImpl->m_list)
	{
		i.Move(up);
	}
}

bool ArchiveSystem::Add()
{
	std::string name_str, author_str, genre_str, countPag_str, count_str;
	bool name_enter = false, author_enter = false, genre_enter = false, countPag_enter = false, count_enter = false;

	sf::RectangleShape menu;
	menu.setSize(sf::Vector2f(500, 300));
	menu.setOrigin(250, 150);
	menu.setPosition(640, 350);
	menu.setOutlineThickness(4);
	menu.setOutlineColor(sf::Color::Black);

	sf::Text addBook("Add book", m_pImpl->m_font, 30);
	addBook.setFillColor(sf::Color::Black);
	addBook.setPosition(580, 200);
	sf::Text name("Name:", m_pImpl->m_font, 24);
	name.setFillColor(sf::Color::Black);
	name.setPosition(400, 240);
	sf::Text author("Author:", m_pImpl->m_font, 24);
	author.setFillColor(sf::Color::Black);
	author.setPosition(400, 280);
	sf::Text genre("Genre:", m_pImpl->m_font, 24);
	genre.setFillColor(sf::Color::Black);
	genre.setPosition(400, 320);
	sf::Text countOfPages("Count of pages:", m_pImpl->m_font, 24);
	countOfPages.setFillColor(sf::Color::Black);
	countOfPages.setPosition(400, 360);
	sf::Text count("Count:", m_pImpl->m_font, 24);
	count.setFillColor(sf::Color::Black);
	count.setPosition(400, 400);
	sf::Text add("Add", m_pImpl->m_font, 30);
	add.setFillColor(sf::Color::Black);
	add.setPosition(615, 450);

	sf::RectangleShape nameForm;
	nameForm.setSize(sf::Vector2f(390, 30));
	nameForm.setPosition(487, 243);
	nameForm.setOutlineThickness(2);
	nameForm.setOutlineColor(sf::Color::Black);
	sf::RectangleShape authorForm;
	authorForm.setSize(sf::Vector2f(390, 30));
	authorForm.setPosition(487, 283);
	authorForm.setOutlineThickness(2);
	authorForm.setOutlineColor(sf::Color::Black);
	sf::RectangleShape genreForm;
	genreForm.setSize(sf::Vector2f(390, 30));
	genreForm.setPosition(487, 323);
	genreForm.setOutlineThickness(2);
	genreForm.setOutlineColor(sf::Color::Black);
	sf::RectangleShape countPagForm;
	countPagForm.setSize(sf::Vector2f(293, 30));
	countPagForm.setPosition(584, 363);
	countPagForm.setOutlineThickness(2);
	countPagForm.setOutlineColor(sf::Color::Black);
	sf::RectangleShape countForm;
	countForm.setSize(sf::Vector2f(150, 30));
	countForm.setPosition(477, 403);
	countForm.setOutlineThickness(2);
	countForm.setOutlineColor(sf::Color::Black);
	sf::RectangleShape addForm;
	addForm.setSize(sf::Vector2f(75, 32));
	addForm.setPosition(605, 453);
	addForm.setOutlineThickness(2);
	addForm.setOutlineColor(sf::Color::Black);

	sf::Event event;
	while (m_window.isOpen())
	{
		while (m_window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				m_window.close();
			if (event.type == sf::Event::MouseButtonReleased && event.key.code == sf::Mouse::Left)
			{
				if (sf::IntRect(nameForm.getGlobalBounds()).contains(sf::Mouse::getPosition(m_window)))
				{
					name_enter = true;
					author_enter = false;
					genre_enter = false;
					countPag_enter = false;
					count_enter = false;
				}
				else if (sf::IntRect(authorForm.getGlobalBounds()).contains(sf::Mouse::getPosition(m_window)))
				{
					name_enter = false;
					author_enter = true;
					genre_enter = false;
					countPag_enter = false;
					count_enter = false;
				}
				else if (sf::IntRect(genreForm.getGlobalBounds()).contains(sf::Mouse::getPosition(m_window)))
				{
					name_enter = false;
					author_enter = false;
					genre_enter = true;
					countPag_enter = false;
					count_enter = false;
				}
				else if (sf::IntRect(countPagForm.getGlobalBounds()).contains(sf::Mouse::getPosition(m_window)))
				{
					name_enter = false;
					author_enter = false;
					genre_enter = false;
					countPag_enter = true;
					count_enter = false;
				}
				else if (sf::IntRect(countForm.getGlobalBounds()).contains(sf::Mouse::getPosition(m_window)))
				{
					name_enter = false;
					author_enter = false;
					genre_enter = false;
					countPag_enter = false;
					count_enter = true;
				}
				else if (sf::IntRect(addForm.getGlobalBounds()).contains(sf::Mouse::getPosition(m_window)))
				{
					m_pImpl->m_list.push_back(Recording(name_str, author_str, genre_str, std::stoi(countPag_str), std::stoi(count_str), m_pImpl->m_font, 0));
					return true;
				}
				else
				{
					name_enter = false;
					author_enter = false;
					genre_enter = false;
					countPag_enter = false;
					count_enter = false;
				}
				if (!sf::IntRect(menu.getGlobalBounds()).contains(sf::Mouse::getPosition(m_window)))
					return false;
			}
			if (event.type == sf::Event::TextEntered && (name_enter || author_enter || genre_enter || countPag_enter || count_enter))
			{
				std::string* str = nullptr;
				if (name_enter)
					str = &name_str;
				else if (author_enter)
					str = &author_str;
				else if (genre_enter)
					str = &genre_str;
				else if (countPag_enter)
					str = &countPag_str;
				else if (count_enter)
					str = &count_str;

				if (str != nullptr)
				{
					if (event.text.unicode == '\b')
					{
						if (str->size() > 0)
						{
							str->pop_back();
							if (name_enter)
								name.setString("Name:   " + *str);
							else if (author_enter)
								author.setString("Author: " + *str);
							else if (genre_enter)
								genre.setString("Genre:  " + *str);
							else if (countPag_enter)
								countOfPages.setString("Count of pages: " + *str);
							else if (count_enter)
								count.setString("Count: " + *str);
						}
					}
					else if (event.text.unicode < 128)
					{
						if (str->size() < 30)
						{
							*str += static_cast<char>(event.text.unicode);
							if (name_enter)
								name.setString("Name:   " + *str);
							else if (author_enter)
								author.setString("Author: " + *str);
							else if (genre_enter)
								genre.setString("Genre:  " + *str);
							else if (countPag_enter)
								countOfPages.setString("Count of pages: " + *str);
							else if (count_enter)
								count.setString("Count: " + *str);
						}
					}
				}
			}
		}

		m_window.draw(menu);

		m_window.draw(nameForm);
		m_window.draw(authorForm);
		m_window.draw(genreForm);
		m_window.draw(countPagForm);
		m_window.draw(countForm);
		m_window.draw(addForm);

		m_window.draw(name);
		m_window.draw(author);
		m_window.draw(genre);
		m_window.draw(countOfPages);
		m_window.draw(count);
		m_window.draw(add);
		m_window.draw(addBook);

		m_window.display();
	}
}

void ArchiveSystem::sort(const int& fieldPos)
{
	if (m_pImpl->m_sorted == -1)
		m_pImpl->m_list.sort([&](const Recording& a, const Recording& b)
			{
				if (fieldPos == 1)
				{
					m_pImpl->m_sorted = 1;
					return a.m_name < b.m_name;
				}
				if (fieldPos == 2)
				{
					m_pImpl->m_sorted = 2;
					return a.m_author < b.m_author;
				}
				if (fieldPos == 3)
				{
					m_pImpl->m_sorted = 3;
					return a.m_genre < b.m_genre;
				}
				if (fieldPos == 4)
				{
					m_pImpl->m_sorted = 4;
					return a.m_countOfPages < b.m_countOfPages;
				}
				if (fieldPos == 5)
				{
					m_pImpl->m_sorted = 5;
					return a.m_count < b.m_count;
				}
				if (fieldPos == 6)
				{
					m_pImpl->m_sorted = 6;
					return a.m_countOfTaked < b.m_countOfTaked;
				}
			});
	else
		m_pImpl->m_list.sort([&](const Recording& a, const Recording& b)
			{
				if (fieldPos == 1)
				{
					m_pImpl->m_sorted = -1;
					return a.m_name > b.m_name;
				}
				if (fieldPos == 2)
				{
					m_pImpl->m_sorted = -1;
					return a.m_author > b.m_author;
				}
				if (fieldPos == 3)
				{
					m_pImpl->m_sorted = -1;
					return a.m_genre > b.m_genre;
				}
				if (fieldPos == 4)
				{
					m_pImpl->m_sorted = -1;
					return a.m_countOfPages > b.m_countOfPages;
				}
				if (fieldPos == 5)
				{
					m_pImpl->m_sorted = -1;
					return a.m_count > b.m_count;
				}
				if (fieldPos == 6)
				{
					m_pImpl->m_sorted = -1;
					return a.m_countOfTaked > b.m_countOfTaked;
				}
			});
}

void ArchiveSystem::erase(const int& i)
{
	auto l_iter = m_pImpl->m_list.begin();
	std::advance(l_iter, i);

	m_pImpl->m_list.erase(l_iter);
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
	sf::Texture t_plus;
	t_plus.loadFromFile("resourses/+.png");
	sf::Sprite s_plus(t_plus);
	sf::Sprite s_closed(t_plus);
	s_closed.rotate(45);
	s_closed.setPosition(1260, -15);
	sf::Texture t_minus;
	t_minus.loadFromFile("resourses/-.png");
	sf::Texture t_edit;
	t_edit.loadFromFile("resourses/edit.png");



	sf::Sprite s_close(t_plus);
	s_close.rotate(45);
	sf::Sprite s_minus(t_minus);
	sf::Sprite s_edit(t_edit);

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
	sf::Text count("Count", m_pImpl->m_font);
	count.setFillColor(sf::Color::Black);
	count.setPosition(980, 55);
	sf::Text taken("Taken", m_pImpl->m_font);
	taken.setFillColor(sf::Color::Black);
	taken.setPosition(1120, 55);

	sf::RectangleShape bottom;
	bottom.setSize(sf::Vector2f(1280, 40));
	bottom.setOrigin(640, 20);
	bottom.setPosition(640, 700);
	bottom.setOutlineThickness(3);
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
	sf::RectangleShape count_shape;
	count_shape.setSize(sf::Vector2f(140, 50));
	count_shape.setPosition(950, 50);
	count_shape.setOutlineThickness(3);
	count_shape.setOutlineColor(sf::Color::Black);
	sf::RectangleShape taken_shape;
	taken_shape.setSize(sf::Vector2f(140, 50));
	taken_shape.setPosition(1090, 50);
	taken_shape.setOutlineThickness(3);
	taken_shape.setOutlineColor(sf::Color::Black);

	sf::RectangleShape wall;
	wall.setSize(sf::Vector2f(3, 700));
	wall.setFillColor(sf::Color::Black);



	sf::Event event;
	while (m_window.isOpen())
	{
		while (m_window.pollEvent(event))
		{
			if (event.type == sf::Event::MouseButtonReleased && event.key.code == sf::Mouse::Left)
			{
				if (sf::IntRect(s_plus.getGlobalBounds()).contains(sf::Mouse::getPosition(m_window)))
				{
					Add();
					PlaceElements();
				}
				if (sf::IntRect(s_closed.getGlobalBounds()).contains(sf::Mouse::getPosition(m_window)))
				{
					std::ofstream fout("resourses/recordings.dat", std::ios::out);
					for (auto i : m_pImpl->m_list)
					{
						if (fout.is_open())
							fout << i.m_name << " " << i.m_author << " " << i.m_genre << " " << i.m_countOfPages << " " << i.m_count << " " << i.m_countOfTaked << '\n';
					}
					m_window.close();
				}
				if (sf::IntRect(names_shape.getGlobalBounds()).contains(sf::Mouse::getPosition(m_window)))
				{
					sort(1);
					PlaceElements();
				}
				if (sf::IntRect(authors_shape.getGlobalBounds()).contains(sf::Mouse::getPosition(m_window)))
				{
					sort(2);
					PlaceElements();
				}
				if (sf::IntRect(genres_shape.getGlobalBounds()).contains(sf::Mouse::getPosition(m_window)))
				{
					sort(3);
					PlaceElements();
				}
				if (sf::IntRect(pages_shape.getGlobalBounds()).contains(sf::Mouse::getPosition(m_window)))
				{
					sort(4);
					PlaceElements();
				}
				if (sf::IntRect(count_shape.getGlobalBounds()).contains(sf::Mouse::getPosition(m_window)))
				{
					sort(5);
					PlaceElements();
				}
				if (sf::IntRect(taken_shape.getGlobalBounds()).contains(sf::Mouse::getPosition(m_window)))
				{
					sort(6);
					PlaceElements();
				}
			}
			if (event.type == sf::Event::MouseWheelMoved)
			{
				if (!m_pImpl->m_list.empty())
				{
					if (event.mouseWheel.delta == 1)   //up
						Move(true);
					else if (event.mouseWheel.delta == -1)   //down
						Move(false);
				}
			}
		}

		m_window.clear(sf::Color::White);

		m_window.draw(menu);

		for (auto i : m_pImpl->m_list)
		{
			m_window.draw(i);
		}

		m_window.draw(names_shape);
		m_window.draw(authors_shape);
		m_window.draw(genres_shape);
		m_window.draw(pages_shape);
		m_window.draw(count_shape);
		m_window.draw(taken_shape);

		m_window.draw(names);
		m_window.draw(authors);
		m_window.draw(genres);
		m_window.draw(pages);
		m_window.draw(count);
		m_window.draw(taken);

		wall.setPosition(347, 50);
		m_window.draw(wall);
		wall.setPosition(647, 50);
		m_window.draw(wall);
		wall.setPosition(847, 50);
		m_window.draw(wall);
		wall.setPosition(947, 50);
		m_window.draw(wall);
		wall.setPosition(1087, 50);
		m_window.draw(wall);

		bottom.setPosition(640, 700);
		bottom.setFillColor(sf::Color(230, 230, 230));
		bottom.setOutlineColor(sf::Color(220, 220, 220));
		m_window.draw(bottom);
		bottom.setPosition(640, 22);
		bottom.setFillColor(sf::Color::White);
		bottom.setOutlineColor(sf::Color::White);
		m_window.draw(bottom);

		m_window.draw(s_plus);
		m_window.draw(s_closed);

		m_window.display();
	}
}