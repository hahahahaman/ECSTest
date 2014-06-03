#ifndef _FPS_H
#define _FPS_H

#include <ctime>
#include <chrono>
#include <ratio>


class FPS
{
public:
    static FPS FPSControl;

    FPS()
    :CurrentTime(std::chrono::steady_clock::now()),
    LastFrameTime(std::chrono::steady_clock::now()),
    PreviousSecond(std::chrono::steady_clock::now()),
    SpeedFactor(0.f),
    Frames(0),
    NumFrames(0)
    {}

    void    OnLoop()
    {
        CurrentTime = std::chrono::steady_clock::now();

        //if a second has passed
        if(std::chrono::duration_cast<std::chrono::duration<float>> (CurrentTime - PreviousSecond).count() > 1.0f )
        {
            //printf("***1 sec has passed***\n");
            PreviousSecond = CurrentTime;

            NumFrames = Frames;

            Frames = 0;
        }

        ++Frames;

        //Time for a single frame
        SpeedFactor = std::chrono::duration_cast<std::chrono::duration<float>> (CurrentTime - LastFrameTime).count();

        LastFrameTime = CurrentTime;
    }

    int     GetFPS() //returns the current FPS
    {
        return NumFrames;
    }

    float   GetSpeedFactor() //returns the time it takes for 1 loop to occur
    {
        return SpeedFactor;
    }

private:
    std::chrono::steady_clock::time_point    CurrentTime;
    std::chrono::steady_clock::time_point    LastFrameTime;
    std::chrono::steady_clock::time_point    PreviousSecond;

    float                       SpeedFactor;

    unsigned short              NumFrames;
    unsigned short              Frames;
};

FPS FPS::FPSControl;

#endif
