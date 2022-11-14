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
					auto np = name_str;
					auto ap = author_str;
					auto gp = genre_str;
					auto cpp = countPag_str;
					auto cp = count_str;
					np.erase(std::remove_if(np.begin(), np.end(), isspace), np.end());
					ap.erase(std::remove_if(ap.begin(), ap.end(), isspace), ap.end());
					gp.erase(std::remove_if(gp.begin(), gp.end(), isspace), gp.end());
					cpp.erase(std::remove_if(cpp.begin(), cpp.end(), isspace), cpp.end());
					cp.erase(std::remove_if(cp.begin(), cp.end(), isspace), cp.end());

					if (np != "" && ap != "" && gp != "" && cpp != "" && cp != "")
					{
						if (std::stoi(count_str) > 0 && std::stoi(countPag_str) > 0)
						{
							m_pImpl->m_list.push_back(Recording(name_str, author_str, genre_str, std::stoi(countPag_str), std::stoi(count_str), m_pImpl->m_font, 0));
								return true;
						}
					}
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

void ArchiveSystem::Edit(const int& i)
{
	auto l_iter = m_pImpl->m_list.begin();
	std::advance(l_iter, i);

	std::string name_str = l_iter->m_name, author_str = l_iter->m_author, genre_str = l_iter->m_genre, countPag_str = std::to_string(l_iter->m_countOfPages), 
		count_str = std::to_string(l_iter->m_count), countTaked_str = std::to_string(l_iter->m_countOfTaked);
	bool name_enter = false, author_enter = false, genre_enter = false, countPag_enter = false, count_enter = false, countTaked_enter = false;

	sf::RectangleShape menu;
	menu.setSize(sf::Vector2f(500, 400));
	menu.setOrigin(250, 200);
	menu.setPosition(640, 350);
	menu.setOutlineThickness(4);
	menu.setOutlineColor(sf::Color::Black);

	sf::Texture t_delete;
	t_delete.loadFromFile("resourses/trash.png");
	sf::Sprite s_delete(t_delete);
	s_delete.setPosition(850, 500);

	sf::Text editBook("Edit book", m_pImpl->m_font, 30);
	editBook.setFillColor(sf::Color::Black);
	editBook.setPosition(580, 150);
	sf::Text name("Name:   " + name_str, m_pImpl->m_font, 24);
	name.setFillColor(sf::Color::Black);
	name.setPosition(400, 240);
	sf::Text author("Author: " + author_str, m_pImpl->m_font, 24);
	author.setFillColor(sf::Color::Black);
	author.setPosition(400, 280);
	sf::Text genre("Genre:  " + genre_str, m_pImpl->m_font, 24);
	genre.setFillColor(sf::Color::Black);
	genre.setPosition(400, 320);
	sf::Text countOfPages("Count of pages: " + countPag_str, m_pImpl->m_font, 24);
	countOfPages.setFillColor(sf::Color::Black);
	countOfPages.setPosition(400, 360);
	sf::Text count("Count: " + count_str, m_pImpl->m_font, 24);
	count.setFillColor(sf::Color::Black);
	count.setPosition(400, 400);
	sf::Text countTaked("Count of taked: " + countTaked_str, m_pImpl->m_font, 24);
	countTaked.setFillColor(sf::Color::Black);
	countTaked.setPosition(400, 440);
	sf::Text edit("Edit", m_pImpl->m_font, 30);
	edit.setFillColor(sf::Color::Black);
	edit.setPosition(615, 490);

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
	sf::RectangleShape countTakedForm;
	countTakedForm.setSize(sf::Vector2f(150, 30));
	countTakedForm.setPosition(577, 443);
	countTakedForm.setOutlineThickness(2);
	countTakedForm.setOutlineColor(sf::Color::Black);
	sf::RectangleShape editForm;
	editForm.setSize(sf::Vector2f(75, 32));
	editForm.setPosition(605, 493);
	editForm.setOutlineThickness(2);
	editForm.setOutlineColor(sf::Color::Black);

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
					countTaked_enter = false;
				}
				else if (sf::IntRect(authorForm.getGlobalBounds()).contains(sf::Mouse::getPosition(m_window)))
				{
					name_enter = false;
					author_enter = true;
					genre_enter = false;
					countPag_enter = false;
					count_enter = false;
					countTaked_enter = false;
				}
				else if (sf::IntRect(genreForm.getGlobalBounds()).contains(sf::Mouse::getPosition(m_window)))
				{
					name_enter = false;
					author_enter = false;
					genre_enter = true;
					countPag_enter = false;
					count_enter = false;
					countTaked_enter = false;
				}
				else if (sf::IntRect(countPagForm.getGlobalBounds()).contains(sf::Mouse::getPosition(m_window)))
				{
					name_enter = false;
					author_enter = false;
					genre_enter = false;
					countPag_enter = true;
					count_enter = false;
					countTaked_enter = false;
				}
				else if (sf::IntRect(countForm.getGlobalBounds()).contains(sf::Mouse::getPosition(m_window)))
				{
					name_enter = false;
					author_enter = false;
					genre_enter = false;
					countPag_enter = false;
					count_enter = true;
					countTaked_enter = false;
				}
				else if (sf::IntRect(countTakedForm.getGlobalBounds()).contains(sf::Mouse::getPosition(m_window)))
				{
					name_enter = false;
					author_enter = false;
					genre_enter = false;
					countPag_enter = false;
					count_enter = false;
					countTaked_enter = true;
				}
				else if (sf::IntRect(editForm.getGlobalBounds()).contains(sf::Mouse::getPosition(m_window)))
				{
					auto np = name_str;
					auto ap = author_str;
					auto gp = genre_str;
					auto cpp = countPag_str;
					auto cp = count_str;
					np.erase(std::remove_if(np.begin(), np.end(), isspace), np.end());
					ap.erase(std::remove_if(ap.begin(), ap.end(), isspace), ap.end());
					gp.erase(std::remove_if(gp.begin(), gp.end(), isspace), gp.end());
					cpp.erase(std::remove_if(cpp.begin(), cpp.end(), isspace), cpp.end());
					cp.erase(std::remove_if(cp.begin(), cp.end(), isspace), cp.end());

					if (np != "" && ap != "" && gp != "" && cpp != "" && cp != "")
					{
						if (std::stoi(count_str) > 0 && std::stoi(countPag_str) > 0)
						{
							if (std::stoi(countTaked_str) > 0 && std::stoi(countTaked_str) <= std::stoi(count_str))
							{
								l_iter->Edit(name_str, author_str, genre_str, std::stoi(countPag_str), std::stoi(count_str), std::stoi(countTaked_str));
								return;
							}
						}
					}
				}
				else if (sf::IntRect(s_delete.getGlobalBounds()).contains(sf::Mouse::getPosition(m_window)))
				{
					m_pImpl->m_list.erase(l_iter);
					return;
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
					return;
			}
			if (event.type == sf::Event::TextEntered && (name_enter || author_enter || genre_enter || countPag_enter || count_enter || countTaked_enter))
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
				else if (countTaked_enter)
					str = &countTaked_str;

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
							else if (countTaked_enter)
								countTaked.setString("Count of taked: " + *str);
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
							else if (countTaked_enter)
								countTaked.setString("Count of taked: " + *str);
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
		m_window.draw(countTakedForm);
		m_window.draw(editForm);

		m_window.draw(name);
		m_window.draw(author);
		m_window.draw(genre);
		m_window.draw(countOfPages);
		m_window.draw(count);
		m_window.draw(countTaked);
		m_window.draw(edit);
		m_window.draw(editBook);
		m_window.draw(s_delete);

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

