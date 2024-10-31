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
    : _jacobian(1, 6), _cachedLambda(6)
{
    _a = a;
    _b = b;
    _aPoint = a->WorldToLocal(anchor);
    _bPoint = b->WorldToLocal(anchor);
    _cachedLambda.Zero();
}

void JointConstraint::PreSolve() {
    // Calculate the jacobian
    const Vec2 pa = _a->LocalToWorld(_aPoint);
    const Vec2 pb = _b->LocalToWorld(_bPoint);

    const Vec2 ra = pa - _a->position;
    const Vec2 rb = pb - _b->position;

    _jacobian.Zero();

    Vec2 j1 = (pa - pb) * 2.f;
    _jacobian.rows[0][0] = j1.x; // linear x
    _jacobian.rows[0][1] = j1.y; // linear y

    float j2 = ra.Cross(pa - pb) * 2.f;
    _jacobian.rows[0][2] = j2; // angular

    Vec2 j3 = (pb - pa) * 2.f;
    _jacobian.rows[0][3] = j3.x; // linear x
    _jacobian.rows[0][4] = j3.y; // linear y

    float j4 = rb.Cross(pb - pa) * 2.f;
    _jacobian.rows[0][5] = j4; // angular

    // Warm star   
    const MatMN jacobianT = _jacobian.Transpose();
    VecN impulses = jacobianT * _cachedLambda;

    // Apply lambda to the bodies
    _a->ApplyImpulseLinear(Vec2(impulses.data[0], impulses.data[1]));
    _a->ApplyImpulseAngular(impulses.data[2]);

    _b->ApplyImpulseLinear(Vec2(impulses.data[3], impulses.data[4]));
    _b->ApplyImpulseAngular(impulses.data[5]);
}

void JointConstraint::Solve() {
    // Calculate lambda = -(jacobian * V + bias) / (jacobian * invM * jacobian^T)
    const VecN V = GetVelocities();
    const MatMN invM = GetInvMassMat();
    const MatMN jacobianT = _jacobian.Transpose();

    // Ax = b
    MatMN lhs = _jacobian * invM * jacobianT;
    VecN rhs = (_jacobian * V) * -1.f;

    VecN lambda = MatMN::GauseSeidel(lhs, rhs);
    _cachedLambda += lambda;

    // Compute final impulses with direction and magnitude
    VecN impulses = jacobianT * lambda;

    // Apply lambda to the bodies
    _a->ApplyImpulseLinear(Vec2(impulses.data[0], impulses.data[1]));
    _a->ApplyImpulseAngular(impulses.data[2]);

    _b->ApplyImpulseLinear(Vec2(impulses.data[3], impulses.data[4]));
    _b->ApplyImpulseAngular(impulses.data[5]);
}

void JointConstraint::PostSolve() {
    _cachedLambda.Zero();
}