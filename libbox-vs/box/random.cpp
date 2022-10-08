#include "random.h"

#include <cstdlib>
#include <ctime>

namespace box {

    Random random;

    void Random::RandomizeTimer() {
        srand(static_cast<unsigned int>(time(0)));
    }   
    
    int Random::Int(int a, int b) {
        return rand()%(b-a+1)+a;
    }
    
    float Random::Float(float a, float b) {
        return a + (b-a) * rand() / RAND_MAX;
    }
}
