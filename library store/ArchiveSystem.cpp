#include "Recording.h"
#include "ArchiveSystem.h"
#include <iterator>
#include <mysql_connection.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/prepared_statement.h>

enum Object
{
	SortNames = 5, SortAuthors, SortGenres, SortPages, SortPrice, SortCount, CheckAvailable, SearchField, Search,
	Delete, Add, Minus, Plus, Save, GoUp, Clear, SearchText = 30, Menu = 31, Name = 39, Author, Genre, Pages, Price, Count, AddBook
};

struct ArchiveIMPL
{
	sf::Font m_font;
	std::list<Recording> m_recordings;
	std::vector<sf::Drawable*> m_objects;
	std::vector<std::string> m_toAddStrings;
	Recording* m_rec;
	int m_sorted;
	bool m_displayOnlyAvailable, m_displaySearched;
	std::string m_search;
	Recording* m_writeRec;
	std::string* m_writeString;
	sf::Text* m_writeText;

	sql::Driver* m_driver;
	sql::Connection* m_con;
	sql::Statement* m_stat;
	sql::PreparedStatement* m_prestat;
	sql::ResultSet* m_res;
};

ArchiveSystem* ArchiveSystem::GetInstance()
{
	static ArchiveSystem* singleton = new ArchiveSystem();
	return singleton;
}

ArchiveSystem::ArchiveSystem()
{
	m_pImpl = new ArchiveIMPL();

	m_window.create(sf::VideoMode(1280, 720), "Library");

	m_pImpl->m_font.loadFromFile("resourses/times.ttf");

	m_pImpl->m_rec = nullptr;
	m_pImpl->m_sorted = -1;
	m_pImpl->m_displayOnlyAvailable = m_pImpl->m_displaySearched = false;
	m_pImpl->m_search = "";
	m_pImpl->m_writeRec = nullptr;
	m_pImpl->m_writeString = nullptr;
	m_pImpl->m_writeText = nullptr;

	for (int i = 0; i < 6; i++)
		m_pImpl->m_toAddStrings.push_back("");

	std::string name, author, genre;
	int countPag, price, count;

	try
	{
		m_pImpl->m_driver = get_driver_instance();
		m_pImpl->m_con = m_pImpl->m_driver->connect("localhost", "root", "12222550g");
	}
	catch (sql::SQLException e)
	{
		std::cerr << "Could connect to server. Error massage: " << e.what() << std::endl;
		system("pause");
		exit(1);
	}

	m_pImpl->m_con->setSchema("bookstore");
	m_pImpl->m_stat = m_pImpl->m_con->createStatement();
	m_pImpl->m_res = m_pImpl->m_stat->executeQuery("SELECT * FROM books");

	while (m_pImpl->m_res->next())
		m_pImpl->m_recordings.push_back(Recording(m_pImpl->m_res, m_pImpl->m_font));

	delete m_pImpl->m_con;
	delete m_pImpl->m_stat;
	delete m_pImpl->m_res;

	m_pImpl->m_con = nullptr;	
	m_pImpl->m_stat = nullptr;
	m_pImpl->m_res = nullptr;

	SetBasicElements();
	PlaceElements();
}

