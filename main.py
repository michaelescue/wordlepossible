# Author: Michael Escue
# Date: 02/15/2021
# Application to help find Wordle words.

import re
import os

# Added for dependent files.
script_path = os.path.dirname(os.path.abspath(__file__))

# Define class
class WordleWord:
    
    # Wordle word size
    size = 5

    # Guesses
    guesses = 6

    # Letter set spaces
    requiredLetters = set()
    possibleLetters = set()
    possibleWords = set()
    yellowPos = set()

    # Ordered final space
    final = ['?'] * size
    # final = ['e','?','g','e','r']

    # Files
    out = f'{script_path}wordle_possible_words.txt'
    charSet = f'{script_path}/possible_set.txt'
    words = f'{script_path}/english_word_set.txt'

    # Initialization
    def __init__(self):

        try:
            # Open, read, and close possible set file.
            stream = open(self.charSet)
            text = stream.read(-1)
            stream.close()

            # Write possible characters to set.
            for each in text:
                self.possibleLetters.add(each)

            # Print possible set to terminal
            print("Possible Letters: " + self.possibleLetters.__str__())

        except OSError:
            print("Couldn't open file: " + self.charSet)

        try:
            # Open possible word file.
            stream = open(self.words)
            count = 0

            for each in stream:
                if len(each) == 6:
                    count = count + 1
                    self.possibleWords.add(each.lower())

            print(f'{count} words added to possibilities')

        except OSError:
            print("Couldn't open file: " + self.words)

        # Close file
        stream.close()

        # Open file for writing possibilities
        stream = open(self.out, 'w')
        stream.write('Possible Words\n\r')
        for each in self.possibleWords:
            stream.write(each)

        stream.close()

    def possibilities(self):

        # Count possibilities
        count = 0

        # Open file for writing.
        stream = open(self.out, 'w')

        # Find list of possible words to remove through iteration
        toRemove = set()
        if self.requiredLetters is not None:
            for each in self.possibleWords:
                for letter in self.requiredLetters:
                    if each.find(letter) == -1:
                        toRemove.add(each)

        # Remove words after iteration
        for each in toRemove:
            self.possibleWords.remove(each)

        # Construct RE pattern with known letter positions and possible chars.
        pattern = ''
        pos = 0
        for each in self.final:
            if each == '?':
                if self.yellowPos.intersection({pos + 1}) != set():
                    pattern = pattern + f'[{self.concat(self.possibleLetters.difference({guessWord[pos]}))}]'
                else:
                    pattern = pattern + f'[{self.concat(self.possibleLetters)}]'
            else:
                pattern = pattern + each
            pos = pos + 1
        #print(f'Pattern: {pattern}')
        rePattern = re.compile(pattern)

        # Search compiled RE with possible word list
        for each in self.possibleWords:
            if (rePattern.match(each)) is not None:
                stream.write(each)
                count = count + 1

        # Close output file
        stream.close()

        # If small enough count print to terminal
        if count < 20:

            print(str(count) + " Possible Wordle Words:")

            try:
                stream = open(self.out)
                for each in stream:
                    print(each)
                stream.close()

            except OSError:
                print("Unable to open file.")

        else:
            print(f'See file {self.out} for possible words. {count} possible.')

    # Get word options
    def help(self):
        return 1
        # Return alphabetical list
        # of possible words.

    # Helper function for sets/lists
    def concat(self, list):
        concatenated = ''
        for each in list:
            concatenated = concatenated + each

        return concatenated

    # Guess a word
    def guess(self, guessWord):

        # Input filtering
        while 1:
            if len(guessWord) < 5 or len(guessWord) > 5:
                guessWord = input("Word must be 5 letters long. Try again: ")
            else:
                # Remove guessed word from possibilites.
                try:
                    self.possibleWords.remove(guessWord + "\n")
                    break
                except KeyError:
                    print("Word not in possibilities. Try again.")
                    return -1

        # Identifying green letters
        correctLetters = ""
        while 1:
            try:
                openPos = self.getOpenPositions()
                if openPos == set():
                    print(f'Congratulations on finishing you {self.size} letter Wordle! Exiting')
                    exit()
                    # Add word to correctly guessed words set in future.
                correctLetterPos = input(f'Green Letter Positions {openPos}? Enter to continue: ')
                if correctLetterPos == "":
                    break

            except IndexError:
                print("Incorrect input. Try again.")

            try:
                # Place the letter of the guessWord in the selected position into the final set.
                if(int(correctLetterPos)>=0) and (int(correctLetterPos) < self.size+1):
                    pos = int(correctLetterPos) - 1
                    self.final[pos] = guessWord[pos]
                else:
                    print(f'The number {int(correctLetterPos)} is out of range. Try again.')

            except ValueError:
                print("Incorrect value type. Try again.")


        # Copy identified green letters into required letters set.
        count = 0
        for each in self.final:
            if each != '?':
                self.requiredLetters.add(each)

        # Identifying yellow letters
        jump = 0
        while 1:

            try:
                requiredLetterPos = input(f'Yellow Letter Position {self.getOpenPositions().difference(self.yellowPos)}? Enter to continue:')
                if requiredLetterPos == "":
                    break
            except IndexError:
                print("Incorrect input. Try again.")

            for each in self.final:
                # Check if required letter is already a green letter.
                if each == requiredLetterPos:
                    print(f'{requiredLetterPos} is a final letter and cannot be a Yellow letter.')
                # Add the letter to required letters if not.
                else:
                    try:
                        # Place the letter of the guessWord in the selected position into the final set.
                        if (int(requiredLetterPos) >= 0) and (int(requiredLetterPos) < self.size + 1):
                            pos = int(requiredLetterPos) - 1
                            self.requiredLetters.add(guessWord[pos])
                            self.yellowPos.add(pos + 1)
                        else:
                            print(f'The number {int(requiredLetterPos)} is out of range. Try again.')

                    except ValueError:
                        print("Incorrect value type. Try again.")

        # Remove assumed gray letters
        guessSet = set()
        for guessWordLetter in guessWord:
            guessSet.add(guessWordLetter)

        for each in self.requiredLetters:
            guessSet.remove(each)

        for each in guessSet:
            self.possibleLetters.remove(each)

    def getOpenPositions(self):
        markedPositions = set()
        i = 0

        for each in self.final:
            if each == '?':
                markedPositions.add(i+1)
            i = i + 1

        return markedPositions






# Press the green button in the gutter to run the script.
if __name__ == '__main__':

    # Create WordleWord object.
    word = WordleWord()

    # Loop guesses and possibilites.
    while 1:
        guessWord = input("Which word did you guess?: ")
        if guessWord == "":
            print("Exiting.")
            break
        if word.guess(guessWord) != -1:
            word.possibilities()
            print(word.final)
            print(word.requiredLetters)
            print(word.possibleLetters)
