#ifndef COURSEWORK_PAC_H
#define COURSEWORK_PAC_H

typedef enum{UPWARD, DOWNWARD, LEFT, RIGHT,STILL} direc;

float X_cord;
float Y_cord;
float lives=3;
bool dead=false;
bool bigBall=false;
auto bigBallEffect_start = std::chrono::steady_clock::now();
auto bigBallEffect_end = std::chrono::steady_clock::now();
 int score;
class Pac
{
public:
    
    direc pacDirect;
    direc nexDirect;
    float angle;
    bool start;
   

    Pac()
    {
        X_cord = 13.5f;
        Y_cord =  7.0f;
        pacDirect = STILL;
        nexDirect = STILL;
        start=true;
        score=0;
        
    }
    friend void* Pacmove(void* pac_void); // Declare Pacmove as a friend function
    
    void draw()
    {  
        if(total_dots==0)
        {
            Finish();
            return;
        }

        glPushMatrix();
        tile temp=getTile(X_cord,Y_cord);
        bigBallEffect_end = std::chrono::steady_clock::now();
        std::chrono::duration<double> elapsedSeconds=bigBallEffect_end-bigBallEffect_start;
        if(elapsedSeconds.count() > 5 )
        {   
            
            bigBall=false;
        }
        if(temp==o)
        {
            score+=10;
            total_dots--;
            maze[(int) floor(X_cord)][(int) floor(Y_cord)]=s;
        }
        else if(temp==O && !bigBall)
        {
            score+=50;
            maze[(int) floor(X_cord)][(int) floor(Y_cord)]=S;
            bigBall=true;
            bigBallEffect_start = std::chrono::steady_clock::now();
            total_dots--;
        }
        else if(temp==T)
        {
            
            if(round(X_cord)==1)
            {
                X_cord=26;
            }
            else
            {
                X_cord=2;
            }
        }

        unsigned int pac_tex;  
        auto currentTime = std::chrono::steady_clock::now();
        auto eTime = std::chrono::duration_cast<std::chrono::seconds>(currentTime - startTime).count();
int textureIndex = (eTime / 15) % 3; // Calculate texture index based on elapsed time
pac_tex = pac_0_tex;

        
        switch(pacDirect){ // Rotates pacman to face in his current travel direction
            case LEFT: angle = 0.0f;
                break;
            case UPWARD: angle = 270.0f;
                break;
            case RIGHT: angle = 180.0f;
                break;
            case DOWNWARD: angle = 90.0f;
                break;
            default: pacDirect = STILL;
        }

        translateBottomLeft(); // Translate to bottom left of screen which is (0,0)
        translateToMazeCoords(X_cord, Y_cord);

        glTranslatef(-3.0f, -3.0f, 0.0f); // Translate to take into account pacmans size

        drawTex(pac_tex, 14, 14, angle);

        glPopMatrix();
       
    }
    
    
    bool isAtCenter()
    {
        return (int)(Y_cord * 10.0f) % 10 == 0 && (int)(Y_cord * 10.0f) % 10 == 0;
    }
    void Finish()
    {
       // RecreateMaze();
        X_cord = 13.5f;
        Y_cord =  7.0f;
        pacDirect = STILL;
        nexDirect = STILL;
        sleep(3);
        start=true;
        dead =false;
    }

    void Restart()
    {
        X_cord = 13.5f;
        Y_cord =  7.0f;
        RecreateMaze();
        pacDirect = STILL;
        nexDirect = STILL;
        start=true;
        dead =false;
    }
};

    tile getFollowingTile(direc dir,float x, float y)
    {
        switch(dir) {
            case LEFT: return getTile((round(x)) - 1, round(y));
            case UPWARD: return getTile(round(x), (round(y)) + 1);
            case RIGHT: return getTile((round(x))+1, round(y));
            case DOWNWARD: return getTile(round(x), round(y) - 1);
            default: return getTile(round(x), round(y));
        }
    }
    bool isWall(direc dir,float x, float y)
    {
        if (getFollowingTile(dir,x,y) == W || getFollowingTile(dir,x,y) == G) {
            return true;
        }

        return false;
    }

void* Pacmove(void* pac_void)
{   sleep(3);
    Pac* pac = static_cast<Pac*>(pac_void);
    while(1)
    {   if(total_dots<=0)
        {
            RecreateMaze();
            X_cord = 13.5f;
            Y_cord =  7.0f;
            pac->pacDirect = STILL;
            pac->nexDirect = STILL;
            pac->start=true;
        }
        if(lives<=0)
        {
            break;
        }
        else if(dead)
        {
            pac->Finish();
        }
        if(pac->isAtCenter()) {
    if (!(isWall(pac->nexDirect,X_cord,Y_cord))) { // If the direction pacman wants to go in is not a wall, set that direction
                {pac->pacDirect = pac->nexDirect;}
            } else if (isWall(pac->pacDirect,X_cord,Y_cord)) {
                pac->pacDirect = STILL; // If the direction pacman wants to travel in is a wall and the direction he is currently travelling is a wall, stop his movement
            }
        }

    if(!pac->isAtCenter() && pac->nexDirect != STILL && pac->start && !isWall(pac->nexDirect,X_cord,Y_cord)){ // Check to not cause pacman to jump at the very beginning of the game
            pac->pacDirect = pac->nexDirect;
            pac->start = false;
        }
    
    // pthread_mutex_lock(&lock);
    switch (pac->pacDirect) {
        case LEFT:
            X_cord -= 0.1f;
            Y_cord = round(Y_cord);
            break;
        case UPWARD:
            Y_cord += 0.1f;
            X_cord = round(X_cord);
            break;
        case RIGHT:
            X_cord += 0.1f;
            Y_cord = round(Y_cord);
            break;
        case DOWNWARD:
            Y_cord -= 0.1f;
            X_cord = round(X_cord);
            break;
        default:
                if(!pac->start) { // Do not round pacman if it is the very start as he begin between two tiles
                    X_cord = round(X_cord);
                    Y_cord = round(Y_cord);
                }
    }
    // pthread_mutex_unlock(&lock);
    usleep(10000);
    }
    pthread_exit(NULL);
}
#endif 