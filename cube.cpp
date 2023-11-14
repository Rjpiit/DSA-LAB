#include <iostream>
#include <vector>
#include <tuple>
#include <algorithm>
#include <string>

class Cube {
public:
    Cube(std::vector<char> f, std::vector<char> b, std::vector<char> r, std::vector<char> l, std::vector<char> u, std::vector<char> d) {
        faces = {{'F', f}, {'B', b}, {'R', r}, {'L', l}, {'U', u}, {'D', d}};
        solved = check_solved();
        coord = get_coordinates();
        pos = get_positions();
    }

    bool check_solved() {
        for (const auto& face : faces) {
            if (std::set<char>(face.second.begin(), face.second.end()).size() != 1) {
                return false;
            }
        }
        return true;
    }

    std::vector<std::tuple<int, int, int>> get_coordinates() {
        std::vector<std::tuple<int, int, int>> coord = {{0, 0, 0}, {0, 0, 1}, {1, 0, 1}, {1, 0, 0}, {0, 1, 0}, {0, 1, 1}, {1, 1, 1}, {1, 1, 0}};
        return coord;
    }

    std::vector<std::tuple<char, char, char>> get_positions() {
        std::vector<std::tuple<char, char, char>> d = {
            {faces["F"][0], faces["L"][1], faces["U"][2]},
            {faces["B"][1], faces["L"][0], faces["U"][0]},
            {faces["B"][0], faces["R"][1], faces["U"][1]},
            {faces["F"][1], faces["R"][0], faces["U"][3]},
            {faces["F"][2], faces["L"][3], faces["D"][0]},
            {faces["B"][3], faces["L"][2], faces["D"][2]},
            {faces["B"][2], faces["R"][3], faces["D"][3]},
            {faces["F"][3], faces["R"][2], faces["D"][1]}
        };
        return d;
    }

    int get_id(const std::vector<char>& colors) {
        if (std::set<char>(colors.begin(), colors.end()) == std::set<char>{'G', 'O', 'W'}) {
            return 0;
        } else if (std::set<char>(colors.begin(), colors.end()) == std::set<char>{'B', 'O', 'W'}) {
            return 1;
        } else if (std::set<char>(colors.begin(), colors.end()) == std::set<char>{'B', 'R', 'W'}) {
            return 2;
        } else if (std::set<char>(colors.begin(), colors.end()) == std::set<char>{'G', 'R', 'W'}) {
            return 3;
        } else if (std::set<char>(colors.begin(), colors.end()) == std::set<char>{'G', 'O', 'Y'}) {
            return 4;
        } else if (std::set<char>(colors.begin(), colors.end()) == std::set<char>{'B', 'O', 'Y'}) {
            return 5;
        } else if (std::set<char>(colors.begin(), colors.end()) == std::set<char>{'B', 'R', 'Y'}) {
            return 6;
        } else if (std::set<char>(colors.begin(), colors.end()) == std::set<char>{'G', 'R', 'Y'}) {
            return 7;
        }
        return -1;
    }

    Cube apply_sequence(const std::vector<std::string>& seq) {
        Cube cube = *this;
        for (const auto& s : seq) {
            if (s == "F") {
                cube = cube.f_cw();
            } else if (s == "F'") {
                cube = cube.f_ccw();
            } else if (s == "F2") {
                cube = cube.f_2();
            } else if (s == "R") {
                cube = cube.r_cw();
            } else if (s == "R'") {
                cube = cube.r_ccw();
            } else if (s == "R2") {
                cube = cube.r_2();
            } else if (s == "U") {
                cube = cube.u_cw();
            } else if (s == "U'") {
                cube = cube.u_ccw();
            } else if (s == "U2") {
                cube = cube.u_2();
            } else {
                std::cout << "Invalid sequence\n";
            }
        }
        return cube;
    }

    std::vector<Cube> apply_all_moves() {
        std::vector<Cube> moves;
        moves.push_back(this->f_cw());
        moves.push_back(this->f_ccw());
        moves.push_back(this->f_2());
        moves.push_back(this->r_cw());
        moves.push_back(this->r_ccw());
        moves.push_back(this->r_2());
        moves.push_back(this->u_cw());
        moves.push_back(this->u_ccw());
        moves.push_back(this->u_2());
        return moves;
    }

