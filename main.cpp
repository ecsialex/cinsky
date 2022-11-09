#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <vector>
#include <algorithm>
#include <chrono>
#include <random>

using namespace std;

string convertToSuit(int i) {
    string suit;
    switch(i) {
        case 0:
            suit = 'D'; //Diamond
            break;
        case 1:
            suit = 'C'; //Club
            break;
        case 2:
            suit = 'H'; //Heart
            break;
        default:
            suit = 'S'; //Spade
    }
    return suit;
}

string convertToCardNumber(int i) {
    string num;
    switch(i) {
        case 12:
            num = 'A';
            break;
        case 11:
            num = 'K';
            break;
        case 10:
            num = 'Q';
            break;
        case 9:
            num = 'J';
            break;
        case 8:
            num = "10";
            break;
        default:
            num = (char)(i + 50);
    }

    return num;
}

string convertToCard(pair<int, int> card) {
    string suit = convertToSuit(card.first);
    string num = convertToCardNumber(card.second);

    return suit + num;
}

void clearCin() {
    cin.clear();
    cin.ignore(INT64_MAX, '\n');
}

int getNPlayers() {
    string s = "Enter a number of players: ";
    int x;

    cout << s << endl;

    while(!(cin >> x) || x <= 1 || x > 6) {
        clearCin();
        cout << "Invalid input. " << s << endl;
    }

    clearCin();
//    cout << "Input = " << x << endl;
    return x;
}

string getName(int playerN) {
    string s = "Enter the name of player " + to_string(playerN) + ": ";
    string x;

    cout << s << endl;

    while(!(cin >> x)) {
        clearCin();
        cout << "Invalid input. " << s << endl;
    }

    clearCin();
//    cout << "Input = " << x << endl;
    return x;
}


bool checkHigh(int n, const vector<pair<int,int>>& cardsInPlay) {
    bool ret = false;
    for (auto i : cardsInPlay) {
        if (i.second == n) {
            ret = true;
            break;
        }
    }
    return ret;
}

bool checkPair(int n, const vector<pair<int,int>>& cardsInPlay) {
    bool ret = false;
    int foundN = 0;
    for (auto i: cardsInPlay) {
        if (i.second == n) ++foundN;
    }
    if (foundN >= 2) ret = true;
    return ret;
}

bool checkDoublePair(int n1, int n2, const vector<pair<int,int>>& cardsInPlay) {
    bool ret = false;
    int foundN1 = 0;
    int foundN2 = 0;
    for (auto i: cardsInPlay) {
        if (i.second == n1) ++foundN1;
        if (i.second == n2) ++foundN2;
    }
    if (foundN1 >= 2 && foundN2 >= 2) ret = true;
    return ret;
}

bool checkTrips(int n, const vector<pair<int,int>>& cardsInPlay) {
    bool ret = false;
    int foundN = 0;
    for (auto i: cardsInPlay) {
        if (i.second == n) ++foundN;
    }
    if (foundN >= 3) ret = true;
    return ret;
}

vector<int> cardNCount(const vector<pair<int,int>>& cardsInPlay) {
    vector<int> foundN;
    for (int i = 0; i < 13; ++i) foundN.push_back(0);
    for (auto c: cardsInPlay) {
        foundN.at(c.second) += 1;
    }
//    for (int i = 0; i < 13; ++i) cout << foundN.at(i) << " ";
//    cout << endl;
    return foundN;
}

bool checkStraight(int n, vector<int> foundN) {
    bool ret = false;
    if (n == 3) {
        if (foundN.at(n) > 0 && foundN.at(n-1) > 0 && foundN.at(n-2) > 0 && foundN.at(n-3) > 0 && foundN.at(12) > 0)
            ret = true;
    } else {
        if (foundN.at(n) > 0 && foundN.at(n-1) > 0 && foundN.at(n-2) > 0 && foundN.at(n-3) > 0 && foundN.at(n-4) > 0)
            ret = true;
    }
    return ret;
}

vector<int> cardSuitCount(const vector<pair<int,int>>& cardsInPlay) {
    vector<int> foundSuit;
    for (int i = 0; i < 4; ++i) foundSuit.push_back(0);
    for (auto c: cardsInPlay) {
        foundSuit.at(c.first) += 1;
    }
//    for (int i = 0; i < 4; ++i) cout << foundSuit.at(i) << " ";
//    cout << endl;
    return foundSuit;
}

