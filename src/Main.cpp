#include <raylib.h>
#include <stack>
#include <vector>

#define SIZE 20   // Base val is 40
#define WIDTH 760 // This is the width and height of Window
#define ROWS 38   // this is WIDTH/SIZE

class Cell {
private:
  // This is the position of cell.
  int x_axis, y_axis;

public:
  bool visitedCell; // This is used for visited cell.
  bool
      visitedPath; // This is used for visited path (in DFS Path Finding Algo.).
  bool foundPath;  // This is used for the correct found path.
  bool walls[4];   // This is for the 4-side walls of the cell.

  // Empty constructor
  Cell() {}

  /*
   *  this is
   *
   * @param x x-axis position.
   * @param y y-axis position.
   */
  Cell(int x, int y) {
    // Initially making all the cells,path,foundPath as unvisited.
    visitedCell = false;
    visitedPath = false;
    foundPath = false;

    // Initially making all wall visible/available.
    walls[0] = true;
    walls[1] = true;
    walls[2] = true;
    walls[3] = true;

    // Assigning the position of the cell.
    x_axis = x * SIZE + 1;
    y_axis = y * SIZE + 1;
  }

  /**
   * This method is used to draw cell and walls
   */
  void draw() {
    // Visited Cell
    if (visitedCell) {
      DrawRectangle(x_axis, y_axis, SIZE, SIZE, RED);
    }

    // Visited Path Cell
    if (visitedPath) {
      DrawRectangle(x_axis, y_axis, SIZE, SIZE, PURPLE);
    }

    // Founded Path Cell
    if (foundPath) {
      DrawRectangle(x_axis, y_axis, SIZE, SIZE, GREEN);
    }

    // Top
    if (walls[0])
      DrawLine(x_axis - 2, y_axis, x_axis + SIZE, y_axis, BLACK);

    // Right
    if (walls[1])
      DrawLine(x_axis + SIZE - 1, y_axis - 2, x_axis + SIZE - 1, y_axis + SIZE,
               BLACK);

    // Bottom
    if (walls[2])
      DrawLine(x_axis - 2, y_axis + SIZE - 1, x_axis + SIZE, y_axis + SIZE - 1,
               BLACK);

    // Left
    if (walls[3])
      DrawLine(x_axis - 1, y_axis - 2, x_axis, y_axis + SIZE, BLACK);
  }

  /**
   * For highlighting the cell.
   */
  void highlight() {
    DrawRectangle(x_axis, y_axis, SIZE - 2, SIZE - 2, ORANGE);
  }
};

/**
 * This function is used to convert 2D index into 1D index.
 */
int getIndex(int i, int j) {
  if (i < 0 || i >= ROWS || j < 0 || j >= ROWS)
    return -1;
  return i * ROWS + j;
}

/**
 * This function is used to get unvisited random neighbor.
 *
 * @param grids A vector of Cell objects representing the grid.
 * @param currentIndex The index of the current cell within the grid.
 * @return The index of an unvisited neighbor cell, or -1 if no unvisited
 * neighbors are found.
 *
 */
int getURN(std::vector<Cell> grids, int currentIndex) {

  int i = currentIndex / ROWS;
  int j = currentIndex % ROWS;

  std::vector<int> neighborIndex;

  int top = getIndex(i, j - 1);
  if (top != -1 && !grids.at(top).visitedCell)
    neighborIndex.push_back(top);

  int right = getIndex(i + 1, j);
  if (right != -1 && !grids.at(right).visitedCell)
    neighborIndex.push_back(right);

  int bottom = getIndex(i, j + 1);
  if (bottom != -1 && !grids.at(bottom).visitedCell)
    neighborIndex.push_back(bottom);

  int left = getIndex(i - 1, j);
  if (left != -1 && !grids.at(left).visitedCell)
    neighborIndex.push_back(left);

  if (neighborIndex.size() > 0) {
    int r = GetRandomValue(0, neighborIndex.size() - 1);
    return neighborIndex.at(r);
  }

  return -1;
}

/**
 * @brief This function is used to remove walls
 *
 * @param grids A vector of Cell objects referencing the grid.
 * @param c The current cell withing the grids.
 * @param n The next cell withing th grids.
 *
 */
void removeWalls(std::vector<Cell> &grids, int c, int n) {
  // c = current, n = next
  if (c < n) {
    if (c - n == -1) {
      // c.bottom, n.top
      grids.at(c).walls[2] = false;
      grids.at(n).walls[0] = false;
    } else {
      // c.right, n.left
      grids.at(c).walls[1] = false;
      grids.at(n).walls[3] = false;
    }
  } else {
    if (c - n == 1) {
      // c.top, n.bottom
      grids.at(c).walls[0] = false;
      grids.at(n).walls[2] = false;
    } else {
      // c.left, n.right
      grids.at(c).walls[3] = false;
      grids.at(n).walls[1] = false;
    }
  }
}

/**
 * @brief This function is used  to create the maze.
 *
 * @param grids A vector of Cell objects referencing the grids.
 * @param currentIndex The index of current cell withing the grid.
 * @param stack A stack of Cell objects referencing the stack.
 * @return Returns either the maze has been generate or not as 'true' or
 * 'false'.
 */
