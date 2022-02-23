/**
 * @file main.cpp
 * @author Michael Escue (michael@escue.dev)
 * @brief Application for generating possible wordle words based on guesses.
 * @version 0.1
 * @date 2022-02-22
 * 
 * @copyright Copyright (c) 2022
 * 
 */

// Libraries and Header files
#include <stdlib.h>
#include "wordleword.h"


int main(void){

    // Create WordleWord object.
    WordleWord word = WordleWord(5,6);

    // Create guessWord instance
    std::string guessWord;

    //Loop guesses and possibilities.
    while(1){
        
        if(word.guess() != -1){
            word.possibilities();
            word.printsets();
        }
        
        else{
            std::cout << "Exiting."<< std::endl;
        }
        
   // On off chance of loop break
   return -1; 
};