vector<vector<bool>> cardNSuitCount(const vector<pair<int,int>>& cardsInPlay) {
    vector<vector<bool>> foundNSuit;
    for (int i = 0; i < 4; ++i) {
        vector<bool> foundN;
        for (int j = 0; j < 13; ++j) foundN.push_back(false);
        for (auto c: cardsInPlay) {
            if (c.first == i) foundN.at(c.second) = true;
        }
        foundNSuit.push_back(foundN);
    }
//    for (const auto& v : foundNSuit){ for (auto i : v) cout << i << " "; cout << endl;};
//    cout << endl;
    return foundNSuit;
}

bool checkFlush(int n, int suit, vector<int> foundSuit, vector<vector<bool>> foundNSuit) {
    bool ret = false;
    if (!foundNSuit.at(suit).at(n)) return false; //called card is not in play
    if (n == 12) return foundSuit.at(suit) >= 5; //in case we count suit until ace

    int foundN = 1;
    if ((foundNSuit.at(suit).at(12))) ++foundN; //add ace if possible
    for (int i = 0; i < n; ++i) { //add all smaller suited cards
        if (foundNSuit.at(suit).at(i)) ++foundN;
    }

    return (foundN >= 5);
}

bool checkFullHouse(int trip, int doub, const vector<pair<int,int>>& cardsInPlay) {
    bool ret = false;
    int foundTrip = 0;
    int foundDoub = 0;
    for (auto i: cardsInPlay) {
        if (i.second == trip) ++foundTrip;
        if (i.second == doub) ++foundDoub;
    }
    if (foundTrip >= 3 && foundDoub >= 2) ret = true;
    return ret;
}

bool checkQuads(int n, const vector<pair<int,int>>& cardsInPlay) {
    bool ret = false;
    int foundN = 0;
    for (auto i: cardsInPlay) {
        if (i.second == n) ++foundN;
    }
    if (foundN >= 4) ret = true;
    return ret;
}

bool checkStraightFlush(int n, int suit, vector<vector<bool>> foundNSuit) {
    bool ret = false;
    auto foundN = foundNSuit.at(suit);
    if (n == 3) {
        if (foundN.at(n) > 0 && foundN.at(n-1) > 0 && foundN.at(n-2) > 0 && foundN.at(n-3) > 0 && foundN.at(12) > 0)
            ret = true;
    } else {
        if (foundN.at(n) > 0 && foundN.at(n-1) > 0 && foundN.at(n-2) > 0 && foundN.at(n-3) > 0 && foundN.at(n-4) > 0)
            ret = true;
    }
    return ret;
}

class CPlayer {
public:
    const int n;
protected:
    string name;
    int handSize;
    vector<pair<int,int>> cards;
public:
//    CPlayer() : n(0) {}

    CPlayer(const string & name, int n) :
        name(name), n(n) {
        handSize = 5;
    }

    string getName() {
        return name;
    }

    int getHandSize() {
        return handSize;
    }

    int cardsInHand() {
        return cards.size();
    }

    void lostRound() {
        --this->handSize;
        cout << "Player " << this->name << " lost this round!" << endl;

        if (handSize < 0) {
            cout << this->name << " is out of the game!" << endl;
        } else cout << "His new handsize is: " << handSize << " cards." << endl;
    }

    bool isOut() {
        return handSize < 0;
    }

    void receiveCard(pair<int, int> c) {
        cards.push_back(c);
    }

    void printCards() {
        if (handSize > 0) for (auto c : cards) cout << convertToCard(c) << ' ';
        else cout << "empty";
    }

    void throwHand() {
        this->cards.clear();
    }

};

class CTable {
protected:
    vector<CPlayer> playerList;

public:
    CTable(int nPlayers) {
        for (int i = 0; i < nPlayers; ++i) {
            playerList.emplace_back(getName(i + 1), i);
        }
    }

    void startGame(const vector<pair<int, int>>& cardDeck) {
        int playerCount = playerList.size(), roundN = 1;

        while (playerCount > 1) {
            CPlayer& roundLoser = playerList.at(this->newRound(roundN++, cardDeck));
            roundLoser.lostRound();
            if (roundLoser.isOut()) --playerCount;
        }

        for (auto p : playerList) {
            if (!p.isOut()) cout << "Player " << p.getName() << " won!" << endl;
        }
    }

protected:

