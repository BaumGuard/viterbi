// A function for calculating the power of a number
unsigned int power (unsigned int base, unsigned int exp) {
    int n=1;

    for (int i=0; i<exp; i++)
        n *= base;

    return n;
}

// A function for copying strings
void string_copy (char* str1, char* str2) {
    int str_it = 0;
    while ( str2[str_it] == '0' || str2[str_it] == '1' ) {
        str1[str_it] = str2[str_it];
        str_it++;
    }
    str1[str_it] = '\0';
}

// A function for getting a substring
char* substring (char* str, unsigned int start, unsigned int end) {
    char* substr = malloc(end-start+1);

    for (int i=start; i<=end; i++) {
        substr[i-start] = str[i];
    }

    return substr;
}

// Test if a given string is a bit sequence (consisting only of 0 and 1)
bool is_bit_sequence (char* seq) {
    int it = 0;

    while (seq[it] != '\0') {
        if (seq[it] != '0' && seq[it] != '1')
            return false;
        it++;
    }

    return true;
}

// Check if a given number is a bit (0 or 1)
bool is_bit (unsigned int n) {
    if ( n==0 || n==1 )
        return true;
    else
        return false;
}

// Converting a decimal number to a binary bit sequence
char* dec_to_bin (unsigned int n, unsigned int num_bits) {
    char* bin = (char*) malloc(num_bits);

    for (int i=num_bits-1; i>=0; i--) {
        bin[i] = (n % 2)+ASCII_OFFSET;
        n /= 2;
    }

    return bin;
}

// Converting a binary bit sequence to a decimal number
unsigned int bin_to_dec (char* bits) {

    if ( !is_bit_sequence(bits) ) {
        fprintf(stderr, "ERROR: bin_to_dec: The string '%s' is not a bit sequence\n", bits);
        return -1;
    }

    unsigned int num = 0;
    int num_bits = strlen(bits);

    for (int i=0; i<num_bits; i++) {
        num += (bits[i] - ASCII_OFFSET) * power(2, num_bits-i-1);
    }

    return num;
}

// Pushing a bit into the shift register from the left-hand side
int push_bit_left (char* bit_seq, unsigned int bit) {
    if ( !is_bit_sequence(bit_seq) ) {
        fprintf(stderr, "ERROR: push_bit_left: The string '%s' is not a bit sequence\n", bit_seq);
        return -1;
    }
    if ( !is_bit(bit) ) {
        fprintf(stderr, "ERROR: push_bit_left: %d is not a bit (must be 0 or 1)\n", bit);
        return -2;
    }

    unsigned int num_bits = strlen(bit_seq);

    for (int i=num_bits-1; i>0; i--)
        bit_seq[i] = bit_seq[i-1];

    bit_seq[0] = (char) (bit + ASCII_OFFSET);

    return 0;
}

// Pushing a bit into the shift register from the right-hand side
int push_bit_right (char* bit_seq, unsigned int bit) {
    if ( !is_bit_sequence(bit_seq) ) {
        fprintf(stderr, "ERROR: push_bit_right: The string '%s' is not a bit sequence\n", bit_seq);
        return -1;
    }
    if ( !is_bit(bit) ) {
        fprintf(stderr, "ERROR: push_bit_right: %d is not a bit (must be 0 or 1)\n", bit);
        return -2;
    }

    unsigned int num_bits = strlen(bit_seq);

    for (int i=0; i<num_bits-1; i++)
        bit_seq[i] = bit_seq[i+1];

    bit_seq[num_bits-1] = (char) (bit + ASCII_OFFSET);

    return 0;
}