    Cube f_cw() {
        std::vector<std::vector<char>> tempf = faces;
        // rotate front face
        std::rotate(tempf.rbegin(), tempf.rbegin() + 1, tempf.rend());
        std::rotate(tempf.rbegin(), tempf.rbegin() + 1, tempf.rend());
        std::rotate(tempf.rbegin(), tempf.rbegin() + 1, tempf.rend());

        // reassign values
        for (int i = 0; i < 2; ++i) {
            tempf["R"][0 + i * 2] = faces["U"][2 + i];
            tempf["D"][1 - i] = faces["R"][0 + i * 2];
            tempf["L"][3 - i * 2] = faces["D"][1 - i];
            tempf["U"][2 + i] = faces["L"][3 - i * 2];
        }
        return Cube(tempf["F"], tempf["B"], tempf["R"], tempf["L"], tempf["U"], tempf["D"]);
    }

    Cube f_ccw() {
        std::vector<std::vector<char>> tempf = faces;
        // rotate front face
        std::rotate(tempf["F"].begin(), tempf["F"].end() - 2, tempf["F"].end());
        // reassign values
        for (int i = 0; i < 2; ++i) {
            tempf["U"][2 + i] = faces["R"][0 + i * 2];
            tempf["R"][0 + i * 2] = faces["D"][1 - i];
            tempf["D"][1 - i] = faces["L"][3 - i * 2];
            tempf["L"][3 - i * 2] = faces["U"][2 + i];
        }
        return Cube(tempf["F"], tempf["B"], tempf["R"], tempf["L"], tempf["U"], tempf["D"]);
    }

    Cube f_2() {
        std::vector<std::vector<char>> tempf = faces;
        // rotate front face
        std::rotate(tempf["F"].begin(), tempf["F"].end() - 4, tempf["F"].end());
        // reassign values
        for (int i = 0; i < 2; ++i) {
            tempf["D"][1 - i] = faces["U"][2 + i];
            tempf["L"][3 - i * 2] = faces["R"][0 + i * 2];
            tempf["U"][2 + i] = faces["D"][1 - i];
            tempf["R"][0 + i * 2] = faces["L"][3 - i * 2];
        }
        return Cube(tempf["F"], tempf["B"], tempf["R"], tempf["L"], tempf["U"], tempf["D"]);
    }
    Cube b_cw() {
        std::vector<std::vector<char>> tempf = faces;
        // rotate front face
        std::rotate(tempf.rbegin(), tempf.rbegin() + 1, tempf.rend());
        std::rotate(tempf.rbegin(), tempf.rbegin() + 1, tempf.rend());
        std::rotate(tempf.rbegin(), tempf.rbegin() + 1, tempf.rend());

        // reassign values
        for (int i = 0; i < 2; ++i) {
            tempf["L"][0 + i * 2] = faces["U"][1 - i];
            tempf["D"][2 + i] = faces["L"][0 + i * 2];
            tempf["R"][3 - i * 2] = faces["D"][2+i];
            tempf["U"][1 - i] = faces["R"][3 - i * 2];
        }
        return Cube(tempf["F"], tempf["B"], tempf["R"], tempf["L"], tempf["U"], tempf["D"]);
    }

    Cube b_ccw() {
        std::vector<std::vector<char>> tempf = faces;
        // rotate front face
        std::rotate(tempf["F"].begin(), tempf["F"].end() - 2, tempf["F"].end());
        // reassign values
        for (int i = 0; i < 2; ++i) {
            tempf["U"][1- i] = faces["L"][0 + i * 2];
            tempf["L"][0 + i * 2] = faces["D"][2+ i];
            tempf["D"][2+ i] = faces["R"][3 - i * 2];
            tempf["R"][3 - i * 2] = faces["U"][1 - i];
        }
        return Cube(tempf["F"], tempf["B"], tempf["R"], tempf["L"], tempf["U"], tempf["D"]);
    }

    Cube b_2() {
        std::vector<std::vector<char>> tempf = faces;
        // rotate front face
        std::rotate(tempf["F"].begin(), tempf["F"].end() - 4, tempf["F"].end());
        // reassign values
        for (int i = 0; i < 2; ++i) {
            tempf["D"][2 + i] = faces["U"][1 - i];
            tempf["R"][3 - i * 2] = faces["L"][0 + i * 2];
            tempf["U"][1 - i] = faces["D"][2 + i];
            tempf["L"][0 + i * 2] = faces["R"][3 - i * 2];
        }
        return Cube(tempf["F"], tempf["B"], tempf["R"], tempf["L"], tempf["U"], tempf["D"]);
    }
    Cube r_cw() {
        std::vector<std::vector<char>> tempf = faces;
        // rotate front face
        std::rotate(tempf.rbegin(), tempf.rbegin() + 1, tempf.rend());
        std::rotate(tempf.rbegin(), tempf.rbegin() + 1, tempf.rend());
        std::rotate(tempf.rbegin(), tempf.rbegin() + 1, tempf.rend());

        // reassign values
        for (int i = 0; i < 2; ++i) {
            tempf["B"][0 + i * 2] = faces["U"][3 - i * 2];
            tempf["D"][3 - i * 2] = faces["B"][0 + i * 2];
            tempf["F"][3 - i * 2] = faces["D"][3 - i * 2];
            tempf["U"][3 - i * 2] = faces["F"][3 - i * 2];
        }
        return Cube(tempf["F"], tempf["B"], tempf["R"], tempf["L"], tempf["U"], tempf["D"]);
    }

