#ifndef CONSTRAINTS_H
#define CONSTRAINTS_H

#include "Body.h"
#include "MatMN.h"

class Constraints {
public:
    virtual ~Constraints() = default;

    MatMN GetInvMassMat() const;
    VecN GetVelocities() const;

    virtual void PreSolve() {}
    virtual void Solve() = 0;
    virtual void PostSolve() {}

    inline Body* GetBodyA() const { return _a; }
    inline Body* GetBodyB() const { return _b; }

protected:
    Body* _a;
    Body* _b;

    Vec2 _aPoint; // the anchor point in a's local space
    Vec2 _bPoint; // the anchor point in b's local space
};

class JointConstraint : public Constraints {
public:
    JointConstraint(Body* a, Body* b, const Vec2& anchor);

    virtual void PreSolve() override;
    virtual void Solve() override;
    virtual void PostSolve() override;

private:
    MatMN _jacobian;
    VecN _cachedLambda;
};

class PenetrationConstraint : public Constraints {
public:
    virtual void Solve() override;

private:
    MatMN _jacobian;
};

#endif