// Getting the hamming distance of two bit sequences
// The hamming distance tells us how many bits differ when looking at one index at a time
unsigned int hamming_distance (char* bit_seq1, char* bit_seq2) {
    unsigned int dist = 0;

    unsigned int bit_seq1_length = strlen(bit_seq1);
    unsigned int bit_seq2_length = strlen(bit_seq2);

    if (bit_seq1_length != bit_seq2_length) {
        fprintf(stderr, "ERROR: hamming_distance: Unequal number of bits (%s and %s)\n", bit_seq1, bit_seq2);
        return -1;
    }

    for (int i=0; i<bit_seq1_length; i++) {
        if (bit_seq1[i] != bit_seq2[i])
            dist++;
    }

    return dist;
}

// Getting the input bit when transitioning from one state to another
char get_register_input (trellis* tr, unsigned int src_state, unsigned int dest_state) {
  if ( tr->states[src_state].state0_dec == dest_state )
    return '0';
  else if ( tr->states[src_state].state1_dec == dest_state )
    return '1';
  else
    return '\0';
};

// Getting the convolutional code of a bit sequence based on the encoders
char* get_convolutional_code (char* state, encoder* enc, unsigned int num_encoders) {
    if ( !is_bit_sequence(state) ) {
        fprintf(stderr, "ERROR: get_convolutional_code: %s is not a bit sequence (must only consist of 0 and 1)\n", state);
        return NULL;
    }

    unsigned int result_bit;

    static char code [16];

    for (int i=0; i<num_encoders; i++) {
        switch (enc[i].op) {
            case AND:
                result_bit = 1;
                for (int j=0; j<enc[i].num_bits; j++) {
                    result_bit &= (unsigned int)( state[ enc[i].bits[j] ] - ASCII_OFFSET );
                }
                break;

            case OR:
                result_bit = 0;
                for (int j=0; j<enc[i].num_bits; j++) {
                    result_bit |= (unsigned int)( state[ enc[i].bits[j] ] - ASCII_OFFSET );
                }
                break;

            case XOR:
                result_bit = 0;
                for (int j=0; j<enc[i].num_bits; j++) {
                    result_bit ^= (unsigned int)( state[ enc[i].bits[j] ] - ASCII_OFFSET );
                }
                break;

            case NAND:
                result_bit = 1;
                for (int j=0; j<enc[i].num_bits; j++) {
                    result_bit &= (unsigned int)( state[ enc[i].bits[j] ] - ASCII_OFFSET );
                }
                result_bit ^= 1;
                break;


            case NOR:
                result_bit = 0;
                for (int j=0; j<enc[i].num_bits; j++) {
                    result_bit |= (unsigned int)( state[ enc[i].bits[j] ] - ASCII_OFFSET );
                }
                result_bit ^= 1;
                break;

            case NXOR:
                result_bit = 0;
                for (int j=0; j<enc[i].num_bits; j++) {
                    result_bit ^= (unsigned int)( state[ enc[i].bits[j] ] - ASCII_OFFSET );
                }
                result_bit ^= 1;
                break;

            case NOT:
                result_bit = (unsigned int)((state[ enc[i].bits[0] ]-ASCII_OFFSET) ^ 1);
                break;

            case NON:
                result_bit = (unsigned int)(state[ enc[i].bits[0] ]-ASCII_OFFSET);
                break;
        }

        code[i] = (char) (result_bit+ASCII_OFFSET);
    }

    return code;
}


/*-------------------------------------------------------------------*/
//------------------------- USER FUNCTIONS --------------------------*/


// Creating a trellis from an array of encoders and a push_bit function
//  - tr:            Pointer to the trellis to be created
//  - num_bits:      Number of bits in the bit sequence
//  - num_encoders:  Number of encoders -> Determines the number of bits per bit for the
//                   convolutional encoding
//  - push_bit_func: Pointer to the function to push the bits into the shift register
//                   - push_bit_left
//                   - push_bit_right

