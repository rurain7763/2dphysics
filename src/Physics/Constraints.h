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

protected:
    Body* _a;
    Body* _b;

    Vec2 _aPoint; // the anchor point in a's local space
    Vec2 _bPoint; // the anchor point in b's local space
};

class JointConstraint : public Constraints {
public:
    JointConstraint(Body* a, Body* b, const Vec2& anchor);

    virtual void Solve() override;

private:
    MatMN _jacobian;
};

class PenetrationConstraint : public Constraints {
public:
    virtual void Solve() override;

private:
    MatMN _jacobian;
};

#endif