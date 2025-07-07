# Ankr: A Fantasy Adventure Game

Ankr is a text-based fantasy adventure game that allows players to explore a world, fight enemies, and use the unique abilities of different characters. The game offers a choice of characters, including a powerful hero, a princess with unique powers, and a warrior with a sword. Players can interact with the environment, battle enemies, and level up by using special abilities to defeat foes.

## Features

- **Character Choices:** Choose from three different characters, each with unique abilities:
  - **Aether:** A powerful hero with cosmic abilities.
  - **Synax:** A princess who can kill anything she sees with her eyes.
  - **Kahray:** A warrior who protects his friends with his sword.
- **Enemy:** A bad guy that aims to kill everyone.
- **Combat System:** Engage in turn-based combat with enemies, use special abilities, and heal yourself.
- **World Exploration:** Move around a dynamic game world filled with walls, enemies, and treasures.
- **Tile-Based Map:** A grid-based map where walls, enemies, and the player are represented by different tiles.

## Requirements

- C++ Compiler (e.g., g++, clang++)
- A terminal that supports character input (Windows, Linux, or macOS).

## How to Play

1. **Clone the repository to your local machine:**
   ```bash
   git clone https://github.com/KayaxcOff/Ankr.git
   ```

2. **Navigate to the project directory:**
   ```bash
   cd ankr-game
   ```

3. **Compile the game:**
   ```bash
   g++ main.cpp -o ankr
   ```

4. **Run the game:**
   ```bash
   ./ankr
   ```

After starting the game, you'll be prompted to choose a character. From there, you'll enter a world where you can explore, fight enemies, and use your special abilities.

## Game Controls

- `W` or `w`: Move Up
- `S` or `s`: Move Down
- `A` or `a`: Move Left
- `D` or `d`: Move Right
- `Q` or `q`: Quit the game
- `1-4`: Choose your character's special abilities during combat

## Classes and Abilities

### Character Class

- **Aether:** Can use powerful cosmic abilities like "Power Strike," "Cosmic Heal," and "Teleport."
- **Synax:** Has the ability to instantly defeat any enemy she sees with her "Eyes of Death."
- **Kahray:** A warrior who uses his sword to strike enemies with basic damage.

### Map Class

The game map is a grid of tiles, where each tile can either be empty, a wall, a player, an enemy, or a treasure.

### Combat System

Combat takes place in a turn-based system, where players can choose to attack, heal, use special abilities, or run away. The goal is to defeat the enemies while managing health and resources.

## Contributing

Feel free to fork the project and submit pull requests with improvements, bug fixes, or new features.

## Acknowledgements

This game was developed by Muhammet Kaya.

Thanks to the open-source community for their continued support and resources.

---