    Cube r_ccw() {
        std::vector<std::vector<char>> tempf = faces;
        // rotate front face
        std::rotate(tempf["F"].begin(), tempf["F"].end() - 2, tempf["F"].end());
        // reassign values
        for (int i = 0; i < 2; ++i) {
            tempf["U"][3 - i * 2] = faces["B"][0 + i * 2];
            tempf["B"][0 + i * 2] = faces["D"][3 - i * 2];
            tempf["D"][3 - i * 2] = faces["F"][3 - i * 2];
            tempf["F"][3 - i * 2] = faces["U"][3 - i * 2];
        }
        return Cube(tempf["F"], tempf["B"], tempf["R"], tempf["L"], tempf["U"], tempf["D"]);
    }

    Cube r_2() {
        std::vector<std::vector<char>> tempf = faces;
        // rotate front face
        std::rotate(tempf["F"].begin(), tempf["F"].end() - 4, tempf["F"].end());
        // reassign values
        for (int i = 0; i < 2; ++i) {
            tempf["D"][3 - i * 2] = faces["U"][3 - i * 2];
            tempf["F"][3 - i * 2] = faces["B"][0 + i * 2];
            tempf["U"][3 - i * 2] = faces["D"][3 - i * 2];
            tempf["B"][0 + i * 2] = faces["F"][3 - i * 2];
        }
        return Cube(tempf["F"], tempf["B"], tempf["R"], tempf["L"], tempf["U"], tempf["D"]);
    }
    Cube l_cw() {
        std::vector<std::vector<char>> tempf = faces;
        // rotate front face
        std::rotate(tempf.rbegin(), tempf.rbegin() + 1, tempf.rend());
        std::rotate(tempf.rbegin(), tempf.rbegin() + 1, tempf.rend());
        std::rotate(tempf.rbegin(), tempf.rbegin() + 1, tempf.rend());

        // reassign values
        for (int i = 0; i < 2; ++i) {
            tempf["F"][0 + i * 2] = faces["U"][0 + i * 2];
            tempf["D"][0 + i * 2] = faces["F"][0 + i * 2];
            tempf["B"][3 - i * 2] = faces["D"][0 + i * 2];
            tempf["U"][0 + i * 2] = faces["B"][3 - i * 2];
        }
        return Cube(tempf["F"], tempf["B"], tempf["R"], tempf["L"], tempf["U"], tempf["D"]);
    }

    Cube l_ccw() {
        std::vector<std::vector<char>> tempf = faces;
        // rotate front face
        std::rotate(tempf["F"].begin(), tempf["F"].end() - 2, tempf["F"].end());
        // reassign values
        for (int i = 0; i < 2; ++i) {
            tempf["U"][0 + i * 2] = faces["F"][0 + i * 2];
            tempf["F"][0 + i * 2] = faces["D"][0 + i * 2];
            tempf["D"][0 + i * 2] = faces["B"][3 - i * 2];
            tempf["B"][3 - i * 2] = faces["U"][0 + i * 2];
        }
        return Cube(tempf["F"], tempf["B"], tempf["R"], tempf["L"], tempf["U"], tempf["D"]);
    }

