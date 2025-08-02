#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define HEIGHT 20
#define WIDTH 7

#define TOTAL_STEP 20


int board[HEIGHT][WIDTH] ={0};

int next_step[TOTAL_STEP] = {2};

void generate_step(int next_step[TOTAL_STEP]){
  for(int i=1; i<TOTAL_STEP; i++){
    if(rand()%2)
      next_step[i] = next_step[i-1] + 1;
    else 
      next_step[i] = next_step[i-1] - 1;
  }
}

void draw(int next_step[TOTAL_STEP], int now, int direction){
  
  for(int i=0; i<HEIGHT; i++){
    for(int j=0; j<WIDTH; j++){
      board[i][j] = 0;
      }
  }

  for(int i=HEIGHT-1; i>=0; i--){
    if(now + i > TOTAL_STEP -1){
      for(int j=0; j<WIDTH; j++)
        board[HEIGHT-1-i][j] = 1;
    }
    else if(next_step[now + i] - next_step[now] < 4)
      board[HEIGHT-1-i][next_step[now + i] - next_step[now] + WIDTH/2] = 1;
    else 
      for(int j=0; j<WIDTH; j++)
        board[HEIGHT-1-i][j] = 0;
  }

  printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
  // system("cls");
  printf("step : %d\n",now);
  
  for(int i=0; i<HEIGHT-1; i++){
    for(int j=0; j<WIDTH; j++){
      if(board[i][j])
        printf("____");
      else printf("    ");
    }printf("\n\n");
  } 
  if(direction == 1)
    printf("            __(}            \n\n");
  else if(direction == -1)
    printf("            {)__            \n\n");
  
}

// 계단 생성 테스트용
void print_step(int next_step[TOTAL_STEP]){
  for(int i=0; i<TOTAL_STEP; i++){ printf("%2d|",i);
    for(int j=-20; j<20; j++){
      if(next_step[i]==j)
        printf("%2d",next_step[i]);
      else printf(" ");
    }printf("\n");
  }
}

int main(void) {
  srand(time(NULL));
  generate_step(next_step);
  char input;
  int step = 0;
  int direction = 1;

  while(1){
    draw(next_step,step,direction);
    
    input = getchar();
    //getchar();
    if(step == TOTAL_STEP-1){
      printf("\n\n-------------CLEAR----------------\n\n");
      return 0;
    }
    ///////////////////방향키 버전////////////////
    // if(input == 'd' && next_step[step+1] - next_step[step] == 1)
    //   step++;
    // else if (input == 'a' && next_step[step+1] - next_step[step] == -1)
    //   step++;
    // else{
    //   printf("\n\n-----------GAME OVER--------------\n\n");
    //   return 0;
    // }
    /////////////////////////////////////////////
    if(input != '\n')
      direction *= -1;
    else if (input == '\n' && next_step[step+1] - next_step[step] == direction)
      step++;
    else{
      printf("\n\n-----------GAME OVER--------------\n\n");
      return 0;
    }
    
  }
  print_step(next_step);
  return 0;
}
