#include "VecN.h"

VecN::VecN() : n(0), data(nullptr) {}

VecN::VecN(int n)
    : n(n)
{
    data = new float[n];
}

VecN::VecN(const VecN& other) {
    *this = other; 
}

VecN::~VecN() {
    delete[] data;
}

void VecN::Zero() {
    for(int i = 0; i < n; i++) {
        data[i] = 0.f;
    }
}

float VecN::Dot(const VecN& v) const {
    float ret = 0;
    for(int i = 0; i < n; i++) {
        ret += data[i] * v.data[i];
    }
    return ret;
}

VecN& VecN::operator=(const VecN& other) {
    if(data) {
        delete[] data;
    }

    n = other.n;
    data = new float[n];
    for(int i = 0; i < n; i++) {
        data[i] = other.data[i];
    }
    return *this;
}

VecN VecN::operator+(const VecN& v) const {
    VecN ret(n);
    for(int i = 0; i < n; i++) {
        ret.data[i] = data[i] + v.data[i];
    }
    return ret;
}

VecN VecN::operator-(const VecN& v) const {
    VecN ret(n);
    for(int i = 0; i < n; i++) {
        ret.data[i] = data[i] - v.data[i];
    }
    return ret;
}

VecN VecN::operator*(const float v) const {
    VecN ret(n);
    for(int i = 0; i < n; i++) {
        ret.data[i] = data[i] * v;
    }
    return ret;
}

VecN& VecN::operator+=(const VecN& v) {
    for(int i = 0; i < n; i++) {
        data[i] += v.data[i];
    }
    return *this;
}

VecN& VecN::operator-=(const VecN& v) {
    for(int i = 0; i < n; i++) {
        data[i] -= v.data[i];
    }
    return *this;
}

VecN& VecN::operator*=(const float v) {
    for(int i = 0; i < n; i++) {
        data[i] *= v;
    }
    return *this;
}

float VecN::operator[](const int idx) const {
    return data[idx];
}

float& VecN::operator[](const int idx) {
    return data[idx];
}