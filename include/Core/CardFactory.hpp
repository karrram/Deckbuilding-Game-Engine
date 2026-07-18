#pragma once
#include <string>
#include <unordered_map>
#include <memory>
#include <vector>
#include "../Cards/Card.hpp"

namespace DeckBuilder {
    struct CardData {
        std::string name;
        int cost;
        int value;
        std::string type;
    };

    class CardFactory {
    private:
        std::unordered_map<std::string, CardData> database;

    public:
        bool loadDatabase(const std::string& filepath);
        std::unique_ptr<Card> createCard(const std::string& cardName);
        
        // NEW: Method to pull random cards for the draft
        std::vector<std::string> getRandomCardNames(int count);
    };
}