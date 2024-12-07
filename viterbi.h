#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>
#include <stdbool.h>
#include <stdarg.h>


/*---------------------------------------------------*/
/*---------------- DATA STRUCTRES -------------------*/

// TRELLIS

// Structure for representing a state of the trellis:
//  - state:  Bit sequence of the state
//  - state0: Bit sequence of the state to transition to on input 0
//  - state1: Bit sequence of the state to transition to on input 1
//  - code0:  Convolutional code bit sequence of the state transitioned to on input 0
//  - code1:  Convolutional code bit sequence of the state transitioned to on input 1
//  Ending _dec: Decimal representation of the binary string

typedef struct {
    char* state;
    char* state0;
    char* state1;
    char* code0;
    char* code1;
    unsigned int code0_dec;
    unsigned int code1_dec;
    unsigned int state0_dec;
    unsigned int state1_dec;
} trellis_state;

// Structure for representing the trellis
//  - states:       All states with their correpsonding bit sequences, transitions and codes
//  - state_length: Length of the state bit sequences
//  - code_length:  Length of the convolutional codes
//  - num_states:   Number of states in the trellis

typedef struct {
    trellis_state* states;
    unsigned int state_length;
    unsigned int code_length;
    unsigned int num_states;
} trellis;



// ENCODER
// We define an encoder as an entity that picks specified bits from the sequence and performs
// a logical function on them (see enum operations)
//
// At present it is possible to pick 5 bits from the sequence
//
// You usually supply an array of encoders to the functions create_trellis() and viterbi_decode()
// The number of encoders supplied will also determine the number of the bits of the encoded sequence
//
//  - bits:     An array to specify the bits to be computed by the logical function
//              The counting starts at the most significant bit (the bit on the left) at 0
//
//              Example:
//              Let us suppose we have a 4 bit shift register and we want to select the bit 1 and 3
//
//                ¹ ² ³ ⁴
//               |0|0|0|0|
//                ^   ^
//
//  - num_bits: Total number of selected bits
//  - op:       Logical operation to be performed on the selected bits (defined in enum operations)

typedef struct {
    int* bits;
    int num_bits;
    int op;
} encoder;

// Logical operations to be performed on the bits
enum operations {
    AND,
    OR,
    XOR,
    NAND,
    NOR,
    NXOR,
    NOT,
    NON
};

// Pushing a bit into the shift register (used as function pointers)
int push_bit_left (char* bit_seq, unsigned int bit);
int push_bit_right (char* bit_seq, unsigned int bit);

// RESULT
// After decoding the convolutional code, the function viterbi_decode() returns
// not only the decoded bit sequence but also the weight of the last node.
// If the weight is 0, the convolutional code didn't contain errors.
// The higher the weight is, the more uncertain it is if the decoded sequence is correct

typedef struct {
  char* result;
  unsigned int weight;
} viterbi_result;


// Creating a trellis from an array of encoders and a push_bit function
//  - tr:            Pointer to the trellis to be created
//  - num_bits:      Number of bits in the bit sequence
//  - num_encoders:  Number of encoders -> Determines the number of bits per bit for the
//                   convolutional encoding
//  - push_bit_func: Pointer to the function to push the bits into the shift register
//                   - push_bit_left
//                   - push_bit_right

void create_trellis (trellis* tr, unsigned int num_bits, encoder* enc, unsigned int num_encoders, int (*push_bit_func)(char*, unsigned int));


// Decoding a convolutional code using the Viterbi algorithm
//  - code: Bit sequence to be decoded
//  - tr:   Pointer to the trellis to be used for decoding

viterbi_result* viterbi_decode (char* code, trellis* tr);

// Encoding a bit sequence with a convolutional code
//  - seq:           Bit sequence to be encoded
//  - enc:           Array of encoders
//  - num_encoders:  Number of encoders -> Determines the number of bits per per encoded bit
//  - push_bit_func: Pointer to the function to push the bits into the shift register
//                   - push_bit_left
//                   - push_bit_right

char* convolutional_encode (char* seq, encoder* enc, int num_encoders, int state_length, int(*push_bit_func)(char*, unsigned int));


// Creating an encoder for encoding the bit sequence
//
//  - enc:       Pointer to the encoder to be created
//  - operation: Logical operation to be performed on the bits (see enum operations)
//  - num_bits:  Number of bits that should be computed in the encoder
//  - ...      : Indizes of the bits to be computed
//               The counting starts at the most significant bit (the bit on the left) at 0
//               The array element after the last bit index must be set to -1 to mark the end
//
//               Example:
//               Let us suppose we have a 4 bit shift register and we want to select the bit 1 and 3
//               and commpute them with XOR
//
//                ¹ ² ³ ⁴
//               |0|0|0|0|
//                ^   ^
//
//               The resulting function call would look like this:
//               create_encoder(XOR, 2, 1, 3)

void create_encoder(encoder* enc, int operation, int num_bits, ...);

void print_trellis (trellis* tr);
