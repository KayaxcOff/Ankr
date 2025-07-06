#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <random>
#include <chrono>

class Character {
protected:
    std::string character_name;
    int health;
    int maxHealth;
public:
    Character(std::string name, int hp, int maxHp)
        : character_name(name), health(hp), maxHealth(maxHp) {};

    virtual ~Character() {};

    void takeDamage(int damage) {
        health -= damage;
        if (health <= 0) health = 0;
        std::cout << character_name << " takes " << damage << " damage. Health is now " << health << std::endl;
    }

    void heal(int amount) {
        health += amount;
        std::cout << character_name << " heals " << amount << " health. Health is now " << health << std::endl;
    }

    bool isAlive() const{
        return health > 0;
    }

    void setHealth() {
        health = 0;
    }

    std::string getName() const{
        return character_name;
    }

    std::string getHealthStatus() const{
        return std::to_string(health);
    }

    virtual void displayInfo() const = 0;
    virtual void useSpecialAbility(Character& target) = 0;
};

enum class TileType {
    EMPTY = 0,
    WALL,
    PLAYER,
    ENEMY,
    TREASURE
};

class Map {
private:
    int width;
    int height;
    std::vector<std::vector<TileType>> grid;
public:
    Map(int w, int h) : width(w), height(h) {
        grid.resize(height, std::vector<TileType>(width, TileType::EMPTY));

        for (int i = 0; i < width; ++i) {
            grid[0][i] = TileType::WALL;
            grid[height - 1][i] = TileType::WALL;
        }
        for (int i = 0; i < height; ++i) {
            grid[i][0] = TileType::WALL;
            grid[i][width - 1] = TileType::WALL;
        }
    }

    void display() const {
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                switch (grid[y][x]) {
                    case TileType::EMPTY:    std::cout << ". "; break;
                    case TileType::WALL:     std::cout << "# "; break;
                    case TileType::PLAYER:   std::cout << "P "; break;
                    case TileType::ENEMY:    std::cout << "E "; break;
                    case TileType::TREASURE: std::cout << "T "; break;
                }
            }
            std::cout << std::endl;
        }
    }

    void setTile(int x, int y, TileType type) {
        if (y >= 0 && y < height && x >= 0 && x < width) {
            grid[y][x] = type;
        }
    }

    TileType getTile(int x, int y) const {
        if (y >= 0 && y < height && x >= 0 && x < width) {
            return grid[y][x];
        }
        return TileType::WALL;
    }
};

class Aether : public Character {
public:
    Aether() : Character("Aether", 100, 100) {}

    void displayInfo() const override {
        std::cout << "The name is " << character_name << "and the health is " << health << std::endl;
        std::cout << "He is smartest person in the universe and he is just a boy. Aether has most powerful object in the universe. His object can does everything it's master's want";
    }

    void useSpecialAbility(Character& target) override {
        int getCommand;

        std::cout << "1- Power Strike" << std::endl;
        std::cout << "2- Cosmic Heal" << std::endl;
        std::cout << "3- Universe Reset" << std::endl;
        std::cout << "4- Teleport" << std::endl;
        std::cout << "Choose your command: ";
        std::cin >> getCommand;

        switch (getCommand) {
            case 1: powerStrike(target); break;
            case 2: cosmicHeal(); break;
            case 3: universeReset(); break;
            case 4: teleport(target); break;
            default: std::cout << "Invalid command!" << std::endl; break;
        }
    }
private:
    void powerStrike(Character& target) {
        int damage = 500;
        target.takeDamage(damage);
        std::cout << character_name << " uses Power Strike on " << target.getName() << " for " << damage << " damage." << std::endl;
    }

    void cosmicHeal() {
        int healAmount = 100;
        health += healAmount;
        if (health > maxHealth) health = maxHealth;
        std::cout << character_name << " uses Cosmic Heal. Health is now " << health << std::endl;
    }

    int universeReset() {
        std::cout << character_name << " resets the universe. All characters are reset to their initial state." << std::endl;
        return 0;
    }

    int teleport (Character& target) {
        std::cout << "Where do you want to teleport?\n";
        int newX, newY;
        std::cout << "Enter X coordinate: ";
        std::cin >> newX;
        std::cout << "Enter Y coordinate: ";
        std::cin >> newY;

        // I'll look at here later
    }
};

class Synax : public Character {
public:
    Synax() : Character("Synax", 110, 110) {}

