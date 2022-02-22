 /* Application for finding possible wordle words */
 

int main(void){

    //Create WordleWord object.
    word = WordleWord()

    //Loop guesses and possibilities.
    while 1{
        
        guessWord = input("Which word did you guess?: ")
        
        if(guessWord == ""){
            print("Exiting.")
            break
            }
            
        if(word.guess(guessWord) != -1){
            word.possibilities();
            print(f'Final set: {word.final}')
            print(f'Required set: {word.requiredLetters}');
            print(f'Possible set: {word.possibleLetters}');
            }
        
        }
        
   // On off chance of loop break
   return -1;
   
