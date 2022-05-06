//-----------------------------------------------------------------------------
// Main js code
//-----------------------------------------------------------------------------

// main init function
let main_init = function () {
    // put board init code here
    console.log('program started.');
};

// main loop function
let main_loop = function () {
    // turn the light on
    board.set_led(1);
    // log to console
    console.log('led state 1');
    // delay 500 ms
    delay(1000);
    // turn the light off
    board.set_led(0);
    // log to console
    console.log('led state 0');
    // delay 500 ms
    delay(1000);
};

//-----------------------------------------------------------------------------
