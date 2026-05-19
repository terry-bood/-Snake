#include <iostream>
#include <vector>
#include <cstring>
#include <thread>
#include <chrono>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <termios.h>

using namespace std;

#define Row 20
#define Col 20

#define Space  0
#define Body   1
#define Apple  2
#define Wall   9

#define Up      0
#define Down    1
#define Left    2
#define Right   3

struct Pos{
    int x;
    int y;
    Pos() {};
    Pos(int x,int y)
    {
        this->x=x;
        this->y=y;
    }
    
};

char getch(){
    char buf = 0;
    struct termios old = {0};
    if (tcgetattr(0, &old) < 0) perror("tcsetattr()");
    old.c_lflag &= ~ICANON;
    old.c_lflag &= ~ECHO;
    old.c_cc[VMIN] = 1;
    old.c_cc[VTIME] = 0;
    if (tcsetattr(0, TCSANOW, &old) < 0) perror("tcsetattr ICANON");
    if (read(0, &buf, 1) < 0) perror ("read()");
    old.c_lflag |= ICANON;
    old.c_lflag |= ECHO;
    if (tcsetattr(0, TCSANOW, &old) < 0) perror ("tcsetattr ~ICANON");
    return (buf);
}


void cls(){
    system("clear");
}



vector<Pos> snake;
Pos applePos;
int direction =Right;

char map[Row][Col]={
    {9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9},
    {9,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,9},
    {9,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,9},
    {9,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,9},
    {9,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,9},
    {9,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,9},
    {9,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,9},
    {9,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,9},
    {9,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,9},
    {9,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,9},
    {9,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,9},
    {9,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,9},
    {9,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,9},
    {9,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,9},
    {9,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,9},
    {9,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,9},
    {9,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,9},
    {9,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,9},
    {9,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,9},
    {9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9}
};

char frame[Row][Col];

void printMap()
{
    memcpy(frame,map,Row*Col);
    
    for(int i=0;i<snake.size();i++){
        Pos p=snake[i];
        frame[p.y][p.x]=Body;
    }
    
    frame[applePos.y][applePos.x]=Apple;
    
    cls();
    for(int i =0;i<Row;i++){
        for(int j=0;j<Col;j++){
            switch(frame[i][j]){
                case Space: cout<<"  ";break;
                case Body:  cout<<"蛇";break;
                case Apple: cout<<"果";break;
                case Wall:  cout<<"牆";break;
                
            }
        }
        cout<<"\n";
    }
}

void randApple(){
    while(true){
        int x=rand()%Col;
        int y=rand()%Row;
        if (map[y][x]==Space)
        {
            bool onSnake= false;
            for(int i=0;i<snake.size();i++){
                Pos p=snake[i];
                if(p.x==x&&p.y==y){
                    onSnake =true;
                    break;
                }
            }
            if(onSnake) continue;
            
            applePos.x=x;
            applePos.y=y;
            break;
        }
    }
}

bool moveSnake(){
    Pos next = snake[0];
    if(direction==Up) next.y--;
    if(direction==Down) next.y++;
    if(direction==Left) next.x--;
    if(direction==Right) next.x++;
    
    switch(frame[next.y][next.x]){
        
        case Space:
            snake.insert(snake.begin(),next);
            snake.pop_back();
            break;
        case Apple:
            snake.insert(snake.begin(), next);
            map[next.y][next.x]=Space;
            randApple();
            break;
        case Wall:
        case Body:
            return true;
    }
    
    return false;
}

void listenKey(){
    while(true){
        int key=getch();
        Pos p1=snake[0];
        Pos p2=snake[1];
        switch(key){
            
        case 'w':
            p1.y--;
            if(p1.x==p2.x&&p1.y==p2.y) break;
            direction=Up;
            break;
        case 's':
            p1.y++;
            if(p1.x==p2.x&&p1.y==p2.y) break;
            direction =Down;
            break;
        case 'a':
            p1.x--;
            if(p1.x==p2.x&&p1.y==p2.y) break;
            direction=Left;
            break;
        case 'd':
            p1.x++;
            if(p1.x==p2.x&&p1.y==p2.y) break;
            direction =Right;
            break;
        }
    }
}

int main()
{
    srand(time(nullptr));
    thread t(listenKey);
    t.detach();
    
    
    snake.push_back(Pos(5,5));
    snake.push_back(Pos(5,4));
    snake.push_back(Pos(5,3));
    randApple();
    while(true){
        
        printMap();
        this_thread::sleep_for(chrono::milliseconds(300));
        if(moveSnake()){
            cls();
            cout.flush();
            cout<<"\n\n\n\n\n\n";
            cout<<"             遊戲結束";
            cout<<"\n\n\n\n\n\n";
            break;
        }
    }
    return 0;
}