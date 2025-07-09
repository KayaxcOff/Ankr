#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <random>
#include <chrono>
#include <map>
#include <utility>

// Cross-platform input handling
// for Windows - uses conio.h for direct key input
#ifdef _WIN32
#include <conio.h>
char getch_direct() {
    return _getch();
}

// for Linux and macOS - uses termios.h for terminal control
#else
#include <termios.h>
#include <unistd.h>
char getch_direct() {
    char buf = 0;
    struct termios old = {0};
    
    // Get current terminal settings
    if (tcgetattr(0, &old) < 0)
        perror("tcsetattr()");
    
    // Disable canonical mode (line buffering) and echo
    old.c_lflag &= ~ICANON;
    old.c_lflag &= ~ECHO;
    old.c_cc[VMIN] = 1;    // Minimum characters to read
    old.c_cc[VTIME] = 0;   // No timeout
    
    // Apply new settings
    if (tcsetattr(0, TCSANOW, &old) < 0)
        perror("tcsetattr ICANON");
    
    // Read single character
    if (read(0, &buf, 1) < 1)
        perror("read()");
    
    // Restore canonical mode and echo
    old.c_lflag |= ICANON;
    old.c_lflag |= ECHO;
    if (tcsetattr(0, TCSADRAIN, &old) < 0)
        perror("tcsetattr ~ICANON");
    
    return buf;
}
#endif

// Forward declaration for GameManager (needed for teleport function)
class GameManager;

/**
 * Base Character class - Abstract base for all game characters
 * Provides common functionality like health, damage, and special abilities
 */
class Character {
protected:
    std::string character_name;  // Character's name
    int health;                  // Current health points
    int maxHealth;              // Maximum health points
    
public:
    // Constructor - initializes character with name and health values
    Character(std::string name, int hp, int maxHp)
        : character_name(name), health(hp), maxHealth(maxHp) {};

    // Virtual destructor for proper cleanup of derived classes
    virtual ~Character() {};

    // Apply damage to character and display result
    void takeDamage(int damage) {
        health -= damage;
        if (health <= 0) health = 0;  // Prevent negative health
        std::cout << character_name << " takes " << damage << " damage. Health is now " << health << std::endl;
    }

    // Heal character by specified amount
    void heal(int amount) {
        health += amount;
        std::cout << character_name << " heals " << amount << " health. Health is now " << health << std::endl;
    }

    // Check if character is still alive
    bool isAlive() const{
        return health > 0;
    }

    // Set health to 0 (instant death)
    void setHealth() {
        health = 0;
    }

    // Get character's name
    std::string getName() const{
        return character_name;
    }

    // Get current health as string
    std::string getHealthStatus() const{
        return std::to_string(health);
    }

    // Pure virtual functions - must be implemented by derived classes
    virtual void displayInfo() const = 0;
    virtual void useSpecialAbility(Character& target, GameManager* gameManager = nullptr) = 0;
};

/**
 * Enumeration for different tile types on the game map
 */
enum class TileType {
    EMPTY = 0,    // Empty space that can be walked on
    WALL,         // Wall that blocks movement
    PLAYER,       // Player's current position
    ENEMY,        // Enemy position
    TREASURE      // Treasure location (not currently used)
};

/**
 * Map class - Handles the game world grid and tile management
 * Creates a bordered rectangular map with walls on the edges
 */
class Map {
private:
    int width;                                    // Map width in tiles
    int height;                                   // Map height in tiles
    std::vector<std::vector<TileType>> grid;      // 2D grid storing tile types
    
public:
    // Constructor - creates map with specified dimensions and wall borders
    Map(int w, int h) : width(w), height(h) {
        // Initialize grid with empty tiles
        grid.resize(height, std::vector<TileType>(width, TileType::EMPTY));

        // Create top and bottom walls
        for (int i = 0; i < width; ++i) {
            grid[0][i] = TileType::WALL;
            grid[height - 1][i] = TileType::WALL;
        }
        
        // Create left and right walls
        for (int i = 0; i < height; ++i) {
            grid[i][0] = TileType::WALL;
            grid[i][width - 1] = TileType::WALL;
        }
    }

