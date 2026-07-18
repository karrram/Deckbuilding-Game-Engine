#pragma once
#include <string>
#include <iostream>
#include <utility>
#include <map>

namespace DeckBuilder {
    class Entity {
    protected:
        std::string name;
        int hp;
        int maxHp;
        int block; 
        int turnDamageTaken; // NEW: Tracks damage taken in a single round

        std::map<std::string, int> statuses;

    public:
        virtual void triggerOnAttackPlayed() {} // NEW: Hook for relics
        Entity(std::string entityName, int maxHealth) 
            : name(std::move(entityName)), hp(maxHealth), maxHp(maxHealth), block(0), turnDamageTaken(0) {}
        
        virtual ~Entity() = default;

        virtual void takeDamage(int amount) {
            if (getStatus("Vulnerable") > 0) {
                amount += (amount / 2);
                std::cout << name << " is Vulnerable! Damage amplified!" << std::endl;
            }

            if (block > 0) {
                if (block >= amount) {
                    block -= amount;
                    std::cout << name << "'s block absorbed all " << amount << " damage! (Block remaining: " << block << ")" << std::endl;
                    return; 
                } else {
                    std::cout << name << "'s block absorbed " << block << " damage!" << std::endl;
                    amount -= block;
                    block = 0; 
                }
            }
            
            int oldHp = hp;
            hp -= amount;
            if (hp < 0) hp = 0;
            
            int damageTaken = oldHp - hp;
            std::cout << name << " takes " << damageTaken << " damage! (HP: " << hp << "/" << maxHp << ")" << std::endl;

            if (damageTaken > 0) {
                applyStatus("Twist Charge", 1);
                turnDamageTaken += damageTaken; // Add to the single-turn tracker
            }
        }

        virtual void heal(int amount) {
            hp += amount;
            if (hp > maxHp) hp = maxHp;
            std::cout << name << " actively recovers " << amount << " HP! (HP: " << hp << "/" << maxHp << ")" << std::endl;
        }

        virtual void addBlock(int amount) {
            block += amount;
            std::cout << name << " braces for impact! +" << amount << " Block. (Total: " << block << ")" << std::endl;
        }

        virtual void resetBlock() { block = 0; }
        
        // NEW: Turn damage management for Fatigued logic
        int getTurnDamageTaken() const { return turnDamageTaken; }
        void resetTurnDamage() { turnDamageTaken = 0; }

        void applyStatus(const std::string& effectName, int stacks) {
            statuses[effectName] += stacks;
            std::cout << name << " gains " << effectName << " (" << stacks << ")!" << std::endl;
        }

        int getStatus(const std::string& effectName) const {
            auto it = statuses.find(effectName);
            if (it != statuses.end()) {
                return it->second;
            }
            return 0; 
        }

        void reduceStatus(const std::string& effectName, int amount = 1) {
            if (getStatus(effectName) > 0) {
                statuses[effectName] -= amount;
                if (statuses[effectName] <= 0) {
                    statuses.erase(effectName); 
                }
            }
        }

        // NEW: Wipes a status completely (used at end of turn)
        void clearStatus(const std::string& effectName) {
            statuses.erase(effectName);
        }

        bool isDead() const { return hp <= 0; }
        std::string getName() const { return name; }
        int getHp() const { return hp; }
        int getBlock() const { return block; }
    };
}