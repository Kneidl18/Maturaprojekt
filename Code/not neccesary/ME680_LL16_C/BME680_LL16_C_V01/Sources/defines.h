#include <hidef.h>          /* for EnableInterrupts macro */
#include "derivative.h"
#include "MC9S08LL16.h"


/* MAKRO BEFEHLE */
#define Nop()               __asm nop