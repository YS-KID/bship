#include <stdio.h>
#include <stdlib.h>
#include <my-ipc.h>
#include <client-side.h>
#include <redundant.h>
#include <public.h>

const char myName[] = "1714311413";
const char deployment[] = "Ba3a4a5a6 Cc0d0e0 Cc8d8e8 Di2i3 Di5i6 Dg1g2 Sg6 Sd3 Sg4 Se5 ";

enum ship {
  UNKNOWN,
  ROCK,
  NOSHIP,
  BSHIP,
  CSHIP,
  DSHIP,
  SSHIP
};

int cur_x,cur_y;
int point_x,point_y;
enum ship enemy_board[BD_SIZE][BD_SIZE] = {UNKNOWN};    // BD_SIZE is 9 (defined in public.h)

int flag_B = 0;
int flag_C = 0;
int flag_D = 0;

int tern = 0;

char Cur_line;

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

void random_shot(int* x, int* y){
  *x = rand() % BD_SIZE;
  *y = rand() % BD_SIZE;
}

void B_shot(int* x, int* y){
  int dx[] = {1,0,-1,0};
  int dy[] = {0,-1,0,1};
  if(tern >= 4){
    flag_B = 0;
  }
  if(tern == 0){
    *x = point_x + dx[tern];
    *y = point_y + dy[tern];
    tern++;
  }else if(Cur_line == 'B'){
    flag_B = 0;
    tern = 0;
  }else{
    *x = point_x + dx[tern];
    *y = point_y + dy[tern];
    tern++;
  }
}

void C_shot(int* x, int* y){
  int dx[] = {1,0,-1,0};
  int dy[] = {0,-1,0,1};
  if(tern >= 4){
    flag_C = 0;
  }
  if(tern == 0){
    *x = point_x + dx[tern];
    *y = point_y + dy[tern];
    tern++;
  }else if(Cur_line == 'C'){
    flag_C = 0;
    tern = 0;
  }else{
    *x = point_x + dx[tern];
    *y = point_y + dy[tern];
    tern++;
  }
}

void D_shot(int* x, int* y){
  int dx[] = {1,0,-1,0};
  int dy[] = {0,-1,0,1};
  if(tern >= 4){
    flag_D = 0;
  }
  if(tern == 0){
    *x = point_x + dx[tern];
    *y = point_y + dy[tern];
    tern++;
  }else if(Cur_line == 'D'){
    flag_D = 0;
    tern = 0;
  }else{
    *x = point_x + dx[tern];
    *y = point_y + dy[tern];
    tern++;
  }
}

void respond_with_shot(void)
{
  char shot_string[MSG_LEN];
  int x, y;
  while (TRUE)
  {
    if(flag_B == 1){
      B_shot(&x, &y);
    }else if(flag_C == 1){
      C_shot(&x, &y);
    }else if(flag_D == 1){
      D_shot(&x, &y);
    }else{
      random_shot(&x, &y);
    }
    if(x>=0&&x<=8&&y>=0&&y<=8){
      if(enemy_board[x][y] == UNKNOWN){
        break;
      }else{
        Cur_line = 'N';
      }
    }
  }
  printf("[%s] shooting at %d%d ... ", myName, x, y);
  sprintf(shot_string, "%d%d", x, y);
  send_to_ref(shot_string);
  cur_x = x;
  cur_y = y;
}

void corner_NOSHIP(int x, int y){
  int dx[] = {1,1,-1,-1};
  int dy[] = {1,-1,1,-1};
  for(int i = 0; i < 4; i++){
    if(enemy_board[x+dx[i]][y+dy[i]] == UNKNOWN){
      enemy_board[x+dx[i]][y+dy[i]] = NOSHIP;
    }
  }
}

void corner_NOSHIP_S(int x, int y){
  int dx[] = {1,1,1,0,-1,-1,-1,0};
  int dy[] = {1,0,-1,-1,-1,0,1,1};
  for(int i = 0; i < 8; i++){
    if(enemy_board[x+dx[i]][y+dy[i]] == UNKNOWN){
      enemy_board[x+dx[i]][y+dy[i]] = NOSHIP;
    }
  }
}

void record_result(int x,int y,char line[])
{
  Cur_line = line[13];
  if(line[13]=='B')
  {
    //====kokokara====
    flag_B = 1;
    enemy_board[x][y] = BSHIP;
    corner_NOSHIP(x, y);
    point_x = x;
    point_y = y;
    tern = 0;
    //====kokomade====
  }
  else if(line[13]=='C')
  {
    //====kokokara====
    flag_C = 1;
    enemy_board[x][y] = CSHIP;
    corner_NOSHIP(x, y);
    point_x = x;
    point_y = y;
    tern = 0;
    //====kokomade====
  }
  else if(line[13]=='D')
  {
    //====kokokara====
    flag_D = 1;
    enemy_board[x][y] = DSHIP;
    corner_NOSHIP(x, y);
    point_x = x;
    point_y = y;
    tern = 0;
    //====kokomade====
  }
  else if(line[13]=='S')
  {
    //====kokokara====
    enemy_board[x][y] = SSHIP;
    corner_NOSHIP_S(x, y);
    //====kokomade====
  }
  else if(line[13]=='R')
  {
    //====kokokara====
    enemy_board[x][y] = ROCK;
    //====kokomade====
  }
  else
  {
    //====kokokara====
    enemy_board[x][y] = NOSHIP;
    Cur_line = 'N';
    //====kokomade====
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