void ArchiveSystem::SetBasicElements()
{
	sf::RectangleShape* lineBorder = new sf::RectangleShape(sf::Vector2f(1000, 2));
	lineBorder->setOrigin(500, 0);
	lineBorder->setPosition(600, 130);
	lineBorder->setFillColor(sf::Color(80, 80, 80));

	m_pImpl->m_objects.push_back(lineBorder);

	sf::RectangleShape* menu = new sf::RectangleShape(sf::Vector2f(1000, 600));
	menu->setOrigin(500, 300);
	menu->setPosition(600, 380);
	menu->setFillColor(sf::Color(150, 150, 150, 120));
	menu->setOutlineThickness(5);
	menu->setOutlineColor(sf::Color(80, 80, 80));

	m_pImpl->m_objects.push_back(menu);

	sf::RectangleShape* colBorder1 = new sf::RectangleShape(sf::Vector2f(247, 550));
	colBorder1->setFillColor(sf::Color(0, 0, 0, 0));
	colBorder1->setOutlineThickness(3);
	colBorder1->setOutlineColor(sf::Color(80, 80, 80));
	colBorder1->setPosition(350, 130);
	sf::RectangleShape* colBorder2 = new sf::RectangleShape(sf::Vector2f(97, 550));
	colBorder2->setFillColor(sf::Color(0, 0, 0, 0));
	colBorder2->setOutlineThickness(3);
	colBorder2->setOutlineColor(sf::Color(80, 80, 80));
	colBorder2->setPosition(800, 130);
	sf::RectangleShape* colBorder3 = new sf::RectangleShape(sf::Vector2f(100, 550));
	colBorder3->setFillColor(sf::Color(0, 0, 0, 0));
	colBorder3->setOutlineThickness(3);
	colBorder3->setOutlineColor(sf::Color(80, 80, 80));
	colBorder3->setPosition(1000, 130);

	m_pImpl->m_objects.push_back(colBorder1);
	m_pImpl->m_objects.push_back(colBorder2);
	m_pImpl->m_objects.push_back(colBorder3);
		
	sf::RectangleShape* names_shape = new sf::RectangleShape(sf::Vector2f(250, 50));
	names_shape->setPosition(100, 80);
	names_shape->setFillColor(sf::Color(150, 150, 150));
	names_shape->setOutlineThickness(3);
	names_shape->setOutlineColor(sf::Color(80, 80, 80));
	sf::RectangleShape* authors_shape = new sf::RectangleShape(sf::Vector2f(250, 50));
	authors_shape->setPosition(350, 80);
	authors_shape->setFillColor(sf::Color(150, 150, 150));
	authors_shape->setOutlineThickness(3);
	authors_shape->setOutlineColor(sf::Color(80, 80, 80));
	sf::RectangleShape* genres_shape = new sf::RectangleShape(sf::Vector2f(200, 50));
	genres_shape->setPosition(600, 80);
	genres_shape->setFillColor(sf::Color(150, 150, 150));
	genres_shape->setOutlineThickness(3);
	genres_shape->setOutlineColor(sf::Color(80, 80, 80));
	sf::RectangleShape* pages_shape = new sf::RectangleShape(sf::Vector2f(100, 50));
	pages_shape->setPosition(800, 80);
	pages_shape->setFillColor(sf::Color(150, 150, 150));
	pages_shape->setOutlineThickness(3);
	pages_shape->setOutlineColor(sf::Color(80, 80, 80));
	sf::RectangleShape* price_shape = new sf::RectangleShape(sf::Vector2f(100, 50));
	price_shape->setPosition(900, 80);
	price_shape->setFillColor(sf::Color(150, 150, 150));
	price_shape->setOutlineThickness(3);
	price_shape->setOutlineColor(sf::Color(80, 80, 80));
	sf::RectangleShape* count_shape = new sf::RectangleShape(sf::Vector2f(100, 50));
	count_shape->setPosition(1000, 80);
	count_shape->setFillColor(sf::Color(150, 150, 150));
	count_shape->setOutlineThickness(3);
	count_shape->setOutlineColor(sf::Color(80, 80, 80));
	sf::RectangleShape* check_shape = new sf::RectangleShape(sf::Vector2f(40, 40));
	check_shape->setPosition(1062, 20);
	check_shape->setFillColor(sf::Color(150, 150, 150, 120));
	check_shape->setOutlineThickness(3);
	check_shape->setOutlineColor(sf::Color::Black);
	sf::RectangleShape* searchField_shape = new sf::RectangleShape(sf::Vector2f(500, 35));
	searchField_shape->setPosition(198, 20);
	searchField_shape->setFillColor(sf::Color(150, 150, 150, 120));
	searchField_shape->setOutlineThickness(3);
	searchField_shape->setOutlineColor(sf::Color::Black);
	sf::RectangleShape* search_shape = new sf::RectangleShape(sf::Vector2f(35, 35));
	search_shape->setPosition(700, 20);
	search_shape->setFillColor(sf::Color(100, 100, 100, 200));
	search_shape->setOutlineThickness(3);
	search_shape->setOutlineColor(sf::Color::Black);

	m_pImpl->m_objects.push_back(names_shape);
	m_pImpl->m_objects.push_back(authors_shape);
	m_pImpl->m_objects.push_back(genres_shape);
	m_pImpl->m_objects.push_back(pages_shape);
	m_pImpl->m_objects.push_back(price_shape);
	m_pImpl->m_objects.push_back(count_shape);
	m_pImpl->m_objects.push_back(check_shape);
	m_pImpl->m_objects.push_back(searchField_shape);
	m_pImpl->m_objects.push_back(search_shape);

	sf::Texture* t_add = new sf::Texture;
	t_add->loadFromFile("resourses/add.png");
	sf::Texture* t_minus = new sf::Texture;
	t_minus->loadFromFile("resourses/minus.png");
	sf::Texture* t_plus = new sf::Texture;
	t_plus->loadFromFile("resourses/plus.png");
	sf::Texture* t_save = new sf::Texture;
	t_save->loadFromFile("resourses/save.png");
	sf::Texture* t_goUp = new sf::Texture;
	t_goUp->loadFromFile("resourses/up.png");
	sf::Texture* t_clear = new sf::Texture;
	t_clear->loadFromFile("resourses/plus.png");
	sf::Texture* t_check = new sf::Texture;
	t_check->loadFromFile("resourses/check.png");
	sf::Texture* t_search = new sf::Texture;
	t_search->loadFromFile("resourses/search.png");

	sf::Sprite* s_delete = new sf::Sprite(*t_clear);
	s_delete->setOrigin(16, 16);
	s_delete->rotate(45);
	s_delete->setPosition(-100, 0);
	sf::Sprite* s_add = new sf::Sprite(*t_add);
	s_add->setOrigin(16, 16);
	s_add->setPosition(80, 40);
	sf::Sprite* s_minus = new sf::Sprite(*t_minus);
	s_minus->setPosition(-100, 0);
	sf::Sprite* s_plus = new sf::Sprite(*t_plus);
	s_plus->setPosition(-100, 0);
	sf::Sprite* s_save = new sf::Sprite(*t_save);
	s_save->setOrigin(16, 16);
	s_save->setPosition(-100, 0);
	sf::Sprite* s_goUp = new sf::Sprite(*t_goUp);
	s_goUp->setOrigin(16, 16);
	s_goUp->setPosition(-100, 0);
	sf::Sprite* s_clear = new sf::Sprite(*t_clear);
	s_clear->setOrigin(16, 16);
	s_clear->rotate(45);
	s_clear->setPosition(-100, 0);
	sf::Sprite* s_check = new sf::Sprite(*t_check);
	s_check->setOrigin(16, 16);
	s_check->setPosition(-100, 0);
	sf::Sprite* s_search = new sf::Sprite(*t_search);
	s_search->setOrigin(16, 16);
	s_search->setPosition(718, 38);

	m_pImpl->m_objects.push_back(s_delete);
	m_pImpl->m_objects.push_back(s_add);
	m_pImpl->m_objects.push_back(s_minus);
	m_pImpl->m_objects.push_back(s_plus);
	m_pImpl->m_objects.push_back(s_save);
	m_pImpl->m_objects.push_back(s_goUp);
	m_pImpl->m_objects.push_back(s_clear);
	m_pImpl->m_objects.push_back(s_check);
	m_pImpl->m_objects.push_back(s_search);

	sf::Text* names = new sf::Text("Name", m_pImpl->m_font);
	names->setStyle(sf::Text::Bold);
	names->setFillColor(sf::Color::Black);
	names->setOrigin(names->getGlobalBounds().width / 2, names->getGlobalBounds().height / 2);
	names->setPosition(225, 95);
	sf::Text* authors = new sf::Text("Author", m_pImpl->m_font);
	authors->setStyle(sf::Text::Bold);
	authors->setFillColor(sf::Color::Black);
	authors->setOrigin(authors->getGlobalBounds().width / 2, authors->getGlobalBounds().height / 2);
	authors->setPosition(475, 95);
	sf::Text* genres = new sf::Text("Genre", m_pImpl->m_font);
	genres->setStyle(sf::Text::Bold);
	genres->setFillColor(sf::Color::Black);
	genres->setOrigin(genres->getGlobalBounds().width / 2, genres->getGlobalBounds().height / 2);
	genres->setPosition(700, 95);
	sf::Text* pages = new sf::Text("Pages", m_pImpl->m_font);
	pages->setStyle(sf::Text::Bold);
	pages->setFillColor(sf::Color::Black);
	pages->setOrigin(pages->getGlobalBounds().width / 2, pages->getGlobalBounds().height / 2);
	pages->setPosition(850, 98);
	sf::Text* price = new sf::Text("Price", m_pImpl->m_font);
	price->setStyle(sf::Text::Bold);
	price->setFillColor(sf::Color::Black);
	price->setOrigin(price->getGlobalBounds().width / 2, price->getGlobalBounds().height / 2);
	price->setPosition(950, 95);
	sf::Text* count = new sf::Text("Count", m_pImpl->m_font);
	count->setStyle(sf::Text::Bold);
	count->setFillColor(sf::Color::Black);
	count->setOrigin(count->getGlobalBounds().width / 2, count->getGlobalBounds().height / 2);
	count->setPosition(1050, 95);
	sf::Text* available = new sf::Text("Available", m_pImpl->m_font);
	available->setFillColor(sf::Color::Black);
	available->setPosition(1112, 20);
	sf::Text* searchTX = new sf::Text("", m_pImpl->m_font);
	searchTX->setCharacterSize(18);
	searchTX->setFillColor(sf::Color::Black);
	searchTX->setPosition(200, 25);

	m_pImpl->m_objects.push_back(names);
	m_pImpl->m_objects.push_back(authors);
	m_pImpl->m_objects.push_back(genres);
	m_pImpl->m_objects.push_back(pages);
	m_pImpl->m_objects.push_back(price);
	m_pImpl->m_objects.push_back(count);
	m_pImpl->m_objects.push_back(available);
	m_pImpl->m_objects.push_back(searchTX);
}

void ArchiveSystem::PlaceElements()
{
	int n = 0;
	bool display = true;

	for (auto& i : m_pImpl->m_recordings)
	{
		if (m_pImpl->m_displaySearched)
		{
			std::transform(m_pImpl->m_search.begin(), m_pImpl->m_search.end(), m_pImpl->m_search.begin(), ::toupper);

			std::vector<std::string> strings;
			strings.push_back(i.m_name);
			strings.push_back(i.m_author);
			strings.push_back(i.m_genre);
			strings.push_back(std::to_string(i.m_countOfPages));
			strings.push_back(std::to_string(i.m_price));
			strings.push_back(std::to_string(i.m_count));

			for (int j = 0; j < strings.size(); j++)
			{
				std::transform(strings[j].begin(), strings[j].end(), strings[j].begin(), ::toupper);
			}

			if (strings[0].find(m_pImpl->m_search) != std::string::npos)
				display = true;
			else if (strings[1].find(m_pImpl->m_search) != std::string::npos)
				display = true;
			else if (strings[2].find(m_pImpl->m_search) != std::string::npos)
				display = true;
			else if (strings[3].find(m_pImpl->m_search) != std::string::npos)
				display = true;
			else if (strings[4].find(m_pImpl->m_search) != std::string::npos)
				display = true;
			else if (strings[5].find(m_pImpl->m_search) != std::string::npos)
				display = true;
			else
				display = false;
		}
		if (m_pImpl->m_displayOnlyAvailable)
		{
			if (i.m_count == 0)
				display = false;
		}
				
		if (display)
		{
			i.Place(100, 130 + n * 35);
			n++;
		}
		else
		{
			i.Place(-1000, 130 + n * 35);
			display = true;
		}
	}

	((sf::RectangleShape*)m_pImpl->m_objects[0])->setPosition(600, 130);

	((sf::Sprite*)m_pImpl->m_objects[Object::GoUp])->setPosition(-100, 0);
}

