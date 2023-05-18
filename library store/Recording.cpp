#include "Recording.h"

Recording::Recording(sql::ResultSet* res, const sf::Font& font)
{
	m_id = res->getInt(1);
	m_name = res->getString(2);
	m_author = res->getString(3);
	m_genre = res->getString(4);
	m_countOfPages = res->getInt(5);
	m_price = res->getDouble(6);
	m_count = res->getInt(7);

	Init(font);
}

void Recording::Init(const sf::Font& font)
{
	m_nameTx.setFont(font);
	m_nameTx.setFillColor(sf::Color::Black);
	m_nameTx.setCharacterSize(20);
	m_nameTx.setString(m_name);

	m_authorTx.setFont(font);
	m_authorTx.setFillColor(sf::Color::Black);
	m_authorTx.setCharacterSize(20);
	m_authorTx.setString(m_author);

	m_genreTx.setFont(font);
	m_genreTx.setFillColor(sf::Color::Black);
	m_genreTx.setCharacterSize(20);
	m_genreTx.setString(m_genre);

	m_countOfPagesTx.setFont(font);
	m_countOfPagesTx.setFillColor(sf::Color::Black);
	m_countOfPagesTx.setCharacterSize(24);
	m_countOfPagesTx.setString(std::to_string(m_countOfPages));

	m_priceTx.setFont(font);
	m_priceTx.setFillColor(sf::Color::Black);
	m_priceTx.setCharacterSize(24);
	m_priceTx.setString(std::to_string(m_price).substr(0, std::to_string(m_price).find('.') + 3));

	m_countTx.setFont(font);
	m_countTx.setFillColor(sf::Color::Black);
	m_countTx.setCharacterSize(24);
	m_countTx.setString(std::to_string(m_count));

	m_borders.setSize(sf::Vector2f(1000, 35));
	m_borders.setFillColor(sf::Color(0, 0, 0, 0));
}

int Recording::GetID() const
{
	return m_id;
}

sf::FloatRect Recording::getGlobalBounds() const
{
	return m_borders.getGlobalBounds();
}

void Recording::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (m_nameTx.getPosition().y < 100)
		return;

	target.draw(m_nameTx);
	target.draw(m_authorTx);
	target.draw(m_genreTx);
	target.draw(m_countOfPagesTx);
	target.draw(m_priceTx);
	target.draw(m_countTx);
}

void Recording::Place(const int x, const int y)
{
	m_nameTx.setPosition(x + 5, y);
	m_authorTx.setPosition(x + 255, y);
	m_genreTx.setPosition(x + 505, y);
	m_countOfPagesTx.setPosition(x + 705, y);
	m_priceTx.setPosition(x + 805, y);
	m_countTx.setPosition(x + 905, y);

	m_borders.setPosition(x, y);
}

void Recording::SoldOrBuyBook(bool sold)
{
	if (sold)
	{
		if (m_count > 0)
			--m_count;
	}
	else
	{
		++m_count;
	}

	m_countTx.setString(std::to_string(m_count));
}

void Recording::Edit(const std::string& name, const std::string& author, const std::string& genre, const int& countOfPages, const int& count, const int& price)
{
	m_name = name; m_author = author; m_genre = genre; m_countOfPages = countOfPages; m_count = count; m_price = price;
	
	m_nameTx.setString(name);
	m_authorTx.setString(author);
	m_genreTx.setString(genre);
	m_countOfPagesTx.setString(std::to_string(countOfPages));
	m_priceTx.setString(std::to_string(price));
	m_countTx.setString(std::to_string(count));
}

void Recording::Move(const bool up)
{
	if (up)
	{
		m_nameTx.move(0, 15);
		m_authorTx.move(0, 15);
		m_genreTx.move(0, 15);
		m_countOfPagesTx.move(0, 15);
		m_priceTx.move(0, 15);
		m_countTx.move(0, 15);
		m_borders.move(0, 15);
	}
	else
	{
		m_nameTx.move(0, -15);
		m_authorTx.move(0, -15);
		m_genreTx.move(0, -15);
		m_countOfPagesTx.move(0, -15);
		m_priceTx.move(0, -15);
		m_countTx.move(0, -15);
		m_borders.move(0, -15);
	}
}
