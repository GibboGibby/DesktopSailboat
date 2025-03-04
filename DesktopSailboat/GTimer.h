#pragma once
#include <SDL3/SDL_timer.h>

class GTimer
{
public:
    //Initializes variables
    GTimer();

    //The various clock actions
    void Start();
    void Stop();
    void Pause();
    void Unpause();

    //Gets the timer's time
    Uint32 GetTicks();

    //Checks the status of the timer
    bool IsStarted();
    bool IsPaused();

private:
    //The clock time when the timer started
    Uint32 mStartTicks;

    //The ticks stored when the timer was paused
    Uint32 mPausedTicks;

    //The timer status
    bool mPaused;
    bool mStarted;
};