void ArchiveSystem::TakeReturnBook(const bool take, const int i)
{
	auto l_iter = m_pImpl->m_list.begin();
	std::advance(l_iter, i);

	l_iter->TakeReturnBook(take);
}

void ArchiveSystem::Do()
{
	sf::Texture t_add;
	t_add.loadFromFile("resourses/add.png");
	sf::Sprite s_add(t_add);
	sf::Sprite s_closed(t_add);
	s_closed.rotate(45);
	s_closed.setPosition(1260, -15);
	sf::Texture t_minus;
	t_minus.loadFromFile("resourses/minus.png");
	sf::Texture t_plus;
	t_plus.loadFromFile("resourses/plus.png");
	sf::Texture t_edit;
	t_edit.loadFromFile("resourses/edit.png");

	sf::Sprite s_minus(t_minus);
	s_minus.setOrigin(16, 16);
	sf::Sprite s_plus(t_plus);
	s_minus.setOrigin(16, 16);
	sf::Sprite s_edit(t_edit);
	s_edit.setOrigin(24, 24);

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

	int n = -1;

	sf::Event event;
	while (m_window.isOpen())
	{
		while (m_window.pollEvent(event))
		{
			if (event.type == sf::Event::MouseButtonReleased && event.key.code == sf::Mouse::Left)
			{
				if (sf::IntRect(s_add.getGlobalBounds()).contains(sf::Mouse::getPosition(m_window)))
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
				if (sf::IntRect(s_edit.getGlobalBounds()).contains(sf::Mouse::getPosition(m_window)))
				{
					Edit(n);
					PlaceElements();
				}
				if (sf::IntRect(s_minus.getGlobalBounds()).contains(sf::Mouse::getPosition(m_window)))
				{
					TakeReturnBook(false, n);
					PlaceElements();
				}
				if (sf::IntRect(s_plus.getGlobalBounds()).contains(sf::Mouse::getPosition(m_window)))
				{
					TakeReturnBook(true, n);
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

		s_edit.setPosition(-100, 0);
		s_plus.setPosition(-100, 0);
		s_minus.setPosition(-100, 0);
		n = -1;
		for (auto i : m_pImpl->m_list)
		{
			n++;
			if (sf::IntRect(i.getGlobalBounds()).contains(sf::Mouse::getPosition(m_window)))
			{
				s_edit.setPosition(1255, n * 35 + 115);
				s_plus.setPosition(1200, n * 35 + 105);
				s_minus.setPosition(1185, n * 35 + 120);
				break;
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

		m_window.draw(s_edit);
		m_window.draw(s_minus);
		m_window.draw(s_plus);

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

		m_window.draw(s_add);
		m_window.draw(s_closed);

		m_window.display();
	}
}