    // Display the entire map with character symbols
    void display() const {
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                switch (grid[y][x]) {
                    case TileType::EMPTY:    std::cout << ". "; break;  // Empty space
                    case TileType::WALL:     std::cout << "# "; break;  // Wall
                    case TileType::PLAYER:   std::cout << "P "; break;  // Player position
                    case TileType::ENEMY:    std::cout << "E "; break;  // Enemy position
                    case TileType::TREASURE: std::cout << "T "; break;  // Treasure
                }
            }
            std::cout << std::endl;
        }
    }

    // Set a specific tile to a given type (with bounds checking)
    void setTile(int x, int y, TileType type) {
        if (y >= 0 && y < height && x >= 0 && x < width) {
            grid[y][x] = type;
        }
    }

    // Get the type of tile at specified coordinates
    TileType getTile(int x, int y) const {
        if (y >= 0 && y < height && x >= 0 && x < width) {
            return grid[y][x];
        }
        return TileType::WALL;  // Return wall for out-of-bounds coordinates
    }
};

/**
 * Aether Character Class - The most powerful character
 * Has cosmic abilities including teleportation and universe reset
 */
class Aether : public Character {
public:
    // Constructor - initializes Aether with high health
    Aether() : Character("Aether", 100, 100) {}

    // Display character information and lore
    void displayInfo() const override {
        std::cout << "The name is " << character_name << "and the health is " << health << std::endl;
        std::cout << "He is smartest person in the universe and he is just a boy. Aether has most powerful object in the universe. His object can does everything it's master's want";
    }

    // Special ability menu - allows player to choose from multiple cosmic powers
    void useSpecialAbility(Character& target, GameManager* gameManager = nullptr) override {
        int getCommand;

        std::cout << "1- Power Strike" << std::endl;      // High damage attack
        std::cout << "2- Cosmic Heal" << std::endl;       // Full heal ability
        std::cout << "3- Universe Reset" << std::endl;    // Reset game state
        std::cout << "4- Teleport" << std::endl;          // Teleport to new location
        std::cout << "Choose your command: ";
        std::cin >> getCommand;

        switch (getCommand) {
            case 1: powerStrike(target); break;
            case 2: cosmicHeal(); break;
            case 3: universeReset(); break;
            case 4: teleport(target, gameManager); break;
            default: std::cout << "Invalid command!" << std::endl; break;
        }
    }
    
private:
    // Devastating attack that deals massive damage
    void powerStrike(Character& target) {
        int damage = 500;
        target.takeDamage(damage);
        std::cout << character_name << " uses Power Strike on " << target.getName() << " for " << damage << " damage." << std::endl;
    }

    // Restore health to maximum
    void cosmicHeal() {
        int healAmount = 100;
        health += healAmount;
        if (health > maxHealth) health = maxHealth;  // Cap at max health
        std::cout << character_name << " uses Cosmic Heal. Health is now " << health << std::endl;
    }

    // Reset the universe (currently just displays message)
    int universeReset() {
        std::cout << character_name << " resets the universe. All characters are reset to their initial state." << std::endl;
        return 0;
    }

    // Teleport ability - defined after GameManager class
    void teleport (Character& target, GameManager* gameManager);
};

/**
 * Synax Character Class - The princess with deadly eyes
 * Has the ability to instantly kill any target she can see
 */
class Synax : public Character {
public:
    // Constructor - initializes Synax with moderate health
    Synax() : Character("Synax", 110, 110) {}

    // Display character information and lore
    void displayInfo() const override {
        std::cout << "The name is " << character_name << "and the health is " << health << std::endl;
        std::cout << "Synax is a princess, she has special eyes. She can kill everything she can see" << std::endl;
    }

    // Special ability - activate deadly eyes
    void useSpecialAbility(Character& target, GameManager* gameManager = nullptr) override {
        activateEyes(target);
    }

private:
    // Instantly kill the target with special eyes
    void activateEyes(Character& target) {
        std::cout << target.getName() << " is dead." << std::endl;
        target.setHealth();  // Set health to 0 (instant death)
    }
};

/**
 * Kahray Character Class - The warrior with sword skills
 * Balanced fighter with defensive capabilities
 */
class Kahray : public Character {
public:
    // Constructor - initializes Kahray with high health (tank-like)
    Kahray() : Character("Kahray", 150, 150) {}

    // Display character information and lore
    void displayInfo() const override {
        std::cout << "The name is " << character_name << "and the health is " << health << std::endl;
        std::cout << "Kahray is a warrior, he has a sword. He can protect himself and his friends" << std::endl;
    }

    // Special ability - sword attack
    void useSpecialAbility(Character& target, GameManager* gameManager = nullptr) override {
        useSword(target);
    }
    
private:
    // Moderate damage sword attack
    void useSword(Character& target) {
        int damage = 35;
        target.takeDamage(damage);
    }
};

/**
 * Enemy Character Class - Standard enemy type
 * Basic hostile character that attacks the player
 */
