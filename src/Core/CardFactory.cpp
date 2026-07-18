#include "Core/CardFactory.hpp"
#include "Cards/AttackCard.hpp"
#include "Cards/DefenseCard.hpp"
#include "Cards/HealCard.hpp"
#include "Cards/TheTwistCard.hpp"
#include "Cards/PureAdrenalineCard.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <random>    
#include <algorithm> 

namespace DeckBuilder {

    bool CardFactory::loadDatabase(const std::string& filepath) {
        std::ifstream file(filepath);
        if (!file.is_open()) {
            std::cerr << "Error: Could not open CSV file at " << filepath << std::endl;
            return false;
        }

        std::string line;
        std::getline(file, line);

        while (std::getline(file, line)) {
            std::stringstream ss(line);
            std::string nameStr, costStr, valueStr, typeStr;

            std::getline(ss, nameStr, ',');
            std::getline(ss, costStr, ',');
            std::getline(ss, valueStr, ',');
            std::getline(ss, typeStr, ',');

            if (!nameStr.empty() && !costStr.empty() && !valueStr.empty() && !typeStr.empty()) {
                try {
                    CardData data;
                    data.name = nameStr;
                    data.cost = std::stoi(costStr);
                    data.value = std::stoi(valueStr);
                    
                    data.type = typeStr;
                    if (!data.type.empty() && data.type.back() == '\r') {
                        data.type.pop_back();
                    }
                    
                    database[nameStr] = data;
                } catch (...) {
                    std::cerr << "Error parsing row for card: " << nameStr << std::endl;
                }
            }
        }
        file.close();
        return true;
    }

    std::unique_ptr<Card> CardFactory::createCard(const std::string& cardName) {
        auto it = database.find(cardName);
        if (it != database.end()) {
            std::string type = it->second.type;
            int cost = it->second.cost;
            int value = it->second.value;

            if (type == "Attack") return std::make_unique<AttackCard>(cardName, cost, value);
            else if (type == "Defense") return std::make_unique<DefenseCard>(cardName, cost, value);
            else if (type == "Heal") return std::make_unique<HealCard>(cardName, cost, value);
            else if (type == "Ultimate") return std::make_unique<TheTwistCard>(cardName, cost, value);
            else if (type == "Skill") return std::make_unique<PureAdrenalineCard>(cardName, cost, value);
        }
        
        std::cout << "Error: Card '" << cardName << "' not found in database or unknown type!" << std::endl;
        return nullptr; 
    }

    // NEW: Draft generation logic
    std::vector<std::string> CardFactory::getRandomCardNames(int count) {
        std::vector<std::string> allNames;
        for (const auto& pair : database) {
            allNames.push_back(pair.first);
        }

        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(allNames.begin(), allNames.end(), g);

        std::vector<std::string> selected;
        for (size_t i = 0; i < static_cast<size_t>(count) && i < allNames.size(); ++i){
            selected.push_back(allNames[i]);
        }
        return selected;
    }
}