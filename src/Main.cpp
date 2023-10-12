#include <raylib.h>
#include <vector>
#include <stack>
#define SIZE 20 // Base val is 40
#define WIDTH 762 // This is the width and height of Window 
#define ROWS 38 // this is WIDTH/SIZE 
using namespace std;

class Cell{
  private:
    int x_axis, y_axis;
  public:
    bool visited;
    bool walls[4];

    // Empty constructor
    Cell(){
    }

    Cell(int x, int y){
      // Initially making all the cells unvisited
      visited = false;

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
      // Top 
     if(walls[0])
        DrawLine(x_axis, y_axis, x_axis+SIZE, y_axis, WHITE);

      // Right
      if(walls[1])
        DrawLine(x_axis+SIZE, y_axis, x_axis+SIZE, y_axis+SIZE, WHITE);

      // Bottom
      if(walls[2])
        DrawLine(x_axis, y_axis+SIZE, x_axis+SIZE, y_axis+SIZE, WHITE);

      // Left
      if(walls[3])
        DrawLine(x_axis, y_axis, x_axis, y_axis+SIZE, WHITE);

      // Visited Cell
      if(visited){
        DrawRectangle(x_axis, y_axis, SIZE, SIZE, RED);
      }
    }

    void highilght(){
      DrawRectangle(x_axis, y_axis, SIZE-1, SIZE-1, GREEN);
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
  if(top != -1 && !grids.at(top).visited)
    neighborIndex.push_back(top);

  int right = getIndex(i+1,j);
  if(right != -1 && !grids.at(right).visited)
    neighborIndex.push_back(right);
  
  int bottom = getIndex(i,j+1);
  if(bottom != -1 && !grids.at(bottom).visited)
    neighborIndex.push_back(bottom);

  int left = getIndex(i-1,j);
  if(left != -1 && !grids.at(left).visited)
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


//### The MAIN function ###//
int main(){

  SetTargetFPS(60);

  // Initializing rows and cols
  int rows = WIDTH/SIZE;  // 19
  int cols = WIDTH/SIZE;  // 19

  // Declaring a grids variable to store all the cells.
  vector<Cell> grids;
  // Declaring a stack to track the cells [ for backtracking ]
  stack<int> stack;

  // Initializing the grids
  for(int i=0;i<rows;i++){
    for(int j=0;j<cols;j++){
      grids.push_back(Cell(i, j));  
    }
  }

  // Creating a cell to keep track on current cell
  int currentIndex = 0;
  stack.push(currentIndex);
  grids.at(currentIndex).visited = true;

	InitWindow(WIDTH, WIDTH,"MazeGenerator");

	ClearBackground(WHITE);

	while(!WindowShouldClose()){
		BeginDrawing();
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
        grids.at(currentIndex).visited = true;
      }
      else if(!stack.empty()){
        currentIndex = stack.top();
        stack.pop();
      }

      // Drawing the Grids
      for(int i=0;i<grids.size();i++){
          grids.at(i).draw();
          grids.at(currentIndex).highilght();
      }

		EndDrawing();
	}

	CloseWindow();

	return 0;
}
