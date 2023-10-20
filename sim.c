#include <stdio.h>
#include <assert.h>
#include "test.h"
enum {
    NO = 0,
    RED = 1,
    BLUE = 2
};

typedef char board_t[15];
typedef char player_t;

void init_board(board_t board)
{
    for (int i = 0; i < 15; i++) {
        board[i] = NO;
    }
}

player_t other_player(player_t player)
{
    switch (player) {
        case RED: return BLUE;
        case BLUE: return RED;
        default: assert(0);
    }
}

void print_board(board_t board)
{
    for (int i = 0; i < 15; ++i) {
        switch (board[i]) {
            case NO: printf(" . "); break;
            case RED: printf("\x1b[31;1m R \x1b[0m"); break;
            case BLUE: printf("\x1b[34;1m B \x1b[0m"); break;
        }
    }
    printf("\n");
} 

int has_won(board_t board, player_t player){
    player_t x;
    x=other_player(player);
    int z[5]={-1,4,8,11,13};
    int lst[13]={4,7,9,10,0,3,5,6,0,2,3,0,1};
    for (int k=1;k<5;k++){

        for (int i=z[k-1]+1;i<z[k];i++){
                for (int j=i+1;j<=z[k];j++){
                    if (board[i] == x &&
                        board[j] == x &&
                        board[j + lst[i]] ==x && board[i]!=NO) {
                        return 1;
                }
            }
        }
    }
    return 0;
}


typedef struct {
    int line;
    int score;
} move_t;

move_t best_move(board_t board, player_t player, int x){
    for (int i = x; i<15; i++){
        if (board[i]==0){
            board[i]=player;
            if (!has_won(board,other_player(player))){ 
                board[i] = 0 ;
                return (move_t){i,0};
           }
            if (has_won(board,other_player(player))){
                board[i]=0;
                return best_move(board,player,i+1);
                }
            }
        }
    }




int main()
{
    int move, i;
    board_t board;
    move_t response;
    int x;
    printf("Which color you want to be? Enter 1 for RED and 2 for BLUE : ");
    scanf("%d", &x);
    player_t current;
    int chosenColor;

    if (x == 1) {
        current = RED;
    } else if (x == 2) {
        current = BLUE;
    } else {
        printf("\x1b[31;1m Invalid Color : Enter your choice again please!!! \x1b[0m");
        return 1; // Exit the program due to invalid choice
    }

    init_board(board);

    while (1) {
        
        print_board(board);
        printf("\n");

            printf("0  1  2  3  4  5  6  7  8  9 10 11 12 13 14\n");
            printf("\nEnter your move: ");
            scanf("%d", &move);
            i = move;
            if (i >= 0 && i < 15 && board[i] == 0) {
                board[i] = current;
            } 
            else{
                printf("\x1b[31;1m Invalid Move \x1b[0m");
                return 0;
                }

        
        
            current = other_player(current);

            response = best_move(board, current,0);

            board[response.line] = (current);
            printf("Computer played this move : ");
            printf("%d",response.line);
            printf("\n");
            
            if (has_won(board, (current))) {
            print_board(board);
            printf("\x1b[34 ;1m Hurray!!! Player %c has won!\x1b[0m \n", ((current) == RED) ? 'R' : 'B'); 
            break;
            }

        
        current = other_player(current);
    }

    return 0;
}