void ArchiveSystem::CheckClick()
{
	if (m_pImpl->m_writeString != nullptr || m_pImpl->m_writeText != nullptr)
	{
		m_pImpl->m_writeString = nullptr;
		m_pImpl->m_writeText = nullptr;
		m_pImpl->m_writeRec = nullptr;
	}

	int object = -1;

	if (m_pImpl->m_objects.size() > 31)  //add function
	{
		for (int i = Object::Name; i <= Object::AddBook; i++)
		{
			if (sf::IntRect(((sf::RectangleShape*)m_pImpl->m_objects[i])->getGlobalBounds()).contains(sf::Mouse::getPosition(m_window)))
				object = i;
		}
		if (object == -1)
			if (!sf::IntRect(((sf::RectangleShape*)m_pImpl->m_objects[Object::Menu])->getGlobalBounds()).contains(sf::Mouse::getPosition(m_window)))
				object = Object::Menu;
	}

	if (m_pImpl->m_rec != nullptr)
	{
		if (object == -1)
		{
			if ((sf::IntRect(m_pImpl->m_rec->getGlobalBounds().left, m_pImpl->m_rec->getGlobalBounds().top, 250, 35).contains(sf::Mouse::getPosition(m_window))))
			{
				auto it = std::find_if(m_pImpl->m_recordings.begin(), m_pImpl->m_recordings.end(), [&](const Recording& a) {
					return m_pImpl->m_rec->GetID() == a.GetID();
					});

				m_pImpl->m_writeRec = &(*it);
				m_pImpl->m_writeString = &(*it).m_name;
				m_pImpl->m_writeText = &(*it).m_nameTx;

				return;
			}
			else if ((sf::IntRect(m_pImpl->m_rec->getGlobalBounds().left + 250, m_pImpl->m_rec->getGlobalBounds().top, 250, 35).contains(sf::Mouse::getPosition(m_window))))
			{
				auto it = std::find_if(m_pImpl->m_recordings.begin(), m_pImpl->m_recordings.end(), [&](const Recording& a) {
					return m_pImpl->m_rec->GetID() == a.GetID();
					});

				m_pImpl->m_writeRec = &(*it);
				m_pImpl->m_writeString = &(*it).m_author;
				m_pImpl->m_writeText = &(*it).m_authorTx;

				return;
			}
			else if ((sf::IntRect(m_pImpl->m_rec->getGlobalBounds().left + 500, m_pImpl->m_rec->getGlobalBounds().top, 200, 35).contains(sf::Mouse::getPosition(m_window))))
			{
				auto it = std::find_if(m_pImpl->m_recordings.begin(), m_pImpl->m_recordings.end(), [&](const Recording& a) {
					return m_pImpl->m_rec->GetID() == a.GetID();
					});

				m_pImpl->m_writeRec = &(*it);
				m_pImpl->m_writeString = &(*it).m_genre;
				m_pImpl->m_writeText = &(*it).m_genreTx;

				return;
			}
			else if ((sf::IntRect(m_pImpl->m_rec->getGlobalBounds().left + 700, m_pImpl->m_rec->getGlobalBounds().top, 100, 35).contains(sf::Mouse::getPosition(m_window))))
			{
				auto it = std::find_if(m_pImpl->m_recordings.begin(), m_pImpl->m_recordings.end(), [&](const Recording& a) {
					return m_pImpl->m_rec->GetID() == a.GetID();
					});

				m_pImpl->m_writeRec = &(*it);
				m_pImpl->m_writeString = nullptr;
				m_pImpl->m_writeText = &(*it).m_countOfPagesTx;

				return;
			}
			else if ((sf::IntRect(m_pImpl->m_rec->getGlobalBounds().left + 800, m_pImpl->m_rec->getGlobalBounds().top, 100, 35).contains(sf::Mouse::getPosition(m_window))))
			{
				auto it = std::find_if(m_pImpl->m_recordings.begin(), m_pImpl->m_recordings.end(), [&](const Recording& a) {
					return m_pImpl->m_rec->GetID() == a.GetID();
					});

				m_pImpl->m_writeRec = &(*it);
				m_pImpl->m_writeString = nullptr;
				m_pImpl->m_writeText = &(*it).m_priceTx;

				return;
			}
			else if ((sf::IntRect(m_pImpl->m_rec->getGlobalBounds().left + 900, m_pImpl->m_rec->getGlobalBounds().top, 100, 35).contains(sf::Mouse::getPosition(m_window))))
			{
				auto it = std::find_if(m_pImpl->m_recordings.begin(), m_pImpl->m_recordings.end(), [&](const Recording& a) {
					return m_pImpl->m_rec->GetID() == a.GetID();
					});

				m_pImpl->m_writeRec = &(*it);
				m_pImpl->m_writeString = nullptr;
				m_pImpl->m_writeText = &(*it).m_countTx;

				return;
			}
		}
	}

	if (object == -1)
		for (int i = Object::Delete; i <= Object::Clear; i++)
			if (sf::IntRect(((sf::Sprite*)m_pImpl->m_objects[i])->getGlobalBounds()).contains(sf::Mouse::getPosition(m_window)))
				object = i;
	if (object == -1)
		for (int i = Object::SortNames; i <= Object::Search; i++)
			if (sf::IntRect(((sf::RectangleShape*)m_pImpl->m_objects[i])->getGlobalBounds()).contains(sf::Mouse::getPosition(m_window)))
				object = i;

	if (object == -1)
		return;

	sf::Event event;
	while (m_window.isOpen())
	{
		if (m_window.pollEvent(event))
		{
			if (event.type == sf::Event::MouseButtonReleased && event.key.code == sf::Mouse::Left)
			{				
				if (m_pImpl->m_objects.size() > 31)
				{
					if (object == Object::Menu)
					{
						if (!sf::IntRect(((sf::RectangleShape*)m_pImpl->m_objects[Object::Menu])->getGlobalBounds()).contains(sf::Mouse::getPosition(m_window)))
						{
							for (int i = 0; i < 6; i++)
								m_pImpl->m_toAddStrings[i].clear();

							m_pImpl->m_writeString = nullptr;
							m_pImpl->m_writeText = nullptr;
							m_pImpl->m_writeRec = nullptr;

							while (m_pImpl->m_objects.size() > 31)
								m_pImpl->m_objects.pop_back();
						}
					}
					if (object == Object::Name)
					{
						if (sf::IntRect(((sf::RectangleShape*)m_pImpl->m_objects[object])->getGlobalBounds()).contains(sf::Mouse::getPosition(m_window)))
						{
							m_pImpl->m_writeString = &m_pImpl->m_toAddStrings[0];
							m_pImpl->m_writeText = (sf::Text*)m_pImpl->m_objects[object + 7];
						}
					}
					if (object == Object::Author)
					{
						if (sf::IntRect(((sf::RectangleShape*)m_pImpl->m_objects[object])->getGlobalBounds()).contains(sf::Mouse::getPosition(m_window)))
						{
							m_pImpl->m_writeString = &m_pImpl->m_toAddStrings[1];
							m_pImpl->m_writeText = (sf::Text*)m_pImpl->m_objects[object + 7];
						}
					}
					if (object == Object::Genre)
					{
						if (sf::IntRect(((sf::RectangleShape*)m_pImpl->m_objects[object])->getGlobalBounds()).contains(sf::Mouse::getPosition(m_window)))
						{
							m_pImpl->m_writeString = &m_pImpl->m_toAddStrings[2];
							m_pImpl->m_writeText = (sf::Text*)m_pImpl->m_objects[object + 7];
						}
					}
					if (object == Object::Pages)
					{
						if (sf::IntRect(((sf::RectangleShape*)m_pImpl->m_objects[object])->getGlobalBounds()).contains(sf::Mouse::getPosition(m_window)))
						{
							m_pImpl->m_writeString = &m_pImpl->m_toAddStrings[3];
							m_pImpl->m_writeText = (sf::Text*)m_pImpl->m_objects[object + 7];
						}
					}
					if (object == Object::Price)
					{
						if (sf::IntRect(((sf::RectangleShape*)m_pImpl->m_objects[object])->getGlobalBounds()).contains(sf::Mouse::getPosition(m_window)))
						{
							m_pImpl->m_writeString = &m_pImpl->m_toAddStrings[4];
							m_pImpl->m_writeText = (sf::Text*)m_pImpl->m_objects[object + 7];
						}
					}
					if (object == Object::Count)
					{
						if (sf::IntRect(((sf::RectangleShape*)m_pImpl->m_objects[object])->getGlobalBounds()).contains(sf::Mouse::getPosition(m_window)))
						{
							m_pImpl->m_writeString = &m_pImpl->m_toAddStrings[5];
							m_pImpl->m_writeText = (sf::Text*)m_pImpl->m_objects[object + 7];
						}
					}
					if (object == Object::AddBook)
					{
						if (sf::IntRect(((sf::RectangleShape*)m_pImpl->m_objects[object])->getGlobalBounds()).contains(sf::Mouse::getPosition(m_window)))
						{
							if (AddBook())
							{
								((sf::Sprite*)m_pImpl->m_objects[Object::Save])->setPosition(1250, 685);
								PlaceElements();
							}
						}
					}

					return;
				}

				if (object >= Object::SortNames && object <= Object::SortCount)
					if (sf::IntRect(((sf::Sprite*)m_pImpl->m_objects[object])->getGlobalBounds()).contains(sf::Mouse::getPosition(m_window)))
					{
						sort(object - Object::SortNames + 1);
						PlaceElements();
					}
				if (object == Object::CheckAvailable)
					if (sf::IntRect(((sf::RectangleShape*)m_pImpl->m_objects[object])->getGlobalBounds()).contains(sf::Mouse::getPosition(m_window)))
					{
						if (!m_pImpl->m_displayOnlyAvailable)
						{
							((sf::Sprite*)m_pImpl->m_objects[Object::GoUp + 2])->setPosition(1082, 40);
							m_pImpl->m_displayOnlyAvailable = true;
						}
						else
						{
							((sf::Sprite*)m_pImpl->m_objects[Object::GoUp + 2])->setPosition(-100, 0);
							m_pImpl->m_displayOnlyAvailable = false;
						}
						PlaceElements();
					}
				if (object == Object::SearchField)
					if (sf::IntRect(((sf::RectangleShape*)m_pImpl->m_objects[object])->getGlobalBounds()).contains(sf::Mouse::getPosition(m_window)))
					{
						m_pImpl->m_writeString = &m_pImpl->m_search;
						m_pImpl->m_writeText = (sf::Text*)m_pImpl->m_objects[Object::SearchText];
					}
				if (object == Object::Search)
					if (sf::IntRect(((sf::RectangleShape*)m_pImpl->m_objects[object])->getGlobalBounds()).contains(sf::Mouse::getPosition(m_window)))
					{
						m_pImpl->m_writeString = nullptr;
						m_pImpl->m_writeText = nullptr;
						if (m_pImpl->m_search != "")
						{
							m_pImpl->m_displaySearched = true;
							((sf::Sprite*)m_pImpl->m_objects[Object::Clear])->setPosition(687, 30);
							PlaceElements();
						}
					}
				if (object == Object::Save)
					if (sf::IntRect(((sf::Sprite*)m_pImpl->m_objects[object])->getGlobalBounds()).contains(sf::Mouse::getPosition(m_window)))
					{
						Save();
						((sf::Sprite*)m_pImpl->m_objects[object])->setPosition(-100, 0);
					}
				if (object == Object::Clear)
					if (sf::IntRect(((sf::Sprite*)m_pImpl->m_objects[object])->getGlobalBounds()).contains(sf::Mouse::getPosition(m_window)))
					{
						m_pImpl->m_writeString = &m_pImpl->m_search;
						m_pImpl->m_writeText = (sf::Text*)m_pImpl->m_objects[Object::SearchText];
						((sf::Sprite*)m_pImpl->m_objects[object])->setPosition(-100, 0);

						m_pImpl->m_writeString->clear();
						m_pImpl->m_writeText->setString(*m_pImpl->m_writeString);
						PlaceElements();
					}
				if (object == Object::Delete)
					if (sf::IntRect(((sf::Sprite*)m_pImpl->m_objects[object])->getGlobalBounds()).contains(sf::Mouse::getPosition(m_window)))
					{
						((sf::Sprite*)m_pImpl->m_objects[Object::Save])->setPosition(1250, 685);

						DeleteBook();
						PlaceElements();
					}
				if (object == Object::Add)
					if (sf::IntRect(((sf::Sprite*)m_pImpl->m_objects[object])->getGlobalBounds()).contains(sf::Mouse::getPosition(m_window)))
					{
						Add();
					}
				if (object == Object::Minus || object == Object::Plus)
					if (m_pImpl->m_rec != nullptr)
						if (sf::IntRect(((sf::Sprite*)m_pImpl->m_objects[object])->getGlobalBounds()).contains(sf::Mouse::getPosition(m_window)))
						{
							auto it = std::find_if(m_pImpl->m_recordings.begin(), m_pImpl->m_recordings.end(), [&](const Recording& a) {
								return m_pImpl->m_rec->GetID() == a.GetID();
								});

							it->SoldOrBuyBook((object + 1) % 2);

							((sf::Sprite*)m_pImpl->m_objects[Object::Save])->setPosition(1250, 685);
						}
				if (object == Object::GoUp)
					if (sf::IntRect(((sf::Sprite*)m_pImpl->m_objects[object])->getGlobalBounds()).contains(sf::Mouse::getPosition(m_window)))
					{
						PlaceElements();
					}

				return;
			}
		}
	}
}