    Cube l_2() {
        std::vector<std::vector<char>> tempf = faces;
        // rotate front face
        std::rotate(tempf["F"].begin(), tempf["F"].end() - 4, tempf["F"].end());
        // reassign values
        for (int i = 0; i < 2; ++i) {
            tempf["D"][0 + i * 2] = faces["U"][0 + i * 2];
            tempf["B"][3 - i * 2] = faces["F"][0 + i * 2];
            tempf["U"][0 + i * 2] = faces["D"][0 + i * 2];
            tempf["F"][0 + i * 2] = faces["B"][3 - i * 2];
        }
        return Cube(tempf["F"], tempf["B"], tempf["R"], tempf["L"], tempf["U"], tempf["D"]);
    }
    Cube u_cw() {
        std::vector<std::vector<char>> tempf = faces;
        // rotate front face
        std::rotate(tempf.rbegin(), tempf.rbegin() + 1, tempf.rend());
        std::rotate(tempf.rbegin(), tempf.rbegin() + 1, tempf.rend());
        std::rotate(tempf.rbegin(), tempf.rbegin() + 1, tempf.rend());

        // reassign values
        for (int i = 0; i < 2; ++i) {
            tempf["R"][1 - i] = faces["B"][1 - i];
            tempf["F"][1 - i] = faces["R"][1 - i];
            tempf["L"][1 - i] = faces["F"][1 - i];
            tempf["B"][1 - i] = faces["L"][1 - i];
        }
        return Cube(tempf["F"], tempf["B"], tempf["R"], tempf["L"], tempf["U"], tempf["D"]);
    }

    Cube u_ccw() {
        std::vector<std::vector<char>> tempf = faces;
        // rotate front face
        std::rotate(tempf["F"].begin(), tempf["F"].end() - 2, tempf["F"].end());
        // reassign values
        for (int i = 0; i < 2; ++i) {
            tempf["B"][1 - i] = faces["R"][1 - i];
            tempf["R"][1 - i] = faces["F"][1 - i];
            tempf["F"][1 - i] = faces["L"][1 - i];
            tempf["L"][1 - i] = faces["B"][1 - i];
        }
        return Cube(tempf["F"], tempf["B"], tempf["R"], tempf["L"], tempf["U"], tempf["D"]);
    }

    Cube u_2() {
        std::vector<std::vector<char>> tempf = faces;
        // rotate front face
        std::rotate(tempf["F"].begin(), tempf["F"].end() - 4, tempf["F"].end());
        // reassign values
        for (int i = 0; i < 2; ++i) {
            tempf["F"][1 - i] = faces["B"][1 - i];
            tempf["L"][1 - i] = faces["R"][1 - i];
            tempf["B"][1 - i] = faces["F"][1 - i];
            tempf["R"][1 - i] = faces["L"][1 - i];
        }
        return Cube(tempf["F"], tempf["B"], tempf["R"], tempf["L"], tempf["U"], tempf["D"]);
    }
    Cube d_cw() {
        std::vector<std::vector<char>> tempf = faces;
        // rotate front face
        std::rotate(tempf.rbegin(), tempf.rbegin() + 1, tempf.rend());
        std::rotate(tempf.rbegin(), tempf.rbegin() + 1, tempf.rend());
        std::rotate(tempf.rbegin(), tempf.rbegin() + 1, tempf.rend());

        // reassign values
        for (int i = 0; i < 2; ++i) {
            tempf["R"][2+ i] = faces["F"][2+ i];
            tempf["B"][2+ i] = faces["R"][2+ i];
            tempf["L"][2+ i] = faces["B"][2+ i];
            tempf["F"][2+ i] = faces["L"][2+ i];
        }
        return Cube(tempf["F"], tempf["B"], tempf["R"], tempf["L"], tempf["U"], tempf["D"]);
    }

    Cube d_ccw() {
        std::vector<std::vector<char>> tempf = faces;
        // rotate front face
        std::rotate(tempf["F"].begin(), tempf["F"].end() - 2, tempf["F"].end());
        // reassign values
        for (int i = 0; i < 2; ++i) {
            tempf["F"][2+ i] = faces["R"][2+ i];
            tempf["R"][2+ i] = faces["B"][2+ i];
            tempf["B"][2+ i] = faces["L"][2+ i];
            tempf["L"][2+ i] = faces["F"][2+ i];
        }
        return Cube(tempf["F"], tempf["B"], tempf["R"], tempf["L"], tempf["U"], tempf["D"]);
    }

    Cube d_2() {
        std::vector<std::vector<char>> tempf = faces;
        // rotate front face
        std::rotate(tempf["F"].begin(), tempf["F"].end() - 4, tempf["F"].end());
        // reassign values
        for (int i = 0; i < 2; ++i) {
            tempf["B"][2+ i] = faces["F"][2+ i];
            tempf["L"][2+ i] = faces["R"][2+ i];
            tempf["F"][2+ i] = faces["B"][2+ i];
            tempf["R"][2+ i] = faces["L"][2+ i];
        }
        return Cube(tempf["F"], tempf["B"], tempf["R"], tempf["L"], tempf["U"], tempf["D"]);
    }
};

