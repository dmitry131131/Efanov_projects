#include <stdio.h>

#include "function.h"
#include "monte_carlo.h"

int main() {

    Integral integral = {
                        .start    = 1.0,
                        .end      = 2.0,
                        .function = &function,
                        .result   = 0.0 };

    monte_carlo(&integral);

    return 0;
}