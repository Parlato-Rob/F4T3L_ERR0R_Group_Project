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
int addCharacter(Character arr[10],const int, int);
void viewCharMenu(Character arr[10], int);
void viewCharacters(Character arr[10], int, std::string);
void viewParty(Character arr[10], int, std::string);
void viewEnemies(Character arr[10], int, std::string);
void editCharMenu(Character arr[10], int);
void editHp(Character arr[10], int);
int rollDice(int, int, int);
void rollDiceMenu();
void initiativeRoll(Character arr[10], int, std::vector<Character> &);
void setTurnOrder(std::vector<Character> &);
void displayTurnOrder(std::vector<Character>);
void swapInt(int &, int &);
void swapChar(Character &, Character &);
//************************************************************************* 
//* Character class definition block                                      *
//************************************************************************* 
class Character {
    private:
        std::string name;
        int hp;
        int ac;
        int initiative;
        int initModifier;
        char isEnemy;               // Enemy and Party members are seperate flags
        char isPartyMem;            // because of possible ambiguous character cases.
    public:
        void createCharacter();     // Sets the attributes for a Character type object, should be called on creation
        void rollInit();            // Sets the initiative value to a number returned by the rollDice() function    
        void displayStats();
        void modifyHp(int);         // This has been structured to work with the existing editHp function
        void applyDamage(int);      // Accepts an integer as a damage argument, applied damage = Damage - AC.
        // The following functions are for editing characters
        void editCharacter();
        void setName();
        void setHp();
        void setAc();
        void setInitMod();
        void setFlags();            // Allows the Characters party/enemy status to be edited
        // The following get functions provide access to the private attributes
        std::string getName();
        int getHp();
        int getAc();
        int getInitiative();
        char getEnemyFlag();
        char getPartyFlag();
};

// This will be ran when creating a new character
void Character::createCharacter() {
    std::cout << "Enter character name: ";
    std::cin >> name;
    hp = positiveIntegerEntry("Character Health Points(HP)");
    ac = positiveIntegerEntry("Character Armor Class(AC)");
    initModifier = integerEntry("Initiative Modifier");
    // Preroll initiative value for the first turn
    rollInit();
    // Determine the characters Enemy and Party status
    setFlags();
}
// This function Rolls a value for the initiative attribute
void Character::rollInit() {
    // This turned out much simpler than expected,
    // the rollDice function just needs the relavent values.
    initiative = rollDice(1, 20, initModifier);
}
// This function displays the Character's attributes
void Character::displayStats() {
std::cout << "------------------------------------------------------------------\n";
    std::cout << "Name: " << std::left << std::setw(15) << name
                  << " | HP: " << hp
                  << " | AC: " << ac
                  << " | Party Member:" << isPartyMem
                  << " | Enemy:" << isEnemy << "\n"
                  << "Initiative Modifier:" << std::setw(10) << initModifier
                  << " | Initiative Roll: " << initiative << "\n"
                  << "------------------------------------------------------------------\n";
}
// This function adds a postive value to a character's hp
void Character::modifyHp(int change) {
    hp += change;
    // check for negative hp values
    if (hp < 0) {
        hp = 0;
    }
}
// This function subtracts a positive value from character hp
void Character::applyDamage(int damage) {
    // Check for negative values
    if (damage < 0) {
        // set applied damage to 0
        damage = 0;
    }
    hp -= damage;
    // This just prevents negative hp numbers.
    // We can remove the if statement if the overkill
    // amount is desired.
    if (hp < 0)
        hp = 0;
}

//**********************************************************
// Functions for editing Characters                        *
//**********************************************************

