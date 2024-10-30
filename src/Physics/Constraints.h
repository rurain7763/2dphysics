#ifndef CONSTRAINTS_H
#define CONSTRAINTS_H

#include "Body.h"
#include "MatMN.h"

class Constraints {
public:
    virtual ~Constraints() = default;

    MatMN GetInvMassMat() const;
    VecN GetVelocities() const;

    virtual void Solve() = 0;

    Body* a;
    Body* b;
};

class DistanceConstraint : public Constraints {
public:
    virtual void Solve() override;
};

class PenetrationConstraint : public Constraints {
public:
    virtual void Solve() override;
};

#endif