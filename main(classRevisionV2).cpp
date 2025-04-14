#include <iostream>
#include <string>
#include <iomanip>
#include <vector>

// function prototypes for funtions used by the Character class
int positiveIntegerEntry(std::string);
int integerEntry(std::string);
char yesOrNoEntry(std::string);

// character class prototype
class Character;

// function prototypes
int addCharacter(Character arr[],const int, int);
void viewParty(std::vector<Character>, int);
void editHP(Character arr[], int);
int rollDice(int, int, int);
void rollDiceMenu();
//************************************************************************* 
//* Character class definition block                                      *
//************************************************************************* 
class Character
{
    private:
        std::string name;
        int hp;
        int ac;
        int initiative;
        int initModifier;
        char isEnemy;       // Enemy and Party members are seperate flags
        char isPartyMem;    // because of possible ambiguous character cases.
    public:
        void createCharacter();
        void changeHp(int);    // This has been structured to work with the existing editHp function
        void modifyName();
        void modifyAc();
        void rollInit();    
        void setFlags();
        void displayStats();
        // The following get functions provide access to the private attributes
        std::string getName();
        int getHp();
        int getAc();
        int getInitiative();
        char getEnemyFlag();
        char getPartyFlag();
};

// This will be ran when creating a new character
void Character::createCharacter()
{
    std::cout << "Enter character name: ";
    std::cin >> name;
    hp = positiveIntegerEntry("Character HP");
    ac = positiveIntegerEntry("Character AC");
    initModifier = integerEntry("initiative modifier");
    // Preroll initiative value for the first turn
    rollInit();
    // Determine the characters Enemy and Party status
    setFlags();
}

// This function will simply take a change and apply it
// to the characters hp
void Character::changeHp(int change)
{
    hp += change;
    // This just prevents negative hp numbers.
    // We can remove the if statement if the overkill
    // amount is desired.
    if (hp < 0)
        hp = 0;
}
// This function will allow you to change the character name
void Character::modifyName()
{
    // Create a variable for the characters new name.
    std::string newName;
    std::cout << "Enter a new name for " << name << ": ";
    std::cin >> newName;
    name = newName;
}
// This function simply modifies ac
void Character::modifyAc() 
{
    // Create a variable for the characters new ac.
    int newAc;
    std::cout << "Enter a new AC value for " << name << ": ";
    std::cin >> newAc;
    ac = newAc;
}
// This one will need a little work
void Character::rollInit()
{
    // This turned out much simpler than expected,
    // the rollDice function just needs the relavent values.
    initiative = rollDice(1, 20, initModifier);
}
// Sets the Party and Enemy flag status
void Character::setFlags()
{
    // Determine if the character is an enemy
    char enemyStatus = yesOrNoEntry("Is this character an enemy?\n");
    if (enemyStatus == 'Y') {
        std::cout << name << " has been set as an enemy.\n";
    }
    isEnemy = enemyStatus;
    // Determine if the character is a party member
    char partyStatus = yesOrNoEntry("Is this character a party member?\n");
    if (partyStatus == 'Y') {
        std::cout << name << " has been set as a party member.\n";
    }
    isPartyMem = partyStatus;
}

void Character::displayStats()
{
    std::cout << "Name: " << std::left << std::setw(15) << name
                  << " | HP: " << hp
                  << " | AC: " << ac
                  << " | Party Member:" << isPartyMem
                  << " | Enemy:" << isEnemy << "\n"
                  << "Init Adjust:" << std::setw(9) << initModifier
                  << " | Initiative Roll: " << initiative << "\n\n";
}

//******This section contains the getfunctions******
// simply returns the characters name for display purposes
std::string Character::getName() {
    return name;
}

int Character::getHp() {
    return hp;
}

// Returns AC stat
int Character::getAc() {
    return ac;
}

int Character::getInitiative() {
    return initiative;
}

char Character::getEnemyFlag() {
    return isEnemy;
}

char Character::getPartyFlag() {
    return isPartyMem;
}
//**********************************************************
// End of Character Class definition                       *
//**********************************************************