void create_trellis (trellis* tr, unsigned int num_bits, encoder* enc, unsigned int num_encoders, int (*push_bit_func)(char*, unsigned int)) {
    unsigned int highest_number = power(2, num_bits);
    tr->num_states = highest_number;

    tr->states = (trellis_state*) malloc( sizeof(trellis_state) * highest_number );

    for (int i=0; i<=highest_number; i++) {
        tr->states[i].state  = (char*) malloc( num_bits + 1 );
        tr->states[i].state0 = (char*) malloc( num_bits + 1 );
        tr->states[i].state1 = (char*) malloc( num_bits + 1 );
        tr->states[i].code0  = (char*) malloc( num_encoders + 1 );
        tr->states[i].code1  = (char*) malloc( num_encoders + 1 );
    }


    for (int i=0; i<highest_number; i++) {
        string_copy( tr->states[i].state, dec_to_bin(i, num_bits) );
        string_copy( tr->states[i].state0, tr->states[i].state );
        string_copy( tr->states[i].state1, tr->states[i].state );

        push_bit_func(tr->states[i].state0, 0);
        push_bit_func(tr->states[i].state1, 1);

        strcpy( tr->states[i].code0, get_convolutional_code(tr->states[i].state0, enc, num_encoders) );
        strcpy( tr->states[i].code1, get_convolutional_code(tr->states[i].state1, enc, num_encoders) );

        tr->states[i].state0_dec = bin_to_dec( tr->states[i].state0 );
        tr->states[i].state1_dec = bin_to_dec( tr->states[i].state1 );

        tr->states[i].code0_dec = bin_to_dec( tr->states[i].code0 );
        tr->states[i].code1_dec = bin_to_dec( tr->states[i].code1 );
    }

    tr->state_length = num_bits;
    tr->code_length = num_encoders;
}


// Decoding a convolutional code using the Viterbi algorithm
//  - code: Bit sequence to be decoded
//  - tr:   Pointer to the trellis to be used for decoding

viterbi_result* viterbi_decode (char* code, trellis* tr) {
    if ( !is_bit_sequence(code) ) {
        fprintf(stderr, "ERROR: viterbi_decode: %s is not a bit sequence (must of consist of 0 and 1)\n", code);
        return NULL;
    }

    unsigned int code_length = strlen(code);
    unsigned int num_code_segments = code_length / tr->code_length;
    unsigned int num_states = power(2, tr->state_length);


    viterbi_node** viterbi_grid;
    viterbi_grid = (viterbi_node**) malloc( (num_code_segments+1) * sizeof(viterbi_node*) );
    for (int i=0; i<num_code_segments+1; i++)
        viterbi_grid[i] = (viterbi_node*) malloc( num_states * sizeof(viterbi_node) );

    for (int i=0; i<=num_code_segments; i++) {
        for (int j=0; j<num_states; j++) {
            viterbi_grid[i][j].weight = INT_MAX;
            viterbi_grid[i][j].father = NULL;
            viterbi_grid[i][j].state_dec = j;
        }
    }

    for (int i=0; i<num_states; i++)
        viterbi_grid[0][i].weight = 0;

    char* current_code = (char*) malloc(tr->state_length);


    for (int i=0; i<num_code_segments; i++) {
        current_code = substring(code, i*tr->code_length, i*tr->code_length+tr->code_length-1);

        for (int j=0; j<num_states; j++) {
            int hamming_distance0, hamming_distance1;

            hamming_distance0 = hamming_distance( current_code, tr->states[j].code0 );
            hamming_distance1 = hamming_distance( current_code, tr->states[j].code1 );

            if ( viterbi_grid[i][j].weight + hamming_distance0 < viterbi_grid[i+1][ tr->states[j].state0_dec ].weight ) {
              viterbi_grid[i+1][ tr->states[j].state0_dec ].weight = viterbi_grid[i][j].weight + hamming_distance0;
              viterbi_grid[i+1][ tr->states[j].state0_dec ].father = &viterbi_grid[i][j];
            }

            if ( viterbi_grid[i][j].weight + hamming_distance1 < viterbi_grid[i+1][ tr->states[j].state1_dec ].weight ) {
              viterbi_grid[i+1][ tr->states[j].state1_dec ].weight = viterbi_grid[i][j].weight + hamming_distance1;
              viterbi_grid[i+1][ tr->states[j].state1_dec ].father = &viterbi_grid[i][j];
            }
        }
    }


    int smallest_weight = INT_MAX;
    int smallest_weight_state = 0;

    for (int i=0; i<num_states; i++) {
      if ( viterbi_grid[num_code_segments][i].weight < smallest_weight ) {
        smallest_weight = viterbi_grid[num_code_segments][i].weight;
        smallest_weight_state = i;
      }
    }

    viterbi_node* vertex = &viterbi_grid[num_code_segments][smallest_weight_state];

    char* viterbi_decoded_seq = (char*) malloc(num_code_segments);
    int viterbi_decoded_seq_it = 0;

    while ( vertex->father != NULL ) {
      viterbi_decoded_seq[viterbi_decoded_seq_it] = get_register_input( tr, vertex->father->state_dec, vertex->state_dec );
      vertex = vertex->father;
      viterbi_decoded_seq_it++;
    }

    static viterbi_result res;
    res.result = viterbi_decoded_seq;
    res.weight = smallest_weight;

    for (int i=0; i<=num_code_segments; i++)
        free(viterbi_grid[i]);
    free(viterbi_grid);

    return &res;
}

