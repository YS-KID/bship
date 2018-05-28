#include <stdio.h>
#include <stdlib.h>
#include <my-ipc.h>
#include <client-side.h>
#include <redundant.h>
#include <public.h>

const char myName[] = "1714311413";
const char deployment[] = "Bc3c4c5c6 Cc0d0e0 Cc8d8e8 Di3i4 Da2a3 Dc0d0 Sg0 Si5 Sg5 Sg7 ";

enum ship {
  UNKNOWN,
  ROCK,
  NOSHIP,
  BSHIP,
  CSHIP,
  DSHIP,
  SSHIP
};

int f_B = 0;
int f_C = 0;
int f_D = 0;
int tern = 1;

int cur_x,cur_y;
int cur2_x,cur2_y;
int cur3_x,cur3_y;
int cur4_x,cur4_y;

enum ship enemy_board[BD_SIZE][BD_SIZE] = {};    // BD_SIZE is 9 (defined in public.h)

char Cur_line = 'N';
char Cur2_line = 'N';
char Cur3_line = 'N';
char Cur4_line = 'N';

void respond_with_name(void)
{
  char *str = (char *)malloc(sizeof(myName));
  strcpy(str, myName);
  send_to_ref(str);
  free(str);
}

void respond_with_deployment(void)
{
  char *str = (char *)malloc(sizeof(deployment));
  strcpy(str, deployment);
  send_to_ref(str);
  free(str);
}


void init_board(void)
{
  //======kokokara======
  for(int i = 0; i < 9; i++){
    for(int j = 0; j < 9; j++){
      enemy_board[i][j] = UNKNOWN;
    }
  }
  enemy_board[0][0] = ROCK;
  enemy_board[1][0] = ROCK;
  enemy_board[0][1] = ROCK;
  enemy_board[8][8] = ROCK;
  enemy_board[8][7] = ROCK;
  enemy_board[7][8] = ROCK;
  enemy_board[0][8] = ROCK;
  enemy_board[0][7] = ROCK;
  enemy_board[1][8] = ROCK;
  enemy_board[7][0] = ROCK;
  enemy_board[8][0] = ROCK;
  enemy_board[8][1] = ROCK;

  //======kokomade======
}

void random_shot(int *x, int *y){
  (*x) = rand() % BD_SIZE;
  (*y) = rand() % BD_SIZE;
}

void B_shot(int *x, int *y){
  (*x) = rand() % BD_SIZE;
  (*y) = rand() % BD_SIZE;

}

void C_shot(int *x, int *y){
  (*x) = rand() % BD_SIZE;
  (*y) = rand() % BD_SIZE;
  
}

void D_shot(int *x, int *y){
  if(tern == 1){
    *x = cur_x + 1;
    *y = cur_y;
    tern = 2;
  }else if(tern == 2){
    if(Cur_line == 'N'){
      *x = cur2_x;
      *y = cur2_y - 1;
      tern = 3;
    }else if(Cur_line == 'D'){
      f_D = 0;
      tern = 1;
    }
  }else if(tern == 3){
    if(Cur_line == 'N'){
      *x = cur3_x - 1;
      *y = cur3_y;
      tern = 4;
    }else if(Cur_line == 'D'){
      f_D = 0;
      tern = 1;
    }
  }else if(tern == 4){
      *x = cur4_x;
      *y = cur4_y + 1;
      f_D = 0;
      tern = 1;
  }
}

