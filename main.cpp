/* Author: Ethson Villegas
 * -----------------------
 * This program will trick the player into thinking it has superior
 * playing skills for hangman.
 * However it relies on a cheating mechanism where it doesn't commit to a word
 * until it is forced to do so, making victory for the machine
 * much more likely.
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <array>
#include <vector>
#include <map>
#include <string>
#include <string.h>
#include <cctype>
//#include "hello.h"
using namespace std;

/*  Function: read_file
 *  --------------------
 *  Reads the file and populates a map of vectors of strings depending on the size of
 *  the read word.
 */
void read_file(map<int, vector<string>> &wordsInDic){
    ifstream dictionary("dictionary.txt");
    if(!dictionary.is_open())
        cerr << "Cannot find dictionary.txt" << endl;
    string wordToAdd;
    while(dictionary >> wordToAdd){
        wordsInDic[wordToAdd.length()].push_back(wordToAdd);
    }
}

/*  Function: GetInteger
 *  --------------------
 *  Borrowed from chapter 3 from CS106L course reader
 *  Adapted to take a reprompt parameter so as to customize it
 *  to each call.
 */
int GetInteger(string rePrompt){
    while(true) { // Read input until user enters valid data
        stringstream converter;
        string GetLine;
        getline(cin, GetLine);
        converter << GetLine;
        /* Try reading an int, continue if we succeeded. */
        int result;
        if(converter >> result) {
            char remaining;
            if(converter >> remaining)    // Something's left, input is invalid
                cout << "Unexpected character: " << remaining << endl;
            else
                return result;
        } else
            cout << "Please enter an integer." << endl;
        cout << rePrompt;
    }
}

/* Function: prompt_length
 * -----------------------
 * It prompts the user to enter a length, calls get Integer to assure
 * the length is a proper integer
 */
int prompt_length(map<int, vector<string>> &wordsInDic){
    string prompt = "Please insert the length of the word you wish to play: ";
    cout << prompt;
    int lengthToPlay = GetInteger(prompt);
    map<int, vector<string>>::iterator finder = wordsInDic.begin();
    finder = wordsInDic.find(lengthToPlay);
    while(finder == wordsInDic.end()){
        cout << "There are no words with that length. " << prompt << endl;
        lengthToPlay = GetInteger(prompt);
        finder = wordsInDic.find(lengthToPlay);
    }
    return lengthToPlay;
}

/* Function: prompt_guesses
 * ------------------------
 * Prompts user for how many guesses to have.
 */
int prompt_guesses(){
    string prompt = "Please enter the number of guesses you want, it won't help: ";
    cout << prompt;
    int guesses = GetInteger(prompt);
    while(guesses < 1){
        cout << "Come on now, don't give up. You need more than 0" << endl;
        guesses = GetInteger(prompt);
    }
    return guesses;
}

/* Function: prompt_words_left
 * ----------------------------
 * Prompts user whether to see the grading mechanism/how many words left
 */
int prompt_words_left(){
    string answer;
    cout << "Would you like to see the number of words remaining? (Y/N): ";
    while(true){
        getline(cin, answer);
        if(answer.length()>1)
            cout << "That was not a valid choice, please enter (Y/N): ";
        else{
            char answerC;
            answerC = answer[0];
            answerC = tolower(answerC);
            if(answerC == 'y')
                return 1;
            else if(answerC == 'n')
                return 0;
            cout << "That was not a valid choice, please enter (Y/N): ";
        }
    }
}

/* Function: binary_key
 * ---------------------
 * Creates the key necessary for the map of new families to work
 * Makes all found characters 1, all others 0
 */
string binary_key(string value, char wantedChar){
    string key;
    for(size_t i = 0; i < value.length(); i++){
        if(value[i] == wantedChar)
           key.push_back('1');
        else
            key.push_back('0');
    }
    return key;
}

/* Function: prompt_for_guess
 * --------------------------
 * Prompts the user for a guess letter
 * Checks if that letter has already been used and reprompts if thats the case
 */
