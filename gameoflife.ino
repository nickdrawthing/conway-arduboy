#include "Arduboy.h"

Arduboy aboy;

int randomSeedCounter = 0;

enum menu {
  mainMenu,
  life
};

menu menuObject = mainMenu;

byte rulesNum = 2;

bool grid[32][16];
bool newgrid[32][16]; 

void setup() {
  // put your setup code here, to run once:
  aboy.begin();
  aboy.setFrameRate(5);
}

void loop() {
  if (!(aboy.nextFrame())) return;
  aboy.clear();

  switch (menuObject) {
    case mainMenu: doMainMenu(); break;
    case life: doLife(); break;
  }

  aboy.display();
}

void doMainMenu() {
  randomSeedCounter ++;
  if (aboy.pressed(A_BUTTON)) {
    randomSeed(randomSeedCounter);
    menuObject = life;
    randomizeGrid();
  }
  aboy.setCursor((WIDTH/2)-36, (HEIGHT/2)-9);
  aboy.print("RANDOMIZING!");
  aboy.setCursor((WIDTH/2)-21, (HEIGHT/2)+1);
  aboy.print("PRESS A");
}

void doLife() {
  if (aboy.pressed(A_BUTTON)) randomizeGrid();
  for (byte qq = 0; qq < 32; qq++) {
    for (byte ww = 0; ww < 16; ww++) {

      // check for number of live neighbours
      byte nbrs = numberOfLiveNeighbours(qq, ww);

      if (grid[qq][ww] == 0) {
        if (nbrs == (1+rulesNum)){
          newgrid[qq][ww] = 1;
        }
      } else {
        if (nbrs < (0+rulesNum)) {
          newgrid[qq][ww] = 0;
        }
        if (nbrs > (1+rulesNum)){
          newgrid[qq][ww] = 0;
        }
        if (nbrs == (0+rulesNum) || nbrs == (1+rulesNum)){
          newgrid[qq][ww] = 1;
        }
      }
    }
  }
  if (aboy.pressed(UP_BUTTON)){
    rulesNum++;
    if (rulesNum == 6){
      rulesNum = 0;
    }
  } else if (aboy.pressed(DOWN_BUTTON)){
    if (rulesNum >0){
      rulesNum--;
    } else if (rulesNum == 0){
      rulesNum = 5;
    }
  }
  
  for(byte xx = 0; xx < 32; xx++){
    for(byte zz = 0; zz < 16; zz++){
      grid[xx][zz] = newgrid[xx][zz];
      newgrid[xx][zz] = 0;
    }
  }
  
  renderGrid(); 
}

void randomizeGrid() {
  for (byte xz=0; xz < 32; xz++){
    for (byte vc=0; vc < 16; vc++){
      grid[xz][vc] = 0;
    }
  }
  for (byte i = 0; i < random(20); i++) {
    byte a = random(32);
    byte b = random(16);
    grid[a][b] = 1;
  }
  for (byte kk = 0; kk < random(3)+1; kk++){
    byte rnd1 = random(30);
    byte rnd2 = random(12);
    
    grid[0+rnd1][0+rnd2] = 1;
    grid[0+rnd1][1+rnd2] = 1;
    grid[1+rnd1][0+rnd2] = 1;
    grid[1+rnd1][1+rnd2] = 1;
  
    grid[2+rnd1][2+rnd2] = 1;
    grid[2+rnd1][3+rnd2] = 1;
    grid[3+rnd1][2+rnd2] = 1;
    grid[3+rnd1][3+rnd2] = 1;
  }
  for (byte jj = 0; jj < random(3)+1; jj++){
    byte rnd1 = random(30);
    byte rnd2 = random(12);
    grid[0+rnd1][0+rnd2] = 1;
    grid[2+rnd1][0+rnd2] = 1;
    grid[1+rnd1][1+rnd2] = 1;
    grid[2+rnd1][1+rnd2] = 1;
    grid[1+rnd1][2+rnd2] = 1;
  }
}

byte numberOfLiveNeighbours(byte col, byte row) {

  byte live = 0;

  for (byte pp = (0); pp < 3; pp++) {
    for (byte oo = (0); oo < 3; oo++) {
      if ((col+pp) > 0 && (col+pp) < 32 && (row+oo) > 0 && (row+oo) < 16){
        if (pp == 1 && oo == 1) {
          
        } else {
          if (grid[(col + pp)-1][(row + oo)-1] == 1) {
            live ++;
          }
        }
      }
    }
  }

  return live;
}

void renderGrid() {

  for (byte ff = 0; ff < 32; ff++){
    for (byte dd = 0; dd < 16; dd++){
      if (grid[ff][dd] == 1){
        aboy.fillRect(ff*4, dd*4, 4, 4, 1);
      }
    }
  }
  aboy.fillRect(0,0,9,11,0);
  aboy.drawRect(0,0,9,11,1);
  
  aboy.setCursor(2,2);
  aboy.print(rulesNum);
}

//Halt and Catch Fire
