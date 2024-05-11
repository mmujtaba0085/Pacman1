#ifndef GHOST_H
#define GHOST_H


typedef enum{Follow,Run,Box}motion;

class Ghost
{
public:
    float redX;
    float redY;
    motion move_type;
    direc redmov;
    float speed;


    Ghost(float x=0, float y=0)
    {
        redX=x;
        redY=y;
        move_type=Box;
        speed=5.0f;
    }
    friend void* mov(void* ghost); // Declare Pacmove as a friend function
    void draw()
    {   
        glPushMatrix();
        unsigned int pac_tex;    
        pac_tex = pac_1_tex;
        translateBottomLeft(); // Translate to bottom left of screen which is (0,0)
        translateToMazeCoords(redX, redY);

        glTranslatef(-3.0f, -3.0f, 0.0f); // Translate to take into account pacmans size

        drawTex(pac_tex, 14, 14, 0);

        glPopMatrix();
    }

};
float magnitude(float x,float y)
{
    return (x*x)+(y*y);
}

direc movGhost(float x,float y,direc& prevGhostMov)
{
    float pacMag=magnitude(X_cord,Y_cord);
    float dis=pacMag - magnitude(x,y);
    direc mov = STILL;

    if(!isWall(LEFT,x,y) && abs(x-X_cord - 1) < abs(x-X_cord) )
    {
        mov = LEFT;
        dis = (pacMag - magnitude(x-1,y));
    }
    if(!isWall(RIGHT,x,y) && abs(x-X_cord + 1) < abs(x-X_cord) )
    {
        mov = RIGHT;
        dis = (pacMag - magnitude(x+1,y));
    }
    if(!isWall(UPWARD,x,y) && abs(y-Y_cord+1)<abs(y-Y_cord))
    {
        mov = UPWARD;
        dis = abs(pacMag - magnitude(x,y+1));
    }
    if(!isWall(DOWNWARD,x,y) && abs(y-Y_cord-1)<abs(y-Y_cord))
    {
        mov = DOWNWARD;
        dis = (pacMag - magnitude(x,y-1));
    }
    if(mov==STILL && isWall(prevGhostMov,x,y))
    {
        std::cout<<"aa"<<std::endl;
        if(prevGhostMov==LEFT)
        {
            mov=UPWARD;
        }
        else if(prevGhostMov==UPWARD)
        {
            mov=DOWNWARD;
        }
        else if(prevGhostMov==DOWNWARD)
        {
            mov=RIGHT;
        }
        else if(prevGhostMov==RIGHT)
        {
            mov=LEFT;
        }
        
    }
    prevGhostMov=mov;
    return mov;
    
}
void* Redmov(void* ghost_void)
{
    Ghost*  ghost = static_cast<Ghost*>(ghost_void);

    

    while(1)
    {
        direc mov=movGhost(ghost->redX,ghost->redY,ghost->redmov);

        switch (mov)
        {
        case LEFT:
            ghost->redX -= 0.1f;
            ghost->redY = round(ghost->redY);
        break;
        case RIGHT:
            ghost->redX += 0.1f;
            ghost->redY = round(ghost->redY);
        break;    
        case UPWARD:
            ghost->redX=round(ghost->redX);
            ghost->redY+=0.1f;
        break;
        case DOWNWARD:
            ghost->redX=round(ghost->redX);
            ghost->redY-=0.1f;
        break;
        }
        std::cout<<X_cord<<" "<<Y_cord<<" "<<ghost->redX<<" "<<ghost->redY<<" "<<magnitude(ghost->redX,ghost->redY)<<std::endl;
        usleep(15000);

    }
    pthread_exit(NULL);
}

#endif
