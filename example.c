#include "viterbi.h"

/*
This example illustrates how to use the viterbi decoder

The example chosen represents the convolutional encoder and Viterbi encoder
for convolutional codes of LRPT signals, particularly those from the Russian
weather satellite series Meteor.

See: https://dbelloli.com/software/lrpt/

The shift register has a length of 7 bits and two encoders:

   ⁰ ¹ ² ³ ⁴ ⁵ ⁶
        XOR -> 1. Encoder
         |
   -------------
   | | | |     |
  |0|0|0|0|0|0|0|
   |   | |   | |
   -------------
         |
        XOR -> 2. Encoder
*/

int main () {
  // Creating two encoders
  encoder e [2];
  create_encoder(&e[0], XOR, 5, 0, 1, 2, 3, 6); // Encode the bits 0, 1, 2, 3 and 6 with XOR
  create_encoder(&e[1], XOR, 5, 0, 2, 3, 5, 6); // Encode the bits 0, 2, 3, 5 and 6 with XOR

  // Creating a trellis
  trellis t;
  create_trellis( &t, 7, e, 2, push_bit_left ); // State length: 7, 2 encoders, Push bits in from the left

  // Raw bit sequence to be encoded using convolutional encoding
  char bit_sequence [7] = "010011\0";

  // Creating the convolutional code from the raw bit sequence
  char* conv_code = convolutional_encode(bit_sequence, e, 2, 7, push_bit_left);

  // Decoding the convolutional code using the Viterbi algorithm
  viterbi_result* res = viterbi_decode( conv_code, &t );

  printf("Convolutional code: %s\n", conv_code);
  printf("Result: %s Weight: %d\n", res->result, res->weight);

  return 0;
}