void ArchiveSystem::CheckEnteringText(sf::Uint32 code)
{
	if (m_pImpl->m_writeString == nullptr && m_pImpl->m_writeText == nullptr)
		return;

	if (code == 13)
	{
		if (m_pImpl->m_writeString == &m_pImpl->m_search)
		{
			m_pImpl->m_displaySearched = true;
			PlaceElements();
		}

		m_pImpl->m_writeString = nullptr;
		m_pImpl->m_writeText = nullptr;
		m_pImpl->m_writeRec = nullptr;
	}
	else if (code == '\b')
	{		
		if (m_pImpl->m_writeString == nullptr)
		{
			std::string str = m_pImpl->m_writeText->getString();
			if (str.size() > 0)
			{
				str.pop_back();
				if (str.size() == 0)
					str = "0";

				m_pImpl->m_writeText->setString(str);

				if (m_pImpl->m_writeText == &m_pImpl->m_writeRec->m_countOfPagesTx)
					m_pImpl->m_writeRec->m_countOfPages = std::stoi(str);
				else if (m_pImpl->m_writeText == &m_pImpl->m_writeRec->m_priceTx)
					m_pImpl->m_writeRec->m_price = std::stof(str);
				else if (m_pImpl->m_writeText == &m_pImpl->m_writeRec->m_countTx)
					m_pImpl->m_writeRec->m_count = std::stoi(str);
			}
		}
		else if (m_pImpl->m_writeString->size() > 0)
		{
			m_pImpl->m_writeString->pop_back();
			m_pImpl->m_writeText->setString(*m_pImpl->m_writeString);
		}

		if (m_pImpl->m_writeString == &m_pImpl->m_search)
		{
			if (m_pImpl->m_writeString->size() == 0)
				if (m_pImpl->m_writeString == &m_pImpl->m_search)
					((sf::Sprite*)m_pImpl->m_objects[Object::Clear])->setPosition(-100, 0);
		}
		else
			((sf::Sprite*)m_pImpl->m_objects[Object::Save])->setPosition(1250, 685);
	}
	else if (m_pImpl->m_writeString == nullptr || m_pImpl->m_writeString == &m_pImpl->m_toAddStrings[3] || m_pImpl->m_writeString == &m_pImpl->m_toAddStrings[4] || m_pImpl->m_writeString == &m_pImpl->m_toAddStrings[5])
	{
		if ((code >= 48 && code <= 57) || code == 46 || code == 44)
		{
			std::string* str;

			if (m_pImpl->m_writeString == nullptr)
				str = new std::string(m_pImpl->m_writeText->getString());
			else
				str = m_pImpl->m_writeString;

			if (code == 46 || code == 44)
			{
				if (str->find('.') == std::string::npos)
					*str += '.';
			}
			else
			{
				if (*str == "0")
				{
					str->pop_back();
					*str += static_cast<char>(code);
				}
				else if (int pos = str->find('.'); pos != std::string::npos)
				{
					if (str->size() - 3 != pos)
						*str += static_cast<char>(code);
					else
						return;
				}
				else
					*str += static_cast<char>(code);
			}

			if (m_pImpl->m_writeString == nullptr)
			{
				((sf::Sprite*)m_pImpl->m_objects[Object::Save])->setPosition(1250, 685);

				m_pImpl->m_writeText->setString(*str);

				if (m_pImpl->m_writeText == &m_pImpl->m_writeRec->m_countOfPagesTx)
					m_pImpl->m_writeRec->m_countOfPages = std::stoi(m_pImpl->m_writeText->getString().toWideString());
				else if (m_pImpl->m_writeText == &m_pImpl->m_writeRec->m_priceTx)
					m_pImpl->m_writeRec->m_price = std::stof(m_pImpl->m_writeRec->m_priceTx.getString().toWideString());
				else if (m_pImpl->m_writeText == &m_pImpl->m_writeRec->m_countTx)
					m_pImpl->m_writeRec->m_count = std::stoi(m_pImpl->m_writeRec->m_countTx.getString().toWideString());
			}
			else
			{
				m_pImpl->m_writeText->setString(*str);
			}
		}
	}
	else if (code < 128)
	{
		if (code == 32)
		{
			std::string str = *m_pImpl->m_writeString;

			str.erase(std::remove_if(str.begin(), str.end(), isspace), str.end());

			if (str == "")
				return;
		}

		if (m_pImpl->m_writeString == &m_pImpl->m_search)
			((sf::Sprite*)m_pImpl->m_objects[Object::Clear])->setPosition(687, 30);
		else if (m_pImpl->m_objects.size() <= 31)
			((sf::Sprite*)m_pImpl->m_objects[Object::Save])->setPosition(1250, 685);

		if (m_pImpl->m_writeString->size() < 30)
		{
			*m_pImpl->m_writeString += static_cast<char>(code);
			m_pImpl->m_writeText->setString(*m_pImpl->m_writeString);
		}
	}

	FixSize();
}

