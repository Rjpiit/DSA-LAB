#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

enum {
    EMPTY = 0,
    RED = 1,
    BLUE = 2,
};

typedef char board_t[4][5];
typedef char player_t;


int has_won(board_t board, player_t piece, int rows, int cols) {
    // Vertical check
    for (int c = 0; c < cols; c++) {
        for (int r = 0; r < rows - 3; r++) {
            if (board[r][c] == piece && board[r + 1][c] == piece &&
                board[r + 2][c] == piece && board[r + 3][c] == piece) {
                return 1;
            }
        }
    }

    // Horizontal check
    for (int c = 0; c < cols - 3; c++) {
        for (int r = 0; r < rows; r++) {
            if (board[r][c] == piece && board[r][c + 1] == piece &&
                board[r][c + 2] == piece && board[r][c + 3] == piece) {
                return 1;
            }
        }
    }

    // Positively sloped diagonal check
    for (int c = 0; c < cols - 3; c++) {
        for (int r = 0; r < rows - 3; r++) {
            if (board[r][c] == piece && board[r + 1][c + 1] == piece &&
                board[r + 2][c + 2] == piece && board[r + 3][c + 3] == piece) {
                return 1;
            }
        }
    }

    // Negatively sloped diagonal check
    for (int c = 0; c < cols - 3; c++) {
        for (int r = 3; r < rows; r++) {
            if (board[r][c] == piece && board[r - 1][c + 1] == piece &&
                board[r - 2][c + 2] == piece && board[r - 3][c + 3] == piece) {
                return 1;
            }
        }
    }

    return 0;
}


bool is_full(board_t board, int selection, int rows, int cols) {
    // Check if the selected column is within the valid range
    if (selection < 0 || selection >= cols) {
        return 0;
    }

    // Check if the bottom row in the selected column is empty (0)
    if (board[rows - 1][selection] == EMPTY) {
        return 1;
    }

    return 0;
}
void drop_piece(board_t board, int row, int selection, player_t piece) {
    // Place the piece in the specified row and column
    board[row][selection] = piece;
}

typedef struct {
    int col;
    int score;
} move_t;

move_t best_move(board_t board, player_t piece, int rows, int cols) {
    move_t best_move = {-1, 0}; // Initialize with an invalid move
    board_t board_copy;

    for (int c = 0; c < cols; c++) {
        for (int r = 0; r < rows; r++) {
            if (board[r][c] == EMPTY) {
                // Copy the current board to board_copy
                for (int i = 0; i < rows; i++) {
                    for (int j = 0; j < cols; j++) {
                        board_copy[i][j] = board[i][j];
                    }
                }

                // Drop the piece in this column
                int row = get_next_available_row(board_copy, c, rows, cols);
                drop_piece(board_copy, row, c, piece);

                // Check if making this move would lead to a win
                if (has_won(board_copy, piece, rows, cols)==1) {
                    best_move.col = c;
                    return best_move;
                }
            }
        }
    }

    return best_move;
}


void print_board(board_t board, int rows, int cols) {
    // Print the board with rows flipped
    for (int r = rows - 1; r >= 0; r--) {
        for (int c = 0; c < cols; c++) {
            if (board[r][c] == EMPTY) {
                printf("0 "); // Print 0 for empty cell
            } else if (board[r][c] == RED) {
                printf("R "); // Print 1 for RED player
            } else if (board[r][c] == BLUE) {
                printf("B "); // Print 2 for BLUE player
            }
        }
        printf("\n");
    }
}
int get_next_available_row(board_t board, int selection, int rows, int cols) {
    // Iterate through the rows to find the next available row in the selected column
    for (int i = 0; i < rows; i++) {
        if (board[i][selection] == EMPTY) {
            return i;
        }
    }
    // If no available row is found, return -1 (or any value indicating an error)
    return -1;
}



int main() {
    bool game_over = false;
    int rows = 4;
    int cols = 5;
    int turn = 0;
    srand(time(NULL));

    board_t board = {
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0}
    };

    int count = 0;

    int player_color; // User's color choice (RED or BLUE)
    
    // User chooses their color
    printf("Choose your color !!! type 1 for RED and 2 for BLUE!!!:\n");
    scanf("%d", &player_color);

    // Validate the user's color choice
    if (player_color != RED && player_color != BLUE) {
        printf("Invalid choice. Defaulting to RED.\n");
        player_color = RED; // Default to RED in case of an invalid choice
    }

    while (!game_over) {
        // Player 1
        printf("\n");
        if (count > rows * cols) {
            printf("Game Drawn\n");
            game_over = true;
        }

        if (turn == 0) {
            int selection;
            printf("Player 1, please select (0-%d): ", cols - 1);
            scanf("%d", &selection);
            count++;

            // Check if the selected position is valid
            if (is_full(board, selection, rows, cols)==1) {
                // Find the next available row for the piece
                int row;
                for (row = 0; row < rows; row++) {
                    if (board[row][selection] == 0)
                        break;
                }

                board[row][selection] = player_color; // Set the user's chosen color

                // Check for victory
                if (has_won(board, player_color, rows, cols)==1) {
                    printf("Player 1 wins!!!\n");
                    game_over = true;
                }
            }
            else {
                printf("Invalid move !!!! Player 1, please select (0-%d): ", cols - 1);
                scanf("%d", &selection);
                count++;
                if (is_full(board, selection, rows, cols)==1) {
                int row = get_next_available_row(board, selection, rows, cols);
                drop_piece(board, row, selection, player_color); 
                if (has_won(board, player_color, rows, cols)==1) {
                    printf("Player1 wins!!!\n");
                    game_over = true;
                }
                }
            }
        } else {
            move_t winning_move_col = best_move(board, player_color, rows, cols);
            int selection = 0;
            if (winning_move_col.col != -1) {
                selection = winning_move_col.col;
                count++;
            } else {
                // If no winning move, choose a random valid move
                selection = rand() % (cols); 
                count++;
            }

            if (is_full(board, selection, rows, cols)==1) {
                int row = get_next_available_row(board, selection, rows, cols);
                drop_piece(board, row, selection, 3 - player_color); // The computer's color is the opposite of the user's color (3 - player_color)
                if (has_won(board, 3 - player_color, rows, cols)==1) {
                    printf("Computer wins!!!\n");
                    game_over = true;
                }
            }
            
        }

        // Print the current board
        print_board(board, rows, cols);

        turn++;
        turn = turn % 2;
    }

    return 0;
}
