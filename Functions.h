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

void saveFile(int* primeRow, int* retrogradeRow, int* inversionRow, int* retrogradeInversionRow);

void writeRowToFile(int option, int* row, std::ofstream &file);