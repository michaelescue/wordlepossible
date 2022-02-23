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

// User defined types
typedef std::set<char> charSet;
typedef std::set<std::string> stringSet;
typedef std::set<int> intSet;

// Class definition
class WordleWord{

    private:

        // Wordle word size
        static uint32_t size;

        // Guesses
        static uint32_t guesses;

        // Letter set spaces
        static charSet requiredLetters;
        static charSet possibleLetter;
        static stringSet possibleWords;
        static charSet final;
        static intSet yellowPos;

        // Data paths
        std::string out, possibleCharSet, words;

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
        out = "wordle_possible_words.txt";
        possibleCharSet = "possible_set.txt";
        words = "english_word_set.txt";

        // Fill the final set with unkown chars.
        for(int i = 0; i < wordSize; i++){
            final.insert('?');
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
            std::cout << 'Exception with possible character set: ' << e.what() << '\n';
        }

        // Read in possible word set.
        try{
            // Open possible word file.
            istream.open(WordleWord::words);

            // Initialize counter for number of words.
            std::uint64_t count = 0;
            while( std::getline(istream,temp) ){
                if(temp.length() == 6){
                    count++;
                    WordleWord::possibleWords.insert(temp);
                }
            }

            // Close file
            istream.close();

            // Print total words added to set. 
            std::cout << count << ' words added to possibilities.';
        }

        // Catch file related exceptions
        catch (std::exception& e){
            std::cout << 'Exception with possible word set: ' << e.what() << 'n';
        }


        // Open output file for writing possibilities
        try{
            // Open output file
            ostream.open(WordleWord::out);

            // Write header
            ostream << 'Possible Words\n';

            // Write each word from possible word set.
            for (strItr = WordleWord::possibleWords.begin(); strItr != WordleWord::possibleWords.end(); strItr++) {
                std::cout << *charItr << " ";
            }
            std::cout << '\n';

            // Close output file.
            ostream.close();
        }

        // Catch file related exceptions
        catch (std::exception& e){
            std::cout << 'Exception with possible word set: ' << e.what() << 'n';
        }

}; // WordleWord Constructor

#endif // Header guard.