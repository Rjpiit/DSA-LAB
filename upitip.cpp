#include <cstdio>
#include <vector>
#include <array>
#include <algorithm>
#include <tuple>
#include "queue.hpp"
#include <assert.h>

struct board {
    int e[3][3];
};

constexpr int fact(int i)
{
    return (i <= 1) ? 1 : i * fact(i - 1);
}

int pow(int b, int p) {
    int res = 1;
    for (int i = 0; i < p; i++) {
        res *= b;
    }
    return res;
}

long rnd(const board& b) {
    long o = 0;
    int pow = 1;
    for (int r = 2; r >= 0; --r) {
        for (int c = 2; c >= 0; --c) {
            o += b.e[r][c] * pow;
            pow *= 7;
        }
    }
    return o;
}

void print_board(const board& b) {
    for (int r = 0; r < 3; ++r) {
        for (int c = 0; c < 3; ++c) {
            printf("%3d", b.e[r][c]);
        }
        printf("\n");
    }
}

void read_board(board& b) {
    for (int r = 0; r < 3; ++r) {
        for (int c = 0; c < 3; ++c) {
            scanf("%d", &b.e[r][c]);
        }
    }
}

std::tuple<int, int> find_space(const board& b) {
    for (int r = 0; r < 3; ++r)
        for (int c = 0; c < 3; ++c)
            if (b.e[r][c] == 0) return { r, c };
    assert(0);
    return {0,0};
}

board up(const board& b) {
    const std::tuple<int, int> e = find_space(b);
    const int r = std::get<0>(e);
    const int c = std::get<1>(e);
    if (r == 2) return b;
    board o = b;
    if (o.e[r + 1][c] == 1) {
        o.e[r][c] = 3;
    }
    else if (o.e[r + 1][c] == 2) {
        o.e[r][c] = 2;
    }
    else if (o.e[r + 1][c] == 3) {
        o.e[r][c] = 6;
    }
    else if (o.e[r + 1][c] == 4) {
        o.e[r][c] = 4;
    }
    else if (o.e[r + 1][c] == 5) {
        o.e[r][c] = 1;
    }
    else if (o.e[r + 1][c] == 6) {
        o.e[r][c] = 5;
    }
    o.e[r + 1][c] = 0;
    return o;
}

board down(const board& b) {
    const std::tuple<int, int> e = find_space(b);
    const int r = std::get<0>(e);
    const int c = std::get<1>(e);
    if (r == 0) return b;
    board o = b;
    if (o.e[r - 1][c] == 1) {
        o.e[r][c] = 5;
    }
    else if (o.e[r - 1][c] == 2) {
        o.e[r][c] = 2;
    }
    else if (o.e[r - 1][c] == 3) {
        o.e[r][c] = 1;
    }
    else if (o.e[r - 1][c] == 4) {
        o.e[r][c] = 4;
    }
    else if (o.e[r - 1][c] == 5) {
        o.e[r][c] = 6;
    }
    else if (o.e[r - 1][c] == 6) {
        o.e[r][c] = 3;
    }
    o.e[r - 1][c] = 0;
    return o;
}

board left(const board& b) {
    const std::tuple<int, int> e = find_space(b);
    const int r = std::get<0>(e);
    const int c = std::get<1>(e);
    if (c == 2) return b;
    board o = b;

    if (o.e[r][c + 1] == 1) {
        o.e[r][c] = 2;
    }
    else if (o.e[r][c + 1] == 2) {
        o.e[r][c] = 6;
    }
    else if (o.e[r][c + 1] == 3) {
        o.e[r][c] = 3;
    }
    else if (o.e[r][c + 1] == 4) {
        o.e[r][c] = 1;
    }
    else if (o.e[r][c + 1] == 5) {
        o.e[r][c] = 5;
    }
    else if (o.e[r][c + 1] == 6) {
        o.e[r][c] = 4;
    }
    o.e[r][c + 1] = 0;
    return o;
}

board right(const board& b) {
    const std::tuple<int, int> e = find_space(b);
    const int r = std::get<0>(e);
    const int c = std::get<1>(e);
    if (c == 0) return b;
    board o = b;
    if (o.e[r][c - 1] == 1) {
        o.e[r][c] = 4;
    }
    else if (o.e[r][c - 1] == 2) {
        o.e[r][c] = 1;
    }
    else if (o.e[r][c - 1] == 3) {
        o.e[r][c] = 3;
    }
    else if (o.e[r][c - 1] == 4) {
        o.e[r][c] = 6;
    }
    else if (o.e[r][c - 1] == 5) {
        o.e[r][c] = 5;
    }
    else if (o.e[r][c - 1] == 6) {
        o.e[r][c] = 2;
    }
    o.e[r][c - 1] = 0;
    return o;
}

bool is_same(const board& a, const board& b) {
    for (int r = 0; r < 3; ++r)
        for (int c = 0; c < 3; ++c)
            if (a.e[r][c] != b.e[r][c]) return false;
    return true;
}

enum move { L = 1, R = 2, U = 3, D = 4 };

#define MAX_rnd (40353600)
std::vector<int> solve(const board& src, const board& dest) {
    queue<board, MAX_rnd> q;
    int visited[MAX_rnd];
    board parent[MAX_rnd];

    enqueue(q, src);
    visited[rnd(src)] = L;

    while (!is_empty(q)) {
        board u = dequeue(q);
        if (is_same(u, dest)) {
            std::vector<int> moves;
            board c = u;
            int o = rnd(c);
            while (!is_same(c, src)) {
                moves.push_back(visited[o]);
                c = parent[o];
                o = rnd(c);
            }
            std::reverse(moves.begin(), moves.end());
            return moves;
        }

        board a = up(u);
        board b = down(u);
        board c = left(u);
        board d = right(u);

        int arnd = rnd(a);
        int brnd = rnd(b);
        int crnd = rnd(c);
        int drnd = rnd(d);

        if (!visited[arnd]) {
            visited[arnd] = U;
            parent[arnd] = u;
            enqueue(q, a);
        }
        if (!visited[brnd]) {
            visited[brnd] = D;
            parent[brnd] = u;
            enqueue(q, b);
        }
        if (!visited[crnd]) {
            visited[crnd] = L;
            parent[crnd] = u;
            enqueue(q, c);
        }
        if (!visited[drnd]) {
            visited[drnd] = R;
            parent[drnd] = u;
            enqueue(q, d);
        }
    }

    printf("\nBoard cannot be solved\n");
    return { 0 };
}

void print_moves(const std::vector<int>& moves) {
    for (auto m : moves) {
        switch (m) {
        case L: printf("L "); break;
        case R: printf("R "); break;
        case U: printf("U "); break;
        case D: printf("D "); break;
        }
    }
    printf("\n");
}

#include<iostream>
int main() {
    board src, dest;
    printf("Enter your existing board : \n");
    read_board(src);
    printf("Enter your targeted board : \n");
    read_board(dest);

    auto moves = solve(src, dest);
    print_moves(moves);

    return 0;
}