// Encoding a bit sequence with a convolutional code
//  - seq:           Bit sequence to be encoded
//  - enc:           Array of encoders
//  - num_encoders:  Number of encoders -> Determines the number of bits per per encoded bit
//  - push_bit_func: Pointer to the function to push the bits into the shift register
//                   - push_bit_left
//                   - push_bit_right

char* convolutional_encode (char* seq, encoder* enc, int num_encoders, int state_length, int(*push_bit_func)(char*, unsigned int)) {
    if ( !is_bit_sequence(seq) ) {
        fprintf(stderr, "ERROR: convolutional_encode: %s is not a bit sequence (must of consist of 0 and 1)\n", seq);
        return NULL;
    }

    char* current_seq = (char*) malloc(num_encoders+1);
    for (int i=0; i<state_length; i++)
        current_seq[i] = '0';

    int seq_length = strlen(seq);

    char* push_seq;
    char* encoded_seq = (char*) malloc( seq_length * num_encoders + 1 );

    int it = 0;

    if (push_bit_func == push_bit_left) {
        for (int i=seq_length-1; i>=0; i--) {

            push_bit_func(current_seq, seq[i]-ASCII_OFFSET);
            push_seq = get_convolutional_code(current_seq, enc, num_encoders);

            for (int j=0; j<num_encoders; j++) {
                encoded_seq[it*num_encoders+j] = push_seq[j];
            }
            it++;
        }
    }
    else {
        for (int i=0; i<seq_length; i++) {

            push_bit_func(current_seq, seq[i]-ASCII_OFFSET);
            push_seq = get_convolutional_code(current_seq, enc, num_encoders);

            for (int j=0; j<num_encoders; j++) {
                encoded_seq[it*num_encoders+j] = push_seq[j];
            }
            it++;
        }
    }

    return encoded_seq;
}


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

void create_encoder(encoder* enc, int operation, int num_bits, ...) {
    va_list args;

    enc->op = operation;
    enc->num_bits = num_bits;
    enc->bits = (int*) malloc( sizeof(int) * num_bits );

    va_start(args, num_bits);
    for (int i=0; i<num_bits; i++) {
        enc->bits[i] = va_arg(args, int);
    }
    va_end(args);
}

void print_trellis (trellis* tr) {
    for (int i=0; i<tr->num_states; i++) {
        printf("state: %s - state0: %s code0: %s - state1: %s code1: %s\n", tr->states[i].state, tr->states[i].state0, tr->states[i].code0, tr->states[i].state1, tr->states[i].code1);
    }
}