void ArchiveSystem::CheckMousePos()
{
	if (sf::IntRect(((sf::Sprite*)m_pImpl->m_objects[Object::Delete])->getGlobalBounds()).contains(sf::Mouse::getPosition(m_window)))
	{
		((sf::Sprite*)m_pImpl->m_objects[Object::Delete])->setScale(1.1, 1.1);
	}
	else
		((sf::Sprite*)m_pImpl->m_objects[Object::Delete])->setScale(1, 1);

	if (sf::IntRect(((sf::Sprite*)m_pImpl->m_objects[Object::Add])->getGlobalBounds()).contains(sf::Mouse::getPosition(m_window)))
	{
		((sf::Sprite*)m_pImpl->m_objects[Object::Add])->setScale(1.1, 1.1);
	}
	else
		((sf::Sprite*)m_pImpl->m_objects[Object::Add])->setScale(1, 1);

	if (sf::IntRect(((sf::Sprite*)m_pImpl->m_objects[Object::GoUp + 2])->getGlobalBounds()).contains(sf::Mouse::getPosition(m_window)))
	{
		((sf::Sprite*)m_pImpl->m_objects[Object::GoUp + 2])->setScale(1.1, 1.1);
	}
	else
		((sf::Sprite*)m_pImpl->m_objects[Object::GoUp + 2])->setScale(1, 1);

	if (sf::IntRect(((sf::Sprite*)m_pImpl->m_objects[Object::Clear])->getGlobalBounds()).contains(sf::Mouse::getPosition(m_window)))
	{
		((sf::Sprite*)m_pImpl->m_objects[Object::Clear])->setScale(1.1, 1.1);
	}
	else
		((sf::Sprite*)m_pImpl->m_objects[Object::Clear])->setScale(1, 1);

	if (sf::IntRect(((sf::Sprite*)m_pImpl->m_objects[Object::Save])->getGlobalBounds()).contains(sf::Mouse::getPosition(m_window)))
	{
		((sf::Sprite*)m_pImpl->m_objects[Object::Save])->setScale(1.1, 1.1);
	}
	else
		((sf::Sprite*)m_pImpl->m_objects[Object::Save])->setScale(1, 1);

	if (sf::IntRect(((sf::Sprite*)m_pImpl->m_objects[Object::GoUp])->getGlobalBounds()).contains(sf::Mouse::getPosition(m_window)))
	{
		((sf::Sprite*)m_pImpl->m_objects[Object::GoUp])->setScale(1.1, 1.1);
	}
	else
		((sf::Sprite*)m_pImpl->m_objects[Object::GoUp])->setScale(1, 1);

	if (sf::IntRect(((sf::Sprite*)m_pImpl->m_objects[Object::Save])->getGlobalBounds()).contains(sf::Mouse::getPosition(m_window)))
	{
		((sf::Sprite*)m_pImpl->m_objects[Object::Save])->setScale(1.1, 1.1);
	}
	else
		((sf::Sprite*)m_pImpl->m_objects[Object::Save])->setScale(1, 1);

	if (m_pImpl->m_search != "")
		if (sf::IntRect(((sf::Sprite*)m_pImpl->m_objects[Object::GoUp + 3])->getGlobalBounds()).contains(sf::Mouse::getPosition(m_window)))
		{
			((sf::Sprite*)m_pImpl->m_objects[Object::GoUp + 3])->setScale(1.1, 1.1);
		}
		else
			((sf::Sprite*)m_pImpl->m_objects[Object::GoUp + 3])->setScale(1, 1);
}

