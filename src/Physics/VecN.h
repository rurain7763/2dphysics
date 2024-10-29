#ifndef VECN_H
#define VECN_H

struct VecN {
    VecN();
    VecN(int n);
    VecN(const VecN& other);
    ~VecN();

    void Zero();

    float Dot(const VecN& v) const;

    VecN& operator=(const VecN& other);

    VecN operator+(const VecN& v) const;
    VecN operator-(const VecN& v) const;
    VecN operator*(const float v) const;

    VecN& operator+=(const VecN& v);
    VecN& operator-=(const VecN& v);
    VecN& operator*=(const float v);

    float operator[](const int idx) const;
    float& operator[](const int idx);

    int n;
    float* data;
};

#endif