void respond_with_shot(void)
{
  char shot_string[MSG_LEN];
  int x, y;
  
  while (TRUE)
  {
    if(f_B == 1){
      B_shot(&x, &y);
    }else if(f_C == 1){
      C_shot(&x, &y);
    }else if(f_D == 1){
      D_shot(&x, &y);
      if(f_D == 0){
        random_shot(&x, &y);
      }
    }else{
      random_shot(&x, &y);
    }
    if(enemy_board[x][y] == UNKNOWN){
      break;
    }
  }
  printf("%c\n",Cur_line);
  printf("[%s] shooting at %d%d ... ", myName, x, y);
  sprintf(shot_string, "%d%d", x, y);
  send_to_ref(shot_string);
  cur4_x = cur3_x;
  cur3_x = cur2_x;
  cur2_x = cur_x;
  cur_x = x;
  cur4_y = cur3_y;
  cur3_y = cur2_y;
  cur2_y = cur_y;
  cur_y = y;
}
/*
void empty_mass(int x, int y){
  if(x > 8){
    x = 8;
  }else if(x < 0){
    x = 0;
  }else if(y > 8){
    y = 8;
  }else if(y < 0){
    y = 0;
  }
}
*/

void corner_NOSHIP(int x, int y){
  int dx[] = {1,1,-1,-1};
  int dy[] = {1,-1,1,-1};
  for(int i = 0; i < 4; i++){
    enemy_board[x+dx[i]][y+dy[i]] = NOSHIP;
  }
}

void record_result(int x,int y,char line[])
{ 
  Cur4_line = Cur3_line;
  Cur3_line = Cur2_line;
  Cur2_line = Cur_line;
  if(line[13]=='B')
  {
    enemy_board[x][y] = BSHIP;
    corner_NOSHIP(x,y);
    Cur_line = 'B';
    f_B = 1;
  }
  else if(line[13]=='C')
  {
    enemy_board[x][y] = CSHIP;
    corner_NOSHIP(x,y);
    Cur_line = 'C';
    f_C = 1;
  }
  else if(line[13]=='D')
  {
    enemy_board[x][y] = DSHIP;
    corner_NOSHIP(x,y);
    Cur_line = 'D';
    f_D = 1;
  }
  else if(line[13]=='S')
  {
    enemy_board[x][y] = SSHIP;
    enemy_board[x][y-1] = NOSHIP;
    enemy_board[x-1][y] = NOSHIP;
    enemy_board[x][y+1] = NOSHIP;
    enemy_board[x+1][y] = NOSHIP;
    corner_NOSHIP(x,y);
    Cur_line = 'S';
  }
  else if(line[13]=='R')
  {
    enemy_board[x][y] = ROCK;
    Cur_line = 'R';
  }
  else
  {
    enemy_board[x][y] = NOSHIP;
    Cur_line= 'N';
  }
}

void print_board(void){
  int ix, iy;

  for (iy = BD_SIZE - 1; iy >= 0; iy--)
  {
    printf("%2d ", iy);
    for (ix = 0; ix < BD_SIZE; ix++)
    {
      switch(enemy_board[ix][iy])
      {
        case UNKNOWN:
          printf("U ");
          break;
        case NOSHIP:
          printf("N ");
          break;
        case ROCK:
          printf("R ");
          break;
        case BSHIP:
          printf("B ");
          break;
        case CSHIP:
          printf("C ");
          break;
        case DSHIP:
          printf("D ");
          break;
        case SSHIP:
          printf("S ");
          break;
        default:
          break;
      }
    }
    printf("\n");
  }

  printf("  ");
  for (ix = 0; ix < BD_SIZE; ix++)
  {
    printf("%2d", ix);
  }
  printf("\n\n");
}

void handle_messages(void)
{
  char line[MSG_LEN];

  srand(getpid());
  init_board();
  
  while (TRUE)
  {
    receive_from_ref(line);

    if(message_has_type(line, "name?"))
    {
      respond_with_name(); 
    }
    else if(message_has_type(line, "deployment?"))
    {
       respond_with_deployment(); 
    }
    else if(message_has_type(line, "shot?"))
    {
      respond_with_shot(); 
    }
    else if(message_has_type(line, "shot-result:"))
    {
      record_result(cur_x,cur_y,line);
      printf("[%s] result: %c\n", myName, line[13]);
      print_board();
    }
    else if(message_has_type(line, "end:"))
    {
      break;
    }
    else
    {
      printf("[%s] ignoring message: %s", myName, line);
    }
  }
}

int main()
{
  client_make_connection();
  handle_messages();
  client_close_connection();
  return 0;
}
