#include "Constraints.h"

MatMN Constraints::GetInvMassMat() const {
    MatMN ret(6, 6);
    ret.Zero();

    ret.rows[0][0] = _a->invMass;
    ret.rows[1][1] = _a->invMass;
    ret.rows[2][2] = _a->invI;

    ret.rows[3][3] = _b->invMass;
    ret.rows[4][4] = _b->invMass;
    ret.rows[5][5] = _b->invI;

    return ret;
}

VecN Constraints::GetVelocities() const {
    VecN ret(6);

    ret.data[0] = _a->velocity.x;
    ret.data[1] = _a->velocity.y;
    ret.data[2] = _a->angularVelocity;

    ret.data[3] = _b->velocity.x;
    ret.data[4] = _b->velocity.y;
    ret.data[5] = _b->angularVelocity;

    return ret;
}

JointConstraint::JointConstraint(Body* a, Body* b, const Vec2& anchor) 
    : _jacobian(1, 6)
{
    _a = a;
    _b = b;
    _aPoint = a->WorldToLocal(anchor);
    _bPoint = b->WorldToLocal(anchor);
}

void JointConstraint::Solve() {
    // Calculate the jacobian
    // V = GetVelocities();
    // invM = GetInvMassMat();
    // Calculate lambda = -jacobian * V / (jacobian * invM * jacobian^T)
    // Apply lambda to the bodies
}