// This function allows the user to edit a character
void Character::editCharacter() {
    // Create a boolean variable for loop control
    bool inMenu = true;

    //Create a variable for the user selection
    int choice;

    // enter loop
    while (inMenu) {
        // display current character status
        displayStats();
        // display menu options
        std::cout << "\n--- Edit " << name << " Menu ---\n";
        std::cout << "1. Change Name\n2. Change Hp\n3. Change Ac\n4. Change Initiative Modifier\n"
                  << "5. Change Enemy or Party status\n6. Back\n\n";
        // Get the users selection
        choice = integerEntry("menu choice");

        // If else statement menu block
        if (choice == 1) {
            setName();
        }
        else if (choice == 2) {
            setHp();
        }
        else if (choice == 3) {
            setAc();
        }
        else if (choice == 4) {
            setInitMod();
        }
        else if (choice == 5) {
            setFlags();
        }
        else if (choice == 6) {
            std::cout << "Returning to previous menu...\n";
            inMenu = false;
        }
        else {
            std::cout << "Invalid option\n";
        }
    }
}
// This function will allow you to change the character name
void Character::setName() {
    std::cout << "Enter a new name for " << name << ": ";
    std::cin >> name;
    std::cout << "This Character is now " << name << ".\n";
}
// This function sets the HP value
void Character::setHp() {
    // Calls the positiveIntegerEntry function to get an integer from the user
    hp = positiveIntegerEntry("Character Health Points");
    std::cout << name << "'s HP set to " << hp << ".\n";
}
// This function simply modifies ac
void Character::setAc() {
    // Calls the integerEntry function to get an integer from the user.
    ac = integerEntry("Character Armor Class");
    std::cout << name << "'s Armor Class set to " << ac << ".\n";
}
// This function sets the Initiative mod
void Character::setInitMod() {
    initModifier = integerEntry("Initiative Modifier");
    std::cout << name << "'s Initiative Modifier set to " << initModifier << ".\n";
}
// Sets the Party and Enemy flag status
void Character::setFlags() {
    // Determine if the character is an enemy
    isEnemy = yesOrNoEntry("Is this character an enemy?\n");
    if (isEnemy == 'Y') {   //yesOrNoEntry returns 'Y' if the user entered 'y'
        std::cout << name << " has been set as an enemy.\n";
        isPartyMem = 'N';
    }
    // Determine if the character is a party member
    if (isEnemy =='N') {
        isPartyMem = yesOrNoEntry("Is this character a party member?\n");
        if (isPartyMem == 'Y') {   //yesOrNoEntry returns 'N' if the user entered 'n'
            std::cout << name << " has been set as a party member.\n";
        }
    }
}

//**********************************************************
// Functions for returning character attributes            *
//**********************************************************

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
// Main Function                                           *
//**********************************************************
int main() {
    srand(static_cast<unsigned int>(time(0)));  // Random seed
    const int MAX_CHARACTERS = 10;
    Character characters[MAX_CHARACTERS];
    std::vector<Character> turnOrder(0);    // Vector used to hold the turn order for all characters
    int numCharacters = 0;  // Keep track of how many characters have been added
    int numPartyMem = 0;    // Keep track of how many party members have been added
    int numEnemies = 0;     // Keep track of how many enemies have been added

    // Create a bool variable to control main loop
    bool running = true;

    int choice;
    while (running) {
        std::cout << "\n--- D&D DM Tracker ---\n";
        std::cout << "1. Add Character\n2. View Character Menu\n3. Edit Character Menu\n4. Edit Health Points\n5. Roll Dice\n"
                  << "6. Initiative Roll\n7. Exit\n\n";
        choice = integerEntry("menu option");

        if (choice == 1) {
            // This looks odd, but now that numCharacters is local, we need a way of incrementing it.
            numCharacters = addCharacter(characters, MAX_CHARACTERS, numCharacters);
        } else if (choice == 2) {
            // Pass the Character array and vectors to the Character Menu function
            viewCharMenu(characters, numCharacters);
        } else if (choice == 3) {
            // call the editCharacterMenu function to bring up the character menu
            editCharMenu(characters, numCharacters);
        } else if (choice == 4) {
            editHp(characters, numCharacters);
        } else if (choice == 5) {
            rollDiceMenu();
        } else if (choice == 6) {
            initiativeRoll(characters, numCharacters, turnOrder);
            setTurnOrder(turnOrder);
            displayTurnOrder(turnOrder);
        } else if (choice == 7) {
            // establish exit conditions for loop
            running = false;
        } else {
            std::cout << "Invalid option.\n";
        }
    }
    return 0;
}
//**********************************************************
// Input Validation Functions                              *
//**********************************************************

// This function validates input for integers equal to or greater than 0
int positiveIntegerEntry(std::string item) {
    // create variable to hold input
    int entry;
    // set flag for validation loop
    bool valid = false;
    // enter validation loop
    while(!valid) {
        // prompt user for input
        std::cout << "Enter a positive integer for "<< item << ": ";
        std::cin >> entry;
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(20, '\n');
            std::cout << "Invalid entry.\n\n";
        }
        else if(entry >= 0) {
            valid = true;
        }
        else {
            std::cout << "Invalid entry.\n\n";
        }
    }
    return entry;
}

