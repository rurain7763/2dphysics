#include "Constraint.h"

#include <iostream>

MatMN Constraint::GetInvMassMat() const {
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

VecN Constraint::GetVelocities() const {
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
    : _jacobian(1, 6), _cachedLambda(1), _bias(0.f)
{
    _a = a;
    _b = b;
    _aPoint = a->WorldToLocal(anchor);
    _bPoint = b->WorldToLocal(anchor);
    _cachedLambda.Zero();
}

void JointConstraint::PreSolve(float dt) {
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

    // Calculate bias
    const float beta = 0.1f;
    float C = (pa - pb).Dot(pa - pb);
    C = std::max(0.f, C - 0.01f);
    _bias = (beta / dt) * C;
}

void JointConstraint::Solve() {
    // Calculate lambda = -(jacobian * V + bias) / (jacobian * invM * jacobian^T)
    const VecN V = GetVelocities();
    const MatMN invM = GetInvMassMat();
    const MatMN jacobianT = _jacobian.Transpose();

    // Ax = b
    MatMN lhs = _jacobian * invM * jacobianT;
    VecN rhs = _jacobian * V * -1.f;
    rhs.data[0] -= _bias;

    VecN lambda = MatMN::GausseSeidel(lhs, rhs);
    _cachedLambda += lambda;

    // Compute final impulses with direction and magnitude
    VecN impulses = jacobianT * lambda;

    // Apply lambda to the bodies
    _a->ApplyImpulseLinear(Vec2(impulses.data[0], impulses.data[1]));
    _a->ApplyImpulseAngular(impulses.data[2]);

    _b->ApplyImpulseLinear(Vec2(impulses.data[3], impulses.data[4]));
    _b->ApplyImpulseAngular(impulses.data[5]);
}

PenetrationConstraint::PenetrationConstraint(Body* a, Body* b, const Vec2& aContact, const Vec2& bContact, const Vec2& normal)
    : _jacobian(1, 6), _cachedLambda(1), _bias(0.f)
{
    _a = a;
    _b = b;
    _aPoint = a->WorldToLocal(aContact);
    _bPoint = b->WorldToLocal(bContact);
    _normal = a->WorldToLocal(normal);
    _cachedLambda.Zero();
}

void PenetrationConstraint::PreSolve(float dt) {
    // Get the collision points in world space
    const Vec2 pa = _a->LocalToWorld(_aPoint);
    const Vec2 pb = _b->LocalToWorld(_bPoint);
    Vec2 n = _a->LocalToWorld(_normal);

    const Vec2 ra = pa - _a->position;
    const Vec2 rb = pb - _b->position;

    _jacobian.Zero();

    Vec2 J1 = -n;
    _jacobian.rows[0][0] = J1.x; // A linear velocity.x
    _jacobian.rows[0][1] = J1.y; // A linear velocity.y

    float J2 = -ra.Cross(n);
    _jacobian.rows[0][2] = J2;   // A angular velocity

    Vec2 J3 = n;
    _jacobian.rows[0][3] = J3.x; // B linear velocity.x
    _jacobian.rows[0][4] = J3.y; // B linear velocity.y

    float J4 = rb.Cross(n);
    _jacobian.rows[0][5] = J4;   // B angular velocity

    // Warm starting (apply cached lambda)
    const MatMN Jt = _jacobian.Transpose();
    VecN impulses = Jt * _cachedLambda;

    // Apply the impulses to both bodies 
    _a->ApplyImpulseLinear(Vec2(impulses[0], impulses[1])); // A linear impulse
    _a->ApplyImpulseAngular(impulses[2]);                   // A angular impulse
    _b->ApplyImpulseLinear(Vec2(impulses[3], impulses[4])); // B linear impulse
    _b->ApplyImpulseAngular(impulses[5]);                   // B angular impulse

    // Compute the bias term (baumgarte stabilization)
    const float beta = 0.2f;
    float C = (pb - pa).Dot(-n);
    C = std::min(0.0f, C + 0.01f);
    _bias = (beta / dt) * C;
}

void PenetrationConstraint::Solve() {
    const VecN V = GetVelocities();
    const MatMN invM = GetInvMassMat();
  
    const MatMN J = _jacobian;
    const MatMN Jt = _jacobian.Transpose();
    
    // Compute lambda using Ax=b (Gauss-Seidel method) 
    MatMN lhs = J * invM * Jt;  // A
    VecN rhs = J * V * -1.0f;   // b
    rhs[0] -= _bias;
    VecN lambda = MatMN::GausseSeidel(lhs, rhs);
    
    // Accumulate impulses and clamp it within constraint limits
    VecN oldLambda = _cachedLambda;
    _cachedLambda += lambda;
    _cachedLambda[0] = (_cachedLambda[0] < 0.0f) ? 0.0f : _cachedLambda[0];
    lambda = _cachedLambda - oldLambda;

    // Compute the impulses with both direction and magnitude
    VecN impulses = Jt * lambda;

    // Apply the impulses to both bodies 
    _a->ApplyImpulseLinear(Vec2(impulses[0], impulses[1])); // A linear impulse
    _a->ApplyImpulseAngular(impulses[2]);                   // A angular impulse
    _b->ApplyImpulseLinear(Vec2(impulses[3], impulses[4])); // B linear impulse
    _b->ApplyImpulseAngular(impulses[5]);                   // B angular impulse
}