#pragma once
#include <SFML/Graphics.hpp>

struct Recording :public sf::Drawable
{
	Recording(const std::string& name, const std::string& author, const std::string& genre, const int& countOfPages, const int& price, const int& count, const sf::Font& m_font);
	int GetY();
	sf::FloatRect getGlobalBounds() const;
	void Place(const int x, const int y);
	void Edit(const std::string& name, const std::string& author, const std::string& genre, const int& countOfPages, const int& price, const int& count);
	void SoldOrBuyBook(bool sold);
	void Move(const bool up);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	friend class ArchiveSystem;
private:
	std::string m_name;
	std::string m_author;
	std::string m_genre;
	int m_countOfPages;
	float m_price;
	int m_count;

	sf::Text m_nameTx;
	sf::Text m_authorTx;
	sf::Text m_genreTx;
	sf::Text m_countOfPagesTx;
	sf::Text m_priceTx;
	sf::Text m_countTx;
};