// This function validates input for simple yes or no questions
char yesOrNoEntry(std::string question) {
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
        else {
            std::cout << "Invalid entry.\n\n";
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
            std::cout << "Invalid entry.\n\n";
        }
        else {
            valid = true;
        }
    }
    return entry;
}
//**********************************************************
// Function Definitions                                    *
//**********************************************************
// Function to add a new character
int addCharacter(Character characters[10], int const MAX_CHARACTERS, int numCharacters) {
    if (numCharacters < MAX_CHARACTERS) {
        // Create a new instance of the Character class
        Character newCharacter;
        // Call the newCharacter's createCharacter function
        newCharacter.createCharacter();
        // Add the new character to the array and increment numCharacters
        characters[numCharacters++] = newCharacter;
        std::cout << newCharacter.getName() << " created!\n";
    } else {
        std::cout << "Character limit reached.\n";
    }
    return numCharacters;
}

// This function displays a submenu that asks which Character group you would like
// to view.
void viewCharMenu(Character characters[10], int numCharacters)
{
    // Create a boolean variable for loop control
    bool inMenu = true;

    //Create a variable for the user selection
    int choice;

    // enter loop
    while (inMenu) {
        // display menu options
        std::cout << "\n--- View Character Menu ---\n";
        std::cout << "1. View all Characters\n2. View Party Members\n3. View Enemies\n4. Back\n\n";
        // Get the users selection
        choice = integerEntry("menu choice");

        // If else statement menu block
        if (choice == 1) {
            viewCharacters(characters, numCharacters, "--- Character Status ---");
        }
        else if (choice == 2) {
            viewParty(characters, numCharacters, "--- Party Status ---");
        }
        else if (choice == 3) {
            viewEnemies(characters, numCharacters, "--- Enemy Status ---");
        }
        else if (choice == 4) {
            std::cout << "Returning to previous menu...\n";
            inMenu = false;
        }
        else {
            std::cout << "Invalid option\n";
        }
    }
}
// Function to view the characters in an array
void viewCharacters(Character charArray[10], int numCharacters, std::string header) {
    // display header received during function call
    std::cout << header << std::endl;
    for (int index = 0; index < numCharacters; index++) {
        // Display character stats by calling the displayStats()
        // function for each character in the array.
        charArray[index].displayStats();
    }
}

void viewParty(Character charArray[10], int numCharacters, std::string header) {
    // display header received during function call
    std::cout << header << std::endl;
    for (int index = 0; index < numCharacters; index++) {
        // Display character stats by calling the displayStats()
        // function for each character in the array.
        if (charArray[index].getPartyFlag() == 'Y') {
            // If the get party flag function returns 'Y' for yes
            charArray[index].displayStats();
        }
    }
}
// This function displays the stats of all characters set as enemies
void viewEnemies(Character charArray[10], int numCharacters, std::string header) {
    // display header received during function call
    std::cout << header << std::endl;
    for (int index = 0; index < numCharacters; index++) {
        // Display character stats by calling the displayStats()
        // function for each character in the array.
        if (charArray[index].getEnemyFlag() == 'Y') {
            // If the get enemy flag function returns 'Y' for yes
            charArray[index].displayStats();
        }
    }
}
// This function brings up the menu to edit an existing character
void editCharMenu(Character characters[10], int numCharacters) {
    // create a boolean variable for menu status
    bool inMenu = true;
    // enter loop
    while (inMenu) {
        // Displays a numbered list of all the characters in the characters array
        for (int index = 0, number = 1; index < numCharacters; index++, number++) {
            std::cout << number << ". " << characters[index].getName() << "\n";
        }
        // Display the exit option
        std::cout << (numCharacters + 1) << ". Back\n\n";

        // declare a variable for user input, and get a user response
        int menuChoice = positiveIntegerEntry("character you want to edit");

        // Create and set a variable for the desired character's index position
        int charChoice = menuChoice -= 1;

        // if statements for calling the editCharacter function
        if (menuChoice >= 0 && menuChoice < numCharacters) {
            characters[charChoice].editCharacter();
        }
        else if (menuChoice == numCharacters) {
            // This will be the exit option
            inMenu = false;
            std::cout << "Returning to previous menu...\n\n";
        }
        else {
            std::cout << "invalid menu option";
        }
    }
}