char prompt_for_guess(string guessedLetters){

    while(true){
        char answer;
        cout << "Please enter the character to look for in the word: ";
        string GetLine;
        getline(cin, GetLine);
        if(GetLine.length() > 1)
            cout << "Please only enter one character" << endl;
        answer = GetLine[0];
        if(isalpha(answer)){
            if(guessedLetters.find(answer) != -1)
                cout << "You have already entered that value, please try a new one: " << endl;
            else
                return answer;
        } else
            cout << "Please enter a character." << endl;


    }
}

/* Function: update_word
 * ---------------------
 * Gets the current word (hidden chars), the key string and the wantedChar
 * Acts like a mask to get the word updated without itself knowing which final word it
 * will have
 * Returns 0 if there was no change, which deducts from the guesses
 */
int update_word(string &currWord, string &key, char wantedChar){
    int change = 0;
    for(size_t i = 0; i < key.length(); ++i){
        if(key[i] == '1'){      // '1' represents the turned on bit for the key string, where wantedChar is
            currWord[i] = wantedChar;
            change = 1;
        }
    }
    return change;
}

/* Function: end_game
 * ------------------
 * Provides closure for the game.
 * scenario is initialized to 1, the most likely outcome being the user lost
 * If passed an argument other than this it assumes victory
 */
void end_game(string guessedWord, int scenario = 1){
    if(scenario == 1){
        cout << "So close, the word was actually: " << guessedWord << endl;
        cout << "Sorry, you lose." << endl;
        cout << "GG" << endl;
    } else {
        cout << "You have bested the finest hangman player in"
                " the world. Have a virtual cookie" << endl;
    }
}

/* Function: troll
 * ----------------
 * Fun little extension, it gives the user the impression of having a chance
 * at redemption just to take it away.
 */
void troll(){
    cout << "Rematch? (Y/N): ";
    cin.ignore();
    cout << "No re" << endl;
}

int main() {
    map<int, vector<string>> wordsInDic;

    read_file(wordsInDic);

    cout << "Welcome to Deep Hangman, the World's most sophisticated Hangman experience" << endl;
    cout << "Let only those who are true masters of the game challenge the machine" << endl;

    int length = prompt_length(wordsInDic);
    int guesses = prompt_guesses();
    int display = prompt_words_left();
    vector<string> currentFamily = wordsInDic[length];

    string guessedLetters;
    string currWord;
    for(int i =0; i < length; ++i){
        currWord.push_back('_');
    }
    while(guesses != 0){
        map <string, vector<string>> family;
        if(display == 1){
            cout << "There are " << currentFamily.size() << " words remaining" << endl;
            cout << endl;
        }
        cout << "The current word looks like: " << currWord << endl;

        cout << "You have " << guesses << " guesses remaining." << endl;
        cout << "You have guessed: \"" << guessedLetters << "\" so far." << endl;
        char wantedChar = prompt_for_guess(guessedLetters);
        guessedLetters.push_back(wantedChar);


        for(vector<string>::iterator it = currentFamily.begin();
            it != currentFamily.end(); ++it){
            string key = binary_key(*it, wantedChar);
            family[key].push_back(*it);
        }
        int maxSize = 0;
        string key;
        for(map<string, vector<string>>::iterator it = family.begin();
            it != family.end(); ++it){
            int currSize = it->second.size();
            if(currSize > maxSize){
                maxSize = currSize;
                key = it->first;
            }
        }
        currentFamily = family[key];
        int changed = update_word(currWord, key, wantedChar);
        if(changed == 0)
            guesses--;
        cout << endl;
        if(currWord.find('_') == -1){
            end_game(currentFamily[guesses], 0);
            break;
        }
    }
    if(guesses == 0)
        end_game(currentFamily[guesses]);


    //Last function added for laughs, comment to go into normal performance
    troll();

    return 0;
}
