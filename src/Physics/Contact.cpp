#include "Contact.h"

void Contact::ResolvePenetration() {
    if(a->IsStatic() && b->IsStatic()) return;

    const float da = depth / (a->invMass + b->invMass) * a->invMass;
    const float db = depth / (a->invMass + b->invMass) * b->invMass;

    a->position -= normal * da * 0.8f;
    b->position += normal * db * 0.8f; 

    a->shape->UpdateVertices(a->position, a->rotation);
    b->shape->UpdateVertices(b->position, b->rotation);
}

void Contact::ResolveCollision() {
    ResolvePenetration();

    // 두 물체 중에 반발력이 작은 값을 선택한다. (진흙공과 고무공이 부딫치면 반발력은 진흙공을 따라간다)
    float e = std::min(a->restitution, b->restitution);
    float f = std::min(a->friction, b->friction);

    // 현재 2차원 engine이기 때문에 일반적인 cross product을 사용할 수 없다.
    // 따라서 가상의 (0, 0, w)의 가상 벡터와 (vra.x, vra.y, 0)의 cross product를 사용한다.
    // 결과는 아래와 같다.
    Vec2 ra = end - a->position;
    Vec2 rb = start - b->position;
    Vec2 va = a->velocity + Vec2(-a->angularVelocity * ra.y, a->angularVelocity * ra.x);
    Vec2 vb = b->velocity + Vec2(-b->angularVelocity * rb.y, b->angularVelocity * rb.x);

    // 두 물체 사이의 상대 속도. (a가 바라본 b의 속도라고 생각하면 될듯)
    const Vec2 vrel = va - vb;

    // pikuma impulse & momentum 카테고리 영상 확인!
    float vrelDotNormal = vrel.Dot(normal);
    const Vec2 impulseDirectionN = normal;
    const float impulseMagnitudeN = -(1 + e) * vrelDotNormal / ((a->invMass + b->invMass) + ra.Cross(normal) * ra.Cross(normal) * a->invI + rb.Cross(normal) * rb.Cross(normal) * b->invI);
    Vec2 jN = impulseDirectionN * impulseMagnitudeN;

    // friction impulse
    Vec2 tangent = normal.Normal();
    float vrelDotTangent = vrel.Dot(tangent);
    const Vec2 impulseDirectionT = tangent;
    const float impulseMagnitudeT = f * -(1 + e) * vrelDotTangent / ((a->invMass + b->invMass) + ra.Cross(tangent) * ra.Cross(tangent) * a->invI + rb.Cross(tangent) * rb.Cross(tangent) * b->invI);
    Vec2 jT = impulseDirectionT * impulseMagnitudeT;

    Vec2 totalImpulse = jN + jT;

    a->ApplyImpulse(totalImpulse, ra);
    b->ApplyImpulse(-totalImpulse, rb);
}