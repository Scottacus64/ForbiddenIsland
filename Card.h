#ifndef CARD_H
#define CARD_H

#include <string>
using namespace std;

class Card
{
public:
    Card();
    ~Card();
    Card(int fv, int av, int idn, char s, string pv);
    int getFaceValue();
    int getActualValue();
    string getPrintValue();
    bool getFaceUp();
    char getSuit();
    void flipCard();
    void flipFaceUp();
    void printCard();
    void printAV();
    int getID();

private:
    int actualValue;
    int faceValue;
    int id;
    char suit;
    string printValue;
    bool faceUp;
};
#endif // CARD_H