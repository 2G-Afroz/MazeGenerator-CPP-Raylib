#include <iostream>
#include <raylib.h>
#include <vector>
#include <stack>
#define SIZE 40 // Base val is 40
#define WIDTH 760 // This is the width and height of Window 
#define ROWS 19 // this is WIDTH/SIZE 
using namespace std;

class Cell{
  private:
    int x_axis, y_axis;
  public:
    bool visitedCell;
    bool visitedPath;
    bool foundPath;
    bool walls[4];

    // Empty constructor
    Cell(){
    }

    Cell(int x, int y){
      // Initially making all the cells unvisited
      visitedCell = false;
      visitedPath = false;
      foundPath = false;

      // Initially making all wall visible/available.
      walls[0] = true;
      walls[1] = true;
      walls[2] = true;
      walls[3] = true;

      // This is the position of cell on grid.
      x_axis = x * SIZE + 1;
      y_axis = y * SIZE + 1;
    }


    void draw(){
      // Visited Cell
      if(visitedCell){
        DrawRectangle(x_axis, y_axis, SIZE, SIZE, RED);
      }

      if(visitedPath){
        DrawRectangle(x_axis, y_axis, SIZE, SIZE, PURPLE);
      }

      if(foundPath){
        DrawRectangle(x_axis, y_axis, SIZE, SIZE, GREEN);
      }

      // Top 
     if(walls[0])
        DrawLine(x_axis-1, y_axis, x_axis+SIZE, y_axis, BLACK);

      // Right
      if(walls[1])
        DrawLine(x_axis+SIZE-1, y_axis-1, x_axis+SIZE-1, y_axis+SIZE, BLACK);

      // Bottom
      if(walls[2])
        DrawLine(x_axis-1, y_axis+SIZE-1, x_axis+SIZE, y_axis+SIZE-1, BLACK);

      // Left
      if(walls[3])
        DrawLine(x_axis-1, y_axis-1, x_axis, y_axis+SIZE, BLACK);

    }

    void highlight(){
      DrawRectangle(x_axis, y_axis, SIZE-2, SIZE-2, ORANGE);
    }

};

// This function is used to convert 2D index into 1D index.
int getIndex(int i, int j){
  if(i<0 || i>=ROWS || j<0 || j>=ROWS)
    return -1;
  return i * ROWS + j;
}


// This function is used to get unvisited random neighbor.
int getURN(vector<Cell> grids, int currentIndex){

  int i = currentIndex/ROWS;
  int j = currentIndex%ROWS;

  vector<int> neighborIndex;

  int top = getIndex(i,j-1);
  if(top != -1 && !grids.at(top).visitedCell)
    neighborIndex.push_back(top);

  int right = getIndex(i+1,j);
  if(right != -1 && !grids.at(right).visitedCell)
    neighborIndex.push_back(right);
  
  int bottom = getIndex(i,j+1);
  if(bottom != -1 && !grids.at(bottom).visitedCell)
    neighborIndex.push_back(bottom);

  int left = getIndex(i-1,j);
  if(left != -1 && !grids.at(left).visitedCell)
    neighborIndex.push_back(left);

  if(neighborIndex.size()>0){
    int r = GetRandomValue(0,neighborIndex.size()-1);
    return neighborIndex.at(r);
  }

  return -1;
}

// This function is used to remove walls
void removeWalls(vector<Cell> &grids, int c, int n){
  // c = current, n = next
  if(c < n){
    if(c-n == -1){
      // c.bottom, n.top
      grids.at(c).walls[2] = false;
      grids.at(n).walls[0] = false;
    }
    else{
      // c.right, n.left
      grids.at(c).walls[1] = false;
      grids.at(n).walls[3] = false;
    }
  }
  else {
    if(c-n == 1){
      // c.top, n.bottom
      grids.at(c).walls[0] = false;
      grids.at(n).walls[2] = false;
    }
    else {
      // c.left, n.right
      grids.at(c).walls[3] = false;
      grids.at(n).walls[1] = false;
    }
  }
}

