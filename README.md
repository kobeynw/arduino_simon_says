# Simon Says #
An Arduino project created with basic electronics components.

### Components ###
  1. Arduino Nano
  2. multi-colored LED's
  3. push buttons
  4. piezo buzzer (simple tone emitter)
  5. resistors

### Functionality ###
  - Button presses simultaneously light up the corresponding LED and emit a specific tone from the buzzer
  - The player presses buttons in the sequence that the LED's were lit until they press a button out of sequence or reach the max sequence length
  - Upon game over, the LED's flash five times and the game resets
  - Upon winning the game (entering the correct sequence until the max sequence length is reached), a melody is played

<img src="https://github.com/kobeynw/arduino_simon_says/blob/master/simon_says.png" width="50%">
