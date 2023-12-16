#include <iostream>
#include <fstream>
#include <cmath>
#include <random>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "Globals.h"
#include "Functions.h"

void loadTexture(sf::Texture &applicationScreen, int transformationNum, bool favorConsonance, bool enhanceConsonance) {
    std::string texturePath = "UI";

    if (favorConsonance) {
        texturePath.append("/consonance");
        if (enhanceConsonance)
            texturePath.append("/enhance");
    }

    switch (transformationNum) {
        case NONE:
            texturePath.append("/default.png");
            break;
        case PRIME_KEY:
            texturePath.append("/prime.png");
            break;
        case RETROGRADE_KEY:
            texturePath.append("/retrograde.png");
            break;
        case INVERSION_KEY:
            texturePath.append("/inversion.png");
            break;
        case RETROGRADE_INVERSION_KEY:
            texturePath.append("/retrograde_inversion.png");
            break;
    }

    applicationScreen.loadFromFile(texturePath);
}

sf::Int16* convertRowToSamples(int* row, double speed, double volume) {
    sf::SoundBuffer buffer;
    sf::Int16* samplesArray = new sf::Int16[int(SAMPLES / speed)];

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

void generateRow(int* toneRow, bool favorConsonance, bool enhanceConsonance) {
    std::random_device rd;
    std::mt19937 g(rd());

    if (favorConsonance) {
        wipeToneRow(toneRow);
        toneRow[0] = generateRandomTone();

        for (int rowIndex = 1; rowIndex < NUM_TONES; rowIndex++) {
            if (enhanceConsonance) {
                bool chordBuiltSuccessfully = false;
                int chordQualities[2] = { MAJOR, MINOR };
                int chordPositions[3] = { ROOT, FIRST_INVERSION, SECOND_INVERSION };

                std::shuffle(&chordQualities[0], &chordQualities[2], g);
                std::shuffle(&chordPositions[0], &chordPositions[3], g);

                for (int qualityIterator = 0; qualityIterator < sizeof(chordQualities) / sizeof(int); qualityIterator++) {
                    for (int positionIterator = 0; positionIterator < sizeof(chordPositions) / sizeof(int); positionIterator++) {
                        int secondNote, thirdNote;

                        if (chordPositions[positionIterator] == ROOT) {
                            chordQualities[qualityIterator] == MAJOR ? secondNote = toneRow[rowIndex - 1] + 4 : secondNote = toneRow[rowIndex - 1] + 3;
                            thirdNote = toneRow[rowIndex - 1] + 7;
                        }
                        else if (chordPositions[positionIterator] == FIRST_INVERSION) {
                            chordQualities[qualityIterator] == MAJOR ? secondNote = toneRow[rowIndex - 1] + 3 : secondNote = toneRow[rowIndex - 1] + 4;
                            chordQualities[qualityIterator] == MAJOR ? thirdNote = toneRow[rowIndex - 1] + 8 : thirdNote = toneRow[rowIndex - 1] + 9;
                        }
                        else {
                            secondNote = toneRow[rowIndex - 1] + 5;
                            chordQualities[qualityIterator] == MAJOR ? thirdNote = toneRow[rowIndex - 1] + 9 : thirdNote = toneRow[rowIndex - 1] + 8;
                        }

                        if (secondNote > 11)
                            secondNote -= 12;
                        if (thirdNote > 11)
                            thirdNote -= 12;

                        if (!isToneAlreadyUsed(secondNote, toneRow)) {
                            toneRow[rowIndex] = secondNote;
                            chordBuiltSuccessfully = true;
                        }
                        if (!isToneAlreadyUsed(thirdNote, toneRow)) {
                            if (chordBuiltSuccessfully)
                                rowIndex++;
                            toneRow[rowIndex] = thirdNote; // note that this index will never be out of range
                            chordBuiltSuccessfully = true;
                        }

                        if (chordBuiltSuccessfully)
                            break;
                    }
                    if (chordBuiltSuccessfully)
                        break;
                }
                if (!chordBuiltSuccessfully) { // the enhanceConsonance algorithm is no longer viable; switch to the favorConsonance algorithm
                    rowIndex--;
                    enhanceConsonance = false;
                }
            }
            else {
                bool notePlaced = false;
                int consonantIntervals[6] = { 3, 4, 5, 7, 8, 9 };
                int intervalDirections[2] = { UP, DOWN };

                std::shuffle(&consonantIntervals[0], &consonantIntervals[6], g);
                std::shuffle(&intervalDirections[0], &intervalDirections[2], g);

                for (int intervalIterator = 0; intervalIterator < sizeof(consonantIntervals) / sizeof(int); intervalIterator++) {
                    for (int directionIterator = 0; directionIterator < sizeof(intervalDirections) / sizeof(int); directionIterator++) {
                        int tone = toneRow[rowIndex - 1] + (consonantIntervals[intervalIterator] * intervalDirections[directionIterator]);

                        if (isToneInBound(tone) && !isToneAlreadyUsed(tone, toneRow)) {
                            toneRow[rowIndex] = tone;
                            notePlaced = true;
                            break;
                        }
                    }

                    if (notePlaced)
                        break;
                }
                if (!notePlaced) { // as a last-ditch effort, default to using the first available tone regardless of the ensuing interval
                    toneRow[rowIndex] = findFirstMissingTone(toneRow);
                }
            }
        }
    }
    else {
        std::shuffle(&toneRow[0], &toneRow[NUM_TONES], g);
    }
}

void wipeToneRow(int* toneRow) {
    for (int i = 0; i < NUM_TONES; i++)
        toneRow[i] = -1;
}

int generateRandomTone() {
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist12(0, NUM_TONES - 1);

    return dist12(rng);
}

bool isToneInBound(int tone) {
    if (tone < 0 || tone > 11)
        return false;

    return true;
}

bool isToneAlreadyUsed(int tone, int* toneRow) {
    for (int i = 0; i < NUM_TONES; i++)
        if (toneRow[i] == tone)
            return true;

    return false;
}

int findFirstMissingTone(int* toneRow) {
    int hashmap[NUM_TONES] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

    for (int i = 0; i < NUM_TONES; i++)
        if (toneRow[i] != -1)
            hashmap[toneRow[i]] = 1;

    for (int i = 0; i < NUM_TONES; i++)
        if (hashmap[i] == 0)
            return i;
}

void generateTransformations(int* primeRow, int* retrogradeRow, int* inversionRow, int* retrogradeInversionRow) {
    reverseRow(retrogradeRow, primeRow);
    invertRow(inversionRow, primeRow);
    reverseRow(retrogradeInversionRow, inversionRow);
}

void reverseRow(int* resultingRow, int* originalRow) {
    for (int index = 0; index < NUM_TONES; index++)
        resultingRow[index] = originalRow[NUM_TONES - 1 - index];
}

void invertRow(int* resultingRow, int* originalRow) {
    resultingRow[0] = originalRow[0];

    for (int index = 1; index < NUM_TONES; index++) {
        int interval = originalRow[index] - originalRow[index - 1];
        resultingRow[index] = resultingRow[index - 1] + (interval * -1);

        if (resultingRow[index] < 0)
            resultingRow[index] += 12;
        else if (resultingRow[index] > 11)
            resultingRow[index] -= 12;
    }
}

void saveAsFile(int* primeRow, int* retrogradeRow, int* inversionRow, int* retrogradeInversionRow) {
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