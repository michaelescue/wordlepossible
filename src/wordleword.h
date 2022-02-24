/**
 * @file wordleword.h
 * @author Michael Escue (michael@escue.dev)
 * @brief Class file header describing WordleWord class.
 * @version 0.1
 * @date 2022-02-22
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#define DEBUG

#ifndef WORDLEWORD  // Header guard.
#define WORDLEWORD

// Libraries
#include <cstdint>
#include <set>
#include <fstream>
#include <iostream>
#include <string>
#include <exception>
#include <regex>    

using namespace std;

// User defined types
typedef std::set<char> charSet;
typedef std::set<string> stringSet;
typedef std::set<int> intSet;

// Class definition
class WordleWord{

    private:

        // Wordle word size
        uint32_t size;

        // Guesses
        uint32_t guesses;

        // Guess word variable
        string guessWord;

        // Letter set spaces
        charSet requiredLetters;
        charSet possibleLetters;
        charSet guessedSet;
        stringSet possibleWords;
        char* final;
        intSet yellowPos;

        // Data paths
        string out = "../wordle_possible_words.txt";
        string possibleCharSet = "../possible_set.txt";
        string words = "../english_word_set.txt";

        // Helper functions
        std::set<int> getOpenPositions();
        void printSet(stringSet x);
        void printSet(charSet x);
        void printSet(intSet x);
        void printArray(char* x);
        string lowerString(string x);

    public:

        /**
         * @brief Construct a new Wordle Word object.
         * 
         * @param wordSize 
         */
        WordleWord(uint32_t wordSize, uint32_t guessSize);

        void possibilities(void);

        int guess();

        void printsets();



}; // Class WordleWord

// Initialization
WordleWord::WordleWord(uint32_t wordSize, uint32_t guessSize){
        
    // Set Word parameters
    size = wordSize;
    guesses = guessSize;

    // Dynamic allocation of final char array.
    final = new char[size];

    // Fill the final set with unkown chars.
    for(int i = 0; i < wordSize; i++){
        final[i] = '?';
    }

    // Streaming objects, string, and iterator.
    std::ifstream istream;
    std::ofstream ostream;
    std::string temp;
    charSet::iterator charItr;
    stringSet::iterator strItr;

    // Indexing possible characters.
    try{
        // Open possible character set file.
        istream.open(WordleWord::possibleCharSet);
        
        // Read possible character set from file.
        while( std::getline(istream,temp) ){

            // Write possible characters to set.
            for (int i = 0; i < (sizeof(temp)-1); i++){
                WordleWord::possibleLetters.insert(temp[i]);
            }
        }

        #ifdef DEBUG

        // Print set of possible characters.
        std::cout << "\nAdding to possible set: ";
        for (charItr = WordleWord::possibleLetters.begin(); charItr != WordleWord::possibleLetters.end(); charItr++) {
            std::cout << *charItr << " ";
        }
        std::cout << '\n';

        #endif

        // Close file steam
        istream.close();

    }
    
    // Catch file related exceptions
    catch (std::exception& e){
        std::cout << "Exception with possible character set: " << e.what() << '\n';
    }

    // Read in possible word set.
    try{
        // Open possible word file.
        istream.open(WordleWord::words);

        // Initialize counter for number of words.
        std::uint64_t count = 0;
        while( std::getline(istream,temp) ){
            if(temp.length() == 5){
                count++;
                temp = lowerString(temp);
                WordleWord::possibleWords.insert(temp);
            }
        }

        // Close file
        istream.close();

        // Print total words added to set. 
        std::cout << count << " words added to possibilities." << std::endl;
    }

    // Catch file related exceptions
    catch (std::exception& e){
        std::cout << "Exception with possible word set: " << e.what() << '\n';
    }


    // Open output file for writing possibilities
    try{
        // Open output file
        ostream.open(WordleWord::out);

        // Write header
        ostream << "Possible Words";

        // Write each word from possible word set.
        for (strItr = WordleWord::possibleWords.begin(); strItr != WordleWord::possibleWords.end(); strItr++) {
            std::cout << *strItr << " ";
        }

        // Close output file.
        ostream.close();
    }

    // Catch file related exceptions
    catch (std::exception& e){
        std::cout << "Exception with possible word set: " << e.what() << 'n';
    }

    // Newline to prevent 1st letter from being eaten post init.
    std::cout << 'n';

}; // WordleWord Constructor