class Enemy : public Character {
public:
    // Constructor - initializes Enemy with moderate health
    Enemy() : Character("Enemy", 80, 80) {}

    // Display character information and lore
    void displayInfo() const override {
        std::cout << "The name is " << character_name << " and the health is " << health << std::endl;
        std::cout << "Enemy is a bad guy, he wants to kill everyone. He is very dangerous" << std::endl;
    }

    // Special ability - basic attack
    void useSpecialAbility(Character& target, GameManager* gameManager = nullptr) override {
        killHero(target);
    }
    
private:
    // Standard enemy attack
    void killHero(Character& target) {
        int damage = 20;
        target.takeDamage(damage);
        std::cout << character_name << " attacks " << target.getName() << " for " << damage << " damage." << std::endl;
    }
};

/**
 * GameManager Class - Main game controller
 * Handles game state, map management, combat, and player movement
 */
class GameManager {
private:
    std::unique_ptr<Character> player;                              // Player character
    std::map<std::pair<int, int>, std::unique_ptr<Enemy>> enemiesOnMap;  // Enemy positions
    std::string gameMessage;                                        // Message to display to player

    Map gameMap;                                                   // Game world map
    int playerX, playerY;                                          // Player coordinates
    
public:
    // Constructor - initializes game with default map and player position
    GameManager() : gameMap(15, 10), playerX(2), playerY(2), gameMessage("") {
        std::cout << "Welcome to Ankr" << std::endl;
    }

    // Spawn specified number of enemies at random locations
    void spawnEnemies(int count) {
        // Initialize random number generator with current time
        unsigned seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
        std::mt19937 generator(seed);

        // Define coordinate ranges (avoiding walls)
        std::uniform_int_distribution<int> distX(1, 18);
        std::uniform_int_distribution<int> distY(1, 13);

        std::cout << "Spawning " << count << " enemies..." << std::endl;

        for (int i = 0; i < count; ++i) {
            int x, y;

            // Find empty tile for enemy spawn
            do {
                x = distX(generator);
                y = distY(generator);
            } while (gameMap.getTile(x, y) != TileType::EMPTY);

            // Create enemy and place on map
            enemiesOnMap[std::make_pair(x, y)] = std::make_unique<Enemy>();
            gameMap.setTile(x, y, TileType::ENEMY);

            std::cout << "An Enemy has spawned at (" << x << ", " << y << ")." << std::endl;
        }
    }

    // Let player choose their character class
    void chooseCharacter() {
        int choice;
        std::cout << "Choose your character:\n1. Aether\n2. Synax\n3. Kahray\n4. Enemy\n";
        std::cout << "Enter your choice (1-4): ";
        std::cin >> choice;

        // Create player character based on choice
        switch (choice) {
            case 1: player = std::make_unique<Aether>(); break;
            case 2: player = std::make_unique<Synax>(); break;
            case 3: player = std::make_unique<Kahray>(); break;
            case 4: player = std::make_unique<Enemy>(); break;
            default:
                std::cout << "Invalid choice! Defaulting to Aether.\n";
                player = std::make_unique<Aether>();
        }

        // Display chosen character's information
        player->displayInfo();
    }

    // Handle turn-based combat between player and enemy
    void startCombat(int enemyX, int enemyY) {
        auto& enemy = enemiesOnMap.at({enemyX, enemyY});

        // Combat loop continues until one character dies
        while (player->isAlive() && enemy->isAlive()) {
            // Clear screen for better combat display
            // for Windows
            #ifdef _WIN32
            system("cls");
            // for Linux and macOS
            #else
            system("clear");
            #endif

            // Display combat status
            std::cout << "---- Combat ----" << std::endl;
            std::cout << player->getName() << " HP: " << player->getHealthStatus() << std::endl;
            std::cout << enemy->getName() << " HP: " << enemy->getHealthStatus() << std::endl;
            std::cout << "Choose your action:\n1. Attack\n2. Use Special Ability\n3. Heal\n4. Run Away\n";
            
            int action;
            std::cout << "Enter your choice (1-4): ";
            std::cin >> action;

            // Handle player action
            if (action == 1) {
                // Basic attack
                enemy->takeDamage(20);
            } else if (action == 2) {
                // Use character's special ability
                player->useSpecialAbility(*enemy, this);
            } else if (action == 3) {
                // Heal player
                player->heal(15);
            } else if (action == 4) {
                // Flee from combat
                std::cout << player->getName() << " runs away from the combat!" << std::endl;
                return;
            } else {
                std::cout << "Invalid action! Try again." << std::endl;
                continue;
            }

            // Enemy counter-attack (if still alive)
            if (enemy->isAlive()) {
                enemy->useSpecialAbility(*player);
            }
            
            // Pause for player to read combat results
            std::cout << "Devam etmek icin bir tusa basin...";
            std::cin.ignore();
            std::cin.get();
        }
        
        // Handle combat results
        if (!player->isAlive()) {
            std::cout << player->getName() << " is dead." << std::endl;
            std::cout << "Game Over!" << std::endl;
            exit(0);
        } else {
            std::cout << enemy->getName() << " is defeated!" << std::endl;
            enemiesOnMap.erase({enemyX, enemyY});  // Remove defeated enemy
            std::cout << "Press Enter to continue." << std::endl;
            std::cin.get();
        }
    }

