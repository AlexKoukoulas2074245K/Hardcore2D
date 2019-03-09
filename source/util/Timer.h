//
//  Timer.h
//  Hardcore2D
//
//  Created by Alex Koukoulas on 09/03/2019.
//

#ifndef Timer_h
#define Timer_h

#include <functional>

class Timer final
{
public:
    using TimerTickCallback = std::function<void()>;
    
    Timer(const float period, TimerTickCallback timerTickCallback)
        : mPeriod(period)
        , mTimerTickCallback(timerTickCallback)
        , mIsRunning(true)
        , mTimeCounter(mPeriod)
    {
    }
    
    inline void Update(const float dt)
    {
        if (!mIsRunning)
        {
            return;
        }
        
        mTimeCounter -= dt;
        if (mTimeCounter <= 0.0f)
        {
            mTimeCounter = mPeriod;
            mTimerTickCallback();
        }
    }
    
    inline void Pause()
    {
        mIsRunning = false;
    }
    
    inline void Resume()
    {
        mIsRunning = true;
    }
    
private:
    const float mPeriod;
    const TimerTickCallback mTimerTickCallback;
    bool mIsRunning;
    float mTimeCounter;
};

#endif /* Timer_h */
