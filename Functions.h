#pragma once

#include <fstream>
#include <SFML/Audio.hpp>

// converts a tone row to a raw array of samples
// arguments: the tone row as an array of doubles (should always consist of 12 elements), the speed, the volume
// returns the tone row converted to a raw array of samples (elements of which are of the Int16 data type)
sf::Int16* convertRowToSamples(int* row, double speed, double volume);

// generates all three transformations based on a give prime row (i.e., the retrograde, inversion and retrograde inversion rows)
// arguments: an array containing the prime row, the array that will contain the retrograde row, the array that will contain the inversion row,
// the array that will contain the retrograde inversion row (all integer arrays of size NUM_TONES)
// returns nothing
void generateTransformations(int* primeRow, int* retrogradeRow, int* inversionRow, int* retrogradeInversionRow);

// reverses a given tone row
// arguments: the array of tones that will store the reversed tone row, the original tone row to be reversed
// returns nothing
void reverseRow(int* resultingRow, int* originalRow);

// inverts the intervals between tones in a given tone row
// arguments: the array of tones that will store the inverted tone row, the original tone row to be inverted
// returns nothing
// note that the first tone in the resulting row will be identical to that from the original row
void invertRow(int* resultingRow, int* originalRow);

// saves all transformations of the current tone row as a text file in both pitch-class integer notation and letter notation
// arguments: the prime row array, the retrograde row array, the inversion row array, the retrograde inversion row array
// returns nothing
// note that the resulting text file containing the tone row and its transformations is created inside the fuction
void saveAsFile(int* primeRow, int* retrogradeRow, int* inversionRow, int* retrogradeInversionRow);

// writes a given tone row as a single line to a given text file in either pitch-class integer notation or letter notation
// arguments: an option variable indicating which type of notation is to be used (1 for pitch-class integer notation and 2 for letter notation),
// the row to be written as an array of integers, the file the row will be written to (passed by reference)
// returns nothing
void writeRowToFile(int option, int* row, std::ofstream &file);