#include <iostream>
#include <fstream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "Globals.h"
#include "Functions.h"

// user-controlled variables
double speed = 2; // in notes per second
double volume = 0.3; // a value between 0 and 1
int arrowCounter = NONE; // the current transformation of the tone row
bool favorConsonance = false; // toggle for AI-assisted consonance-enforced tone row generation
bool enhanceConsonance = false; // toggle for increased AI-assisted consonance

int main() {
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "ToneRowGEN", sf::Style::Close | sf::Style::Titlebar);

    sf::Texture applicationScreen;
    applicationScreen.loadFromFile("UI/default.png");
    sf::Sprite application;

    // audio variables
    sf::Sound sound;
    sf::SoundBuffer buffer;

    // initialize tone rows
    int primeRow[NUM_TONES] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 };
    int retrogradeRow[NUM_TONES] = { 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 };
    int inversionRow[NUM_TONES] = { 0, 11, 10, 9 , 8, 7, 6, 5, 4, 3, 2, 1 };
    int retrogradeInversionRow[NUM_TONES] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 0 };
    int previousPrimeRow[NUM_TONES] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 };

    while (window.isOpen()) {
        sf::Event evt;
        while (window.pollEvent(evt)) {
            if (evt.type == sf::Event::Closed) {
                window.close();
            }

            // operating the application with the mouse
            if (evt.type == sf::Event::MouseButtonPressed) {
                sf::Vector2i mousePosition = sf::Mouse::getPosition(window);

                // top section of screen
                if (mousePosition.y > 105 && mousePosition.y < 235) {
                    if (mousePosition.x > 105 && mousePosition.x < 500) { // play prime row
                        arrowCounter = PRIME_KEY;
                        buffer.loadFromSamples(convertRowToSamples(primeRow, speed, volume), SAMPLES / speed, 1, SAMPLE_RATE);
                        sound.setBuffer(buffer);
                        sound.play();
                    }

                    else if (mousePosition.x > 545 && mousePosition.x < 940) { // play retrograde row
                        arrowCounter = RETROGRADE_KEY;
                        buffer.loadFromSamples(convertRowToSamples(retrogradeRow, speed, volume), SAMPLES / speed, 1, SAMPLE_RATE);
                        sound.setBuffer(buffer);
                        sound.play();
                    }

                    else if (mousePosition.x > 980 && mousePosition.x < 1375) { // play inversion row
                        arrowCounter = INVERSION_KEY;
                        buffer.loadFromSamples(convertRowToSamples(inversionRow, speed, volume), SAMPLES / speed, 1, SAMPLE_RATE);
                        sound.setBuffer(buffer);
                        sound.play();
                    }

                    else if (mousePosition.x > 1420 && mousePosition.x < 1815) { // play retrograde inversion row
                        arrowCounter = RETROGRADE_INVERSION_KEY;
                        buffer.loadFromSamples(convertRowToSamples(retrogradeInversionRow, speed, volume), SAMPLES / speed, 1, SAMPLE_RATE);
                        sound.setBuffer(buffer);
                        sound.play();
                    }

                    loadTexture(applicationScreen, arrowCounter, favorConsonance, enhanceConsonance);
                }

                // middle section of screen
                else if (mousePosition.y > 370 && mousePosition.y < 725) {
                    if (mousePosition.x > 110 && mousePosition.x < 700) {
                        if (mousePosition.y < 510) { // speed control
                            if (mousePosition.x < 260) {
                                if (speed < 3.5) {
                                    speed += 0.1;
                                }
                            }
                            else if (mousePosition.x > 550) {
                                if (speed > 0.6) {
                                    speed -= 0.1;
                                }
                            }
                        }
                        else if (mousePosition.y > 585) { // volume control
                            if (mousePosition.x < 260) {
                                if (volume < 1) {
                                    volume += 0.1;
                                }
                            }
                            else if (mousePosition.x > 550) {
                                if (volume > 0.2) {
                                    volume -= 0.1;
                                }
                            }
                        }
                    }

                    else if (mousePosition.x > 1215 && mousePosition.x < 1805) {
                        if (mousePosition.y < 520) { // generate new tone row (and subsequent transformations)
                            for (int toneIndex = 0; toneIndex < (sizeof(primeRow) / sizeof(int)); toneIndex++) // save current prime row in case user wants to go back
                                previousPrimeRow[toneIndex] = primeRow[toneIndex];

                            generateRow(primeRow, favorConsonance, enhanceConsonance); // create new prime
                            generateTransformations(primeRow, retrogradeRow, inversionRow, retrogradeInversionRow); // generate ensuing retrograde, inversion and retrograde inversion rows

                            // play prime transformation of new row
                            arrowCounter = PRIME_KEY; // reset arrow counter
                            loadTexture(applicationScreen, arrowCounter, favorConsonance, enhanceConsonance);
                            buffer.loadFromSamples(convertRowToSamples(primeRow, speed, volume), SAMPLES / speed, 1, SAMPLE_RATE);
                            sound.setBuffer(buffer);
                            sound.play();
                        }

                        else if (mousePosition.y > 590) {
                            if (mousePosition.x > 1270 && mousePosition.x < 1320) {
                                if (mousePosition.y < 640) { // toggle consonance-enforcement
                                    favorConsonance = !favorConsonance;

                                    if (!favorConsonance)
                                        enhanceConsonance = false;
                                }
                                else if (mousePosition.y > 670) { // toggle enhanced consonance-enforcement
                                    if (favorConsonance) {
                                        enhanceConsonance = !enhanceConsonance;
                                    }
                                }

                                loadTexture(applicationScreen, arrowCounter, favorConsonance, enhanceConsonance);
                            }
                        }
                    }
                }

                // bottom section of screen
                else if (mousePosition.y > 825 && mousePosition.y < 975) {
                    if (mousePosition.x > 675 && mousePosition.x < 1245) { // save current tone row
                        saveAsFile(primeRow, retrogradeRow, inversionRow, retrogradeInversionRow);
                    }
                }
            }

            // keyboard shortcuts
            if (evt.type == sf::Event::KeyPressed) {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) { // replay the current row
                    if (arrowCounter == NONE) // initialize arrowCounter if necessary
                        arrowCounter = PRIME_KEY;

                    switch (arrowCounter) { // determine which transformation of the current row user is on
                        case PRIME_KEY:
                            buffer.loadFromSamples(convertRowToSamples(primeRow, speed, volume), SAMPLES / speed, 1, SAMPLE_RATE);
                            sound.setBuffer(buffer);
                            sound.play();
                            break;
                        case RETROGRADE_KEY:
                            buffer.loadFromSamples(convertRowToSamples(retrogradeRow, speed, volume), SAMPLES / speed, 1, SAMPLE_RATE);
                            sound.setBuffer(buffer);
                            sound.play();
                            break;
                        case INVERSION_KEY:
                            buffer.loadFromSamples(convertRowToSamples(inversionRow, speed, volume), SAMPLES / speed, 1, SAMPLE_RATE);
                            sound.setBuffer(buffer);
                            sound.play();
                            break;
                        case RETROGRADE_INVERSION_KEY:
                            buffer.loadFromSamples(convertRowToSamples(retrogradeInversionRow, speed, volume), SAMPLES / speed, 1, SAMPLE_RATE);
                            sound.setBuffer(buffer);
                            sound.play();
                            break;
                    }

                    loadTexture(applicationScreen, arrowCounter, favorConsonance, enhanceConsonance);
                }

                if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::F)) {
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
                        if (speed > 0.6) {
                            speed -= 0.1;
                        }
                    }
                    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::F)) {
                        if (speed < 3.5) {
                            speed += 0.1;
                        }
                    }
                }

                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
                    if (arrowCounter == NONE) // initialize arrowCounter if necessary
                        arrowCounter = PRIME_KEY;

                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) { // switch to next transformation of the current row
                        if (arrowCounter < RETROGRADE_INVERSION_KEY)
                            arrowCounter++;
                        else
                            arrowCounter = PRIME_KEY;
                    }
                    else { // switch to previous transformation of the current row
                        if (arrowCounter > PRIME_KEY)
                            arrowCounter--;
                        else
                            arrowCounter = RETROGRADE_INVERSION_KEY;
                    }

                    switch (arrowCounter) { // play new transformation of the current row
                        case PRIME_KEY:
                            buffer.loadFromSamples(convertRowToSamples(primeRow, speed, volume), SAMPLES / speed, 1, SAMPLE_RATE);
                            sound.setBuffer(buffer);
                            sound.play();
                            break;
                        case RETROGRADE_KEY:
                            buffer.loadFromSamples(convertRowToSamples(retrogradeRow, speed, volume), SAMPLES / speed, 1, SAMPLE_RATE);
                            sound.setBuffer(buffer);
                            sound.play();
                            break;
                        case INVERSION_KEY:
                            buffer.loadFromSamples(convertRowToSamples(inversionRow, speed, volume), SAMPLES / speed, 1, SAMPLE_RATE);
                            sound.setBuffer(buffer);
                            sound.play();
                            break;
                        case RETROGRADE_INVERSION_KEY:
                            buffer.loadFromSamples(convertRowToSamples(retrogradeInversionRow, speed, volume), SAMPLES / speed, 1, SAMPLE_RATE);
                            sound.setBuffer(buffer);
                            sound.play();
                            break;
                    }

                    loadTexture(applicationScreen, arrowCounter, favorConsonance, enhanceConsonance);
                }

                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) { // generate new prime row (and subsequent transformations)
                    for (int toneIndex = 0; toneIndex < (sizeof(primeRow) / sizeof(int)); toneIndex++) // save current prime row in case user wants to go back
                        previousPrimeRow[toneIndex] = primeRow[toneIndex];

                    generateRow(primeRow, favorConsonance, enhanceConsonance); // create new prime row
                    generateTransformations(primeRow, retrogradeRow, inversionRow, retrogradeInversionRow); // generate ensuing retrograde, inversion and retrograde inversion rows

                    // play the prime transformation of the new row
                    arrowCounter = PRIME_KEY; // reset arrow counter
                    loadTexture(applicationScreen, arrowCounter, favorConsonance, enhanceConsonance);
                    buffer.loadFromSamples(convertRowToSamples(primeRow, speed, volume), SAMPLES / speed, 1, SAMPLE_RATE);
                    sound.setBuffer(buffer);
                    sound.play();
                }

                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) { // return to previous tone row
                    for (int toneIndex = 0; toneIndex < (sizeof(primeRow) / sizeof(int)); toneIndex++) // restore previous prime row
                        primeRow[toneIndex] = previousPrimeRow[toneIndex];

                    arrowCounter = PRIME_KEY; // reset arrow counter
                    generateTransformations(primeRow, retrogradeRow, inversionRow, retrogradeInversionRow); // generate ensuing retrograde, inversion and retrograde inversion rows

                    // play the prime transformation of the new row
                    loadTexture(applicationScreen, arrowCounter, favorConsonance, enhanceConsonance);
                    buffer.loadFromSamples(convertRowToSamples(primeRow, speed, volume), SAMPLES / speed, 1, SAMPLE_RATE);
                    sound.setBuffer(buffer);
                    sound.play();
                }
            }
        }

        // update screen
        window.clear();
        application.setTexture(applicationScreen);
        window.draw(application);
        window.display();
    }

    return 0;
}
