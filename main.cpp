#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <memory>
#include <random>
#include <chrono>
#include <map>
#include <utility>
#include <limits>

#ifdef _WIN32
#include <conio.h>
char getch_direct() {
    return _getch();
}

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

class GameManager;
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
        if (health > maxHealth) health = maxHealth;
        std::cout << character_name << " heals " << amount << " health. Health is now " << health << std::endl;
    }

    bool isAlive() const {
        return health > 0;
    }

    void setHealth() {
        health = 0;
    }

    std::string getName() const {
        return character_name;
    }

    std::string getHealthStatus() const {
        return std::to_string(health);
    }

    virtual void displayInfo() const = 0;
    virtual void useSpecialAbility(Character& target, GameManager* gameManager = nullptr) = 0;
};

enum class TileType {
    EMPTY = 0,
    WALL,
    HERO,
    ENEMY
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
                    case TileType::HERO:     std::cout << "H "; break;
                    case TileType::ENEMY:    std::cout << "E "; break;
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

    int getWidth() const { return width; }
    int getHeight() const { return height; }
};

class Aether : public Character {
public:
    Aether() : Character("Aether", 65, 80) {}
    void displayInfo() const override {
        std::cout << "The name is " << character_name << " and the health is " << health << std::endl;
        std::cout << "Aether is smartest person in the universe, he made a object with magic called Sphere of Everything. I believe object's name is clear" << std::endl;
    }

    void useSpecialAbility(Character& target, GameManager* gameManager = nullptr) override {
        int sphereOfEverything;
        std::cout << "1- Power Strike\n2- Cosmic Heal\n3- Universe Reset\n4- Teleport" << std::endl;
        std::cout << "Enter command of Sphere of Everything: ";
        std::cin >> sphereOfEverything;
        switch (sphereOfEverything) {
            case 1: powerStrike(target); break;
            case 2: cosmicHeal(); break;
            case 3: universeReset(); break;
            case 4: teleport(target, gameManager); break;
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
        if (health > maxHealth) maxHealth = health;
        std::cout << character_name << " uses Cosmic Heal. Health is now " << health << std::endl;
    }

    int universeReset() {
        std::cout << character_name << " uses Universe Reset!" << std::endl;
        std::exit(0);
        return 0;
    }

    void teleport(Character& target, GameManager* gameManager);
};

class Synax : public Character {
public:
    Synax() : Character("Synax", 76, 110) {}
    void displayInfo() const override {
        std::cout << "The name is " << character_name << " and the health is " << health << std::endl;
        std::cout << "Synax is a princess, she has special eyes. She can kill everything she can see" << std::endl;
    }

    void useSpecialAbility(Character& target, GameManager* gameManager = nullptr) override {
        int activateEyes;
        std::cout << "1- Eyes of Death\n2- Eyes of Heal" << std::endl;
        std::cout << "Activate red eyes: ";
        std::cin >> activateEyes;
        switch (activateEyes) {
            case 1: eyes_death(target); break;
            case 2: eyes_heal(target); break;
            default: std::cout << "Invalid command!" << std::endl; break;
        }
    }
private:
    void eyes_death(Character& target) {
        if (target.isAlive()) {
            std::cout << target.getName() << " is killed by Eyes of Death!" << std::endl;
            target.setHealth();
        } else {
            std::cout << target.getName() << " is already dead!" << std::endl;
        }
    }

    void eyes_heal(Character& target) {
        int amount;
        std::cout << "Enter amount to heal: ";
        std::cin >> amount;
        if (amount <= 0) { std::cout << "Invalid amount!" << std::endl; return; }
        target.takeDamage(amount);
        health += amount;
        if (health > maxHealth) maxHealth = health;
        std::cout << character_name << " steals " << amount << " health from " << target.getName() << std::endl;
    }
};

class Kahray : public Character {
public:
    Kahray() : Character("Kahray", 120, 150) {}
    void displayInfo() const override {
        std::cout << "The name is " << character_name << " and the health is " << health << std::endl;
        std::cout << "Kahray is a warrior, he has a sword. He can protect himself and his friends" << std::endl;
    }

    void useSpecialAbility(Character& target, GameManager* gameManager = nullptr) override {
        int warriorAbility;
        std::cout << "Choose one of Kahray's abilities " << std::endl;
        std::cout << "1- Use Sword\n2- Wall Shield\n-> ";
        std::cin >> warriorAbility;
        switch (warriorAbility) {
            case 1: useSword(target); break;
            case 2: wallShield(); break;
            default: std::cout << "Invalid command!" << std::endl; break;
        }
    }
private:
    void useSword(Character& target) {
        int damage = 35;
        target.takeDamage(damage);
        std::cout << character_name << " uses sword on " << target.getName() << " for " << damage << " damage." << std::endl;
    }
    void wallShield() {
        health = maxHealth;
        std::cout << character_name << " uses Wall Shield. Health is now " << health << std::endl;
    }
};

class Yroy : public Character {
public:
    Yroy() : Character("Yroy", 50, 55) {}
    void displayInfo() const override {
        std::cout << "The name is " << character_name << " and the health is " << health << std::endl;
        std::cout << "Yroy is a child, he wants to be a hero. He is not strong, but he is smart and his idol is Aether" << std::endl;
    }

