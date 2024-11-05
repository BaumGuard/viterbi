# viterbi
This is a small **convolutional encoder** and **Viterbi decoder** written just for fun and for learning purposes.

## Usage
Include the header file `viterbi.h` in your **C** or **C++** program:
```C
#include "viterbi.h"
```

The header file already provides a good documentation of the functions and data structures. So here is a brief introduction.

## General

### Bit sequences
Bit sequences are represented as strings, e.g. `"010011"`

### Encoders
The essence of convolutional codes is a shift register into which bits of a bit sequence are being pushed sequentially and one at a time.<br />
An **Encoder** selects certain bits defined by the user from the shift register and performs a logical operation on them. The bits from the encoder are then being concatenated into a new string which is the current convolutional code.<br />
Therefore, an encoder is defined by
- A **logical operation**
- A **series of bit indexes**

### Trellis
A **Trellis** is a diagram  that shows to which state a certain state transitions if a 0 or a 1 is being push into the shift register and which resulting convolutional code it outputs during a transition.<br />
Therefore, a trellis is defined by
- The **number of bits** in a state
- A **series of encoders** (The number of encoders determines the length of the convolutional code being outputted)
- A **push function** (This determines whether the bits are being pushed into the shift array from the left-hand side (most significant bit) or from the right-hand side (least significant bit)

## Getting started
In order to perform **convolutional encoding** and **Viterbi decoding**, you first have to do the following things in order:
1. Create a **series of encoders**
2. Create and initialize a **trellis**

These steps are also described and illustrated in the file `example.c`.

### Encoder
First, create an array of encoders:
```C
encoder encs [<number of encoders>];
```
Then initialize them using the function `create_encoder`.
<table>
  <tr>
    <th>Return type</th>
    <td><code>void</code></td>
  </tr>
  <tr>
    <th>Parameters</th>
    <td>
      <ul>
        <li><code>encoder* enc</code>: Array of encoders</li>
        <li><code>int operation</code>: Logical operation (defined in <code>enum operations</code>)</li>
        <li><code>int num_bits</code>: Number of bit indexes</li>
        <li><code>...</code>: As many bit indexes as <code>int</code> as you have defined in <code>num_bits</code></li>
      </ul>
    </td>
  </tr>
</table>


### Trellis
Then we can create a trellis:
```C
trellis t;
```
Now we can use the function `create_trellis` to initialize the trellis.
<table>
  <tr>
    <th>Return type</th>
    <td><code>void</code></td>
  </tr>
  <tr>
    <th>Parameters</th>
    <td>
      <ul>
        <li><code>trellis* tr</code>: Pointer to the trellis</li>
        <li><code>int num_bits</code>: Number of bits in the shift register</li>
        <li><code>encoder* enc</code>: Pointer to the array of encoders</li>
        <li><code>int num_encoders</code>: Array length of <code>enc</code></li>
        <li><code>int (*push_bit_func)(char*, unsigned int)</code>: Pointer to the push bit function</li>
      </ul>
    </td>
  </tr>
</table>

### Convolutional encoding
You can encode a bit sequence using the function `convolutional_encode`.
<table>
  <tr>
    <th>Return type</th>
    <td><code>char*</code>: Encoded bit sequence</td>
  </tr>
  <tr>
    <th>Parameters</th>
    <td>
      <ul>
        <li><code>char* seq</code>: Raw bit sequence to encode</li>
        <li><code>encoder* enc</code>: Array of encoders</li>
        <li><code>int num_encoders</code>: Number of encoders in the array <code>enc</code></li>
        <li><code>int state_length</code>: Number of bits in the shift register</li>
        <li><code>int(*push_bit_func)(char*, unsigned int)</code>: Pointer to the push bit function</li>
      </ul>
    </td>
  </tr>
</table>

### Viterbi decoding
You can decode a bit sequence using the function `viterbi_decode`.
<table>
  <tr>
    <th>Return type</th>
    <td><code>viterbi_result*</code>: Decoded bit sequence</td>
  </tr>
  <tr>
    <th>Parameters</th>
    <td>
      <ul>
        <li><code>char* code</code>: Convolutional code</li>
        <li><code>trellis* tr</code>: Pointer to the trellis</li>
      </ul>
    </td>
  </tr>
</table>

The function `viterbi_decode` does not only return the decoded bit sequence, but also the weight of the last node in the Viterbi decoder grid. This indicates if the convolutional code contains errors and if the decoded sequence is correct:
<table>
  <tr>
    <th>Weight</th>
    <th>Meaning</th>
  </tr>
  <tr>
    <td>= 0</td>
    <td>The convolutional code does not contain errors and the decoded bit sequence is correct with a certainty of 100 %</td>
  </tr>
  <tr>
    <td>> 0</td>
    <td>The convolutional code contains errors and the certainty of the decoded bit sequence being correct decreases with the weight increasing</td>
  </tr>
</table>
