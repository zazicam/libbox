#pragma once

#include <vector>
#include <climits>

namespace box {
    class Random
    {
    public:
        void RandomizeTimer(); 
        int Int(int a, int b);
        float Float(float a, float b);
        
        template<typename T>
        T choice(std::vector<T> options) {
            int numOptions = options.size() % INT_MAX;
            int index = Int(0, numOptions - 1);
            return options.at(index);
        }
    };
    
    extern Random random;
}

