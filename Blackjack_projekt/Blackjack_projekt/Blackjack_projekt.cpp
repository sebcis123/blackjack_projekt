// Blackjack_projekt.cpp: definiuje punkt wejścia dla aplikacji.
//

#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <random>
#include <algorithm>

class Card {
protected:
    std::string suit;
    std::string rank;
    int value;

public:
    Card(std::string s, std::string r, int v) : suit(s), rank(r), value(v) {}

    int getValue() const {
        return value;
    }

    void display() const {
        std::cout << rank << "koloru " << suit;
    }
};

class Player {
protected:
    std::vector<Card*> hand;

public:
    virtual void drawCard(Card* card) = 0;
    virtual void displayHand(bool showFirstCard = true) const = 0;
    virtual int getHandValue() const = 0;
};

class BlackjackPlayer : public Player {
public:
    void drawCard(Card* card) override {
        hand.push_back(card);
    }

    void displayHand(bool showFirstCard = true) const override {
        std::cout << "Reka gracza: ";
        for (const auto& card : hand) {
            card->display();
            std::cout << " (" << card->getValue() << "), ";
        }
        std::cout << std::endl;
    }

    int getHandValue() const override {
        int value = 0;
        int numAces = 0;
        for (const auto& card : hand) {
            value += card->getValue();
            if (card->getValue() == 11) {
                numAces++;
            }
        }
        while (value > 21 && numAces > 0) {
            value -= 10;
            numAces--;
        }
        return value;
    }
};

class Dealer : public Player {
public:
    void drawCard(Card* card) override {
        hand.push_back(card);
    }

    void displayHand(bool showFirstCard = true) const override {
        std::cout << "Reka krupiera: ";
        if (!hand.empty()) {
            for (size_t i = 0; i < hand.size(); ++i) {
                if (i == 0 && !showFirstCard) {
                    std::cout << "[ukryta karta], ";
                }
                else {
                    hand[i]->display();
                    std::cout << " (" << hand[i]->getValue() << "), ";
                }
            }
        }
        std::cout << std::endl;
    }

    int getHandValue() const override {
        int value = 0;
        for (const auto& card : hand) {
            value += card->getValue();
        }
        return value;
    }
};

class Deck {
private:
    std::vector<Card*> cards;

public:
    Deck() {
        const std::string suits[] = { "Pik", "Kier", "Trefl", "Karo" };
        const std::string ranks[] = { "2", "3", "4", "5", "6", "7", "8", "9", "10", "Walet", "Dama", "Krol", "As" };
        for (const auto& suit : suits) {
            for (const auto& rank : ranks) {
                int value = 0;
                if (rank == "As") {
                    value = 11;
                }
                else if (rank == "Krol" || rank == "Dama" || rank == "Walet") {
                    value = 10;
                }
                else {
                    value = std::stoi(rank);
                }
                cards.push_back(new Card(suit, rank, value));
            }
        }
        shuffle();
    }

    ~Deck() {
        for (const auto& card : cards) {
            delete card;
        }
    }

    void shuffle() {
        std::random_device rd;
        std::mt19937 generator(rd());
        std::shuffle(cards.begin(), cards.end(), generator);
    }

    Card* drawCard() {
        if (cards.empty()) {
            return nullptr;
        }
        Card* card = cards.back();
        cards.pop_back();
        return card;
    }
};

class BlackjackGame {
private:
    Deck deck;
    Dealer* dealer;
    BlackjackPlayer player;
    int funds;

public:
    BlackjackGame() {
        dealer = new Dealer();
        funds = 1000;
    }

    ~BlackjackGame() {
        delete dealer;
    }

    void play() {
        std::cout << "Witaj w grze Blackjack!" << std::endl;

        do {
            std::cout << "Dostepne srodki: $" << funds << std::endl;
            if (funds <= 0) {
                std::cout << "Nie masz juz srodkow. Koniec gry." << std::endl;
                break;
            }

            int bet;
            do {
                std::cout << "Postaw zaklad (minimum $1, maksimum $" << funds << "): $";
                std::cin >> bet;
            } while (bet < 1 || bet > funds);

            funds -= bet;

            player.drawCard(deck.drawCard());
            dealer->drawCard(deck.drawCard());
            player.drawCard(deck.drawCard());
            dealer->drawCard(deck.drawCard());
            player.displayHand();
            dealer->displayHand(false);

            while (true) {
                std::cout << "Czy chcesz dobrac karte (H)it czy spasowac (S)tand? ";
                char choice;
                std::cin >> choice;

                if (choice == 'H' || choice == 'h') {
                    player.drawCard(deck.drawCard());
                    player.displayHand();
                    int handValue = player.getHandValue();
                    std::cout << "Wartosc reki: " << handValue << std::endl;

                    if (handValue > 21) {
                        std::cout << "Przegrales." << std::endl;
                        break;
                    }
                }
                else if (choice == 'S' || choice == 's') {
                    break;
                }
            }

            int playerHandValue = player.getHandValue();

            if (playerHandValue <= 21) {
                while (dealer->getHandValue() < 17) {
                    dealer->drawCard(deck.drawCard());
                }
                dealer->displayHand(true);
                int dealerHandValue = dealer->getHandValue();
                std::cout << "Wartosc reki gracza: " << playerHandValue << std::endl;
                std::cout << "Wartosc reki krupiera: " << dealerHandValue << std::endl;

                if (dealerHandValue > 21) {
                    std::cout << "Wygrales. Krupier przekroczyl 21 punktow!" << std::endl;
                    funds += 2 * bet;
                }
                else if (dealerHandValue > playerHandValue) {
                    std::cout << "Przegrales." << std::endl;
                }
                else if (dealerHandValue < playerHandValue) {
                    std::cout << "Wygrales!" << std::endl;
                    funds += 2 * bet;
                }
                else {
                    std::cout << "Remis." << std::endl;
                    funds += bet;
                }
            }
            else {
                std::cout << "Przekroczyles 21 punktow!" << std::endl;
            }

            std::cout << "Dostepne srodki: $" << funds << std::endl;
            std::cout << "Czy chcesz zagrac ponownie? (T/N): ";
            char playAgain;
            std::cin >> playAgain;

            if (playAgain == 'N' || playAgain == 'n') {
                break;
            }
            else {
                // zresetowanie talii oraz kart na nową runde
                deck.shuffle();
                player = BlackjackPlayer();
                delete dealer;
                dealer = new Dealer();
            }
        } while (true);
    }
};

int main() {
    BlackjackGame game;
    game.play();

    return 0;
}