std::set<int> WordleWord::getOpenPositions(){
        std::set<int> markedPositions;
        std::uint32_t i = 0;
        charSet::iterator charItr;

        for (uint32_t i = 0; i < WordleWord::size; i++) {
            if(WordleWord::final[i] ==  '?')
                markedPositions.insert(i+1);
        }

        return markedPositions;
}

void WordleWord::printSet(intSet x){
    intSet::iterator y;
    for(y = x.begin(); y != x.end(); y++){
        cout << " " << *y;
    }
    cout << '\n';
}

void WordleWord::printSet(charSet x){
    charSet::iterator y;
    for(y = x.begin(); y != x.end(); y++){
        cout << " " << *y;
    }
    cout << '\n';
}

void WordleWord::printSet(stringSet x){
    stringSet::iterator y;
    for(y = x.begin(); y != x.end(); y++){
        cout << " " << *y;
    }
    cout << '\n';
}

void WordleWord::printArray(char *x){
    for(uint32_t i = 0; i < sizeof(x) ; i++){
        cout << " " << x[i];
    }
    cout << '\n';
}

void WordleWord::printsets(){

    cout << "Required Letters: ";
    printSet(requiredLetters);
    
    cout << "Possible Letters: ";
    printSet(possibleLetters);

    cout << "Guessed Letters: ";
    printSet(guessedSet);

    cout << "Final Letters: ";
    printArray(final);

    //cout << "Possible Words:\n";
    //printSet(possibleWords);

    cout << "Past Yellow letter positions: ";
    printSet(yellowPos);

}

string WordleWord::lowerString(string x){

    for(uint32_t i = 0; i < sizeof(x); i++){
        x[i] = tolower(x[i]);
    }

    return x;
}

 // Guess a word
int WordleWord::guess(){

        // Input filtering
        while(1){

        // Prompt for guess
        std::cout << "Which word did you guess?: ";

        // Read guess
        std::getline(std::cin, guessWord);
        
        // Exit if blank.
        if(guessWord.compare("") == 0){
            return -1;
            }
        
        // Check size is valid.
        uint32_t length = guessWord.length();
        if((length < 5) || (length > 5)){
            std::cout << "Word must be 5 letters long. Try again: ";
            continue;
        }
        
        else{
            // Remove guessed word from possibilites.
            if(WordleWord::possibleWords.erase(guessWord)){
                break;
            }
            
            // Guessed word not in possibilities.
            else{
                cout << "Word not in possibilities. Try again.";
                continue;
            }

            }
        }

    // Identifying green letters
    string correctLetterPos;
    intSet openPos;
    intSet::iterator itr;

    while(1){

        
        try{
            // Check for currently open positions in the final set
            openPos = WordleWord::getOpenPositions();

            if(openPos.empty()){
                cout << "Congratulations on finishing your " << WordleWord::size << " letter Wordle!\n";
                return -1;
                // Add word to correctly guessed words set in future.
                }

            // Prompt for Green letter positions.
            cout << "Green Letter Position {";
            for(itr = openPos.begin(); itr != openPos.end(); itr++){
                cout << " " << *itr;
            }
            
            cout << "} Enter to continue: ";

            // Scan input for Green letter positions.
            getline(cin, correctLetterPos);
            if(correctLetterPos == "")
                break;
        }

        catch (std::exception& e){
            cout << "Incorrect input. Try again.";
        }

        try{
            // Place the letter of the guessWord in the selected position into the final set.
            uint32_t letterPosInt = stoi(correctLetterPos, nullptr, 10);
            if( (letterPosInt >= 0) && (letterPosInt < WordleWord::size + 1) ){
                uint32_t pos = letterPosInt - 1;
                WordleWord::final[pos] = guessWord[pos];
            }

            else{
                cout << "The number " << letterPosInt << "is out of range. Try again.";
            }
        }

        catch (std::exception& e){
            cout << "Incorrect value type. Try again.";
        }
    }

    // Copy identified green letters into required letters set.
    int count = 0;
    for(uint32_t i = 0; i < WordleWord::size; i++){
        if(WordleWord::final[i] != '?')
            WordleWord::requiredLetters.insert(final[i]);
    }

    // Identifying yellow letters
    bool jump = 0;
    string requiredLetter;
    uint32_t requiredLetterPos;
    set<int> difference;
    charSet::iterator charItr;

    while(1){

        try{
            difference = openPos;
            // Get difference in YellowPos and openPos sets.
            for(itr = yellowPos.begin(); itr != yellowPos.end(); itr++){
                difference.erase(*itr);
            }

            // Prompt for Yellow letter positions.
            cout << "Yellow Letter Position {";

            // Print available positions.
            for(itr = difference.begin(); itr != difference.end(); itr++){
                cout << " " << *itr;
            }

            // Cap end of message.
            cout << "}? Enter to continue:";

            // Get input.
            getline(cin, requiredLetter);
            if(requiredLetter == "")
                break;
            requiredLetterPos = stoi(requiredLetter, nullptr, 10);
        }

        catch (std::exception& e){
            cout << "Incorrect input. Try again.";
        }

        // Check if required letter is already a green letter.
        for(uint32_t i = 0; i < WordleWord::size; i++){

            if(final[i] == (requiredLetterPos)){
                cout << requiredLetter << "is a final letter and cannot be a Yellow letter.";
            // Add the letter to required letters if not.
            }

            else{
                try{

                    // Place the letter of the guessWord in the selected position into the final set.
                    if (((requiredLetterPos) >= 0) && ((requiredLetterPos) < size + 1)){
                        uint32_t pos = (requiredLetterPos) - 1;
                        requiredLetters.insert(guessWord[pos]);
                        yellowPos.insert(pos + 1);
                    }

                    else{
                        cout << "The number " << requiredLetter << " is out of range. Try again.";
                    }
                }

                catch (std::exception& e){
                        cout << "Incorrect value type. Try again.";
                }
            }
        }
    }

        // Remove assumed gray letters
        for (int i = 0; i < size; i++)
            guessedSet.insert(guessWord[i]);

        for(charItr = requiredLetters.begin(); charItr != requiredLetters.end(); charItr++)
            guessedSet.erase(*charItr);

        for(charItr = guessedSet.begin(); charItr != guessedSet.end(); charItr++)
            possibleLetters.erase(*charItr);
    
        return 0;
}

