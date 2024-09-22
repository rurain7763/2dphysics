#include "Particle.h"
#include "iostream"

Particle::Particle(float x, float y, float mass) 
    : position(x, y), mass(mass)
{
    std::cout << "Particle constructor called!\n";
}

Particle::~Particle() {
    std::cout << "Particle destructor called!\n";
}