    void displayInfo() const override {
        std::cout << "The name is " << character_name << "and the health is " << health << std::endl;
        std::cout << "Synax is a princess, she has special eyes. She can kill everything she can see" << std::endl;
    }

    void useSpecialAbility(Character& target) override {
        activateEyes(target);
    }

private:
    void activateEyes(Character& target) {
        std::cout << target.getName() << " is dead." << std::endl;
        target.setHealth();
    }
};

class Kahray : public Character {
public:
    Kahray() : Character("Kahray", 150, 150) {}

    void displayInfo() const override {
        std::cout << "The name is " << character_name << "and the health is " << health << std::endl;
        std::cout << "Kahray is a warrior, he has a sword. He can protect himself and his friends" << std::endl;
    }

    void useSpecialAbility(Character& target) override {
        useSword(target);
    }
private:
    void useSword(Character& target) {
        int damage = 35;
        target.takeDamage(damage);
    }
};

class Enemy : public Character {
public:
    Enemy() : Character("Enemy", 80, 80) {}

    void displayInfo() const override {
        std::cout << "The name is " << character_name << " and the health is " << health << std::endl;
        std::cout << "Enemy is a bad guy, he wants to kill everyone. He is very dangerous" << std::endl;
    }

    void useSpecialAbility(Character& target) override {
        killHero(target);
    }
private:
    void killHero(Character& target) {
        int damage = 20;
        target.takeDamage(damage);
        std::cout << character_name << " attacks " << target.getName() << " for " << damage << " damage." << std::endl;
    }
};

class GameManager {
private:
    std::unique_ptr<Character> player;
    std::unique_ptr<Enemy> enemies;

    Map gameMap;
    int sizeX, sizeY;
public:
    GameManager() : gameMap(15, 10), sizeX(2), sizeY(2) {
        std::cout << "Welcome to Ankr" << std::endl;
    }

    void spawnEnemies(int count) {
        unsigned seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
        std::mt19937 generator(seed);

        std::uniform_int_distribution<int> distX(1, 18);
        std::uniform_int_distribution<int> distY(1, 13);

        std::cout << "Spawning " << count << " enemies..." << std::endl;

        for (int i = 0; i < count; ++i) {
            int x, y;

            do {
                x = distX(generator);
                y = distY(generator);
            } while (gameMap.getTile(x, y) != TileType::EMPTY);

            enemies.push_back(std::make_unique<Enemy>());
            gameMap.setTile(x, y, TileType::ENEMY);

            std::cout << "An Enemy has spawned at (" << x << ", " << y << ")." << std::endl;
        }
    }

    void chooseCharacter() {
        int choice;
        std::cout << "Choose your character:\n1. Aether\n2. Synax\n3. Kahray\n4. Enemy\n";
        std::cout << "Enter your choice (1-4): ";
        std::cin >> choice;

        switch (choice) {
            case 1: player = std::make_unique<Aether>(); break;
            case 2: player = std::make_unique<Synax>(); break;
            case 3: player = std::make_unique<Kahray>(); break;
            case 4: player = std::make_unique<Enemy>(); break;
            default:
                std::cout << "Invalid choice! Defaulting to Aether.\n";
                player = std::make_unique<Aether>();
        }

        player->displayInfo();
    }

    void explorationLoop() {
        char input = ' ';
        while (input != 'q') {

            #ifdef _WIN32
            system("cls");
            #else
            system("clear");
            #endif

            std::cout << "You are at (" << sizeX << ", " << sizeY << "). Use WASD to move. Press 'q' to quit.\n";
            gameMap.display();

            std::cout << "Move: ";
            std::cin >> input;


            gameMap.setTile(sizeX, sizeY, TileType::EMPTY);

            int nextX = sizeX;
            int nextY = sizeY;

            switch (input) {
                case 'w': case 'W': nextY--; break;
                case 's': case 'S': nextY++; break;
                case 'a': case 'A': nextX--; break;
                case 'd': case 'D': nextX++; break;
                case 'q': case 'Q': std::cout << "Exiting game.\n"; return;
                default: std::cout << "Invalid command!\n"; continue;
            }


            if (gameMap.getTile(nextX, nextY) != TileType::WALL) {

                sizeX = nextX;
                sizeY = nextY;
            } else {
                std::cout << "You hit a wall!\n";
            }

            gameMap.setTile(sizeX, sizeY, TileType::PLAYER);
        }
    }

    void runGame() {
        chooseCharacter();

        gameMap.setTile(sizeX, sizeY, TileType::PLAYER);
        explorationLoop();
    }
};

int main() {

}
