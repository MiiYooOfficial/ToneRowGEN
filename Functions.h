#include <fstream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

// loads the appropriate application screen from the UI directory into a given object of the sf::Texture class
// arguments: an object of class sf::Texture, which transformation the user is currently on,
// whether the user is favoring consonance, whether the user is enhancing consonance
// returns nothing
// note that the application screen object is passed by reference
void loadTexture(sf::Texture &applicationScreen, int transformationNum, bool favorConsonance, bool enhanceConsonance);

// converts a tone row to a raw array of samples
// arguments: the tone row as an array of doubles (should always consist of 12 elements), the speed, the volume
// returns the tone row converted to a raw array of samples (elements of which are of the Int16 data type)
sf::Int16* convertRowToSamples(int* row, double speed, double volume);

// generates a new tone row either via complete randomness or with consonance-enforcement
// arguments: the array to contain the prime row, whether the user wishes to enforce consonance,
// whether the user wishes to enhance consonance-enforcement
// returns nothing
void generateRow(int* toneRow, bool favorConsonance, bool enhanceConsonance);

// erases all contents of a given tone row by replacing all elements with -1
// arguments: the array containing the tone row to be wiped
// returns nothing
void wipeToneRow(int* toneRow);

// generates a random tone (0-11)
// arguments: none
// returns the randomly generated tone in pitch-class integer notation
int generateRandomTone();

// builds a diatonic triad based on the last tone in the row so far given the desired quality and positions of the ensuing chord
// arguments: the array containing the tone row, the index of the current tone in the row,
// the desired quality of the chord, an array containing the three possible chord positions in the desired order for the algorithm to attempt
// returns true if a chord was successfully built using either of the three chord positions (i.e., at least one tone was available for use)
// note that the tone row index is passed by reference so it can be automatically updated if necessary
bool buildChord(int* toneRow, int &rowIndex, int chordQuality, int* chordPosition);

// checks if a tone is within the confines of an octave
// arguments: the tone to check for (in pitch-class integer notation)
// returns true if the tone is within bounds, false otherwise
bool isToneInBound(int tone);

// checks if a tone is already present in a given tone row
// arguments: the tone to check for (in pitch-class integer notation), the array containing the tone row (so far)
// returns true if the tone was found in the tone row, false otherwise
bool isToneAlreadyUsed(int tone, int* toneRow);

// finds the first missing tone in a given tone row
// arguments: the array containing the tone row
// returns the first missing tone from the row in pitch-class integer notation
int findFirstMissingTone(int* toneRow);

// generates all three transformations based on a give prime row (i.e., the retrograde, inversion and retrograde inversion rows)
// arguments: an array containing the prime row, the array that will contain the retrograde row, the array that will contain the inversion row,
// the array that will contain the retrograde inversion row (all integer arrays of size NUM_TONES)
// returns nothing
void generateTransformations(int* primeRow, int* retrogradeRow, int* inversionRow, int* retrogradeInversionRow);

// reverses a given tone row
// arguments: the array of tones that will store the reversed tone row, the original tone row to be reversed
// returns nothing
void reverseRow(int* resultingRow, int* originalRow);

// flips the intervals between every tone in a given tone row
// arguments: the array of tones that will store the inverted tone row, the original tone row to be inverted
// returns nothing
// note that the first tone in the resulting row will be identical to that from the original row
void invertRow(int* resultingRow, int* originalRow);

// saves all transformations of the current tone row as a text file in both pitch-class integer notation and letter notation
// arguments: the prime row array, the retrograde row array, the inversion row array, the retrograde inversion row array
// returns nothing
// note that the resulting text file containing the tone row and its transformations is created inside the function
void saveAsFile(int* primeRow, int* retrogradeRow, int* inversionRow, int* retrogradeInversionRow);

// writes a given tone row as a single line to a given text file in either pitch-class integer notation or letter notation
// arguments: an option variable indicating which type of notation is to be used (1 for pitch-class integer notation and 2 for letter notation),
// the row to be written as an array of integers, the file the row will be written to (passed by reference)
// returns nothing
void writeRowToFile(int option, int* row, std::ofstream &file);