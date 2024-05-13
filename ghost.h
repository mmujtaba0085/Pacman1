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
        if(dis - 0.3f> magnitude((x)-1,y))
        {
            std::cout<<"left"<<std::endl;
        dis = magnitude(x-1,y);
        mov = LEFT;
        }
    }
    if(!isWall(UPWARD,x,y) && prevGhostMov!=DOWNWARD)
    {
        if(dis - 0.3f> magnitude(x,y+1))
        {
             std::cout<<"up"<<std::endl;
        dis = magnitude(x,y+1);
        mov = UPWARD;
        }
    }
    if(!isWall(RIGHT,x,y) &&  prevGhostMov!=LEFT)
    {
        if(dis - 0.3f> magnitude(x+1,y))
        {
             std::cout<<"right"<<std::endl;
        dis = magnitude(x+1,y);
        mov = RIGHT;
        }
    }
    if(!isWall(DOWNWARD,x,y) && prevGhostMov!=UPWARD)
    {
        if(dis - 0.3f > magnitude(x,y-1))
        {
             std::cout<<"down"<<std::endl;
        dis = magnitude(x,y-1);
        mov= DOWNWARD;
        }
    }
    std::cout<<mov<<" "<<dis<<std::endl;
    return mov;
    }
    else{
            std::cout<<"here"<<std::endl;
            //  if (isWall(prevGhostMov,x,y)) { // Checks whether if it were to keep moving in it current direction if it would hit a wall
                if (!isWall(LEFT,x,y) &&  prevGhostMov != RIGHT) { // Checks its new direction wouldnt make it hit a wall AND that its not reversing direction
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
    

    while(1)
    {
        direc mov=movGhost(ghost->redX,ghost->redY,ghost->redmov,change_dir);
        ghost->redmov=mov;
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