    string highestComb(const vector<pair<int,int>>& cardsInPlay) {
        string retString;
        bool found = false;
        auto cnc = cardNCount(cardsInPlay);
        auto csc = cardSuitCount(cardsInPlay);
        auto cnsc = cardNSuitCount(cardsInPlay);

        for (int n = 12; n >= 3; --n) {
            for (int suit = 3; suit >= 0; --suit) {
                if (checkStraightFlush(n, suit, cnsc)) {
                    if (n == 12) return "Royal Straight Flush of " + convertToSuit(suit);
                    else return convertToCardNumber(n) + " Straight Flush of " + convertToSuit(suit);
                }
            }
        }

        for (int n = 12; n >= 0; --n) {
            if (checkQuads(n, cardsInPlay)) {
                return "Quads of " + convertToCardNumber(n);
            }
        }

        for (int n = 12; n >= 0; --n) {
            for (int m = 12; m >= 0; --m) {
                if (m == n) continue;
                if (checkFullHouse(n, m, cardsInPlay)) {
                    return "Full House of " + convertToCardNumber(n) + " " + convertToCardNumber(m);
                }
            }
        }

        for (int n = 3; n <= 12; ++n) {
            for (int suit = 3; suit >= 0; --suit) {
                if (checkFlush(n, suit, csc, cnsc)) {
                    return convertToSuit(suit) + " Flush at " + convertToCardNumber(n);
                }
            }
        }

        for (int n = 12; n >= 3; --n) {
            if (checkStraight(n, cnc)) {
                return convertToCardNumber(n) + " Straight";
            }
        }

        for (int n = 12; n >= 0; --n) {
            if (checkTrips(n, cardsInPlay)) {
                return "Trips of " + convertToCardNumber(n);
            }
        }

        for (int n = 12; n >= 1; --n) {
            for (int m = n - 1; m >= 0; --m) {
                if (checkDoublePair(n, m, cardsInPlay)) {
                    return "Double pair of " + convertToCardNumber(n) + " " + convertToCardNumber(m);
                }
            }
        }

        for (int n = 12; n >= 0; --n) {
            if (checkPair(n, cardsInPlay)) {
                return "Pair of " + convertToCardNumber(n);
            }
        }

        for (int n = 12; n >= 0; --n) {
            if (checkHigh(n, cardsInPlay)) {
                return "High Card of " + convertToCardNumber(n);
            }
        }
    }

    int newRound(int roundN, const vector<pair<int, int>>& cardDeck) {
        int loser = 0;
        cout << endl << "Round " << roundN << " starts!" << endl;

        auto roundDeck = cardDeck;
        unsigned seed = chrono::system_clock::now().time_since_epoch().count();
        shuffle(roundDeck.begin(), roundDeck.end(), default_random_engine(seed));
        int cardsToDeal = 0;
        for (auto p : playerList) if (p.getHandSize() > 0) cardsToDeal += p.getHandSize();

        //deal cards from card deck
        vector<pair<int,int>> cardsInPlay;

        while (cardsInPlay.size() < cardsToDeal) {
            cardsInPlay.push_back(roundDeck.back());
            roundDeck.pop_back();
        }

        // give cards to players
        int i = 0;
        for (auto & p : playerList) {
            if (p.isOut()) continue;
            while (p.cardsInHand() < p.getHandSize()) {
                p.receiveCard(cardsInPlay.at(i));
//                cout << "Rozdal som kartu hracovi " << p.getName() << endl;
                ++i;
            }
        }

        //sort cardsInPlay for evaluation
        std::sort(cardsInPlay.begin(), cardsInPlay.end(), std::greater<>());
//        for (auto card : cardsInPlay) cout << convertToCard(card) << endl;

        for (auto & p : playerList) {
            cout << p.getName() << " had cards: ";
            p.printCards();
            p.throwHand();
            cout << endl;
        }

        cout << "Highest combination in the game was: " << endl;
        auto combo = highestComb(cardsInPlay);
        cout << combo << endl;

        cardsInPlay.clear();
        return loser;
    }

};


int main() {
    int nPlayers = getNPlayers();
    CTable table = CTable(nPlayers);

    //create cards
    vector<pair<int, int>> cardDeck;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 13; ++j) {
            cardDeck.emplace_back(i, j);
        }
    }

    table.startGame(cardDeck);

    return 0;
}
