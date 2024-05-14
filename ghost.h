#ifndef GHOST_H
#define GHOST_H


typedef enum{Follow,RunAway,Scatter,Box,Dead}motion;
typedef enum{RED,BLUE,PINK,YELLOW}color;

pthread_mutex_t key;

float initalRed_X=12.0f;
float initailRed_Y=19.0f;
float initalBlue_X=14.0f;
float initailBlue_Y=19.0f;
float initialPink_X=12.5f;
float initialPink_Y=15.3;
float initialYellow_X=13.5f;
float initialYellow_Y=15.3;
motion AllGhostMov=Box;
bool isGateOpen = true; // Flag to indicate if the gate is open


float xx[4]{1.0f,26.0f,1.0f,26.0f};
float yy[4]{29.0f,1.0f,1.0f,29.0f};

bool GisWall(direc dir,float x, float y)
    {
        if (getFollowingTile(dir,x,y) == W || (getFollowingTile(dir,x,y) == G && dir==DOWNWARD) ) {
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
    int total_scatter=3;
    int died;
    bool is_fast;
    Ghost(float x, float y, color col,float speeed)
    {
        move_type=Box;
        speed=speeed;
        redmov=UPWARD;
        originX=x;
        originY=y;
        X=x;
        Y=y;
        ghost_col=col;
        died=false;
        is_fast=false;
        if(speed>=0.1)
        {
            is_fast=true;
        }
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
        if(!bigBall)
        {
            if(ghost_col==RED ) 
            {
                Ghst_tex = ghost_r_0_tex;
            }
            else if(ghost_col==YELLOW)
            {
                Ghst_tex = ghost_y_0_tex;
            }
            else if(ghost_col==BLUE )
            {
                Ghst_tex = ghost_b_0_tex;
            }
            else if(ghost_col==PINK)
            {
                Ghst_tex = ghost_p_0_tex;
            }
        }
        else
        {
            Ghst_tex=ghost_scared_0_tex;
            
        }
        drawTex(Ghst_tex, 14, 14, 0);
        glPopMatrix();
    }

};
float disINBtwPnG(float x1,float y1,float x2,float y2)          //to calculate distance in between pacman and given Coordinates;
{
    return sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));
}

int PosToPac()
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



void WhoDied(float &x, float &y,Ghost* ghost)
{
    if(round(x)==round(X_cord) && round(y)==round(Y_cord) && !dead && !bigBall)
    {//if pac dies
        lives--;
        dead=true;
    }
    else if(round(x)==round(X_cord) && round(y)==round(Y_cord) && bigBall)
    {
        ghost->died=true;
    }
}