// Function to edit HP of a character
void editHp(Character characters[10], int numCharacters) {
    // create a boolean variable for menu status
    bool inMenu = true;
    // enter loop
    while (inMenu) {
        // Displays a numbered list of all the characters in the characters array
        for (int index = 0, number = 1; index < numCharacters; index++, number++) {
            std::cout << number << ". " << characters[index].getName() << "\n";
            // display the current hp for the user
            std::cout << "Current Health Points: " << characters[index].getHp() << std::endl;
        }
        // Display the exit option
        std::cout << (numCharacters + 1) << ". Back\n\n";
        // declare a variable for user input, and get a user response
        int menuChoice = positiveIntegerEntry("the character you want to adjust");

        // Create and set a variable for the desired character's index position
        int charChoice = menuChoice -= 1;

        // Confirmation message
        std::cout << "Adjusting " << characters[charChoice].getName() << "'s health points.\n";

        // If statements for calling the editCharacter function
        if (menuChoice >= 0 && menuChoice < numCharacters) {
            // declare and get the hp change amount from the user
            int change = integerEntry("change amount (e.g., -5 or 10)\nPositive values will heal, negative values will damage");
            characters[charChoice].modifyHp(change);
            std::cout << characters[charChoice].getName() << " now has " << characters[charChoice].getHp()
                      << " health points.\n\n";
        }
        else if (menuChoice == numCharacters) {
            // This will be the exit option
            inMenu = false;
            std::cout << "Returning to previous menu...\n\n";
        }
        else {
            std::cout << "invalid character selection.\n\n";
        }
    }

}

// Roll dice function
int rollDice(int numDice, int diceSides, int modifier) {
    int total = 0;
    for (int index = 0; index < numDice; ++index) {
        total += rand() % diceSides + 1;
    }
    return total + modifier;
}

// Function to handle dice rolls
void rollDiceMenu() {
    // I integrated the input validation funcitions to prevent input Errors,
    // but it might be good to have additional range parameters...
    int numDice, diceSides, mod;
    // Get a positive integer for the number of dice from the input validation function
    numDice = positiveIntegerEntry("the number of dice");
    // Get a positive integer for the number of sides
    diceSides = positiveIntegerEntry("the dice sides (e.g., 20 for d20)");
    // Get an integer for the role modifier
    mod = integerEntry("role modifier (can be 0)");
    int result = rollDice(numDice, diceSides, mod); // calls rollDice function
    std::cout << "You rolled: " << result << "\n";
}

// Function that calls the public function rollinit() for all characters in Characters
void initiativeRoll(Character characters[10], int numCharacters, std::vector<Character>&turnOrder) {
    // Prepare the turnOrder vector for a fresh initiative roll
    turnOrder.clear();
    for (int index = 0; index < numCharacters; index++) {
        // Roll initiative
        characters[index].rollInit();
        // Add the character to the unordered turnOrder vector
        turnOrder.push_back(characters[index]);
    }
}
// Sorts the Characters in the turn Order in Descending order,
// based off the Initiative attribute
void setTurnOrder(std::vector<Character>&turnOrder) {
    // Create a variable for the vector size
    int size = 0;

    // Create a parralel vector
    std::vector<int> initiative(0);
    
    // run a for loop to get the vector size
    for (Character character : turnOrder) {
        initiative.push_back(character.getInitiative());
        size++;
    }

    // Create a variable to track the index position
    int maxElement;
    int index;
    
    for (maxElement = initiative.size() - 1; maxElement > 0; maxElement--) {
        for (index = 0; index < maxElement; index++) {
            if (initiative[index] < initiative[index + 1]) {
                // swap position in reference vector
                swapInt(initiative[index], initiative[index + 1]);

                // swap position of the corresponding character
                swapChar(turnOrder[index], turnOrder[index + 1]);
            }
        }
    }
}

// This function simply swaps to reference variables
void swapInt(int &a, int &b) {
    int temp = a;
    a = b;
    b = temp;
}

// This function swaps two iterations of the Character class
void swapChar(Character &charA, Character &charB) {
    Character tempChar = charA;
    charA = charB;
    charB = tempChar;
}
// Displays the names of the characters stored in the turnOrder vector
void displayTurnOrder(std::vector<Character> turnOrder) {
    std::cout << "\n****************\n"
                << "***Turn Order***\n"
                << "****************\n\n";
    int turn = 1;   // Create a variable to track the turn
    for (Character character : turnOrder) {
        std::cout << turn << ". " << character.getName() << "|  Roll: "<< character.getInitiative() << "\n";
        turn++;
    }
}