void ArchiveSystem::FixSize()
{
	if (m_pImpl->m_writeRec != nullptr)
	{
		if (m_pImpl->m_writeString != nullptr)
		{
			if (m_pImpl->m_writeString == &m_pImpl->m_writeRec->m_name || m_pImpl->m_writeString == &m_pImpl->m_writeRec->m_author)
			{
				if (m_pImpl->m_writeText->getGlobalBounds().width >= 240)
				{
					m_pImpl->m_writeString->pop_back();
					m_pImpl->m_writeText->setString(*m_pImpl->m_writeString);
				}
			}
			else if (m_pImpl->m_writeString == &m_pImpl->m_writeRec->m_genre)
			{
				if (m_pImpl->m_writeText->getGlobalBounds().width >= 190)
				{
					m_pImpl->m_writeString->pop_back();
					m_pImpl->m_writeText->setString(*m_pImpl->m_writeString);
				}
			}
		}
		else
		{
			if (m_pImpl->m_writeText == &m_pImpl->m_writeRec->m_countOfPagesTx || m_pImpl->m_writeText == &m_pImpl->m_writeRec->m_priceTx || m_pImpl->m_writeText == &m_pImpl->m_writeRec->m_countTx)
			{
				if (m_pImpl->m_writeText->getGlobalBounds().width >= 90)
				{
					std::string str = m_pImpl->m_writeText->getString();
					str.pop_back();

					if (m_pImpl->m_writeText == &m_pImpl->m_writeRec->m_countOfPagesTx)
					{
						m_pImpl->m_writeRec->m_countOfPages = std::stoi(str);
						m_pImpl->m_writeText->setString(str);
					}
					else if (m_pImpl->m_writeText == &m_pImpl->m_writeRec->m_priceTx)
					{
						m_pImpl->m_writeRec->m_countOfPages = std::stof(str);
						m_pImpl->m_writeText->setString(str);
					}
					else if (m_pImpl->m_writeText == &m_pImpl->m_writeRec->m_countTx)
					{
						m_pImpl->m_writeRec->m_countOfPages = std::stoi(str);
						m_pImpl->m_writeText->setString(str);
					}
				}
			}
		}
	}
	else
	{
		if (m_pImpl->m_writeString == &m_pImpl->m_search)
		{
			if (m_pImpl->m_writeText->getGlobalBounds().width >= 490)
			{
				m_pImpl->m_writeString->pop_back();
				m_pImpl->m_writeText->setString(*m_pImpl->m_writeString);
			}
		}
		else if (m_pImpl->m_writeString == &m_pImpl->m_toAddStrings[0] || m_pImpl->m_writeString == &m_pImpl->m_toAddStrings[1])
		{
			if (m_pImpl->m_writeText->getGlobalBounds().width >= 240)
			{
				m_pImpl->m_writeString->pop_back();
				m_pImpl->m_writeText->setString(*m_pImpl->m_writeString);
			}
		}
		else if (m_pImpl->m_writeString == &m_pImpl->m_toAddStrings[2])
		{
			if (m_pImpl->m_writeText->getGlobalBounds().width >= 190)
			{
				m_pImpl->m_writeString->pop_back();
				m_pImpl->m_writeText->setString(*m_pImpl->m_writeString);
			}
		}
		else if (m_pImpl->m_writeString == &m_pImpl->m_toAddStrings[3] || m_pImpl->m_writeString == &m_pImpl->m_toAddStrings[4] || m_pImpl->m_writeString == &m_pImpl->m_toAddStrings[5])
		{
			if (m_pImpl->m_writeText->getGlobalBounds().width >= 90)
			{
				m_pImpl->m_writeString->pop_back();
				m_pImpl->m_writeText->setString(*m_pImpl->m_writeString);
			}
		}
	}
}

void ArchiveSystem::Move(const bool up)
{
	if (up)
		if (m_pImpl->m_recordings.begin()->getGlobalBounds().top >= 130)
			return;

	if (up)
		((sf::RectangleShape*)m_pImpl->m_objects[0])->move(0, 15);
	else
		((sf::RectangleShape*)m_pImpl->m_objects[0])->move(0, -15);

	for (auto& i : m_pImpl->m_recordings)
	{
		i.Move(up);
	}

	if (m_pImpl->m_recordings.begin()->getGlobalBounds().top >= 130)
		((sf::Sprite*)m_pImpl->m_objects[Object::GoUp])->setPosition(-100, 0);
	else
		((sf::Sprite*)m_pImpl->m_objects[Object::GoUp])->setPosition(56, 666);
}

void ArchiveSystem::Add()
{
	sf::RectangleShape* menu = new sf::RectangleShape(sf::Vector2f(500, 400));
	menu->setOrigin(250, 200);
	menu->setPosition(640, 360);
	menu->setOutlineThickness(4);
	menu->setOutlineColor(sf::Color::Black);
	menu->setFillColor(sf::Color(100, 100, 100));

	m_pImpl->m_objects.push_back(menu);

	sf::Text* addBook = new sf::Text("Add book", m_pImpl->m_font, 30);
	addBook->setFillColor(sf::Color::Black);
	addBook->setPosition(580, 160);
	sf::Text* name = new sf::Text("Name", m_pImpl->m_font, 24);
	name->setOrigin(name->getGlobalBounds().width / 2, name->getGlobalBounds().height / 2);
	name->setFillColor(sf::Color::Black);
	name->setPosition(540, 210);
	sf::Text* author = new sf::Text("Author", m_pImpl->m_font, 24);
	author->setOrigin(author->getGlobalBounds().width / 2, author->getGlobalBounds().height / 2);
	author->setFillColor(sf::Color::Black);
	author->setPosition(540, 310);
	sf::Text* genre = new sf::Text("Genre", m_pImpl->m_font, 24);
	genre->setOrigin(genre->getGlobalBounds().width / 2, genre->getGlobalBounds().height / 2);
	genre->setFillColor(sf::Color::Black);
	genre->setPosition(540, 410);
	sf::Text* countOfPages = new sf::Text("Pages", m_pImpl->m_font, 24);
	countOfPages->setOrigin(countOfPages->getGlobalBounds().width / 2, countOfPages->getGlobalBounds().height / 2);
	countOfPages->setFillColor(sf::Color::Black);
	countOfPages->setPosition(815, 210);
	sf::Text* price = new sf::Text("Price", m_pImpl->m_font, 24);
	price->setOrigin(price->getGlobalBounds().width / 2, price->getGlobalBounds().height / 2);
	price->setFillColor(sf::Color::Black);
	price->setPosition(815, 310);
	sf::Text* count = new sf::Text("Count", m_pImpl->m_font, 24);
	count->setOrigin(count->getGlobalBounds().width / 2, count->getGlobalBounds().height / 2);
	count->setFillColor(sf::Color::Black);
	count->setPosition(815, 410);

	m_pImpl->m_objects.push_back(addBook);
	m_pImpl->m_objects.push_back(name);
	m_pImpl->m_objects.push_back(author);
	m_pImpl->m_objects.push_back(genre);
	m_pImpl->m_objects.push_back(countOfPages);
	m_pImpl->m_objects.push_back(price);
	m_pImpl->m_objects.push_back(count);

	sf::RectangleShape* nameForm = new sf::RectangleShape(sf::Vector2f(250, 30));
	nameForm->setPosition(415, name->getPosition().y + name->getGlobalBounds().height + 15);
	nameForm->setOutlineThickness(1);
	nameForm->setOutlineColor(sf::Color::Black);
	nameForm->setFillColor(sf::Color(230, 230, 230));
	sf::RectangleShape* authorForm = new sf::RectangleShape(sf::Vector2f(250, 30));
	authorForm->setPosition(415, author->getPosition().y + author->getGlobalBounds().height + 15);
	authorForm->setOutlineThickness(1);
	authorForm->setOutlineColor(sf::Color::Black);
	authorForm->setFillColor(sf::Color(230, 230, 230));
	sf::RectangleShape* genreForm = new sf::RectangleShape(sf::Vector2f(200, 30));
	genreForm->setPosition(440, genre->getPosition().y + genre->getGlobalBounds().height + 15);
	genreForm->setOutlineThickness(1);
	genreForm->setOutlineColor(sf::Color::Black);
	genreForm->setFillColor(sf::Color(230, 230, 230));
	sf::RectangleShape* countPagForm = new sf::RectangleShape(sf::Vector2f(100, 30));
	countPagForm->setPosition(765, name->getPosition().y + name->getGlobalBounds().height + 15);
	countPagForm->setOutlineThickness(1);
	countPagForm->setOutlineColor(sf::Color::Black);
	countPagForm->setFillColor(sf::Color(230, 230, 230));
	sf::RectangleShape* priceForm = new sf::RectangleShape(sf::Vector2f(100,30));
	priceForm->setPosition(765, price->getPosition().y + price->getGlobalBounds().height + 15);
	priceForm->setOutlineThickness(1);
	priceForm->setOutlineColor(sf::Color::Black);
	priceForm->setFillColor(sf::Color(230, 230, 230));
	sf::RectangleShape* countForm = new sf::RectangleShape(sf::Vector2f(100, 30));
	countForm->setPosition(765, count->getPosition().y + count->getGlobalBounds().height + 15);
	countForm->setOutlineThickness(1);
	countForm->setOutlineColor(sf::Color::Black);
	countForm->setFillColor(sf::Color(230, 230, 230));
	sf::RectangleShape* addForm = new sf::RectangleShape(sf::Vector2f(75, 30));
	addForm->setPosition(605, 515);
	addForm->setOutlineThickness(3);
	addForm->setOutlineColor(sf::Color::Black);
	addForm->setFillColor(sf::Color::White);

	m_pImpl->m_objects.push_back(nameForm);
	m_pImpl->m_objects.push_back(authorForm);
	m_pImpl->m_objects.push_back(genreForm);
	m_pImpl->m_objects.push_back(countPagForm);
	m_pImpl->m_objects.push_back(priceForm);
	m_pImpl->m_objects.push_back(countForm);
	m_pImpl->m_objects.push_back(addForm);

	sf::Text* nameTX = new sf::Text("", m_pImpl->m_font, 20);
	nameTX->setFillColor(sf::Color::Black);
	nameTX->setPosition(nameForm->getPosition().x + 5, nameForm->getPosition().y);
	sf::Text* authorTX = new sf::Text("", m_pImpl->m_font, 20);
	authorTX->setFillColor(sf::Color::Black);
	authorTX->setPosition(authorForm->getPosition().x + 5, authorForm->getPosition().y);
	sf::Text* genreTX = new sf::Text("", m_pImpl->m_font, 20);
	genreTX->setFillColor(sf::Color::Black);
	genreTX->setPosition(genreForm->getPosition().x + 5, genreForm->getPosition().y);
	sf::Text* countOfPagesTX = new sf::Text("", m_pImpl->m_font, 20);
	countOfPagesTX->setFillColor(sf::Color::Black);
	countOfPagesTX->setPosition(countPagForm->getPosition().x + 5, countPagForm->getPosition().y);
	sf::Text* priceTX = new sf::Text("", m_pImpl->m_font, 20);
	priceTX->setFillColor(sf::Color::Black);
	priceTX->setPosition(priceForm->getPosition().x + 5, priceForm->getPosition().y);
	sf::Text* countTX = new sf::Text("", m_pImpl->m_font, 20);
	countTX->setFillColor(sf::Color::Black);
	countTX->setPosition(countForm->getPosition().x + 5, countForm->getPosition().y);
	sf::Text* add = new sf::Text("Add", m_pImpl->m_font, 30);
	add->setFillColor(sf::Color::Black);
	add->setPosition(615, 510);

	m_pImpl->m_objects.push_back(nameTX);
	m_pImpl->m_objects.push_back(authorTX);
	m_pImpl->m_objects.push_back(genreTX);
	m_pImpl->m_objects.push_back(countOfPagesTX);
	m_pImpl->m_objects.push_back(priceTX);
	m_pImpl->m_objects.push_back(countTX);
	m_pImpl->m_objects.push_back(add);
}

