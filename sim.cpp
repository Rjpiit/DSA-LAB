#include <iostream>
#include <array>
#include <queue>
#include <cassert>

enum {
    NO = 0,
    RED = 1,
    BLUE = 2
};

using board_t = std::array<char, 15>;
using player_t = char;

struct move_t {
    int line;
    int score;
};

struct bfs_node_t {
    board_t board;
    player_t player;
    int move;
};

void init_board(board_t& board) {
    board.fill(NO);
}

player_t other_player(player_t player) {
    switch (player) {
        case RED: return BLUE;
        case BLUE: return RED;
        default: assert(0);
    }
}

void print_board(const board_t& board) {
    for (int i = 0; i < 15; ++i) {
        switch (board[i]) {
            case NO: std::cout << " . "; break;
            case RED: std::cout << "\x1b[31;1m R \x1b[0m"; break;
            case BLUE: std::cout << "\x1b[34;1m B \x1b[0m"; break;
        }
    }
    std::cout << "\n";
}

int has_won(const board_t& board, player_t player) {
    player_t x;
    x = other_player(player);
    int z[5] = {-1, 4, 8, 11, 13};
    int lst[13] = {4, 7, 9, 10, 0, 3, 5, 6, 0, 2, 3, 0, 1};
    for (int k = 1; k < 5; k++) {
        for (int i = z[k - 1] + 1; i < z[k]; i++) {
            for (int j = i + 1; j <= z[k]; j++) {
                if (board[i] == x &&
                    board[j] == x &&
                    board[j + lst[i]] == x && board[i] != NO) {
                    return 1;
                }
            }
        }
    }
    return 0;
}

move_t best_move(const board_t& board, player_t player) {
    // Initialize BFS
    std::queue<bfs_node_t> bfs_queue;

    // Enqueue initial node
    bfs_node_t initial_node = {board, player, -1};
    bfs_queue.push(initial_node);

    while (!bfs_queue.empty()) {
        // Dequeue a node
        bfs_node_t current_node = bfs_queue.front();
        bfs_queue.pop();
        board_t current_board = current_node.board;
        player_t current_player = current_node.player;
        int last_move = current_node.move;

        // Check if the current player has won
        if (has_won(current_board, current_player)) {
            // Return the last move if the current player wins
            return {last_move, 0};
        }

        // Enqueue child nodes for further exploration
        for (int i = 0; i < 15; i++) {
            if (current_board[i] == NO) {
                // Make a move
                board_t new_board = current_board;
                new_board[i] = current_player;

                // Enqueue the new node
                bfs_node_t new_node = {new_board, other_player(current_player), i};
                bfs_queue.push(new_node);
            }
        }
    }

    // If no winning move found, return a random move (you may implement a scoring mechanism)
    return {-1, 0};
}

int main() {
    int move, i;
    board_t board;
    move_t response;
    int x;
    std::cout << "Which color do you want to be? Enter 1 for RED and 2 for BLUE: ";
    std::cin >> x;
    player_t current;
    int chosenColor;

    if (x == 1) {
        current = RED;
    } else if (x == 2) {
        current = BLUE;
    } else {
        std::cout << "\x1b[31;1m Invalid Color: Enter your choice again, please!!! \x1b[0m";
        return 1;
    }

    init_board(board);

    while (1) {
        print_board(board);
        std::cout << "\n";

        std::cout << "0  1  2  3  4  5  6  7  8  9 10 11 12 13 14\n";
        std::cout << "\nEnter your move: ";
        std::cin >> move;
        i = move;
        if (i >= 0 && i < 15 && board[i] == NO) {
            board[i] = current;
        } else {
            std::cout << "\x1b[31;1m Invalid Move \x1b[0m";
            return 0;
        }

        current = other_player(current);

        response = best_move(board, current);

        if (response.line != -1) {
            board[response.line] = current;
            std::cout << "Computer played this move : " << response.line << "\n";
        }

        if (has_won(board, current)) {
            print_board(board);
            std::cout << "\x1b[34 ;1m Hurray!!! Player " << ((current == RED) ? 'R' : 'B') << " has won!\x1b[0m \n";
            break;
        }

        current = other_player(current);
    }

    return 0;
}
