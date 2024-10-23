#include "Contact.h"

void Contact::ResolvePenetration() {
    if(a->IsStatic() && b->IsStatic()) return;

    const float da = depth / (a->invMass + b->invMass) * a->invMass;
    const float db = depth / (a->invMass + b->invMass) * b->invMass;

    a->position -= normal * da;
    b->position += normal * db; 
}

void Contact::ResolveCollision() {
    ResolvePenetration();

    // 두 물체 중에 반발력이 작은 값을 선택한다. (진흙공과 고무공이 부딫치면 반발력은 진흙공을 따라간다)
    float e = std::min(a->restitution, b->restitution);

    // 두 물체 사이의 상대 속도. (a가 바라본 b의 속도라고 생각하면 될듯)
    const Vec2 vrel = a->velocity - b->velocity;

    // pikuma impulse & momentum 카테고리 영상 확인!
    const Vec2 impulseDir = normal;
    const float impulseMag = -(1.f + e) * vrel.Dot(normal) / (a->invMass + b->invMass);

    Vec2 impulse = impulseDir * impulseMag;

    a->ApplyImpulse(impulse);
    b->ApplyImpulse(-impulse);
}