void ArchiveSystem::sort(const int& fieldPos)
{
	if (m_pImpl->m_sorted != fieldPos)
		m_pImpl->m_recordings.sort([&](const Recording& a, const Recording& b)
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
					return a.m_price > b.m_price;
				}
				if (fieldPos == 6)
				{
					m_pImpl->m_sorted = 6;
					return a.m_count < b.m_count;
				}
			});
	else
		m_pImpl->m_recordings.sort([&](const Recording& a, const Recording& b)
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
					return a.m_price < b.m_price;
				}
				if (fieldPos == 6)
				{
					m_pImpl->m_sorted = -1;
					return a.m_count > b.m_count;
				}
			});
}

void ArchiveSystem::Save()
{
	try
	{
		m_pImpl->m_con = m_pImpl->m_driver->connect("localhost", "root", "12222550g");
	}
	catch (sql::SQLException e)
	{
		std::cerr << "Could connect to server. Error massage: " << e.what() << std::endl;
		system("pause");
		exit(1);
	}

	m_pImpl->m_con->setSchema("bookstore");

	m_pImpl->m_prestat = m_pImpl->m_con->prepareStatement("SELECT 1 FROM books WHERE ID = ?");
	sql::PreparedStatement* insert = m_pImpl->m_con->prepareStatement("INSERT INTO books(ID, name, author, genre, pages, price, count) Values(?, ?, ?, ?, ?, ?, ?)");

	for (auto i : m_pImpl->m_recordings)
	{
		m_pImpl->m_prestat->setInt(1, i.GetID());

		m_pImpl->m_res = m_pImpl->m_prestat->executeQuery();

		if (m_pImpl->m_res->next())
		{
			continue;
		}
		else
		{
			insert->setInt(1, i.m_id);
			insert->setString(2, i.m_name);
			insert->setString(3, i.m_author);
			insert->setString(4, i.m_genre);
			insert->setInt(5, i.m_countOfPages);
			insert->setInt(6, i.m_price);
			insert->setInt(7, i.m_count);

			insert->executeUpdate();
		}
	}

	delete m_pImpl->m_prestat;
	delete insert;
	if (m_pImpl->m_res != nullptr)
		delete m_pImpl->m_res;

	m_pImpl->m_prestat = m_pImpl->m_con->prepareStatement("DELETE FROM books WHERE ID = ?");
	m_pImpl->m_stat = m_pImpl->m_con->createStatement();
	m_pImpl->m_res = m_pImpl->m_stat->executeQuery("SELECT ID FROM books");

	while (m_pImpl->m_res->next())
	{
		if (std::find_if_not(m_pImpl->m_recordings.begin(), m_pImpl->m_recordings.end(), [&](Recording& a) {
			return m_pImpl->m_res->getInt(1) != a.GetID();
			}) == m_pImpl->m_recordings.end())
		{
			m_pImpl->m_prestat->setInt(1, m_pImpl->m_res->getInt(1));
			m_pImpl->m_prestat->executeUpdate();
		}
	}

	delete m_pImpl->m_stat;
	delete m_pImpl->m_prestat;
	if (m_pImpl->m_res != nullptr)
		delete m_pImpl->m_res;

	m_pImpl->m_prestat = m_pImpl->m_con->prepareStatement("UPDATE books SET name = ?, author = ?, genre = ?, pages = ?, price = ?, count = ? WHERE ID = ?");

	for (auto i : m_pImpl->m_recordings)
	{
		m_pImpl->m_prestat->setString(1, i.m_name);
		m_pImpl->m_prestat->setString(2, i.m_author);
		m_pImpl->m_prestat->setString(3, i.m_genre);
		m_pImpl->m_prestat->setInt(4, i.m_countOfPages);
		m_pImpl->m_prestat->setDouble(5, i.m_price);
		m_pImpl->m_prestat->setInt(6, i.m_count);
		m_pImpl->m_prestat->setInt(7, i.m_id);

		m_pImpl->m_prestat->executeUpdate();
	}

	delete m_pImpl->m_prestat;
	delete m_pImpl->m_con;

	m_pImpl->m_con = nullptr;
	m_pImpl->m_stat = nullptr;
	m_pImpl->m_res = nullptr;
}

bool ArchiveSystem::AddBook()
{
	auto np = m_pImpl->m_toAddStrings[0];
	auto ap = m_pImpl->m_toAddStrings[1];
	auto gp = m_pImpl->m_toAddStrings[2];
	auto cpp = m_pImpl->m_toAddStrings[3];
	auto pp = m_pImpl->m_toAddStrings[4];
	auto cp = m_pImpl->m_toAddStrings[5];
	np.erase(std::remove_if(np.begin(), np.end(), isspace), np.end());
	ap.erase(std::remove_if(ap.begin(), ap.end(), isspace), ap.end());
	gp.erase(std::remove_if(gp.begin(), gp.end(), isspace), gp.end());
	cpp.erase(std::remove_if(cpp.begin(), cpp.end(), isspace), cpp.end());
	pp.erase(std::remove_if(pp.begin(), pp.end(), isspace), pp.end());
	cp.erase(std::remove_if(cp.begin(), cp.end(), isspace), cp.end());

	if (np == "" && ap == "" && gp == "" && cpp == "" && pp == "" && cp == "")
		return false;

	m_pImpl->m_recordings.push_back(Recording(m_pImpl->m_recordings.back().GetID() + 1, m_pImpl->m_toAddStrings[0], m_pImpl->m_toAddStrings[1], m_pImpl->m_toAddStrings[2],
		std::stoi(m_pImpl->m_toAddStrings[3]), std::stof(m_pImpl->m_toAddStrings[4]), std::stoi(m_pImpl->m_toAddStrings[5]), m_pImpl->m_font));

	for (int i = 0; i < 6; i++)
		m_pImpl->m_toAddStrings[i].clear();

	m_pImpl->m_writeString = nullptr;
	m_pImpl->m_writeText = nullptr;
	m_pImpl->m_writeRec = nullptr;

	while (m_pImpl->m_objects.size() > 31)
		m_pImpl->m_objects.pop_back();

	return true;
}