bool createMaze(vector<Cell> &grids, int &currentIndex, stack<int> &stack){
      // Update the neighborIndex here
      // Taking the random unvisited neighbor
      int next = getURN(grids, currentIndex);
      if(next != -1){
        // Remove wall from current and next
        removeWalls(grids, currentIndex, next);

        // Setting the current cell to next cell
        currentIndex = next;
        // Pushing to stack
        stack.push(currentIndex);

        // Marking the current cell as visited
        grids.at(currentIndex).visitedCell = true;
      }
      else if(!stack.empty()){
        currentIndex = stack.top();
        stack.pop();
      }
      else{
        return true;
      }

    return false;
}

void drawMaze(vector<Cell> &grids, int &currentIndex){
  // Drawing the Grids
  for(int i=0;i<grids.size();i++){
    grids.at(i).draw();
  }
 
}

// This function is used to get the NextPossiblePath(NPP)
int getNPP(vector<Cell> grids, int currentIndex){
  int i = currentIndex/ROWS;
  int j = currentIndex%ROWS;

  int top = getIndex(i,j-1);
  if(top != -1 && !grids.at(top).visitedPath && !grids.at(currentIndex).walls[0]){
    return top;
  }

  int right = getIndex(i+1,j);
  if(right != -1 && !grids.at(right).visitedPath && !grids.at(currentIndex).walls[1]){
    return right;
  }
  
  int bottom = getIndex(i,j+1);
  if(bottom != -1 && !grids.at(bottom).visitedPath && !grids.at(currentIndex).walls[2]){
    return bottom;
  }

  int left = getIndex(i-1,j);
  if(left != -1 && !grids.at(left).visitedPath && !grids.at(currentIndex).walls[3]){
    return left;
  }

  return -1;
}

//### The MAIN function ###//
int main(){

  SetTargetFPS(60);

  // Initializing rows and cols
  int rows = WIDTH/SIZE;  // 19
  int cols = WIDTH/SIZE;  // 19

  // Declaring a grids variable to store all the cells.
  vector<Cell> grids;
  // Declaring a stack to track the cells [ for backtracking ]
  std::stack<int> stack;

  // Initializing the grids
  for(int i=0;i<rows;i++){
    for(int j=0;j<cols;j++){
      grids.push_back(Cell(i, j));  
    }
  }

  // Creating a cell to keep track on current cell
  int currentIndex = 0;

  // These are for DFS Path finding algorithm
  int start = -1;
  int current = -1;
  int end = -1;
  std::stack<int> pathStack;

  stack.push(currentIndex);
  grids.at(currentIndex).visitedCell = true;

  bool mazeCreated = false;

	InitWindow(WIDTH, WIDTH,"MazeGenerator");


	while(!WindowShouldClose()){
		BeginDrawing();
	  ClearBackground(BLACK);

      // Drawing the Maze
      drawMaze(grids, currentIndex);

      // Creating the Maze if not created
      if(!mazeCreated){
        mazeCreated = createMaze(grids, currentIndex, stack);
        grids.at(currentIndex).highlight();
      }
      else{
        // Maze has been generated
        // Apply DFS algorithm here.

        // Taking Starting position from user
        if(current == -1){
          DrawText("Pick START Position", 200, WIDTH/2-5, 40, BLACK);
          if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
            int i = GetMouseX()/SIZE;
            int j = GetMouseY()/SIZE;
            current = getIndex(i, j);
            start = current;
            pathStack.push(current);
            grids.at(current).visitedPath = true;
            grids.at(current).foundPath = true;
	          ClearBackground(BLACK);
          }
        }
        else{
          // Taking Ending position from user
          if(end == -1){
            DrawText("Pick END Position", 200, WIDTH/2-5, 40, BLACK);
            if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
              int i = GetMouseX()/SIZE;
              int j = GetMouseY()/SIZE;
              end = getIndex(i, j);
	            ClearBackground(BLACK);
            }
          }
          else {
            if(current != end){
              int next = getNPP(grids, current);
                if(next != -1){
                  current = next;
                  pathStack.push(current);
                  grids.at(current).visitedPath = true;
                  grids.at(current).foundPath = true;
                }
                else{
                  if(!pathStack.empty()){
                    grids.at(current).foundPath = false;
                    pathStack.pop();
                    current = pathStack.top();
                  }
                }
            }
            else{
              DrawText("FOUND", 250, WIDTH/2, 50, BLACK);
            }
            grids.at(start).highlight();
            grids.at(end).highlight();
          }
        }
      }
		EndDrawing();
	}

	CloseWindow();

	return 0;
}
