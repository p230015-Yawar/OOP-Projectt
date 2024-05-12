#include <iostream>
#include <ctime>
#include <cstdlib>
#include<fstream>

using namespace std;

// Base class representing a player
class Player {
protected:
    string name;

public:
    // Constructor to initialize the player's name
    Player(const string& playerName) : name(playerName) {}

    // Pure virtual function to be implemented by derived classes
    virtual void takeTurn() = 0;

    // Virtual function to communicate a message to another player
    virtual void communicateMessage(Player* receiver) {
        cout << name << " says to " << receiver->getName() << ": Watch out!" << endl;
    }

    // Getter function for the player's name
    string getName() const {
        return name;
    }

    // Friend function to facilitate communication between players
    friend void communicate(Player* sender, Player* receiver, const string& message);
~Player(){
}
};

// Function to communicate a message between players
void communicate(Player* sender, Player* receiver, const string& message) {
    if (message.empty()) {
        // If the message is empty, do nothing
    } else {
        // Otherwise, print the message
        cout << sender->getName() << " says to " << receiver->getName() << ": " << message << endl;
    }
}

// Derived class representing a player in a Russian Roulette game
class RussianRoulettePlayer : public Player {
public:
    // Constructor to initialize the player's name
    RussianRoulettePlayer(const string& playerName) : Player(playerName) {}

    // Implementation of the takeTurn function for a Russian Roulette player
    void takeTurn() override {
        cout << name << ", it's your turn. Press enter to pull the trigger: ";
        cin.ignore(); // Clear the input buffer
        cin.get();
    }

    // Override of the communicateMessage function to provide a specific message
    void communicateMessage(Player* receiver) override {
        cout << name << " says to " << receiver->getName() << ": Watch out!" << endl;
    }
};

// Class representing a game of Russian Roulette
class RussianRoulette {
private:
    int numChambers;    // Number of chambers in the revolver
    int currentChamber; // Current chamber with the bullet

public:
    // Constructor to initialize the number of chambers
    RussianRoulette(int chambers) : numChambers(chambers), currentChamber(0) {}

    // Function to spin the chamber and randomly select a chamber for the bullet
    void spinChamber() {
        currentChamber = rand() % numChambers + 1;
    }

    // Function to simulate firing the gun and checking if the bullet is fired
    bool fire() {
        return (rand() % numChambers + 1) == currentChamber;
    }

    // Getter function for the number of chambers
    int getNumChambers() const {
        return numChambers;
    }

    // Getter function for the current chamber with the bullet
    int getCurrentChamber() const {
        return currentChamber;
    }
    //destructor of russian roulette
    ~RussianRoulette(){
	}
};

int main() {
    srand(static_cast<unsigned>(time(0))); // Seed the random number generator

    int chambers, numPlayers;

    // Get the number of chambers in the revolver from the user
    cout << "Enter the number of chambers in the revolver: ";
    cin >> chambers;

    // Get the number of players from the user
    cout << "Enter the number of players: ";
    cin >> numPlayers;

    // Create an instance of the Russian Roulette game
    RussianRoulette game(chambers);

    // Array to store pointers to RussianRoulettePlayer objects
    RussianRoulettePlayer* players[10]; // Maximum 10 players

    // Get the names of the players from the user and create player objects
    for (int i = 0; i < numPlayers; ++i) {
        string playerName;
        cout << "Enter the name of player " << i + 1 << ": ";
        cin >> playerName;
        players[i] = new RussianRoulettePlayer(playerName);
    }

    // Print game information
    cout << "Let's play Russian Roulette with " << numPlayers << " players!" << endl;
    cout << "You have a " << (1.0 / game.getNumChambers()) * 100 << "% chance of firing a bullet on each trigger pull." << endl;

    // Spin the chamber to randomly select a chamber for the bullet
    game.spinChamber();
    cout << "Spinning the chamber..." << endl;

    int currentPlayer = 0;
    bool gameOver = false;

    // Main game loop
    while (!gameOver) {
        // Communicate a warning message to the next player
        players[currentPlayer]->communicateMessage(players[(currentPlayer + 1) % numPlayers]);

        // Get a custom message from the current player to send to the next player
        string customMessage;
        cout << "Enter a custom message to send to " << players[(currentPlayer + 1) % numPlayers]->getName() << ": ";
        cin.ignore();
        getline(cin, customMessage);
        communicate(players[currentPlayer], players[(currentPlayer + 1) % numPlayers], customMessage);

        // Allow the current player to take their turn
        players[currentPlayer]->takeTurn();

        // Check if the bullet is fired
        if (game.fire()) {
            cout << "Bang! " << players[currentPlayer]->getName() << " fired the bullet! Game over." << endl;
        // Saves the records of players who won    
            ofstream append("E:/text.txt",ios::app);
            append<<players[currentPlayer]->getName()<<" win the game"<<endl;
            gameOver = true;
        } else {
            cout << "Click! it was empty Opponent survived" << "  Passing the gun to the next player." << endl;
            currentPlayer = (currentPlayer + 1) % numPlayers; // Move to the next player
        }
    }
    // Delete dynamically allocated player objects to prevent memory leaks
    for (int i = 0; i < numPlayers; ++i) {
        delete players[i];
    }

    return 0;
}
