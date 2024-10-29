#include "MatMN.h"

MatMN::MatMN() : m(0), n(0), rows(nullptr) {}

MatMN::MatMN(int m, int n) 
    : m(m), n(n)
{
    rows = new VecN[m];
    for(int i = 0; i < m; i++) {
        rows[i] = VecN(n);
    }
}

MatMN::MatMN(const MatMN& other) {
    *this = other;
}

MatMN::~MatMN() {
    delete[] rows;
}

void MatMN::Zero() {
    for(int i = 0; i < m; i++) {
        rows[i].Zero();
    }
}

MatMN MatMN::Transpose() const {
    MatMN ret(n, m);
    for(int i = 0; i < m; i++) {
        for(int j = 0; j < n; j++) {
            ret.rows[j][i]= rows[i][j];
        }
    }
    return ret;
}

MatMN& MatMN::operator=(const MatMN& other) {
    if(rows) {
        delete[] rows;
    }

    m = other.m;
    n = other.n;
    rows = new VecN[m];
    for(int i = 0; i < m; i++) {
        rows[i] = other.rows[i];
    } 

    return *this;
}

VecN MatMN::operator*(const VecN& v) const {
    VecN ret(m);

    for(int i = 0; i < m; i++) {
        ret.data[i] = 0.f;
        for(int j = 0; j < n; j++) {
            ret.data[i] += rows[i][j] * v.data[j];
        }
    }

    return ret;
}

MatMN MatMN::operator*(const MatMN& v) const {
    MatMN ret(m, v.n);

    for(int i = 0; i < m; i++) {
        for(int j = 0; j < v.n; j++) {
            ret.rows[i][j] = 0.f;
            for(int k = 0; k < n; k++) {
                ret.rows[i][j] += rows[i][k] * v.rows[k][j];
            }
        }
    }
    
    return ret;
}


