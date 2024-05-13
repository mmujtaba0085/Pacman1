#ifndef GHOST_H
#define GHOST_H


typedef enum{Follow,Run,Box}motion;

float initalRed_X=6.0f;
float initailRed_Y=16.0f;

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
        redmov=RIGHT;
    }
    friend void* mov(void* ghost); // Declare Pacmove as a friend function
    void draw()
    {   
        glPushMatrix();
        unsigned int redGhst_tex;    
        redGhst_tex = ghost_r_0_tex;
        translateBottomLeft(); // Translate to bottom left of screen which is (0,0)
        translateToMazeCoords(redX, redY);

        glTranslatef(-3.0f, -3.0f, 0.0f); // Translate to take into account pacmans size

        drawTex(redGhst_tex, 14, 14, 0);

        glPopMatrix();
    }

};
float magnitude(float x,float y)
{
    return sqrt((x-X_cord)*(x-X_cord)+(y-Y_cord)*(y-Y_cord));
}

int possible_exit(float x,float y)
{
    int count=0;
    if(!isWall(LEFT,x,y))
    {
        count++;
    }
    if(!isWall(RIGHT,x,y))
    {
        count++;
    }
    if(!isWall(UPWARD,x,y))
    {
        count++;
    }
    if(!isWall(DOWNWARD,x,y))
    {
        count++;
    }
    return count;
    
}

void WhoDied(float &x, float &y)
{
    if(round(x)==round(X_cord) && round(y)==round(Y_cord) && !dead)
    {
        lives--;
        dead=true;
        x=initalRed_X;
        y=initailRed_Y;
    }
}

direc movGhost(float &x,float &y,direc& prevGhostMov,bool change_dir)
{
    float pacMag=magnitude(X_cord,Y_cord);
    float dis=2000;
    direc mov = STILL;
    int count=0;


    if(getTile(x,y)==T)
    {
        if(prevGhostMov==LEFT)
        {
            x=26;
        }
        else if(prevGhostMov=RIGHT)
        {
            x=2;
        }
    }
    


    if(possible_exit(x,y)>2 && change_dir)
    {
       
    if(!isWall(LEFT,x,y) && prevGhostMov!=RIGHT )
    {
        if(dis > magnitude((x)-1,y))
        {
        dis = magnitude(x-1,y);
        mov = LEFT;
        }
    }
    if(!isWall(UPWARD,x,y) && prevGhostMov!=DOWNWARD)
    {
        if(dis > magnitude(x,y+1))
        {
        dis = magnitude(x,y+1);
        mov = UPWARD;
        }
    }
    if(!isWall(RIGHT,x,y) &&  prevGhostMov!=LEFT)
    {
        if(dis> magnitude(x+1,y))
        {
        dis = magnitude(x+1,y);
        mov = RIGHT;
        }
    }
    if(!isWall(DOWNWARD,x,y) && prevGhostMov!=UPWARD)
    {
        if(dis  > magnitude(x,y-1))
        {
        dis = magnitude(x,y-1);
        mov= DOWNWARD;
        }
    }
    return mov;
    }
    else{
           
            
                if (!isWall(LEFT,x,y) &&  prevGhostMov != RIGHT) { 
                    mov = LEFT;
                } else if (!isWall(UPWARD,x,y) && prevGhostMov != DOWNWARD) {
                    mov = UPWARD;
                } else if (!isWall(RIGHT,x,y) && prevGhostMov != LEFT) {
                    mov = RIGHT;
                } else {
                    mov = DOWNWARD;
                }
                change_dir=false;
        }
        if(mov==STILL && isWall(prevGhostMov,x,y))
        {
            change_dir=true;
            mov=movGhost(x,y,prevGhostMov,change_dir);
        }
        else if(!isWall(prevGhostMov,x,y))
        {
            return prevGhostMov;
        }
    return mov;
    
}
void* Redmov(void* ghost_void)
{
    Ghost*  ghost = static_cast<Ghost*>(ghost_void);
    bool change_dir=true;
    auto startTime = std::chrono::steady_clock::now();
    auto endTime = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsedSeconds;
    while(1)
    {
        endTime = std::chrono::steady_clock::now();
        elapsedSeconds = endTime - startTime;

        WhoDied(ghost->redX,ghost->redY);

        if (elapsedSeconds.count() >= 0.5 || isWall(ghost->redmov,ghost->redX,ghost->redY) || getTile(ghost->redX,ghost->redY)==T ) {
            // Run timed function
            direc mov=movGhost(ghost->redX,ghost->redY,ghost->redmov,change_dir);
            ghost->redmov=mov;
            // Reset start time
            startTime = std::chrono::steady_clock::now();
        }


      
       
        switch (ghost->redmov)
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
        default:
        std::cout<<X_cord<<" "<<Y_cord<<" "<<ghost->redX<<" "<<ghost->redY<<" "<<magnitude(ghost->redX,ghost->redY)<<std::endl;
            ghost->redX=round(ghost->redX);
            ghost->redY = round(ghost->redY);
        break;
        }
        
        usleep(15000);

    }
    pthread_exit(NULL);
}

#endif