# DeckBuilderEngine

A procedural, terminal-based roguelike deckbuilding game engine built entirely in C++. 

This project implements a fully functional state machine handling turn-based combat, dynamic deck drafting, and an extensible passive item (Relic) system, rendered entirely through the terminal using ANSI escape codes.

## 🚀 Features

*   **Infinite Dungeon Loop:** Procedurally scaling enemy encounters (HP and Damage increase per floor) powered by a robust combat state machine.
*   **Dynamic Deck Drafting:** A post-combat reward system that randomly pulls cards from a CSV database, allowing the player to build a custom persistent Master Deck per run.
*   **Relic System (Event Hooks):** An object-oriented passive item system. Relics (like the *Vampiric Core*) hook directly into combat events to trigger automatic effects.
*   **Advanced Combat Mechanics:** 
    *   Energy and Block management.
    *   Status effects including *Progressive Overload* (stacking attack multipliers) and *Fatigue* (damage mitigation against heavy attacks).
*   **Data-Driven Design:** Cards are generated dynamically via a `CardFactory` reading from an external `cards.csv` database.

## 🛠️ Tech Stack

*   **Language:** C++17
*   **Build System:** CMake
*   **Compiler:** GCC / MinGW

## 🎮 How to Build and Run

### Prerequisites
You will need [CMake](https://cmake.org/) and a C++ compiler (like MinGW or GCC) installed on your system.

### Build Instructions
1. Clone the repository:
   ```bash
   git clone [https://github.com/karrram/Deckbuilding-Game-Engine.git](https://github.com/karrram/Deckbuilding-Game-Engine.git)
   cd Deckbuilding-Game-Engine
