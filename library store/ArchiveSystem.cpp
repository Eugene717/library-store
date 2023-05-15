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
	Add, Minus, Plus, Save, GoUp, Clear, SearchText = 29
};

struct ArchiveIMPL
{
	sf::Font m_font;
	std::list<Recording> m_recordings;
	std::vector<sf::Drawable*> m_objects;
	int m_sorted;
	bool m_displayOnlyAvailable;
	bool m_writeField;
	std::string m_search;
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

	m_pImpl->m_sorted = -1;
	m_pImpl->m_displayOnlyAvailable = false;
	m_pImpl->m_writeField = false;
	m_pImpl->m_search = "";
	m_pImpl->m_writeString = nullptr;
	m_pImpl->m_writeText = nullptr;

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
	//s_save->setPosition(1250, 685);
	sf::Sprite* s_goUp = new sf::Sprite(*t_goUp);
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
		if (m_pImpl->m_search != "")
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

void ArchiveSystem::CheckClick(int recording)
{
	if (m_pImpl->m_writeField)
	{
		m_pImpl->m_writeField = false;
		m_pImpl->m_writeString = nullptr;
	}

	int object = -1;

	for (int i = Object::Add; i <= Object::Clear; i++)
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
							((sf::Sprite*)m_pImpl->m_objects[20])->setPosition(1082, 40);
							m_pImpl->m_displayOnlyAvailable = true;
						}
						else
						{
							((sf::Sprite*)m_pImpl->m_objects[20])->setPosition(-100, 0);
							m_pImpl->m_displayOnlyAvailable = false;
						}
						PlaceElements();
					}
				if (object == Object::SearchField)
					if (sf::IntRect(((sf::RectangleShape*)m_pImpl->m_objects[object])->getGlobalBounds()).contains(sf::Mouse::getPosition(m_window)))
					{
						m_pImpl->m_writeField = true;
						m_pImpl->m_writeString = &m_pImpl->m_search;
						m_pImpl->m_writeText = (sf::Text*)m_pImpl->m_objects[Object::SearchText];
					}
				if (object == Object::Search)
					if (sf::IntRect(((sf::RectangleShape*)m_pImpl->m_objects[object])->getGlobalBounds()).contains(sf::Mouse::getPosition(m_window)))
					{
						m_pImpl->m_writeField = false;
						m_pImpl->m_writeString = nullptr;
						m_pImpl->m_writeText = nullptr;
						if (m_pImpl->m_search != "")
							((sf::Sprite*)m_pImpl->m_objects[Object::Clear])->setPosition(687, 30);
						PlaceElements();
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
						m_pImpl->m_writeField = true;
						m_pImpl->m_writeString = &m_pImpl->m_search;
						m_pImpl->m_writeText = (sf::Text*)m_pImpl->m_objects[Object::SearchText];
						((sf::Sprite*)m_pImpl->m_objects[object])->setPosition(-100, 0);

						m_pImpl->m_writeString->clear();
						m_pImpl->m_writeText->setString(*m_pImpl->m_writeString);
						PlaceElements();
					}
				if (object == Object::Add)
					if (sf::IntRect(((sf::Sprite*)m_pImpl->m_objects[object])->getGlobalBounds()).contains(sf::Mouse::getPosition(m_window)))
					{
						((sf::Sprite*)m_pImpl->m_objects[object])->setScale(1, 1);
						Add();
						PlaceElements();
					}
				if (object == Object::Minus || object == Object::Plus)
					if (recording != -1)
						if (sf::IntRect(((sf::Sprite*)m_pImpl->m_objects[object])->getGlobalBounds()).contains(sf::Mouse::getPosition(m_window)))
						{
							SoldOrBuyBook(object % 2, recording);
							PlaceElements();
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

void ArchiveSystem::Move(const bool up)
{
	if (up)
		if (m_pImpl->m_recordings.begin()->GetY() >= 130)
			return;

	if (up)
		((sf::RectangleShape*)m_pImpl->m_objects[0])->move(0, 15);
	else
		((sf::RectangleShape*)m_pImpl->m_objects[0])->move(0, -15);

	for (auto& i : m_pImpl->m_recordings)
	{
		i.Move(up);
	}

	if (m_pImpl->m_recordings.begin()->GetY() >= 130)
		((sf::Sprite*)m_pImpl->m_objects[Object::GoUp])->setPosition(-100, 0);
	else
		((sf::Sprite*)m_pImpl->m_objects[Object::GoUp])->setPosition(50, 650);
}

void ArchiveSystem::SoldOrBuyBook(const bool sold, const int i)
{
	auto l_iter = m_pImpl->m_recordings.begin();
	std::advance(l_iter, i);

	l_iter->SoldOrBuyBook(sold);
}

bool ArchiveSystem::Add()
{
	std::string name_str, author_str, genre_str, countPag_str, price_str, count_str;
	bool name_enter = false, author_enter = false, genre_enter = false, countPag_enter = false, price_enter = false, count_enter = false;

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
	sf::Text price("Price:", m_pImpl->m_font, 24);
	price.setFillColor(sf::Color::Black);
	price.setPosition(400, 400);
	sf::Text count("Count:", m_pImpl->m_font, 24);
	count.setFillColor(sf::Color::Black);
	count.setPosition(400, 450);
	sf::Text add("Add", m_pImpl->m_font, 30);
	add.setFillColor(sf::Color::Black);
	add.setPosition(615, 500);

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
	sf::RectangleShape priceForm;
	priceForm.setSize(sf::Vector2f(150, 30));
	priceForm.setPosition(527, 403);
	priceForm.setOutlineThickness(2);
	priceForm.setOutlineColor(sf::Color::Black);
	sf::RectangleShape countForm;
	countForm.setSize(sf::Vector2f(150, 30));
	countForm.setPosition(477, 453);
	countForm.setOutlineThickness(2);
	countForm.setOutlineColor(sf::Color::Black);
	sf::RectangleShape addForm;
	addForm.setSize(sf::Vector2f(75, 32));
	addForm.setPosition(605, 503);
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
					price_enter = false;
					count_enter = false;
				}
				else if (sf::IntRect(authorForm.getGlobalBounds()).contains(sf::Mouse::getPosition(m_window)))
				{
					name_enter = false;
					author_enter = true;
					genre_enter = false;
					countPag_enter = false;
					price_enter = false;
					count_enter = false;
				}
				else if (sf::IntRect(genreForm.getGlobalBounds()).contains(sf::Mouse::getPosition(m_window)))
				{
					name_enter = false;
					author_enter = false;
					genre_enter = true;
					countPag_enter = false;
					price_enter = false;
					count_enter = false;
				}
				else if (sf::IntRect(countPagForm.getGlobalBounds()).contains(sf::Mouse::getPosition(m_window)))
				{
					name_enter = false;
					author_enter = false;
					genre_enter = false;
					countPag_enter = true;
					price_enter = false;
					count_enter = false;
				}
				else if (sf::IntRect(priceForm.getGlobalBounds()).contains(sf::Mouse::getPosition(m_window)))
				{
					name_enter = false;
					author_enter = false;
					genre_enter = false;
					countPag_enter = false;
					price_enter = true;
					count_enter = false;
				}
				else if (sf::IntRect(countForm.getGlobalBounds()).contains(sf::Mouse::getPosition(m_window)))
				{
					name_enter = false;
					author_enter = false;
					genre_enter = false;
					countPag_enter = false;
					price_enter = false;
					count_enter = true;
				}
				else if (sf::IntRect(addForm.getGlobalBounds()).contains(sf::Mouse::getPosition(m_window)))
				{
					auto np = name_str;
					auto ap = author_str;
					auto gp = genre_str;
					auto cpp = countPag_str;
					auto pp = price_str;
					auto cp = count_str;
					np.erase(std::remove_if(np.begin(), np.end(), isspace), np.end());
					ap.erase(std::remove_if(ap.begin(), ap.end(), isspace), ap.end());
					gp.erase(std::remove_if(gp.begin(), gp.end(), isspace), gp.end());
					cpp.erase(std::remove_if(cpp.begin(), cpp.end(), isspace), cpp.end());
					pp.erase(std::remove_if(pp.begin(), pp.end(), isspace), pp.end());
					cp.erase(std::remove_if(cp.begin(), cp.end(), isspace), cp.end());

					//if (np != "" && ap != "" && gp != "" && cpp != "" && pp != "" && cp != "")
						//m_pImpl->m_recordings.push_back(Recording(name_str, author_str, genre_str, std::stoi(countPag_str), std::stoi(price_str), std::stoi(count_str), m_pImpl->m_font));
				}
				else
				{
					name_enter = false;
					author_enter = false;
					genre_enter = false;
					countPag_enter = false;
					price_enter = false;
					count_enter = false;
				}
				if (!sf::IntRect(menu.getGlobalBounds()).contains(sf::Mouse::getPosition(m_window)))
					return false;
			}
			if (event.type == sf::Event::TextEntered && (name_enter || author_enter || genre_enter || countPag_enter || price_enter || count_enter))
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
				else if (price_enter)
					str = &price_str;
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
							else if (price_enter)
								price.setString("Price: " + *str);
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
							else if (price_enter)
								price.setString("Price: " + *str);
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
		m_window.draw(priceForm);
		m_window.draw(countForm);
		m_window.draw(addForm);

		m_window.draw(name);
		m_window.draw(author);
		m_window.draw(genre);
		m_window.draw(countOfPages);
		m_window.draw(price);
		m_window.draw(count);
		m_window.draw(add);
		m_window.draw(addBook);

		m_window.display();
	}
}

