// two times pi
const double TWO_PI = 6.28318;

// the twelfth root of two to the nearest millionth
const double TWELFTH_ROOT_OF_TWO = 1.059463;

// the frequency in Hz of middle C (i.e., pitch class integer 0)
const double C4_FREQUENCY = 261.6256;

// the number tones per row
const int NUM_TONES = 12;

// the default number of samples for an entire row (i.e., when speed == 1)
const unsigned SAMPLES = 529200;

// the sample rate (standard)
const unsigned SAMPLE_RATE = 44100;

// the full scale audio input without scaling
const int VOLUME_FACTOR = 32767;

// integer equivalents for arrow keys
const int PRIME_KEY = 1;
const int RETROGRADE_KEY = 2;
const int INVERSION_KEY = 3;
const int RETROGRADE_INVERSION_KEY = 4;

// integer options when writing tone rows to a file
const int PITCH_CLASS_NOTATION = 1;
const int LETTER_NOTATION = 2;