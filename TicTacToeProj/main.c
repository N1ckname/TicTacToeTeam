#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define WIDTH 3
#define HEIGHT  3

/* States of board cells.
Attention! if EMPTY needed to be changed, initializer of board, line_capt and cols_capt should be written */
enum {/*NO_WIN = -1,*/ EMPTY = 0, P1 = 1, P2 = 2};

int create_board (int*** board) {

    *board = (int**) calloc(HEIGHT, sizeof(int*));           //calloc check!!
    //printf("0x%x\n", (*board));
    
    int* tmp = (int*) calloc(HEIGHT*WIDTH, sizeof(int));    //calloc check!!

    for (int i=0; i < HEIGHT; ++i) {
        (*board)[i] = &tmp[i*WIDTH];
        //printf("0x%x\n", (*board)[i]);
    }

    return 0;
}

void destroy_board (int** board) {
    free(board[0]);
    free(board);    //check it!
    board = NULL;
}

void print_board (int** board) {
    for (int line=0; line < HEIGHT; ++line) {
        for (int column=0; column < WIDTH; ++column) {
            printf("%d|", board[line][column]); //some error checks?
        }
        printf("\n");
        
        for (int i = 0; i < WIDTH; ++i) {
            printf("- ");
        }
        printf("\n");
    }
    printf("\n");
}

void print_menu (void) {
    printf("1.E to start game\n");
    printf("2.Q to exit game\n");
    printf("Game tip: WASD sequence + Enter determines cell to be chosen\n");
}

void clearbuf(void) {    //error checks?
    char tmp = ' ';

    while ((tmp = getchar()) != '\n' && tmp != EOF) {}   
}

/*For choosing cell use WASD and wasd, q and Q - to qiut game. Moves are calculated as on toroidal board.*/
int user_game_input (int* line, int* column, const int user_id) {    //error checks?
    assert(line != NULL && column != NULL);
    *line = 0;  //starting cell is always at top left corner
    *column = 0;

    printf("User's %d move:", user_id);

    char input = ' ';
    while ((input = getchar()) != '\n' && input != EOF) {
        switch (input) {
            /*case e: case E: {   //change to define or enum
                printf("User decided to quit\n");
                clearbuf();
                return 1;
            }*/
            case 'q': case 'Q': {
                printf("User decided to quit\n");
                //clearbuf();
                return 0;
            }
            case 'w': case 'W': {
                *line += HEIGHT - 1;
                *line %= HEIGHT;  
                break;
            }
            case 's': case 'S': {
                *line += 1;
                *line %= HEIGHT;
                break;
            }
            case 'a': case 'A': {
                *column += WIDTH - 1;
                *column %= WIDTH;  
                break;
            }
            case 'd': case 'D': {
                *column += 1;
                *column %= WIDTH;
                break;
            }
            default: {break;}    //ignoring invalid input            
        }
    }
    printf("\n");

    assert(*line >= 0 && *line < HEIGHT);
    assert(*column >= 0 && *column < WIDTH);

    return 1;
}

int user_start_input () {    //error checks?
   
    char input = ' ';
    while ((input = getchar()) != '\n' && input != EOF) {
        switch (input) {
            case 'e': case 'E': {   //change to define or enum
                printf("Starting game\n");
                clearbuf();
                return 1;
            }
            case 'q': case 'Q': {
                printf("User decided to quit\n");
                //clearbuf();
                return 0;
            }
            default: {break;}    //ignoring invalid input            
        }
    }

    return 0;
}

/*returns winners id (id = state from enum of states of board cell) or 0 if no winner yet*/
int check_to_win (int** board, const int user_id) {
    int sum_of_line = 0;
    int sum_of_right_diag = 0;
    int sum_of_left_diag = 0;

    for (int line = 0; line < HEIGHT; ++line) {     //checking lines and diagonals
        for (int col = 0; col < WIDTH; ++col) {
            
            if (board[line][col] == user_id) {

                sum_of_line += board[line][col];
                
                if (line == col) {sum_of_left_diag += board[line][col];}
                
                if ( (line + col) == (WIDTH - 1) ) {sum_of_right_diag += board[line][col];}
            }   
        }
        if (sum_of_line == user_id * WIDTH) {return user_id;}
        else {sum_of_line = 0;}
    }

    if (sum_of_right_diag == (user_id * WIDTH) || sum_of_left_diag == (user_id * WIDTH)) {return user_id;}

    sum_of_line = 0;    

    for (int cols = 0; cols < WIDTH; ++cols) {          //checking columns
        for (int lines = 0; lines < HEIGHT; ++lines) {
            if (board[lines][cols] == user_id) {sum_of_line += board[lines][cols];}
        }
        if (sum_of_line == user_id * HEIGHT) {return user_id;}
        else {sum_of_line = 0;}
    }

    return 0;
}

int main (int argc, char** argv) {

    assert(WIDTH == HEIGHT);    //this version supports only square field

    print_menu();
    if(!user_start_input()) {return 0;}
    
    int** board = NULL;
    //printf("0x%x\n", board);
    create_board(&board);
    //printf("0x%x\n", board);

    int line = 0;   //both represent coordinates of user's move
    int column = 0;
    int move_counter = 0;   //counts moves of both users
    int winner_id = 0;
    int cur_user = P1;

    while (winner_id == 0 && move_counter < (WIDTH*HEIGHT)) {
        print_board(board);
        while (1) {
            if (!user_game_input(&line, &column, cur_user)) {
                destroy_board(board);
                return 0;
            }
            if (board[line][column] == EMPTY) {
                board[line][column] = cur_user;
                ++move_counter;
                break;
            }
            else {
                printf("Cell is occupied. Please, choose another cell.\n");
            }
        }
        
        winner_id = check_to_win(board, cur_user);

        switch (cur_user) {     //switching players
            case P1: {
                cur_user = P2;
                break;
            }
            case P2: {
                cur_user = P1;
                break;
            }
            default: {
                assert(0);
            }
        }       
    }
    
    print_board(board);
      
    if (winner_id != 0) {printf("Player %d has won!\n", winner_id);}
    else {printf("It's a draw!\n");}
    
    destroy_board(board);
    return 0;
}

