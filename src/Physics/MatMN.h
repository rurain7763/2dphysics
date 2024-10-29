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

    MatMN& operator=(const MatMN& other);

    VecN operator*(const VecN& v) const;
    MatMN operator*(const MatMN& v) const;

    int m, n;
    VecN* rows;
};

#endif