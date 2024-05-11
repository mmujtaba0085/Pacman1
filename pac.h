#ifndef COURSEWORK_PAC_H
#define COURSEWORK_PAC_H

typedef enum{UPWARD, DOWNWARD, LEFT, RIGHT,STILL} direc;


class Pac
{
public:
    float X_cord;
    float Y_cord;
    direc pacDirect;
    direc nexDirect;
    float angle;
    bool start;
    int score;

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
        if(temp==o)
        {
            score+=10;
            total_dots--;
            maze[(int) floor(X_cord)][(int) floor(Y_cord)]=s;
        }
        else if(temp==O)
        {
            score+=50;
            maze[(int) floor(X_cord)][(int) floor(Y_cord)]=S;
            total_dots--;
        }
        else if(temp==F)
        {
            score+=100;
            maze[(int) floor(X_cord)][(int) floor(Y_cord)]=s;
        }
        else if(temp==T)
        {
            std::cout<<X_cord<<" "<<Y_cord<<std::endl;
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
        pac_tex = pac_1_tex;
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
    tile getFollowingTile(direc dir)
    {
        switch(dir) {
            case LEFT: return getTile((round(X_cord)) - 1, round(Y_cord));
            case UPWARD: return getTile(round(X_cord), (round(Y_cord)) + 1);
            case RIGHT: return getTile((round(X_cord))+1, round(Y_cord));
            case DOWNWARD: return getTile(round(X_cord), round(Y_cord) - 1);
            default: return getTile(round(X_cord), round(Y_cord));
        }
    }
    bool isWall(direc dir)
    {
        if (getFollowingTile(dir) == W || getFollowingTile(dir) == G) {
            return true;
        }

        return false;
    }
    
     bool isAtCenter()
    {
        return (int)(Y_cord * 10.0f) % 10 == 0 && (int)(Y_cord * 10.0f) % 10 == 0;
    }
    void Finish()
    {
        RecreateMaze();
        X_cord = 13.5f;
        Y_cord =  7.0f;
        pacDirect = STILL;
        nexDirect = STILL;
        start=true;
    }
};



void* Pacmove(void* pac_void)
{
    Pac* pac = static_cast<Pac*>(pac_void);
    while(1)
    {
        if(pac->isAtCenter()) {
    if (!(pac->isWall(pac->nexDirect))) { // If the direction pacman wants to go in is not a wall, set that direction
                {pac->pacDirect = pac->nexDirect;}
            } else if (pac->isWall(pac->pacDirect)) {
                pac->pacDirect = STILL; // If the direction pacman wants to travel in is a wall and the direction he is currently travelling is a wall, stop his movement
            }
        }

    if(!pac->isAtCenter() && pac->nexDirect != STILL && pac->start && !pac->isWall(pac->nexDirect)){ // Check to not cause pacman to jump at the very beginning of the game
            pac->pacDirect = pac->nexDirect;
            pac->start = false;
        }

    // pthread_mutex_lock(&lock);
    switch (pac->pacDirect) {
        case LEFT:
            pac->X_cord -= 0.1f;
            pac->Y_cord = round(pac->Y_cord);
            break;
        case UPWARD:
            pac->Y_cord += 0.1f;
            pac->X_cord = round(pac->X_cord);
            break;
        case RIGHT:
            pac->X_cord += 0.1f;
            pac->Y_cord = round(pac->Y_cord);
            break;
        case DOWNWARD:
            pac->Y_cord -= 0.1f;
            pac->X_cord = round(pac->X_cord);
            break;
        default:
                if(!pac->start) { // Do not round pacman if it is the very start as he begin between two tiles
                    pac->X_cord = round(pac->X_cord);
                    pac->Y_cord = round(pac->Y_cord);
                }
    }
    
    // pthread_mutex_unlock(&lock);
    usleep(10000);
    }
    pthread_exit(NULL);
}
#endif 