    void useSpecialAbility(Character &target, GameManager *gameManager = nullptr) override {
        int miniAbility;
        std::cout << "1- Mini Attack\n2- Mini Heal" << std::endl;
        std::cout << "Choose your ability: ";
        std::cin >> miniAbility;
        switch (miniAbility) {
            case 1: miniAttack(target); break;
            case 2: miniHeal(target); break;
            default: std::cout << "Invalid command!" << std::endl; break;
        }
    }
private:
    void miniAttack(Character &target) {
        int damage = 10;
        std::cout << character_name << " uses Mini Attack on " << target.getName() << " for " << damage << " damage." << std::endl;
        target.takeDamage(damage);
    }

    void miniHeal(Character &target) {
        int damage = 10;
        std::cout << character_name << " steals " << damage << " health from " << target.getName() << std::endl;
        target.takeDamage(damage);
        health += damage;
        if (health > maxHealth) health = maxHealth;
    }
};

class Batley : public Character {
public:
    Batley() : Character("Batley", 80, 90) {}
    void displayInfo() const override {
        std::cout << "The name is " << character_name << " and the health is " << health << std::endl;
        std::cout << "What a old man. In past, he was a great warrior. But now, he is old and weak. He can not fight anymore, but universe need him" << std::endl;
    }

    void useSpecialAbility(Character &target, GameManager *gameManager = nullptr) override {
        lastAttack(target);
    }
private:
    void lastAttack(Character &target) {
        int damage = 10;
        std::cout << character_name << " uses Last Attack on " << target.getName() << " for " << damage << " damage." << std::endl;
        target.takeDamage(damage);
    }
};

class Enemy : public Character {
public:
    Enemy(std::string name = "Generic Enemy", int hp = 50, int maxHp = 50)
        : Character(name, hp, maxHp) {}
    void displayInfo() const override {
        std::cout << "The name is " << character_name << " and the health is " << health << std::endl;
        std::cout << "A generic enemy." << std::endl;
    }
    void useSpecialAbility(Character &target, GameManager *gameManager = nullptr) override {
        int damage = 10;
        std::cout << character_name << " attacks " << target.getName() << " for " << damage << " damage." << std::endl;
        target.takeDamage(damage);
    }
};

class Skeleton : public Enemy {
public:
    Skeleton() : Enemy("Skeleton", 40, 40) {}
    void displayInfo() const override {
        std::cout << "The name is " << character_name << " and the health is " << health << std::endl;
        std::cout << "A Skeleton, a mindless creature, it attacks anything that moves." << std::endl;
    }

    void useSpecialAbility(Character &target, GameManager *gameManager = nullptr) override {
        int damage = 15;
        std::cout << character_name << " uses Bone Crush on " << target.getName() << " for " << damage << " damage." << std::endl;
        target.takeDamage(damage);
    }
};

class Witch : public Enemy {
public:
    Witch() : Enemy("Witch", 60, 65) {}
    void displayInfo() const override {
        std::cout << "The name is " << character_name << " and the health is " << health << std::endl;
        std::cout << "A Witch, she casts dark spells to weaken her enemies." << std::endl;
    }

    void useSpecialAbility(Character &target, GameManager *gameManager = nullptr) override {
        unsigned seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
        std::mt19937 generator(seed);
        std::uniform_int_distribution<int> dist(1, 2);
        int spellChoice = dist(generator);
        switch (spellChoice) {
            case 1: std::cout << getName() << " casts Death Magic!" << std::endl; deathMagic(target); break;
            case 2: std::cout << getName() << " casts Curse!" << std::endl; curse(target); break;
        }
    }
private:
    void deathMagic(Character &target) { int magic = 20; target.takeDamage(magic); }
    void curse(Character &target) { int curse = 10; target.takeDamage(curse); }
};

class Kheon : public Enemy {
public:
    Kheon() : Enemy("Kheon", 70, 125) {}
    void displayInfo() const override {
        std::cout << "The name is " << character_name << " and the health is " << health << std::endl;
        std::cout << "Kheon and Aether, they were friends. When Kheon's love Oix is dead, Kheon wanted use Sphere of Everything to bring her to life but Aether was against that idea" << std::endl;
    }

    void useSpecialAbility(Character &target, GameManager *gameManager = nullptr) override {
        unsigned seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
        std::mt19937 generator(seed);
        std::uniform_int_distribution<int> dist(1, 3);
        int fireStrike = dist(generator);
        switch (fireStrike) {
            case 1: burnEverything(target); break;
            case 2: fireShield(target); break;
            case 3: burnMap(); break;
            default: std::cout << "Invalid command!" << std::endl; break;
        }
    }
private:
    void burnEverything(Character &target) {
        std::cout << target.getName() << " is burned to death!" << std::endl;
        target.setHealth();
    }

