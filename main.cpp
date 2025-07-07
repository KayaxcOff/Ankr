#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <random>
#include <chrono>
#include <map>
#include <utility>

// for Windows
#ifdef _WIN32
#include <conio.h>
char getch_direct() {
    return _getch();
}

// for Linux and macOS
#else
#include <termios.h>
#include <unistd.h>
char getch_direct() {
    char buf = 0;
    struct termios old = {0};
    if (tcgetattr(0, &old) < 0)
        perror("tcsetattr()");
    old.c_lflag &= ~ICANON;
    old.c_lflag &= ~ECHO;
    old.c_cc[VMIN] = 1;
    old.c_cc[VTIME] = 0;
    if (tcsetattr(0, TCSANOW, &old) < 0)
        perror("tcsetattr ICANON");
    if (read(0, &buf, 1) < 1)
        perror("read()");
    old.c_lflag |= ICANON;
    old.c_lflag |= ECHO;
    if (tcsetattr(0, TCSADRAIN, &old) < 0)
        perror("tcsetattr ~ICANON");
    return buf;
}
#endif

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
    std::map<std::pair<int, int>, std::unique_ptr<Enemy>> enemiesOnMap;
    std::string gameMessage;

    Map gameMap;
    int playerX, playerY;
public:
    GameManager() : gameMap(15, 10), playerX(2), playerY(2), gameMessage("") {
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

            enemiesOnMap[std::make_pair(x, y)] = std::make_unique<Enemy>();
            //enemies.push_back(std::make_unique<Enemy>());
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

    void startCombat(int enemyX, int enemyY) {
        auto& enemy = enemiesOnMap.at({enemyX, enemyY});

        while (player->isAlive() && enemy->isAlive()) {
            #ifdef _WIN32
            system("cls");
            #else
            system("clear");
            #endif

            std::cout << "---- Combat ----" << std::endl;
            std::cout << player->getName() << " HP: " << player->getHealthStatus() << std::endl;
            std::cout << enemy->getName() << " HP: " << enemy->getHealthStatus() << std::endl;
            std::cout << "Choose your action:\n1. Attack\n2. Use Special Ability\n3. Heal\n4. Run Away\n";
            int action;
            std::cout << "Enter your choice (1-4): ";
            std::cin >> action;

            if (action == 1) {
                enemy->takeDamage(20);
            } else if (action == 2) {
                player->useSpecialAbility(*enemy);
            } else if (action == 3) {
                player->heal(15);
            } else if (action == 4) {
                std::cout << player->getName() << " runs away from the combat!" << std::endl;
                return;
            } else {
                std::cout << "Invalid action! Try again." << std::endl;
                continue;
            }

            if (enemy->isAlive()) {
                enemy->useSpecialAbility(*player);
            }
            std::cout << "Devam etmek icin bir tusa basin...";
            std::cin.ignore();
            std::cin.get();
        }
            if (!player->isAlive()) {
                std::cout << player->getName() << " is dead." << std::endl;
                std::cout << "Game Over!" << std::endl;
                exit(0);
            } else {
                std::cout << enemy->getName() << " is defeated!" << std::endl;
                enemiesOnMap.erase({enemyX, enemyY});
                std::cout << "Press Enter to continue." << std::endl;
                std::cin.get();
            }
    }

    void explorationLoop() {
        char input = ' ';
        while (input != 'q' && input != 'Q') {

            #ifdef _WIN32
            system("cls");
            #else
            system("clear");
            #endif

            std::cout << "You are at (" << playerX << ", " << playerY << "). Use WASD to move. Press 'q' to quit.\n";
            std::cout << "Hp: " << player->getHealthStatus() << std::endl;
            gameMap.display();
            std::cout << "\n" << gameMessage << std::endl;
            gameMessage = "";

            std::cout << "Move: ";
            input = getch_direct();


            gameMap.setTile(playerX, playerY, TileType::EMPTY);

            int nextX = playerX;
            int nextY = playerY;

            switch (input) {
                case 'w': case 'W': nextY--; break;
                case 's': case 'S': nextY++; break;
                case 'a': case 'A': nextX--; break;
                case 'd': case 'D': nextX++; break;
                case 'q': case 'Q': std::cout << "Exiting game.\n"; return;
                default: std::cout << "Invalid command!\n"; continue;
            }

            TileType nextTile = gameMap.getTile(nextX, nextY);

            if (nextTile == TileType::WALL) {
                std::cout << "You hit the wall!\n";
            } else if (nextTile == TileType::ENEMY) {
                std::cout << "You met the enemy\n";
                startCombat(nextX, nextY);
                gameMap.setTile(nextX, nextY, TileType::EMPTY);
                playerX = nextX;
                playerY = nextY;
            } else {
                playerX = nextX;
                playerY = nextY;
            }
            gameMap.setTile(playerX, playerY, TileType::PLAYER);
        }
    }

    void runGame() {
        chooseCharacter();
        spawnEnemies(3);
        gameMap.setTile(playerX, playerY, TileType::PLAYER);
        explorationLoop();
    }
};

int main() {
    GameManager game;
    game.runGame();
    return 0;
}
