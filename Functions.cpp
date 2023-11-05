#include <iostream>
#include <fstream>
#include <cmath>
#include <SFML/Audio.hpp>

#include "Globals.h"
#include "Functions.h"

sf::Int16* convertRowToSamples(int* row, double speed, double volume) {
    sf::SoundBuffer buffer;
    sf::Int16* samplesArray = new sf::Int16[SAMPLES / speed];

    int sampleIndex = 0;
    for (int toneIndex = 0; toneIndex < NUM_TONES; toneIndex++) {
        double increment = (C4_FREQUENCY * pow(TWELFTH_ROOT_OF_TWO, row[toneIndex])) / SAMPLE_RATE;
        double x = 0;
        while (sampleIndex < ((SAMPLES / speed) / NUM_TONES) * (toneIndex + 1)) {
            samplesArray[sampleIndex] = (volume * VOLUME_FACTOR) * sin(x * TWO_PI);
            x += increment;
            sampleIndex++;
        }
    }

    return samplesArray;
}

void generateTransformations(int* primeRow, int* retrogradeRow, int* inversionRow, int* retrogradeInversionRow) {
    reverseRow(retrogradeRow, primeRow);
    invertRow(inversionRow, primeRow);
    reverseRow(retrogradeInversionRow, inversionRow);
}

void reverseRow(int* resultingRow, int* originalRow) {
    int forwardIterator = 0;
    for (int reverseIterator = NUM_TONES - 1; reverseIterator >= 0; reverseIterator--) {
        resultingRow[forwardIterator] = originalRow[reverseIterator];
        forwardIterator++;
    }
}

void invertRow(int* resultingRow, int* originalRow) {
    resultingRow[0] = originalRow[0];

    for (int index = 1; index < NUM_TONES; index++) {
        resultingRow[index] = resultingRow[index - 1];
        int interval = std::abs(originalRow[index] - originalRow[index - 1]);

        for (int intervalCounter = 0; intervalCounter < interval; intervalCounter++) {
            if (originalRow[index] - originalRow[index - 1] > 0) { // inverted interval will be descending
                if (resultingRow[index] > 0)
                    resultingRow[index]--;
                else
                    resultingRow[index] = 11;
            }
            else { // inverted interval will be ascending
                if (resultingRow[index] < NUM_TONES - 1)
                    resultingRow[index]++;
                else
                    resultingRow[index] = 0;
            }
        }
    }
}


void saveFile(int* primeRow, int* retrogradeRow, int* inversionRow, int* retrogradeInversionRow) {
    std::ofstream toneRows("ToneRows.txt");

    toneRows << "Tone rows in pitch-class integer notation\n";
    toneRows << "Prime: ";
    writeRowToFile(PITCH_CLASS_NOTATION, primeRow, toneRows);
    toneRows << "Retrograde: ";
    writeRowToFile(PITCH_CLASS_NOTATION, retrogradeRow, toneRows);
    toneRows << "Inversion: ";
    writeRowToFile(PITCH_CLASS_NOTATION, inversionRow, toneRows);
    toneRows << "Retrograde Inversion: ";
    writeRowToFile(PITCH_CLASS_NOTATION, retrogradeInversionRow, toneRows);

    toneRows << "\n\nTone rows in letter notation\n";
    toneRows << "Prime: ";
    writeRowToFile(LETTER_NOTATION, primeRow, toneRows);
    toneRows << "Retrograde: ";
    writeRowToFile(LETTER_NOTATION, retrogradeRow, toneRows);
    toneRows << "Inversion: ";
    writeRowToFile(LETTER_NOTATION,inversionRow, toneRows);
    toneRows << "Retrograde Inversion: ";
    writeRowToFile(LETTER_NOTATION, retrogradeInversionRow, toneRows);

    toneRows.close();
}

void writeRowToFile(int option, int* row, std::ofstream &file) {
    for (int i = 0; i < NUM_TONES; i++) {
        if (option == PITCH_CLASS_NOTATION) {
            if (row[i] < 10)
                file << row[i] << " ";
            else {
                switch (row[i]) {
                    case 10:
                        file << "t ";
                        break;
                    case 11:
                        file << "e ";
                        break;
                }
            }
        }
        else {
            switch (row[i]) {
                case 0:
                    file << "C ";
                    break;
                case 1:
                    file << "C# ";
                    break;
                case 2:
                    file << "D ";
                    break;
                case 3:
                    file << "D# ";
                    break;
                case 4:
                    file << "E ";
                    break;
                case 5:
                    file << "F ";
                    break;
                case 6:
                    file << "F# ";
                    break;
                case 7:
                    file << "G ";
                    break;
                case 8:
                    file << "G# ";
                    break;
                case 9:
                    file << "A ";
                    break;
                case 10:
                    file << "A# ";
                    break;
                case 11:
                    file << "B ";
                    break;
            }
        }
    }

    file << std::endl;
}