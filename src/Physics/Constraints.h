#ifndef CONSTRAINTS_H
#define CONSTRAINTS_H

#include "Body.h"

class Constraints {
public:
    void Solve();
    //MatMN GetInvM();

    Body* a;
    Body* b;
};

#endif