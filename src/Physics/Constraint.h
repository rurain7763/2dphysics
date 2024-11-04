#ifndef CONSTRAINTS_H
#define CONSTRAINTS_H

#include "Body.h"
#include "MatMN.h"

class Constraint {
public:
    virtual ~Constraint() = default;

    MatMN GetInvMassMat() const;
    VecN GetVelocities() const;

    virtual void PreSolve(float dt) {}
    virtual void Solve() = 0;
    virtual void PostSolve() {}

protected:
    Body* _a;
    Body* _b;

    Vec2 _aPoint; // the anchor point in a's local space
    Vec2 _bPoint; // the anchor point in b's local space
};

class JointConstraint : public Constraint {
public:
    JointConstraint(Body* a, Body* b, const Vec2& anchor);

    virtual void PreSolve(float dt) override;
    virtual void Solve() override;

private:
    MatMN _jacobian;
    VecN _cachedLambda;
    float _bias;
};

class PenetrationConstraint : public Constraint {
public:
    PenetrationConstraint(Body* a, Body* b, const Vec2& aContact, const Vec2& bContact, const Vec2& normal);

    virtual void PreSolve(float dt) override;
    virtual void Solve() override;

private:
    MatMN _jacobian;
    VecN _cachedLambda;
    float _bias;

    Vec2 _normal;
};

#endif