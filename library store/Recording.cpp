#include "Recording.h"

Recording::Recording(const std::string& name, const std::string& author, const std::string& genre, const int& countOfPages, const int& count, const sf::Font& font, const int& countOfTaked) : m_name(name), m_author(author), m_genre(genre), m_countOfPages(countOfPages), m_count(count)
{
	m_countOfTaked = countOfTaked;
	//m_people = nullptr;
	m_openList = false;

	m_nameTx.setFont(font);
	m_nameTx.setFillColor(sf::Color::Black);
	m_nameTx.setCharacterSize(28);
	m_nameTx.setString(name);

	m_authorTx.setFont(font);
	m_authorTx.setFillColor(sf::Color::Black);
	m_authorTx.setCharacterSize(28);
	m_authorTx.setString(author);

	m_genreTx.setFont(font);
	m_genreTx.setFillColor(sf::Color::Black);
	m_genreTx.setCharacterSize(28);
	m_genreTx.setString(genre);

	m_countOfPagesTx.setFont(font);
	m_countOfPagesTx.setFillColor(sf::Color::Black);
	m_countOfPagesTx.setCharacterSize(28);
	m_countOfPagesTx.setString(std::to_string(countOfPages));

	m_countTx.setFont(font);
	m_countTx.setFillColor(sf::Color::Black);
	m_countTx.setCharacterSize(28);
	m_countTx.setString(std::to_string(count));

	m_countOfTakedTx.setFont(font);
	m_countOfTakedTx.setFillColor(sf::Color::Black);
	m_countOfTakedTx.setCharacterSize(28);
	m_countOfTakedTx.setString(std::to_string(countOfTaked));

	m_underline.setSize(sf::Vector2f(1180, 3));
	m_underline.setFillColor(sf::Color::Black);
}

int Recording::GetY()
{
	return m_nameTx.getGlobalBounds().top;
}

sf::FloatRect Recording::getGlobalBounds() const
{
	return sf::FloatRect(m_nameTx.getGlobalBounds().left - 6, m_nameTx.getGlobalBounds().top - 10, 1220, 33);
}

void Recording::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_nameTx);
	target.draw(m_authorTx);
	target.draw(m_genreTx);
	target.draw(m_countOfPagesTx);
	target.draw(m_countTx);
	target.draw(m_countOfTakedTx);
	target.draw(m_underline);
}

void Recording::Place(const int x, const int y)
{
	m_nameTx.setPosition(x + 5, y);
	m_authorTx.setPosition(x + 305, y);
	m_genreTx.setPosition(x + 600, y);
	m_countOfPagesTx.setPosition(x + 800, y);
	m_countTx.setPosition(x + 900, y);
	m_countOfTakedTx.setPosition(x + 1040, y);
	m_underline.setPosition(x, y + 35);
}

void Recording::Edit(const std::string& name, const std::string& author, const std::string& genre, const int& countOfPages, const int& count, const int& countOfTaked)
{
	m_name = name; m_author = author; m_genre = genre; m_countOfPages = countOfPages; m_count = count; m_countOfTaked = countOfTaked;
	
	m_nameTx.setString(name);
	m_authorTx.setString(author);
	m_genreTx.setString(genre);
	m_countOfPagesTx.setString(std::to_string(countOfPages));
	m_countTx.setString(std::to_string(count));
	m_countOfTakedTx.setString(std::to_string(countOfTaked));
}

void Recording::TakeReturnBook(bool take)
{
	if (take)
	{
		if (m_countOfTaked < m_count)
			++m_countOfTaked;
	}
	else
	{
		if (m_countOfTaked > 0)
			--m_countOfTaked;
	}

	m_countOfTakedTx.setString(std::to_string(m_countOfTaked));
}

void Recording::Move(const bool up)
{
	if (up)
	{
		m_nameTx.move(0, 20);
		m_authorTx.move(0, 20);
		m_genreTx.move(0, 20);
		m_countOfPagesTx.move(0, 20);
		m_countTx.move(0, 20);
		m_countOfTakedTx.move(0, 20);
		m_underline.move(0, 20);
	}
	else
	{
		m_nameTx.move(0, -20);
		m_authorTx.move(0, -20);
		m_genreTx.move(0, -20);
		m_countOfPagesTx.move(0, -20);
		m_countTx.move(0, -20);
		m_countOfTakedTx.move(0, -20);
		m_underline.move(0, -20);
	}
}