#ifndef GHOST_H
#define GHOST_H


typedef enum{Follow,Run,Box,Dead}motion;
typedef enum{RED,BLUE}color;


float initalRed_X=13.5f;
float initailRed_Y=19.0f;
float initalBlue_X=11.5f;
float initailBlue_Y=16.0f;

float xx[4]{1.0f,26.0f,1.0f,26.0f};
float yy[4]{29.0f,1.0f,1.0f,29.0f};

bool GisWall(direc dir,float x, float y)
    {
        if (getFollowingTile(dir,x,y) == W ) {
            return true;
        }

        return false;
    }

class Ghost
{
public:
    float X;
    float Y;
    float originX;
    float originY;
    motion move_type;
    direc redmov;
    float speed;
    color ghost_col;

    Ghost(float x, float y, color col)
    {
        move_type=Box;
        speed=5.0f;
        redmov=RIGHT;
        originX=x;
        originY=y;
        X=x;
        Y=y;
        ghost_col=col;
    }
    friend void* mov(void* ghost); // Declare Pacmove as a friend function
    void draw()
    {   
        glPushMatrix();
        unsigned int Ghst_tex;    
        Ghst_tex = ghost_r_0_tex;
        translateBottomLeft(); // Translate to bottom left of screen which is (0,0)
        translateToMazeCoords(X, Y);

        glTranslatef(-3.0f, -3.0f, 0.0f); // Translate to take into account pacmans size
        if(ghost_col==RED)
        {
            Ghst_tex = ghost_r_0_tex;
        }
        else if(ghost_col==BLUE)
        {
            Ghst_tex = ghost_b_0_tex;
        }
        drawTex(Ghst_tex, 14, 14, 0);
        glPopMatrix();
    }

};
float disINBtwPnG(float x1,float y1,float x2,float y2)          //to calculate distance in between pacman and given Coordinates;
{
    return sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));
}

int nearestPosToPac()
{
    float dis=2000;
    int j=0;
    for(int i=0;i<4;i++)
    {
        float temp = disINBtwPnG(xx[i],yy[i],X_cord,Y_cord);
        
        if(temp<dis)
        {
            dis=temp;
            j=i;
        }
    }
    return j;
}

int possible_exit(float x,float y)
{
    int count=0;
    if(!GisWall(LEFT,x,y))
    {
        count++;
    }
    if(!GisWall(RIGHT,x,y))
    {
        count++;
    }
    if(!GisWall(UPWARD,x,y))
    {
        count++;
    }
    if(!GisWall(DOWNWARD,x,y))
    {
        count++;
    }
    return count;
    
}



void WhoDied(float &x, float &y)
{
    if(round(x)==round(X_cord) && round(y)==round(Y_cord) && !dead)
    {//if pac dies
        lives--;
        dead=true;
    }
}

direc movGhost(float &x,float &y,direc& prevGhostMov,bool change_dir,color col )
{
    float dis=2000;
    direc mov = STILL;
    int count=0;
    float x2,y2;

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
    
    if(col==RED)   
    {
        x2=X_cord;
        y2=Y_cord;
    }
    else if(col==BLUE)
    {
        int temp=nearestPosToPac();
        x2=xx[temp];
        y2=yy[temp];
    }
    if(possible_exit(x,y)>2 && change_dir)
    {
    
    if(!GisWall(LEFT,x,y) && prevGhostMov!=RIGHT )
    {
        if(dis > disINBtwPnG((x)-1,y,x2,y2))
        {
        dis = disINBtwPnG(x-1,y,x2,y2);
        mov = LEFT;
        }
    }
    if(!GisWall(UPWARD,x,y) && prevGhostMov!=DOWNWARD)
    {
        if(dis > disINBtwPnG(x,y+1,x2,y2))
        {
        dis = disINBtwPnG(x,y+1,x2,y2);
        mov = UPWARD;
        }
    }
    if(!GisWall(RIGHT,x,y) &&  prevGhostMov!=LEFT)
    {
        if(dis> disINBtwPnG(x+1,y,x2,y2))
        {
        dis = disINBtwPnG(x+1,y,x2,y2);
        mov = RIGHT;
        }
    }
    if(!GisWall(DOWNWARD,x,y) && prevGhostMov!=UPWARD)
    {
        if(dis  > disINBtwPnG(x,y-1,x2,y2))
        {
        dis = disINBtwPnG(x,y-1,x2,y2);
        mov= DOWNWARD;
        }
    }
    return mov;
    }
    else{
           
            
                if (!GisWall(LEFT,x,y) &&  prevGhostMov != RIGHT) { 
                    mov = LEFT;
                } else if (!GisWall(UPWARD,x,y) && prevGhostMov != DOWNWARD) {
                    mov = UPWARD;
                } else if (!GisWall(RIGHT,x,y) && prevGhostMov != LEFT) {
                    mov = RIGHT;
                } else {
                    mov = DOWNWARD;
                }
                change_dir=false;
        }
        if(mov==STILL && GisWall(prevGhostMov,x,y))
        {
            change_dir=true;
            return movGhost(x,y,prevGhostMov,change_dir,col);
        }
        if(!GisWall(prevGhostMov,x,y))
        {
            return prevGhostMov;
        }
    return mov;
    
}


void* GnrlMov(void* ghost_void)
{

    sleep(3);
    Ghost*  ghost = static_cast<Ghost*>(ghost_void);
    bool change_dir=true;
    auto startTime = std::chrono::steady_clock::now();
    auto endTime = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsedSeconds;
    while(1)
    {
        WhoDied(ghost->X,ghost->Y);
        if(dead)
        {
            ghost->X=ghost->originX;
            ghost->Y=ghost->originY;
            std::cout<<ghost->X<<" "<<ghost->Y<<std::endl;

        }
        endTime = std::chrono::steady_clock::now();
        elapsedSeconds = endTime - startTime;

       

        if (elapsedSeconds.count() >= 0.5 || GisWall(ghost->redmov,ghost->X,ghost->Y) || getTile(ghost->X,ghost->Y)==T ) {
            // Run timed function
            direc mov=movGhost(ghost->X,ghost->Y,ghost->redmov,change_dir,ghost->ghost_col);
            ghost->redmov=mov;
            // Reset start time
            startTime = std::chrono::steady_clock::now();
        }
        
      
       
        switch (ghost->redmov)
        {
        case LEFT:
            ghost->X -= 0.1f;
            ghost->Y = round(ghost->Y);
        break;
        case RIGHT:
            ghost->X += 0.1f;
            ghost->Y = round(ghost->Y);
        break;    
        case UPWARD:
            ghost->X=round(ghost->X);
            ghost->Y+=0.1f;
        break;
        case DOWNWARD:
            ghost->X=round(ghost->X);
            ghost->Y-=0.1f;
        break;
        default:
        std::cout<<X_cord<<" "<<Y_cord<<" "<<ghost->X<<" "<<ghost->Y<<std::endl;
            ghost->X=round(ghost->X);
            ghost->Y = round(ghost->Y);
        break;
        }
        
        usleep(15000);

    }
    pthread_exit(NULL);
}

#endif