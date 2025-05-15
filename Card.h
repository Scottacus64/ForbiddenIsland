#ifndef CARD_H
#define CARD_H

#include <string>
#include <vector>
using namespace std;

class Card
{
public:
    Card();
    ~Card();
    Card(int fv, int av, int idn, string pv, int tv, int cv);
    int  getFaceValue();
    int  getActualValue();
    bool getFaceUp();
    void setFaceUp(bool fUp);
    void flipCard();
    void flipFaceUp();
    void printCard();
    void printAV();
    int  getID();
    string getPrintValue();
    int  getBlackRed();
    static const vector<string> pvList;
    void floodCard();
    void shoreUpCard();
    void setState(int newState);
    int  getState();
    int  getTreasureValue();
    int  getCharacterValue();

private:
    int actualValue;
    int faceValue;
    int id;
    int treasureValue;
    int characterValue;
    string printValue;
    bool faceUp;
    int state;
    };
#endif // CARD_H