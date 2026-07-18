#include "Core/Deck.hpp"
#include <stdexcept>

namespace DeckBuilder {

    // Transfer ownership of the card pointer into our deck vector
    void Deck::addCard(std::unique_ptr<Card> card) {
        if (card) {
            cards.push_back(std::move(card));
        }
    }

    // Safely remove and return the top card of the deck
    std::unique_ptr<Card> Deck::drawCard() {
        if (isEmpty()) {
            throw std::out_of_range("Cannot draw from an empty deck!");
        }
        
        // Grab the last card in the vector (the "top" of the deck)
        std::unique_ptr<Card> drawnCard = std::move(cards.back());
        cards.pop_back(); // Remove the empty pointer from the vector
        
        return drawnCard;
    }

    // Fulfills the "std::shuffle" STL requirement
    void Deck::shuffle() {
        std::random_device rd;
        std::mt19937 g(rd());
        
        std::shuffle(cards.begin(), cards.end(), g);
    }

    bool Deck::isEmpty() const {
        return cards.empty();
    }

    size_t Deck::getSize() const {
        return cards.size();
    }
}