class Node {
public:
    Cube cube;
    int g;  // path cost from root
    std::vector<std::string> sequence;  // sequence of moves
    int h;

    Node(const Cube& c, int cost, const std::vector<std::string>& seq) : cube(c), g(cost), sequence(seq), h(heu(c)) {}

    int heu(const Cube& cube) {
        int h = 0;
        for (const auto& face : cube.faces) {
            if (face.first == "F") {
                h += (4 - std::count(face.second.begin(), face.second.end(), 'G'));
            } else if (face.first == "B") {
                h += (4 - std::count(face.second.begin(), face.second.end(), 'B'));
            } else if (face.first == "R") {
                h += (4 - std::count(face.second.begin(), face.second.end(), 'R'));
            } else if (face.first == "L") {
                h += (4 - std::count(face.second.begin(), face.second.end(), 'O'));
            } else if (face.first == "U") {
                h += (4 - std::count(face.second.begin(), face.second.end(), 'W'));
            } else if (face.first == "D") {
                h += (4 - std::count(face.second.begin(), face.second.end(), 'Y'));
            }
        }
        return h / 4;
    }
};

std::vector<std::string> cube_solver(const Cube& cube) {
    std::vector<Node> frontier = {Node(cube, 0, {"START"})};

    std::vector<Node> visited;
    std::map<char, char> opposite{{'B', 'F'}, {'L', 'R'}, {'D', 'U'}};
    while (!frontier.empty()) {
        int min_f = 100000;
        int index = -1;
        for (int i = 0; i < frontier.size(); ++i) {
            // select best node in frontier
                    if (frontier[i].g + frontier[i].h < min_f) {
                    min_f = frontier[i].g + frontier[i].h;
                    index = i;
                }
    }

    Node current_node = frontier[index];
    frontier.erase(frontier.begin() + index);

    if (current_node.cube.is_solved()) {
        return current_node.sequence;
    }

    visited.push_back(current_node);

    std::vector<std::string> moves = {"F", "B", "L", "R", "U", "D"};
    for (const auto& move : moves) {
        Cube new_cube = current_node.cube;
        new_cube.make_move(move);

        Node child_node(new_cube, current_node.g + 1, current_node.sequence);
        child_node.sequence.push_back(move);

        auto it_visited = std::find(visited.begin(), visited.end(), child_node);
        auto it_frontier = std::find(frontier.begin(), frontier.end(), child_node);

        if (it_visited == visited.end() && it_frontier == frontier.end()) {
            frontier.push_back(child_node);
        } else if (it_frontier != frontier.end() && child_node.g < it_frontier->g) {
            frontier.erase(it_frontier);
            frontier.push_back(child_node);
        } else if (it_visited != visited.end() && child_node.g < it_visited->g) {
            visited.erase(it_visited);
            frontier.push_back(child_node);
        }
    }
}

// Return an empty vector if no solution is found
return {};
}
int main() {
    // Define cube faces using arrays
    std::array<std::string, 4> F{"G", "G", "G", "G"};
    std::array<std::string, 4> B{"B", "B", "B", "B"};
    std::array<std::string, 4> R{"R", "R", "R", "R"};
    std::array<std::string, 4> L{"O", "O", "O", "O"};
    std::array<std::string, 4> U{"W", "W", "W", "W"};
    std::array<std::string, 4> D{"Y", "Y", "Y", "Y"};

    // Create a Cube object
    Cube cube(F, B, R, L, U, D);
    while (true) {
        std::cout << "Use the following symbols separated by spaces to scramble the cube:\n";
        std::cout << "F  F' F2 R  R' R2  U  U' U2\n\n";
        std::cout << "Enter sequence: ";

        // Read user move sequence
        std::vector<std::string> seq;
        std::string move;
        while (std::cin >> move) {
            if (move == "exit") {
                break;
            }
            seq.push_back(move);
        }

        // Apply sequence
        std::cout << "Let's Scramble cube !!!\n";
        cube = cube.apply_sequence(seq);
        std::cout << "Solving.....\n";
        break;
    }
    std::vector<std::string> solution = cube_solver(cube);

    std::string moves_string;
    for (const auto& move : solution) {
        moves_string += " " + move;
    }
    std::cout << "Sequence to solve the cube:" << moves_string << "\n";

    // Apply moves
    for (const auto& move : solution) {
        std::cout << "Applying move: " << move << "\n";
        cube = cube.apply_sequence({move});
    }

    std::cout << "The cube is now solved!!!!!!\n";

    return 0;
}