    // Main game loop - handles player movement and exploration
    void explorationLoop() {
        char input = ' ';
        
        // Continue until player quits
        while (input != 'q' && input != 'Q') {
            // Clear screen for fresh display
            // for Windows
            #ifdef _WIN32
            system("cls");
            // for Linux and macOS  
            #else
            system("clear");
            #endif

            // Display game status and instructions
            std::cout << "You are at (" << playerX << ", " << playerY << "). Use WASD to move. Press 'q' to quit.\n";
            std::cout << "Hp: " << player->getHealthStatus() << std::endl;
            gameMap.display();
            std::cout << "\n" << gameMessage << std::endl;
            gameMessage = "";  // Clear message after displaying

            std::cout << "Move: ";
            input = getch_direct();  // Get single character input without Enter

            // Clear current player position
            gameMap.setTile(playerX, playerY, TileType::EMPTY);

            // Calculate next position based on input
            int nextX = playerX;
            int nextY = playerY;

            switch (input) {
                case 'w': case 'W': nextY--; break;  // Move up
                case 's': case 'S': nextY++; break;  // Move down
                case 'a': case 'A': nextX--; break;  // Move left
                case 'd': case 'D': nextX++; break;  // Move right
                case 'q': case 'Q': 
                    std::cout << "Exiting game.\n"; 
                    return;
                default: 
                    std::cout << "Invalid command!\n"; 
                    continue;
            }

            // Check what's at the next position
            TileType nextTile = gameMap.getTile(nextX, nextY);

            if (nextTile == TileType::WALL) {
                // Can't move into walls
                std::cout << "You hit the wall!\n";
            } else if (nextTile == TileType::ENEMY) {
                // Encounter enemy - start combat
                std::cout << "You met the enemy\n";
                startCombat(nextX, nextY);
                gameMap.setTile(nextX, nextY, TileType::EMPTY);  // Clear enemy position
                playerX = nextX;
                playerY = nextY;
            } else {
                // Valid move - update player position
                playerX = nextX;
                playerY = nextY;
            }
            
            // Update player position on map
            gameMap.setTile(playerX, playerY, TileType::PLAYER);
        }
    }

    // Initialize and start the game
    void runGame() {
        chooseCharacter();           // Let player select character
        spawnEnemies(3);            // Place enemies on map
        gameMap.setTile(playerX, playerY, TileType::PLAYER);  // Place player on map
        explorationLoop();          // Start main game loop
    }

    // Public accessors for other classes
    Map& getMap() {
        return gameMap;
    }
    
    // Update player position (used by teleport ability)
    void setPlayerPosition(int x, int y) {
        gameMap.setTile(playerX, playerY, TileType::EMPTY);  // Clear old position
        playerX = x;
        playerY = y;
        gameMap.setTile(playerX, playerY, TileType::PLAYER);  // Set new position
    }
};

// Implementation of Aether's teleport ability
// (Must be defined after GameManager class is complete)
void Aether::teleport(Character& target, GameManager* gameManager) {
    std::cout << "Where do you want to teleport?\n";
    int newX, newY;
    std::cout << "Enter X coordinate: ";
    std::cin >> newX;
    std::cout << "Enter Y coordinate: ";
    std::cin >> newY;

    Map& map = gameManager->getMap();

    // Check if target location is valid (empty tile)
    if (map.getTile(newX, newY) == TileType::EMPTY) {
        gameManager->setPlayerPosition(newX, newY);
        std::cout << character_name << " teleports to (" << newX << ", " << newY << ")." << std::endl;
        target.setHealth();  // Kill target as side effect of teleportation
    } else {
        std::cout << "Cannot teleport to (" << newX << ", " << newY << "). Tile is not empty." << std::endl;
    }
}

/**
 * Main Function - Entry point of the game
 * Creates GameManager instance and starts the game
 */
int main() {
    GameManager game;
    game.runGame();
    return 0;
}