direc movGhost(float &x,float &y,direc& prevGhostMov,bool change_dir,color col ,Ghost*  ghost)
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
    if(ghost->move_type==Box)
    {
        x2=13.5f;
        y2=19.0f;
    
    }
    if(ghost->move_type==Box && round(x)==round(x2) && round(y)==round(y2))
    {
        ghost->move_type = Follow;
    }
    
    if(ghost->move_type==Follow)
    {
        if(col==RED || col==YELLOW)   
        {
            x2=X_cord;
            y2=Y_cord;
        }
        else if(col==BLUE || col==PINK)
        {
            int temp=PosToPac();
            x2=xx[temp];
            y2=yy[temp];
        }
    }
    else if(ghost->move_type==Scatter)
    {
        if(col==BLUE)   
        {
            x2=xx[3];
            y2=yy[3];
        }
        else if(col==PINK)
        {
            x2=xx[2];
            y2=yy[2];
        }
        else if(col==RED)
        {

            x2=xx[1];
            y2=yy[1];
        }
        else if(col==YELLOW)
        {
            x2=xx[0];
            y2=yy[0];
        }
    }
    else if(ghost->move_type==RunAway)
    {
        if(col==RED || col==YELLOW)   
        {
            x2=X_cord;
            y2=Y_cord;
        }
        else if(col==BLUE || col==PINK)
        {
            int temp=PosToPac();
            x2=xx[temp];
            y2=yy[temp];
        }
        dis = 0;
    }
    if((possible_exit(x,y)>2 && change_dir) || ghost->move_type==Box)
    {
    
    if((!GisWall(LEFT,x,y) && prevGhostMov!=RIGHT) || ghost->move_type==Box )
    {
        if((dis > disINBtwPnG((x)-1,y,x2,y2) && ghost->move_type!=RunAway) || (dis < disINBtwPnG((x)-1,y,x2,y2) && ghost->move_type==RunAway) )
        {
        dis = disINBtwPnG(x-1,y,x2,y2);
        mov = LEFT;
        }
    }
    if((!GisWall(UPWARD,x,y) && prevGhostMov!=DOWNWARD) || ghost->move_type==Box)
    {
        if((dis > disINBtwPnG(x,y+1,x2,y2)&& ghost->move_type!=RunAway) || (dis < disINBtwPnG(x,y+1,x2,y2)&& ghost->move_type==RunAway))
        {
        dis = disINBtwPnG(x,y+1,x2,y2);
        mov = UPWARD;
        }
    }
    if((!GisWall(RIGHT,x,y) &&  prevGhostMov!=LEFT) || ghost->move_type==Box)
    {
        if((dis > disINBtwPnG(x+1,y,x2,y2)&& ghost->move_type!=RunAway) || (dis < disINBtwPnG(x+1,y,x2,y2)&& ghost->move_type==RunAway))
        {
        dis = disINBtwPnG(x+1,y,x2,y2);
        mov = RIGHT;
        }
    }
    if((!GisWall(DOWNWARD,x,y) && prevGhostMov!=UPWARD)|| ghost->move_type==Box)
    {
        if((dis  > disINBtwPnG(x,y-1,x2,y2)&& ghost->move_type!=RunAway) || (dis  < disINBtwPnG(x,y-1,x2,y2)&& ghost->move_type==RunAway))
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
            return movGhost(x,y,prevGhostMov,change_dir,col,ghost);
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
    auto DirStartTime = std::chrono::steady_clock::now();
    auto DirEndTime = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsedSeconds;
    bool scareChange=true;
    bool takengate=false;
    int count=0;
    while(1)
    {
        WhoDied(ghost->X,ghost->Y,ghost);
        if(dead)
        {
            ghost->X=ghost->originX;
            ghost->Y=ghost->originY;
            // std::cout<<ghost->X<<" "<<ghost->Y<<std::endl;
        }
        else if(ghost->died)
        {   ghost->X=11.5f;
            ghost->Y=16.0f;
            ghost->died=false;
        }
        pthread_mutex_lock(&key);
        if ((getFollowingTile(ghost->redmov, ghost->X, ghost->Y) == G ) && isGateOpen) {
            isGateOpen = false; // Close the gate for other ghosts
            takengate = true;
            std::cout<<"GateLocked"<<std::endl;
            ghost->Y+=1.0f;
        } 
        pthread_mutex_unlock(&key);
        // else if (getFollowingTile(ghost->redmov,ghost->X, ghost->Y) == G && !isGateOpen ) {
        //     // Wait outside the gate until another ghost opens it
            
        //     while (getFollowingTile(ghost->redmov,ghost->X, ghost->Y) == G && !isGateOpen) {
        //     sleep(1); // Sleep for 10 milliseconds
        //     }
        //  continue;   
        // }
        
        
        auto currentTime = std::chrono::steady_clock::now();
        auto eTime = std::chrono::duration_cast<std::chrono::seconds>(currentTime - startTime).count();

        if(bigBall)
        {
            ghost->speed=0.05;
            if(ghost->move_type!=Box)
            {
                ghost->move_type = RunAway;
            }
            if(scareChange)
            {
                switch (ghost->move_type)
                {
                case LEFT:
                    ghost->redmov=RIGHT;
                    break;
                case RIGHT:
                    ghost->redmov=LEFT;
                    break;
                case UPWARD:
                    ghost->redmov=DOWNWARD;
                    break;
                case DOWNWARD:
                    ghost->redmov=UPWARD;
                    break;
                default:
                    break;
                }

                scareChange=false;
            }
        }
        else if ((eTime % 35 <= 15) && ghost->move_type!=Scatter && ghost->total_scatter > 0 && ghost->move_type!=Box) 
        {   if(ghost->is_fast)
            {   
                ghost->speed=0.1;
            }    
            else
            {
                ghost->speed=0.08;
            }
            ghost->move_type = Scatter;
            ghost->total_scatter--;
            scareChange=true;
        }
        else if((eTime % 35 > 15) && ghost->move_type!=Follow && ghost->move_type!=Box)
        {
            if(ghost->is_fast)
            {   
                ghost->speed=0.1;
            }    
            else
            {
                ghost->speed=0.08;
            }
            ghost->move_type = Follow;
            scareChange=true;
        }
        
        
        DirEndTime = std::chrono::steady_clock::now();
        elapsedSeconds = DirEndTime - DirStartTime;

      
        if (elapsedSeconds.count() >= 0.5 || GisWall(ghost->redmov,ghost->X,ghost->Y) || getTile(ghost->X,ghost->Y)==T ) {
            // Run timed function
            direc mov=movGhost(ghost->X,ghost->Y,ghost->redmov,change_dir,ghost->ghost_col,ghost);
            ghost->redmov=mov;
            // Reset start time
            DirStartTime = std::chrono::steady_clock::now();
        }
        
      
       if(getFollowingTile(ghost->redmov,ghost->X,ghost->Y)==G && isGateOpen && !takengate)
       {
        continue;
       }
        switch (ghost->redmov)
        {
        case LEFT:
            ghost->X -= ghost->speed;
            ghost->Y = round(ghost->Y);
        break;
        case RIGHT:
            ghost->X += ghost->speed;
            ghost->Y = round(ghost->Y);
        break;    
        case UPWARD:
            ghost->X=round(ghost->X);
            ghost->Y+=ghost->speed;
        break;
        case DOWNWARD:
            ghost->X=round(ghost->X);
            ghost->Y-=ghost->speed;
        break;
        default:
        //std::cout<<X_cord<<" "<<Y_cord<<" "<<ghost->X<<" "<<ghost->Y<<std::endl;
            ghost->X=round(ghost->X);
            ghost->Y = round(ghost->Y);
        break;
        }
        if(takengate)
        {
            
            takengate=false;
            isGateOpen=true;
            std::cout<<"GateUnlocked"<<ghost->ghost_col<<std::endl;
        }
        usleep(15000);

    }
    pthread_exit(NULL);
}

#endif