void ArchiveSystem::DeleteBook()
{
	auto it = std::find_if(m_pImpl->m_recordings.begin(), m_pImpl->m_recordings.end(), [&](const Recording& a) {
		return m_pImpl->m_rec->GetID() == a.GetID();
		});

	m_pImpl->m_recordings.erase(it);
}

bool ArchiveSystem::Exit()
{
	sf::RectangleShape* exit = new sf::RectangleShape(sf::Vector2f(500, 300));
	exit->setOrigin(250, 150);
	exit->setPosition(640, 360);
	exit->setOutlineThickness(3);
	exit->setOutlineColor(sf::Color::Black);
	exit->setFillColor(sf::Color(120, 120, 120));

	sf::RectangleShape* no = new sf::RectangleShape(sf::Vector2f(150, 50));
	no->setOrigin(75, 25);
	no->setPosition(520, 460);
	no->setOutlineThickness(3);
	no->setOutlineColor(sf::Color::Black);
	no->setFillColor(sf::Color(100, 100, 100));

	sf::RectangleShape* yes = new sf::RectangleShape(sf::Vector2f(150, 50));
	yes->setOrigin(75, 25);
	yes->setPosition(760, 460);
	yes->setOutlineThickness(3);
	yes->setOutlineColor(sf::Color::Black);
	yes->setFillColor(sf::Color(100, 100, 100));

	sf::Text* noTX = new sf::Text("No", m_pImpl->m_font);
	noTX->setFillColor(sf::Color::Black);
	noTX->setOrigin(noTX->getGlobalBounds().width / 2, noTX->getGlobalBounds().height / 2);
	noTX->setPosition(520, 450);

	sf::Text* yesTX = new sf::Text("Yes", m_pImpl->m_font);
	yesTX->setFillColor(sf::Color::Black);
	yesTX->setOrigin(yesTX->getGlobalBounds().width / 2, yesTX->getGlobalBounds().height / 2);
	yesTX->setPosition(760, 450);

	sf::Text* exitTX = new sf::Text("Do you want to save the files?", m_pImpl->m_font);
	exitTX->setFillColor(sf::Color::Black);
	exitTX->setOrigin(exitTX->getGlobalBounds().width / 2, exitTX->getGlobalBounds().height / 2);
	exitTX->setPosition(640, 320);

	if (m_pImpl->m_objects.size() > 31)
	{
		for (int i = 0; i < 6; i++)
			m_pImpl->m_toAddStrings[i].clear();

		m_pImpl->m_writeString = nullptr;
		m_pImpl->m_writeText = nullptr;
		m_pImpl->m_writeRec = nullptr;

		while (m_pImpl->m_objects.size() > 31)
			m_pImpl->m_objects.pop_back();
	}

	m_pImpl->m_objects.push_back(exit);
	m_pImpl->m_objects.push_back(no);
	m_pImpl->m_objects.push_back(yes);
	m_pImpl->m_objects.push_back(noTX);
	m_pImpl->m_objects.push_back(yesTX);
	m_pImpl->m_objects.push_back(exitTX);

	sf::Event event;
	while(m_window.isOpen())
	{
		while (m_window.pollEvent(event))
		{
			if (event.type == sf::Event::MouseButtonPressed && event.key.code == sf::Mouse::Left)
			{
				if (sf::IntRect(no->getGlobalBounds()).contains(sf::Mouse::getPosition(m_window)))
				{
					m_window.close();
					return true;
				}
				else if (sf::IntRect(yes->getGlobalBounds()).contains(sf::Mouse::getPosition(m_window)))
				{
					Save();
					m_window.close();
					return true;
				}
				else if (!sf::IntRect(exit->getGlobalBounds()).contains(sf::Mouse::getPosition(m_window)))
				{
					while (m_pImpl->m_objects.size() > 31)
						m_pImpl->m_objects.pop_back();

					return false;
				}
			}
		}
		
		Draw();
	}

	return false;
}

void ArchiveSystem::Draw()
{
	m_window.clear(sf::Color(210, 210, 210));

	m_window.draw(*m_pImpl->m_objects[1]);

	{
		sf::RectangleShape* i = (sf::RectangleShape*)(m_pImpl->m_objects[0]);
		int startPos = i->getGlobalBounds().top;
		while (i->getGlobalBounds().top < 680)
		{
			if (i->getPosition().y >= 100)
				m_window.draw(*i);
			i->setPosition(i->getGlobalBounds().left + i->getOrigin().x, i->getGlobalBounds().top + 35);
		}
		i->setPosition(i->getGlobalBounds().left + i->getOrigin().x, startPos);
	}

	for (auto i : m_pImpl->m_recordings)
	{
		m_window.draw(i);
	}

	for (int i = 2; i < m_pImpl->m_objects.size(); i++)
	{
		m_window.draw(*m_pImpl->m_objects[i]);
	}
	
	m_window.display();
}

void ArchiveSystem::Do()
{
	sf::Event event;
	while (m_window.isOpen())
	{
		while (m_window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				if (Exit())
					return;
			}
			if (event.type == sf::Event::MouseButtonPressed && event.key.code == sf::Mouse::Left)
			{
				CheckClick();
			}
			if (event.type == sf::Event::MouseWheelMoved)
			{
				if (!m_pImpl->m_recordings.empty())
				{
					if (event.mouseWheel.delta == 1)   //up
						Move(true);
					else if (event.mouseWheel.delta == -1)   //down
						Move(false);
				}
			}
			if (event.type == sf::Event::TextEntered)
			{
				CheckEnteringText(event.text.unicode);
			}
		}

		CheckMousePos();

		if (!sf::IntRect(((sf::Sprite*)m_pImpl->m_objects[Object::Minus])->getPosition().x - 10, ((sf::Sprite*)m_pImpl->m_objects[Object::Minus])->getPosition().y, 74, 42).contains(sf::Mouse::getPosition(m_window)) &&
			!sf::IntRect(((sf::Sprite*)m_pImpl->m_objects[Object::Delete])->getPosition().x - 16, ((sf::Sprite*)m_pImpl->m_objects[Object::Delete])->getPosition().y - 16, 42, 32).contains(sf::Mouse::getPosition(m_window)))
		{
			m_pImpl->m_rec = nullptr;
			((sf::Sprite*)m_pImpl->m_objects[Object::Plus])->setPosition(-100, 0);
			((sf::Sprite*)m_pImpl->m_objects[Object::Minus])->setPosition(-100, 0);
			((sf::Sprite*)m_pImpl->m_objects[Object::Delete])->setPosition(-100, 0);

			for (auto i : m_pImpl->m_recordings)
			{
				if (i.getGlobalBounds().top >= 100)
				{
					if (sf::IntRect(i.getGlobalBounds()).contains(sf::Mouse::getPosition(m_window)))
					{
						m_pImpl->m_rec = &i;
						((sf::Sprite*)m_pImpl->m_objects[Object::Minus])->setPosition(1105, i.getGlobalBounds().top);
						((sf::Sprite*)m_pImpl->m_objects[Object::Plus])->setPosition(1137, i.getGlobalBounds().top);
						((sf::Sprite*)m_pImpl->m_objects[Object::Delete])->setPosition(80, i.getGlobalBounds().top + 16);
						break;
					}
				}
			}
		}

		Draw();
	}
}
