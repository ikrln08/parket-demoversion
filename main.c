#include <stdio.h>
const int ROWS = 1000;
const int COLUMNS = 2000;
int warehouse[1000][2000];
int ROW, COL;
int allids[1000];
int idscnt;
struct Object {
  int id;
  int row;
  int col;
  int height;
  int width;
};
struct Object objects[1000];
int loadWarehouse(const char filename[], int warehouse[][COLUMNS]) {
  FILE *filei = fopen(filename, "r");
  if (filei == NULL) {
    printf("File error\n");
    return 1;
  }
  if (fscanf(filei, "%d %d", &ROW, &COL) != 2) {
    printf("File error\n");
    fclose(filei);
    return 1;
  }
  // printf("%d %d\n", ROW, COL);
  for (int i = 0; i < ROW; i++) {
    for (int j = 0; j < COL; j++) {
      if (fscanf(filei, "%d", &warehouse[i][j]) != 1) {
        printf("File error\n");
        fclose(filei);
        return 1;
      }
    }
  }
  fclose(filei);
  return 0;
}

void printArea(int warehouse[][COLUMNS], int startRow, int startCol, int height,
               int width) {
  for (int i = startRow - 1; i < startRow + height - 1; i++) {
    for (int j = startCol - 1; j < startCol + width - 1; j++) {
      printf("%d ", warehouse[i][j]);
    }
    printf("\n");
  }
  printf("\n");
}

int isAreaFree(int warehouse[][COLUMNS], int row, int col, int height,
               int width, int objectId

) {
  if (row + height - 1 > ROW) {
    return 0;
  }
  if (col + width - 1 > COL) {
    return 0;
  }
  for (int i = row - 1; i < row + height - 1; i++) {
    for (int j = col - 1; j < col + width - 1; j++) {
      if (warehouse[i][j] != 0) {
        return 0;
      }
    }
  }
  printf("\n");
  return 1;
}

int addObject(int warehouse[][COLUMNS], int objectId, int row, int col,
              int height, int width) {
  if (!isAreaFree(warehouse, row, col, height, width, objectId)) {
    return 1;
  }
  if (row < 1 || row > ROW) {
    return 1;
  }
  if (col < 1 || col > COL) {
    return 1;
  }
  objects[objectId].id = objectId;
  objects[objectId].row = row;
  objects[objectId].col = col;
  objects[objectId].height = height;
  objects[objectId].width = width;
  for (int i = row - 1; i < row + height - 1; i++) {
    for (int j = col - 1; j < col + width - 1; j++) {
      warehouse[i][j] = objectId;
    }
  }
  printf("\n");
  allids[idscnt] = objectId;
  idscnt++;
  return 0;
}

int removeObject(int warehouse[][COLUMNS], int objectId) {
  int row, col, height, width;
  row = objects[objectId].row;
  col = objects[objectId].col;
  height = objects[objectId].height;
  width = objects[objectId].width;
  for (int i = row - 1; i < row + height - 1; i++) {
    for (int j = col - 1; j < col + width - 1; j++) {
      warehouse[i][j] = 0;
    }
  }
  printf("\n");
  return 0;
}
int findObject(int warehouse[][COLUMNS], int objectId, int row, int col,
               int height, int width) {
  if (row < 1 || row + height > ROW + 1) {
    return 0;
  }
  if (col < 1 || col + width > COL + 1) {
    return 0;
  }
  for (int i = row - 1; i < row + height - 1; i++) {
    for (int j = col - 1; j < col + width - 1; j++) {
      if (warehouse[i][j] != 0 && warehouse[i][j] != objectId)
        return 0;
    }
  }
  return 1;
}
int moveObject(int warehouse[][COLUMNS], int objectId, int newRow, int newCol) {
  if (!findObject(warehouse, objectId, newRow, newCol, objects[objectId].height,
                  objects[objectId].width)) {
    return 1;
  }
  removeObject(warehouse, objectId);
  objects[objectId].row = newRow;
  objects[objectId].col = newCol;
  addObject(warehouse, objectId, newRow, newCol, objects[objectId].height,
            objects[objectId].width);
  return 0;
}
int rotateObject(int warehouse[][COLUMNS], int objectId) {
  if (!findObject(warehouse, objectId, objects[objectId].row,
                  objects[objectId].col, objects[objectId].width,
                  objects[objectId].height)) {
    return 1;
  }
  removeObject(warehouse, objectId);
  addObject(warehouse, objectId, objects[objectId].row, objects[objectId].col,
            objects[objectId].width, objects[objectId].height);
  return 0;
}
int findfind(int idid, int warehouse[][COLUMNS], int h, int w);
int findFreeArea(int warehouse[][COLUMNS], int height, int width, int flag,
                 int pos) {
  // printf("%d %d\n", ROW, COL);
  for (int i = 0; i < ROW; i++) {
    for (int j = 0; j < COL; j++) {
      if (isAreaFree(warehouse, i + 1, j + 1, height, width, 0) == 1) {
        if (flag == 0)
          printf("Coordinates: %d, %d\n", i + 1, j + 1);
        else {
          printf("Rotate an object %d and put new in %d, %d\n", pos, i + 1,
                 j + 1);
          rotateObject(warehouse, pos);
        }
        return 1;
      }
    }
  }
  if (flag == 0) {
    if (findfind(0, warehouse, height, width) != 1) {
      printf("Cant rotate an object right way\n");
    }
  }
  return 0;
}
int findfind(int idid, int warehouse[][COLUMNS], int h, int w) {
  if (idid >= idscnt) {
    return 0;
  }

  if (allids[idid] == -1) {
    return findfind(idid + 1, warehouse, h, w);
  }

  if (rotateObject(warehouse, allids[idid]) == 0) {
    if (findFreeArea(warehouse, h, w, 1, allids[idid]) == 1) {
      return 1;
    }
    if (findfind(idid + 1, warehouse, h, w) == 1) {
      return 1;
    }
    rotateObject(warehouse, allids[idid]);
  }

  return findfind(idid + 1, warehouse, h, w);
}
void printStatistics(int warehouse[][COLUMNS]) {
  int nottaken = 0, taken = 0, idx = 0, f;
  int arr[100] = {0};
  for (int i = 0; i < ROW; i++) {
    for (int j = 0; j < COL; j++) {
      f = 1;
      if (warehouse[i][j] == 0) {
        nottaken++;
      }
      if (warehouse[i][j] != 0) {
        taken++;
      }
      for (int u = 0; u < idx + 10; u++) {
        if (arr[u] == warehouse[i][j]) {
          f = 0;
          break;
        }
      }
      if (f == 1) {
        arr[idx] = warehouse[i][j];
        idx++;
      }
    }
  }
  printf("\n");
  float all = taken + nottaken;
  float voltaken = (taken / all) * 100;
  printf("Statistics:\nTaken: %d\n Not taken: %d\n Volume taken: %.02f\n "
         "Objects Quantity: %d\n",
         taken, nottaken, voltaken, idx);
}

