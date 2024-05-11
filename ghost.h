#ifndef GHOST_H
#define GHOST_H


typedef enum{Follow,Run,Box}motion;

class Ghost
{
public:
    float X_cord;
    float Y_cord;
    motion move_type;
    float speed;

    Ghost(float x=0, float y=0)
    {
        X_cord=x;
        Y_cord=y;
        move_type=Box;
        speed=5.0f;
    }

};



#endif
