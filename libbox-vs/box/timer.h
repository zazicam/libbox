#pragma once

#include <chrono>

namespace box {
    
    class Timer
    {
    		std::chrono::steady_clock::time_point start_time;
    	public:
    		Timer();
    
    		int Elapsed() const;
    		void Reset(); 
    		int SyncLoopTo(int ms); 
    		void Wait(int ms);
    };
    
    extern Timer timer;
}