bool createMaze(std::vector<Cell> &grids, int &currentIndex,
                std::stack<int> &stack) {
  // Taking the random unvisited neighbor
  int next = getURN(grids, currentIndex);
  if (next != -1) {
    // Remove wall from current and next
    removeWalls(grids, currentIndex, next);

    // Setting the current cell as the next cell
    currentIndex = next;

    // Pushing to stack
    stack.push(currentIndex);

    // Marking the current cell as visited
    grids.at(currentIndex).visitedCell = true;
  } else if (!stack.empty()) {
    currentIndex = stack.top();
    stack.pop();
  } else {
    return true;
  }

  return false;
}

/**
 * @brief This function is used to draw the maze.
 *
 * @param grids A vector of Cell objects representing the grids.
 *
 */
void drawMaze(std::vector<Cell> grids) {
  for (int i = 0; i < grids.size(); i++) {
    grids.at(i).draw();
  }
}

/**
 * @brief This function is used to get the NextPossiblePath(NPP)
 *
 * @param grids A vector of Cell objects representing grids.
 * @param currentIndex The index of current cell within the grids.
 * @return The next possible path for path finding algorithm.
 *
 */
int getNPP(std::vector<Cell> grids, int currentIndex) {
  int i = currentIndex / ROWS;
  int j = currentIndex % ROWS;

  int top = getIndex(i, j - 1);
  if (top != -1 && !grids.at(top).visitedPath &&
      !grids.at(currentIndex).walls[0]) {
    return top;
  }

  int right = getIndex(i + 1, j);
  if (right != -1 && !grids.at(right).visitedPath &&
      !grids.at(currentIndex).walls[1]) {
    return right;
  }

  int bottom = getIndex(i, j + 1);
  if (bottom != -1 && !grids.at(bottom).visitedPath &&
      !grids.at(currentIndex).walls[2]) {
    return bottom;
  }

  int left = getIndex(i - 1, j);
  if (left != -1 && !grids.at(left).visitedPath &&
      !grids.at(currentIndex).walls[3]) {
    return left;
  }

  return -1;
}

/**
 * @brief This function is used to find path from start to end in the maze.
 *
 * @param
 */
void findPath(int &start, int &current, int &end, std::vector<Cell> &grids,
              std::stack<int> &pathStack) {
  // Taking Starting position from user
  if (start == -1) {
    DrawText("Pick START Position", 200, WIDTH / 2 - 5, 40, WHITE);
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
      int i = GetMouseX() / SIZE;
      int j = GetMouseY() / SIZE;
      start = getIndex(i, j);
      current = start;
      pathStack.push(current);
      grids.at(start).visitedPath = true;
      grids.at(start).foundPath = true;
      ClearBackground(BLACK);
    }
  } else {
    // Taking Ending position from user
    if (end == -1) {
      DrawText("Pick END Position", 200, WIDTH / 2 - 5, 40, WHITE);
      if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        int i = GetMouseX() / SIZE;
        int j = GetMouseY() / SIZE;
        end = getIndex(i, j);
        ClearBackground(BLACK);
      }
    } else {
      if (current != end) {
        int next = getNPP(grids, current);
        if (next != -1) {
          current = next;
          pathStack.push(current);
          grids.at(current).visitedPath = true;
          grids.at(current).foundPath = true;
        } else {
          if (!pathStack.empty()) {
            grids.at(current).foundPath = false;
            pathStack.pop();
            current = pathStack.top();
          }
        }
      } else {
        DrawText("Path FOUND!", 230, WIDTH / 2, 50, WHITE);
      }
      grids.at(start).highlight();
      grids.at(end).highlight();
    }
  }
}

  //### The MAIN function ###//
  int main() {

    SetTargetFPS(60);

    // Initializing rows and cols
    int rows = WIDTH / SIZE;
    int cols = WIDTH / SIZE;

    // Declaring a grids variable to store all the Cell objects.
    std::vector<Cell> grids;
    // Declaring a stack to track the cells [ for backtracking ]
    std::stack<int> stack;

    // Assigning the objects of Cell with index as 'i' and 'j' the grids
    for (int i = 0; i < rows; i++) {
      for (int j = 0; j < cols; j++) {
        grids.push_back(Cell(i, j));
      }
    }

    // Creating an index to keep track of the index of current Cell.
    int currentIndex = 0;
    stack.push(currentIndex);
    grids.at(currentIndex).visitedCell = true;

    // These are for DFS Path finding algorithm
    int start = -1;
    int current = -1;
    int end = -1;
    // This stack is used to store the indexes of actual path between start and
    // end.
    std::stack<int> pathStack;

    InitWindow(WIDTH, WIDTH, "MazeGenerator");

    while (!WindowShouldClose()) {
      BeginDrawing();
      ClearBackground(BLACK);

      // Drawing the Maze
      drawMaze(grids);

      // Creating the Maze if not created
      if (!createMaze(grids, currentIndex, stack)) {
        grids.at(currentIndex).highlight();
      } 
      else {
        // Maze has been generated
        // Apply DFS algorithm here.
        findPath(start, current, end, grids, pathStack);
      }
    EndDrawing();
  }

  CloseWindow();

  return 0;
}
