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


Deck::Deck(int numberOfCards, bool custom)
{
    makeDeck(numberOfCards);
}


Deck::~Deck()
{
}


void Deck::makeDeck()
{
    int av;
    int id = 0;
    int cv = 0;
    int tv = 0;
    string pv;

    for (int fv=0; fv<24; fv++)
    {
        id = fv;
        pv = Card::pvList[fv];
        av = fv;
        if (fv < 6){cv = fv+1;}
        if (fv > 5 and fv < 8){tv = 1;}
        if (fv > 7 and fv < 10){tv = 2;}
        if (fv > 9 and fv < 12){tv = 3;}
        if (fv > 11 and fv < 14){tv = 4;}
        m_deck.push_back(new Card(fv, av, id, pv, tv, cv));
        cv = 0;
        tv = 0;
    }
}


void Deck::makeDeck(int numberOfCards)
{
    int av = 0;
    int id = 0;
    int cv = 0;
    int fv = 0;
    int tv;
    string pv;

    for (int i=0; i<numberOfCards; i++)
    {
        id++;
        if (i < 5){tv = 1;}
        if (i > 4 and i < 10){tv = 2;}
        if (i > 9 and i < 15){tv = 3;}
        if (i > 14 and i < 20){tv = 4;}
        if (i > 19 and i < 23){tv = 5;}
        if (i > 22 and i < 25){tv = 6;}
        if (i > 24 and i < 28){tv = 7;}
        pv = to_string(tv);
        m_deck.push_back(new Card(fv, av, id, pv, tv, cv));
    }
}


void Deck::printDeck() 
{
    int i = 0;
    for (auto pCard : m_deck) 
    {
        i++;
        pCard->printCard(); 
        cout << " ";
        
        if (i%12 == 0) {cout << "\n";}
    }
    cout << endl;
}


void Deck::shuffle()
{
    std::random_device rd;
    std::mt19937 rng(rd());
    std::shuffle(m_deck.begin(), m_deck.end(), rng);
}


Card* Deck::deal()
{
    if(m_deck.empty())
    {
        return nullptr;
    }
    Card* pCard = m_deck.back();
    m_deck.pop_back();
    return pCard;
}


Card* Deck::dealCardAt(int position)
{
    if (position >= 0 && static_cast<size_t>(position) < m_deck.size())
    {
        auto cardToDeal = m_deck.begin() + position;
        Card* pCard = *cardToDeal;
        m_deck.erase(cardToDeal);
        return pCard;
    }
    else
    {
        return nullptr;
    }
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


void Deck::eraseDeck()
{
    m_deck.clear();
}



void Deck::addCard(Card* pCard)
{
    m_deck.push_back(pCard);
}


void Deck::addCardAt(Card* pCard, int slot)
{
    m_deck.insert(m_deck.begin() + slot, pCard);
}


void Deck::printTopCard()
{
    if (m_deck.size()>0)
    {
        Card* pCard = m_deck[m_deck.size() -1];
        pCard->printCard();
    }
    else{
        cout << "empty";
    }
}


Card* Deck::getTopDeckCard()
{
    Card* pCard = m_deck[m_deck.size() -1];
    return pCard;
}


Card* Deck::getDeckCardAt(int slot)
{
    Card* pCard = m_deck[slot];
    return pCard;
}


int Deck::deckSize()
{
    return m_deck.size();
}


void Deck::recycleDeck(Deck* draw)
{
    int size = this->deckSize();
    for(int i=0; i<size; i++)
    {
        Card* pCard = this->deal();
        draw->addCard(pCard);
    }
    draw->shuffle();
}


void Deck::resetState()
{
    int size = this->deckSize();
    for(int i=0; i<size; i++)
    {
        Card* pCard = this->getDeckCardAt(i);
        pCard->setState(2);
    }
}