// Produce possible letter combinations.
void WordleWord::possibilities(void){

        // Count possibilities
        uint32_t count = 0;

        // Open file for writing.
        std::ofstream ostream;
        ostream.open(out);

        // Find list of possible words to remove through iteration
        stringSet difference;
        stringSet::iterator stritr;
        charSet::iterator charitr;
        if (requiredLetters.empty() != 0){
            for(stritr = possibleWords.begin(); stritr != possibleWords.end(); stritr++){
                for(charitr = requiredLetters.begin(); charitr != requiredLetters.end(); charitr++){
                    string temp = *stritr;
                    if(temp.find(*charitr) == string::npos)
                        difference.insert(*stritr);
                }
            }
        }

        // Remove words after iteration
        for(stritr = difference.begin(); stritr != difference.end(); stritr++)
            possibleWords.erase(*stritr);

        // Construct RE pattern with known letter positions and possible chars.
        string pattern = "";
        difference.clear();
        for(uint32_t i = 0; i < size; i++){
            if(final[i] == '?'){
                // First bracket
                pattern = pattern + '[';

                // Iterate through possible letters.
                // Only chars which do NOT match guess word char at same position and
                // and is a yellow letter position.
                for(charitr = possibleLetters.begin(); charitr != possibleLetters.end(); charitr++)
                    if( (*charitr != guessWord[i]) && (yellowPos.count(i + 1) != 0))
                        pattern = pattern + *charitr;

                // Second bracket
                pattern = pattern + ']';
            }

            else{
                pattern = pattern + final[i];
            }
        }

        #ifdef DEBUG
        
        cout << pattern;

        #endif

        // Create regex instance.
        regex re (pattern);

        // Search compiled RE with possible word list
        for(stritr = possibleWords.begin(); stritr != possibleWords.end(); stritr++){
            if (regex_match(*stritr, re)){
                ostream << *stritr << '\n';
                count++;
            }
        }

        // Close output file
        ostream.close();

        cout << count << " Possible Wordle Words." << '\n';

        // If small enough count print to terminal
        //if(count < 20)
           // printSet(possibleWords);
        //else
            cout << " See file {self.out} for possible words. " << count << "possible.\n";
}

#endif // Header guard.