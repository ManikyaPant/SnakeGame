#include "bits/stdc++.h"
#include "ctime"
#include "raylib.h"
#include "raymath.h"

using namespace std;
 
double lastTime=0;


bool checkCollisionWithWall(pair<int,int> head, int xCell, int yCell ) {
    if(head.second==-1) {
    return true;
    }
    if(head.second==yCell+1) {
    return true;
    }
    if(head.first==-1) {
       return true;
    }
    if(head.first== xCell+1) {
      return true;
    }
    return false;

}

bool checkCollisionWithFood(pair <int, int> head, pair <int,int> food ) {
  if(head.first==food.first && head.second== food.second) {
  return true;
  }
  return false;
  
}

bool timePassed(double interval) {
    double currentTime= GetTime();

    if(currentTime-lastTime>=interval) {
      lastTime=currentTime;
      return true;
    }
    return false;
}

class Game;
class Snake
{
public:
  // need to define the positon of the snake
  float roundness = 0.2f;
  deque<pair<int, int>> snakePos = {{5, 3}, {4, 3}, {3, 3}};
  pair<int, int> direction = {1, 0};

  // default start with snake of size 3
  void showSnake(int cellSize)
  {
    for (int i = 0; i < snakePos.size(); i++)
    {
      Rectangle rec = {snakePos[i].first * cellSize, snakePos[i].second * cellSize, cellSize, cellSize};
      DrawRectangleRounded(rec, roundness, 0, BLACK);
    }
  }

  void updateSnake(bool updateLength)
   {
       if(updateLength) {
        // add a new block
          pair < int ,int > toAdd= {snakePos.front().first + direction.first, snakePos.front().second + direction.second};
          snakePos.push_front(toAdd) ;
          }
    snakePos.pop_back();
    pair<int, int> toPush = {snakePos.front().first + direction.first, snakePos.front().second + direction.second};
    snakePos.push_front(toPush);
  }

  bool checkCollisionWithSelf() {
     int n= snakePos.size();
     for(int i=1;i<n;i++) {
        if(snakePos[i]==snakePos[0]) {
          return true;
        }
     }
     return false;
  }
};

class Food
{
public:
  pair<int, int> pos;


  bool randHelper(int x, int y, deque <pair<int,int>>& pos) {
        for(auto &it:pos){
          if(it==make_pair(x,y)) {
            return true;
          }
        }
        return false;
  }

  void randPos(int xCells, int yCells, deque <pair <int,int>>& snakePos) 
  {
    // need to generate a random position for the Food
   int x= rand()% xCells;
   int y= rand()% yCells;    
    
   while(randHelper(x, y , snakePos)) {
     x=rand()% xCells;
     y= rand()% yCells;
   }

   pos.first=x;
   pos.second=x;

   // need to check if this position doesn't conincides with the snake body 
  
      
  }
};

class Game
{
public:
  // setting the paratmers of the game
  const int cellSize = 20;
  const int xCells = 40;
  const int yCells = 40;
  double speed= 0.1;
  int score=0;
  
};

int main()
{
  srand(time(NULL));
  Game *game = new Game();
  Food food;
  Snake snake;
  food.randPos(game->xCells, game->yCells,snake.snakePos);
  const int screenWidth = (game->cellSize) * (game->xCells);
  const int screenHeight = (game->cellSize) * (game->yCells);
  InitWindow(screenWidth, screenHeight, "Here we go again");
  SetTargetFPS(60);
  while (!WindowShouldClose())
  {
    game->speed= (game->score/10)*0.1+ 0.1;
    BeginDrawing();
    ClearBackground(GRAY);
    DrawRectangle(food.pos.first * game->cellSize, food.pos.second * game->cellSize, game->cellSize, game->cellSize, WHITE);
    snake.showSnake(game->cellSize);
    if(snake.checkCollisionWithSelf()) {
      EndDrawing(); 
      break;
    }
    
    
    if(checkCollisionWithFood(snake.snakePos.front(),food.pos)) {
      // first increase the size of the snake and then add food at a new pos
      // generate a signal to the snake update positon that a 
      game->score++;
      snake.updateSnake(true);
      food.randPos(game->xCells, game->yCells,snake.snakePos) ;
    }
    
    if (IsKeyPressed(KEY_UP) && snake.direction!=make_pair(0,1))
    {
      snake.direction = {0, -1};
    }
    if (IsKeyPressed(KEY_DOWN) && snake.direction!=make_pair(0,-1))
    {
      snake.direction = {0,1};
    }
    
    if (IsKeyPressed(KEY_RIGHT) && snake.direction!=make_pair(-1,0))
    {
      snake.direction = {1, 0};
    }
    
    if (IsKeyPressed(KEY_LEFT) && snake.direction!=make_pair(1,0))
    {
      snake.direction = {-1, 0};
    }
    
    if(timePassed(game->speed)) {
      snake.updateSnake(false);
    }
    bool val = checkCollisionWithWall(snake.snakePos.front(),game->xCells,game->yCells);
    if(val) {
    EndDrawing();
      break;
    } 
    EndDrawing();
  }
  CloseWindow();



  InitWindow(screenWidth, screenHeight, "Score time");
  SetTargetFPS(60);
  while(!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(GRAY);

    DrawText(TextFormat("Score: %d",game->score),(game->cellSize*game->yCells)/2-10*game->cellSize,(game->cellSize*game->yCells)/2,100,BLACK);
    EndDrawing();
  }
  game->score=0;
  CloseWindow();
  return 0;
}