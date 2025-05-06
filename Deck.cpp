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
    int cv = 0;
    int tv = 0;
    string pv;

    for (int fv=0; fv<24; fv++)
    {
        id++;
        pv = Card::pvList[fv];
        av = fv;
        if (fv < 6){cv = fv+1;}
        if (fv > 5 and fv < 8){tv = 1;}
        if (fv > 8 and fv < 10){tv = 2;}
        if (fv > 10 and fv < 12){tv = 3;}
        if (fv > 12 and fv < 14){tv = 4;}

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