// Main menu loop
int main() {
    srand(static_cast<unsigned int>(time(0)));  // Random seed
    const int MAX_CHARACTERS = 10;
    Character characters[MAX_CHARACTERS];
    std::vector<Character> party(0);
    std::vector<Character> enemies(0);
    int numCharacters = 0;  // Keep track of how many characters have been added
    int numPartyMem = 0;    // Keep track of how many party members have been added
    int numEnemies = 0;     // Keep track of how many enemies have been added

    int choice;
    while (true) {
        std::cout << "\n--- D&D DM Tracker ---\n";
        std::cout << "1. Add Character\n2. View Party\n3. Edit HP\n4. Roll Dice\n5. Exit\n";
        choice = integerEntry("menu option");

        if (choice == 1) {
            // with the additional character arrays I needed to add some extra steps
            // create a variable to hold the current number of characters.
            int currentCharacters = numCharacters;
            // This looks odd, but now that numCharacters is local, we need a way of incrementing it.
            numCharacters = addCharacter(characters, MAX_CHARACTERS, numCharacters);
            if (currentCharacters != numCharacters) {
                // If an additional character was added, we need to place it in the appropriate
                // array(s). I tried to do this in a function, but I couldn't quite figure it out.
                if (characters[numCharacters-1].getPartyFlag() == 'Y') {
                    party.push_back(characters[numCharacters-1]);
                    numPartyMem++;
                    std::cout << characters[numCharacters -1].getName() << " has been added to the party!\n";
                }
                
                if (characters[numCharacters-1].getEnemyFlag() == 'Y') {
                    enemies.push_back(characters[numCharacters-1]);
                    numEnemies++;
                    std::cout << characters[numCharacters -1].getName() << " has been added to the enemy list!\n";
                }
                
            }
        } else if (choice == 2) {
            viewParty(party, numPartyMem);
        } else if (choice == 3) {
            editHP(characters, numCharacters);
        } else if (choice == 4) {
            rollDiceMenu();
        } else if (choice == 5) {
            return 0;
        } else {
            std::cout << "Invalid option.\n";
        }
    }

    return 0;
}
// Function to add a new character
int addCharacter(Character characters[], int const MAX_CHARACTERS, int numCharacters) {
    if (numCharacters < MAX_CHARACTERS) {
        Character c;
        c.createCharacter();

        characters[numCharacters++] = c;
        std::cout << c.getName() << " created!\n";
    } else {
        std::cout << "Character limit reached.\n";
    }
    return numCharacters;
}

// Function to view party characters
void viewParty(std::vector<Character> party, int numPartyMem) {
    std::cout << "\n--- Party Status ---\n";
    for (int i = 0; i < numPartyMem; i++) {
        // Display character stats by calling the displayStats()
        // function for each character in the array.
        party[i].displayStats();
    }
    std::cout << "---------------------\n";
}

// Function to edit HP of a character
void editHP(Character characters[], int numCharacters) {
    std::string name;
    int change;
    std::cout << "Enter character name to modify HP: ";
    std::cin >> name;
    for (int i = 0; i < numCharacters; i++) {  // Use index-based loop
        if (characters[i].getName() == name) {
            std::cout << "Current HP: " << characters[i].getHp() << ".";
            change = integerEntry("change amount (e.g., -5 or 10)");
            characters[i].changeHp(change);
            std::cout << characters[i].getName() << " now has " << characters[i].getHp() << " HP.\n";
            return;
        }
    }
    std::cout << "Character not found.\n";
}

// Roll dice function
int rollDice(int numDice, int diceSides, int modifier = 0) {
    int total = 0;
    for (int i = 0; i < numDice; ++i) {
        total += rand() % diceSides + 1;
    }
    return total + modifier;
}

// Function to handle dice rolls
void rollDiceMenu() {
    int numDice, diceSides, mod;
    std::cout << "Enter number of dice: ";
    std::cin >> numDice;
    std::cout << "Enter dice sides (e.g., 20 for d20): ";
    std::cin >> diceSides;
    std::cout << "Enter modifier (can be 0): ";
    std::cin >> mod;

    int result = rollDice(numDice, diceSides, mod);
    std::cout << "You rolled: " << result << "\n";
}
// This function validates input for integers equal to or greater than 0
int positiveIntegerEntry(std::string item)
{
    // create variable to hold input
    int entry;
    // set flag for validation loop
    bool valid = false;
    // enter validation loop
    while(!valid) {
        // prompt user for input
        std::cout << "enter a positive integer for "<< item << ": ";
        std::cin >> entry;
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(20, '\n');
        }
        else if(entry > 0 || entry == 0) {
            std::cout << item << " set to " << entry << ".\n";
            valid = true;
        }
    }
    return entry;
}
// This function validates input for simple yes or no questions
char yesOrNoEntry(std::string question)
{
    // create variable to store the response
    char response;
    // create flag for validation loop
    bool valid = false;
    // display the question
    std::cout << question;
    // enter validation loop
    while (!valid) {
        // prompt user
        std::cout << "Enter Y for yes or N for no: ";
        std::cin >> response;
        if (response == 'y' || response == 'Y') {
            // standardize yes response
            response = 'Y';
            valid = true;
        }
        else if (response == 'n' || response == 'N') {
            // standardize no response
            response = 'N';
            valid = true;
        }
    }
    return response;
}
// This function validates input for any integer value
int integerEntry(std::string item) {
    // create a variable to store input
    int entry;
    // create a flag for the validation loop
    bool valid = false;
    // enter validation loop
    while (!valid) {
        // prompt the user
        std::cout << "Enter an integer for the " << item << ": ";
        std::cin >> entry;
        if (std::cin.fail()) { // if the input causes cin.fail() to return true
            std::cin.clear();
            std::cin.ignore(20,'\n');
        }
        else {
            valid = true;
        }
    }
    return entry;
}

