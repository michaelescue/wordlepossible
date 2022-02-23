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
#include <vector>

// User defined types
typedef std::set<char> charSet;
typedef std::set<std::string> stringSet;
typedef std::set<int> intSet;

// Class definition
class WordleWord{

    private:

        // Wordle word size
        uint32_t size;

        // Guesses
        uint32_t guesses;

        // Letter set spaces
        charSet requiredLetters;
        charSet possibleLetter;
        stringSet possibleWords;
        char* final;
        intSet yellowPos;

        // Data paths
        std::string out, possibleCharSet, words;

        // Helper functions
        std::set<int> getOpenPositions();


    public:

        /**
         * @brief Construct a new Wordle Word object.
         * 
         * @param wordSize 
         */
        WordleWord(uint32_t wordSize, uint32_t guessSize);

        void possibilities(void);

        int guess(std::string guessWord);

}; // Class WordleWord

// Initialization
WordleWord::WordleWord(uint32_t wordSize, uint32_t guessSize){
        
    // Set Word parameters
    size = wordSize;
    guesses = guessSize;
    out = "../wordle_possible_words.txt";
    possibleCharSet = "../possible_set.txt";
    words = "../english_word_set.txt";

    // Fill the final set with unkown chars.
    final = new char[size];
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
                WordleWord::possibleLetter.insert(temp[i]);
            }
        }

        #ifdef DEBUG

        // Print set of possible characters.
        std::cout << "\nAdding to possible set: ";
        for (charItr = WordleWord::possibleLetter.begin(); charItr != WordleWord::possibleLetter.end(); charItr++) {
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
            std::cout << *charItr << " ";
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

using namespace std;

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

 // Guess a word
int WordleWord::guess(std::string guessWord){

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
    set<int> openPos = WordleWord::getOpenPositions();
    intSet::iterator itr;

    while(1){

        
        try{
            // Check for currently open positions in the final set
            if(openPos.empty()){
                cout << "Congratulations on finishing your" << WordleWord::size << "letter Wordle!";
                return -1;
                // Add word to correctly guessed words set in future.
                }

            // Prompt for Green letter positions.
            cout << "Green Letter Positions";
            for(itr = openPos.begin(); itr != openPos.end(); itr++){
                cout << *itr;
            }

            cout << "Enter to continue: ";

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


    // Copy identified green letters into required letters set.
    int count = 0;
    for(uint32_t i = 0; i < WordleWord::size; i++){
        if(WordleWord::final[i] != '?')
            WordleWord::requiredLetters.insert(final[i]);
    }

    // Identifying yellow letters
    bool jump = 0;
    set<int> difference;

    while(1){

        try{
            //Prompt for Yellow letter positions.
            string requiredLetterPos;
            cout << "Yellow Letter Position";
            for(itr = openPos.begin(); itr != openPos.end(); itr++){
                for(itr = yellowPos.begin(); itr != yellowPos.end(); itr++){

                }
            }
            
            getline(cin, requiredLetterPos);
            if(requiredLetterPos == "")
                break;
        }

        catch (std::exception& e){
            cout << "Incorrect input. Try again.";
        }

        for(uint32_t i = 0; i < WordleWord::size; i++){
            // Check if required letter is already a green letter.
            if each == requiredLetterPos:
                print(f'{requiredLetterPos} is a final letter and cannot be a Yellow letter.')
            // Add the letter to required letters if not.
            else:
                try:
                    // Place the letter of the guessWord in the selected position into the final set.
                    if (int(requiredLetterPos) >= 0) and (int(requiredLetterPos) < self.size + 1):
                        pos = int(requiredLetterPos) - 1
                        self.requiredLetters.add(guessWord[pos])
                        self.yellowPos.add(pos + 1)
                    else:
                        print(f'The number {int(requiredLetterPos)} is out of range. Try again.')

                except ValueError:
                    print("Incorrect value type. Try again.")

    // Remove assumed gray letters
    guessSet = set()
    for guessWordLetter in guessWord:
        guessSet.add(guessWordLetter)

    for each in self.requiredLetters:
        guessSet.remove(each)

    for each in guessSet:
        self.possibleLetters.remove(each)
    }
    }

#endif // Header guard.