void ArchiveSystem::Edit(const int& i)
{
	/*auto l_iter = m_pImpl->m_recordings.begin();
	std::advance(l_iter, i);

	std::string name_str = l_iter->m_name, author_str = l_iter->m_author, genre_str = l_iter->m_genre, countPag_str = std::to_string(l_iter->m_countOfPages), 
		count_str = std::to_string(l_iter->m_count), countTaked_str = std::to_string(l_iter->m_price);
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
					m_pImpl->m_recordings.erase(l_iter);
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
	}*/
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

	delete m_pImpl->m_con;
	delete m_pImpl->m_prestat;
}

bool ArchiveSystem::Exit()
{
	if (true)
	{
		Save();
		m_window.close();
		return true;
	}

	return false;
}

void ArchiveSystem::Draw()
{
	m_window.clear(sf::Color::White);

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
	int n = -1;

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
				CheckClick(n);
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
				if (m_pImpl->m_writeString != nullptr)
				{
					if (event.text.unicode == 13)
					{
						if (m_pImpl->m_writeString == &m_pImpl->m_search)
						{
							((sf::Sprite*)m_pImpl->m_objects[Clear])->setPosition(687, 30);
							PlaceElements();
						}

						m_pImpl->m_writeField = false;
						m_pImpl->m_writeString = nullptr;
						m_pImpl->m_writeText = nullptr;
					}
					else if (event.text.unicode == '\b')
					{
						if (m_pImpl->m_writeString->size() > 0)
						{
							m_pImpl->m_writeString->pop_back();
							m_pImpl->m_writeText->setString(*m_pImpl->m_writeString);
						}

						if (m_pImpl->m_writeString->size() == 0)
							if (m_pImpl->m_writeString == &m_pImpl->m_search)
								((sf::Sprite*)m_pImpl->m_objects[Clear])->setPosition(-100, 0);
					}
					else if (event.text.unicode < 128)
					{
						if (m_pImpl->m_writeString == &m_pImpl->m_search)
							((sf::Sprite*)m_pImpl->m_objects[Clear])->setPosition(687, 30);

						if (m_pImpl->m_writeString->size() < 30)
						{
							*m_pImpl->m_writeString += static_cast<char>(event.text.unicode);
							m_pImpl->m_writeText->setString(*m_pImpl->m_writeString);
						}
					}
				}
			}
		}

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

		if (!sf::IntRect(((sf::Sprite*)m_pImpl->m_objects[Object::Minus])->getPosition().x - 10, ((sf::Sprite*)m_pImpl->m_objects[Object::Minus])->getPosition().y, 74, 42).contains(sf::Mouse::getPosition(m_window)))
		{
			n = -1;
			((sf::Sprite*)m_pImpl->m_objects[Object::Plus])->setPosition(-100, 0);
			((sf::Sprite*)m_pImpl->m_objects[Object::Minus])->setPosition(-100, 0);

			for (auto i : m_pImpl->m_recordings)
			{
				n++;
				if (sf::IntRect(i.getGlobalBounds()).contains(sf::Mouse::getPosition(m_window)))
				{
					((sf::Sprite*)m_pImpl->m_objects[Object::Minus])->setPosition(1105, i.GetY() - 5);
					((sf::Sprite*)m_pImpl->m_objects[Object::Plus])->setPosition(1137, i.GetY() - 5);
					break;
				}
			}
		}

		Draw();
	}
}