int saveWarehouse(const char filename[], int warehouse[][COLUMNS]) {
  FILE *fileo = fopen(filename, "w");
  if (fileo == NULL) {
    printf("File error\n");
    return 1;
  }
  fprintf(fileo, "%d %d\n", ROW, COL);
  for (int i = 0; i < ROW; i++) {
    for (int j = 0; j < COL; j++) {
      fprintf(fileo, "%d ", warehouse[i][j]);
    }
    fprintf(fileo, "\n");
  }
  printf("\n");
  fclose(fileo);
  return 0;
}
int main(void) {
  int n;
  while (1) {
    printf("Select option: ");
    if (scanf("%d", &n) != 1) {
      printf("Invalid input.\n");
      break;
    }
    printf("\n");
    if (n == 0) {
      printf("Exiting...\n");
      break;
    } else if (n == 1) {
      loadWarehouse("input.txt", warehouse);
      printf("Input done.\n");
    } else if (n == 2) {
      int row, col, hgt, wdt;
      printf("Enter coordinates, height, width:\n");
      if (scanf("%d %d %d %d", &row, &col, &hgt, &wdt) != 4) {
        printf("Invalid input\n");
        break;
      }
      printf("Searched area:\n");
      printArea(warehouse, row, col, hgt, wdt);
    } else if (n == 3) {
      int id, row, col, hgt, wdt;
      printf("Enter ID, coordinates, height, width:\n");
      if (scanf("%d %d %d %d %d", &id, &row, &col, &hgt, &wdt) != 5) {
        printf("Invalid input\n");
        break;
      }
      if (addObject(warehouse, id, row, col, hgt, wdt) == 1) {
        printf("Operation impossible.\n");
      } else {
        printf("Object added.\n");
      }
    } else if (n == 4) {
      int id;
      printf("Enter ID: ");
      if (scanf("%d", &id) != 1) {
        printf("Invalid input.\n");
        break;
      }
      removeObject(warehouse, id);
      printf("Object removed.\n");
    } else if (n == 5) {
      int id, l, r;
      printf("Enter ID, new coordinates:\n");
      if (scanf("%d %d %d", &id, &l, &r) != 3) {
        printf("Invalid input.\n");
        break;
      }
      if (moveObject(warehouse, id, l, r) == 1) {
        printf("Operation impossible.\n");
      } else
        printf("Object moved.\n");
    } else if (n == 6) {
      int id;
      printf("Enter ID:\n");
      if (scanf("%d", &id) != 1) {
        printf("Invalid input.\n");
        break;
      }
      if (rotateObject(warehouse, id) == 1) {
        printf("Operation impossible.\n");
      } else
        printf("Object rotated.\n");
    } else if (n == 7) {
      int id;
      printf("Enter ID:\n");
      if (scanf("%d", &id) != 1) {
        printf("Invalid input.\n");
        break;
      }
      printf("Object data: %d %d %d %d\n", objects[id].row, objects[id].col,
             objects[id].height, objects[id].width);
    } else if (n == 8) {
      int hgt, wdt;
      printf("Enter height, width:\n");
      if (scanf("%d %d", &hgt, &wdt) != 2) {
        printf("Invalid input.\n");
        break;
      }
      findFreeArea(warehouse, hgt, wdt, 0, 0);
    } else if (n == 9) {
      printStatistics(warehouse);
    } else if (n == 10) {
      saveWarehouse("output.txt", warehouse);
      printf("Saved.\n");
    }
  }
  return 0;
}