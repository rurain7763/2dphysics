#include "Constraints.h"

MatMN Constraints::GetInvMassMat() const {
    MatMN ret(6, 6);
    ret.Zero();

    ret.rows[0][0] = a->invMass;
    ret.rows[1][1] = a->invMass;
    ret.rows[2][2] = a->invI;

    ret.rows[3][3] = b->invMass;
    ret.rows[4][4] = b->invMass;
    ret.rows[5][5] = b->invI;

    return ret;
}

VecN Constraints::GetVelocities() const {
    VecN ret(6);

    ret.data[0] = a->velocity.x;
    ret.data[1] = a->velocity.y;
    ret.data[2] = a->angularVelocity;

    ret.data[3] = b->velocity.x;
    ret.data[4] = b->velocity.y;
    ret.data[5] = b->angularVelocity;

    return ret;
}