    void fireShield(Character &target) {
        int fireDamage = 60;
        target.takeDamage(fireDamage);
        health += (fireDamage/2);
        if (health > maxHealth) health = maxHealth;
        std::cout << character_name << " uses Fire Shield, dealing " << fireDamage << " damage and healing for " << (fireDamage/2) << std::endl;
    }

    int burnMap() { 
        std::exit(0);
        return 0; 
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
        std::uniform_int_distribution<int> distX(1, gameMap.getWidth() - 2);
        std::uniform_int_distribution<int> distY(1, gameMap.getHeight() - 2);
        std::uniform_int_distribution<int> enemyTypeDist(0, 2);
        for (int i = 0; i < count; ++i) {
            int x, y;
            do {
                x = distX(generator);
                y = distY(generator);
            } while (gameMap.getTile(x, y) != TileType::EMPTY || (x == playerX && y == playerY));
            int enemyType = enemyTypeDist(generator);
            std::unique_ptr<Enemy> newEnemy;
            switch (enemyType) {
                case 0: newEnemy = std::make_unique<Skeleton>(); break;
                case 1: newEnemy = std::make_unique<Witch>(); break;
                case 2: newEnemy = std::make_unique<Kheon>(); break;
            }
            std::cout << newEnemy->getName() << " has spawned at (" << x << ", " << y << ")." << std::endl;
            enemiesOnMap[std::make_pair(x, y)] = std::move(newEnemy);
            gameMap.setTile(x, y, TileType::ENEMY);
        }
    }

    void chooseCharacter() {
        int choice;
        std::cout << "Choose your character:\n1. Aether\n2. Synax\n3. Kahray\n4. Yroy\n5. Batley\n";
        std::cout << "Enter your choice (1-5): ";
        std::cin >> choice;

        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch (choice) {
            case 1: player = std::make_unique<Aether>(); break;
            case 2: player = std::make_unique<Synax>(); break;
            case 3: player = std::make_unique<Kahray>(); break;
            case 4: player = std::make_unique<Yroy>(); break;
            case 5: player = std::make_unique<Batley>(); break;
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
                std::cout << player->getName() << " attacks " << enemy->getName() << " for 20 damage." << std::endl;
            } else if (action == 2) {
                player->useSpecialAbility(*enemy, this);
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
                enemy->useSpecialAbility(*player, this);
            }
            std::cout << "Press Enter to continue...";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Savaş içindeki girdiler için de buffer temizliği
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
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Savaş sonrası için de buffer temizliği
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
            std::cout << "HP: " << player->getHealthStatus() << std::endl;
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
                case 'q': case 'Q':
                    std::cout << "Exiting game.\n";
                    return;
                default:
                    gameMessage = "Invalid command!";
                    gameMap.setTile(playerX, playerY, TileType::HERO);
                    continue;
            }
            TileType nextTile = gameMap.getTile(nextX, nextY);
            if (nextTile == TileType::WALL) {
                gameMessage = "You hit the wall!";
                gameMap.setTile(playerX, playerY, TileType::HERO);
            } else if (nextTile == TileType::ENEMY) {
                gameMessage = "You met an enemy!";
                startCombat(nextX, nextY);
                gameMap.setTile(nextX, nextY, TileType::EMPTY);
                playerX = nextX;
                playerY = nextY;
                gameMap.setTile(playerX, playerY, TileType::HERO);
            } else {
                playerX = nextX;
                playerY = nextY;
                gameMap.setTile(playerX, playerY, TileType::HERO);
            }
        }
    }

    void runGame() {
        chooseCharacter();
        std::cout << "\nPress Enter to start the game...";
        std::cin.get();
        spawnEnemies(3);
        gameMap.setTile(playerX, playerY, TileType::HERO);
        explorationLoop();
    }

    Map& getMap() {
        return gameMap;
    }

    void setPlayerPosition(int x, int y) {
        gameMap.setTile(playerX, playerY, TileType::EMPTY);
        playerX = x;
        playerY = y;
        gameMap.setTile(playerX, playerY, TileType::HERO);
    }
};

void Aether::teleport(Character& target, GameManager* gameManager) {
    if (!gameManager) {
        std::cout << "Teleportation failed: Game context not available." << std::endl;
        return;
    }
    std::cout << "Where do you want to teleport?\n";
    int newX, newY;
    std::cout << "Enter X coordinate: ";
    std::cin >> newX;
    std::cout << "Enter Y coordinate: ";
    std::cin >> newY;

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    Map& map = gameManager->getMap();
    if (newX > 0 && newX < map.getWidth() - 1 && newY > 0 && newY < map.getHeight() - 1 && map.getTile(newX, newY) == TileType::EMPTY) {
        gameManager->setPlayerPosition(newX, newY);
        std::cout << character_name << " teleports to (" << newX << ", " << newY << ")." << std::endl;
    } else {
        std::cout << "Cannot teleport to (" << newX << ", " << newY << "). Tile is not empty or out of bounds." << std::endl;
    }
}

int main() {
    GameManager game;
    game.runGame();
    return 0;
}
