#include "Deck.h"
#include <iostream>

Deck::Deck()
{
    makeDeck();
}


Deck::Deck(int numberOfDecks)
{
    for (int i=0; i<numberOfDecks; i++)
    {
        makeDeck();
    }
}


Deck::~Deck()
{
}


void Deck::makeDeck()
{
    int av;
    int id = 0;
    string pv;
    char suits[4] = {'C', 'S', 'H', 'D'};
    for (int s=0; s<4; s++)
    {
        for (int fv=1; fv<14; fv++)
        {
            id++;
            switch (fv)
            {
            case 1:
                av = 11;
                pv = " A";
                break;
            case 2: case 3: case 4: case 5: case 6: case 7: case 8: case 9:
                av = fv;
                pv = " " + to_string(av);
                break;
            case 10:
                av = fv;
                pv = to_string(av);
                break;
            case 11:
                av = 10;
                pv = " J";
                break;
            case 12:
                av = 10;
                pv = " Q";
                break;
            case 13:
                av = 10;
                pv = " K";
                break;
            }
            m_deck.push_back(Card(fv, av, id, suits[s], pv));
        }
    }
}


void Deck::printDeck() 
{
    int i = 0;
    for (auto& card : m_deck) 
    {
        i++;
        card.printCard(); 
        cout << " ";
        
        if (i%13 == 0) {cout << "\n";}
    }
}


void Deck::shuffle()
{
    std::random_device rd;
    std::mt19937 rng(rd());
    std::shuffle(m_deck.begin(), m_deck.end(), rng);
}


Card Deck::deal()
{
    Card c = m_deck.back();
    m_deck.pop_back();
    return c;
}


int Deck::cardsLeft()
{
    int remaining = m_deck.size();
    return remaining;
}


void Deck::clearDeck(int numberOfDecks)
{
    m_deck.clear();
    for (int i=0; i<numberOfDecks; i++)
    {
        makeDeck();
    }

}



void Deck::addCard(Card c)
{
    m_deck.push_back(c);
}


void Deck::printTopCard()
{
    if (m_deck.size()>0)
    {
        Card c = m_deck[m_deck.size() -1];
        c.printCard();
    }
    else{
        cout << "empty";
    }
}


Card* Deck::getTopDeckCard()
{
    Card* c = &m_deck[m_deck.size() -1];
    //Card* c = &m_deck.back();
    return c;
}


Card* Deck::getDeckCardAt(int slot)
{
    std::cout << "\ngetDeckCardAt's slot = " << slot << "\n";
   
    Card c = m_deck.at(slot);
    Card* p_c = &c;
    std::cout << "**Card Adress " << p_c << "\n\n";
    return p_c;
}