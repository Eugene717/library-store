#pragma once
#include <SFML/Graphics.hpp>
//#include <list>

struct Recording :public sf::Drawable
{
	std::string m_name;
	std::string m_author;
	std::string m_genre;
	int m_countOfPages;
	int m_count;
	int m_countOfTaked;

	bool m_openList;   //not shown

	//std::list<std::pair<std::string, int>>* m_people;

	Recording(const std::string& name, const std::string& author, const std::string& genre, const int& countOfPages, const int& count, const sf::Font& m_font, const int& countOfTaked);
	int GetY();
	sf::FloatRect getGlobalBounds() const;
	void Place(const int x, const int y);
	void Edit(const std::string& name, const std::string& author, const std::string& genre, const int& countOfPages, const int& count, const int& countOfTaked);
	void TakeReturnBook(bool take);
	void Move(const bool up);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
private:
	sf::Text m_nameTx;
	sf::Text m_authorTx;
	sf::Text m_genreTx;
	sf::Text m_countOfPagesTx;
	sf::Text m_countTx;
	sf::Text m_countOfTakedTx;
	sf::RectangleShape m_underline;
};
