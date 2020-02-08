#ifndef LEDEBUG_HH_
#define LEDEBUG_HH_

// Print message and exit program
void panic();

// Panic if `cond` is false
void panic_ifn(int cond);

#endif //! LEDEBUG_HH_
