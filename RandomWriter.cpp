/*
 * File: RandomWriter.cpp
 * ----------------------
 * Name: [TODO: Amy R. Weiner]
 * Section: [TODO: Sammy Nguyen]
 * NOTE: I am taking a late day, and I was in class both today and yesterday. 
 * This program reads a source text, builds an order-k Markov model for it, 
 * and generates random output that follows the frequency patterns of the model.
 */

#include <fstream>
#include <iostream>
#include <string>
#include "console.h"
#include "simpio.h"
#include "map.h"
#include "random.h"
#include "strlib.h"
#include "vector.h"
using namespace std;

/* Constant values */
const int MAX_CHARACTERS = 2000;

/* Funciton prototypes */
string promptUserForFile(ifstream & infile, string prompt = "");
void trackCurrentSeed(string & charactersInText, int seedLength, Map<string, Vector<char> > & seedsToFollowingCharacters);
string findStartSeed(Map<string, Vector<char> > & seedsToFollowingCharacters);
void generateRandomText(string startseed, Map<string, Vector<char> > & seedsToFollowingCharacters);

/* Main program */
int main() {
	ifstream infile;
    promptUserForFile(infile, "Please enter filename containing source text: ");
    int seedLength = getInteger("What order of analysis (1 to 10): ");
    cout << "Analyzing ... please wait." << endl;
    string charactersInText = "";
    Map<string, Vector<char> > seedsToFollowingCharacters;
    while (true) {
        int ch = infile.get();
        if (ch == EOF) break;
        charactersInText += ch;
    }
    infile.close();
    trackCurrentSeed(charactersInText, seedLength, seedsToFollowingCharacters);
    string startSeed = findStartSeed(seedsToFollowingCharacters);
    cout << startSeed;
    generateRandomText(startSeed, seedsToFollowingCharacters);
	return 0;
}

/* Function: promptUserForFile
 * Usage: string filename promptUserForFile(infile, prompt);
 * ---------------------------------------------------------
 * This function is taken from Figure 4-3 of the course 
 * reader. 
 */
string promptUserForFile(ifstream & infile, string prompt) {
    while (true) {
        cout << prompt;
        string filename;
        getline(cin, filename);
        infile.open(filename.c_str());
        if (!infile.fail()) return filename;
        infile.clear();
        cout << "Unable to open that file. Try again." << endl;
        if (prompt == "") prompt = "Please enter filename containing source text: ";
    }
}

/* Function: trackCurrentSeed
 * Usage: trackCurrentSeed(charactersInText, seedLength, seedsToFollowingCharacters);
 * ----------------------------------------------------------------------------------
 * This function creates a map of all of seeds of length k in the source text, where 
 * k is the Markov model order input by the user, to a vector of characters that 
 * follow ecah instance of a given seed. 
 */
void trackCurrentSeed(string & charactersInText, int seedLength, Map<string, Vector<char> > & seedsToFollowingCharacters) {
    for (int i = 0; i < charactersInText.size() - seedLength; i++) {
        string seed = charactersInText.substr(i,seedLength);
        char followingCharacter = charactersInText[i +seedLength];
        if(!seedsToFollowingCharacters.containsKey(seed)) {
            Vector<char> followingCharacters;
            seedsToFollowingCharacters.put(seed, followingCharacters);
        }
        seedsToFollowingCharacters[seed].add(followingCharacter);
    }
}

/* Function: findStartSeed
 * Usage: string startSeed = findStartSeed(seedsToFollowingCharacters);
 * --------------------------------------------------------------------
 * This function resturns the most frequently occurring string of 
 * Markov-order length k in the source text as the starting seed.
 */
string findStartSeed(Map<string, Vector<char> > & seedsToFollowingCharacters) {
    int largest = 0;
    string result = "";
    foreach(string seed in seedsToFollowingCharacters) {
        Vector<char> characters = seedsToFollowingCharacters.get(seed);
        int frequency = characters.size();
        if (frequency > largest) {
            largest = frequency;
            result = seed;
        }
    }
    return result;
}

/* Function: generateRandomText
 * Usage: generateRandomText(string startSeed, seedsToFollowingCharacters);
 * ------------------------------------------------------------------------
 * This function generates random text of length MAX_CHARACTERS plus the 
 * length of the starting seed, as a function of the order of the Markov 
 * order as well as the starting seed. 
 */
void generateRandomText(string startSeed, Map<string, Vector<char> > & seedsToFollowingCharacters) {
    string nextSeed = startSeed;
    int charactersGenerated = 0;
    while (charactersGenerated < MAX_CHARACTERS) {
        Vector<char> characters = seedsToFollowingCharacters[nextSeed];
        if(!seedsToFollowingCharacters.containsKey(nextSeed)) break;
        int index = randomInteger(0, characters.size() -1);
        char nextCharacter = characters[index];
        cout << nextCharacter;
        nextSeed = nextSeed.substr(1) + nextCharacter;
        charactersGenerated++;
    }
}
















