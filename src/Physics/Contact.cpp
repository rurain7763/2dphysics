#include "Contact.h"

void Contact::ResolvePenetration() {
    if(a->IsStatic() && b->IsStatic()) return;

    const float da = depth / (a->invMass + b->invMass) * a->invMass;
    const float db = depth / (a->invMass + b->invMass) * b->invMass;

    a->position -= normal * da;
    b->position += normal * db; 
}