#ifndef MATMN_H
#define MATMN_H

#include "VecN.h"

struct MatMN {
    MatMN();
    MatMN(int m, int n);
    MatMN(const MatMN& other);
    ~MatMN();

    void Zero();
    MatMN Transpose() const;

    void operator=(const MatMN& other);

    VecN operator*(const VecN& v) const;
    MatMN operator*(const MatMN& v) const;

    static VecN GausseSeidel(const MatMN& lhs, const VecN& rhs);

    int m, n;
    VecN* rows;
};

#endif