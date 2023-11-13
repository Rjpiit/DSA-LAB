import numpy as np
import time
import numpy as np
import copy

class Cube:
    def __init__(self, f, b, r, l, u, d):
        self.faces = {'F': f, 'B': b, 'R': r, 'L': l, 'U': u, 'D': d}
        self.solved = self.check_solved()  
        self.coord = self.get_coordinates() 
        self.pos = self.get_positions() 

    def check_solved(self):
        # checks if the cube is solved
        for face in self.faces.values():
            if not len(set(face)) == 1:
                return False
        return True

    def get_coordinates(self):
        
        coord = {}
        coord["0"] = (0, 0, 0)
        coord["1"] = (0, 0, 1)
        coord["2"] = (1, 0, 1)
        coord["3"] = (1, 0, 0)
        coord["4"] = (0, 1, 0)
        coord["5"] = (0, 1, 1)
        coord["6"] = (1, 1, 1)
        coord["7"] = (1, 1, 0)
        return coord

    def get_positions(self):
        # links color orientations with corner id number
        d = dict()
        d["0"] = (self.faces["F"][0], self.faces["L"][1], self.faces["U"][2])
        d["1"] = (self.faces["B"][1], self.faces["L"][0], self.faces["U"][0])
        d["2"] = (self.faces["B"][0], self.faces["R"][1], self.faces["U"][1])
        d["3"] = (self.faces["F"][1], self.faces["R"][0], self.faces["U"][3])
        d["4"] = (self.faces["F"][2], self.faces["L"][3], self.faces["D"][0])
        d["5"] = (self.faces["B"][3], self.faces["L"][2], self.faces["D"][2])
        d["6"] = (self.faces["B"][2], self.faces["R"][3], self.faces["D"][3])
        d["7"] = (self.faces["F"][3], self.faces["R"][2], self.faces["D"][1])
        return d

    def get_id(self, colors):
        # get corner number id based on faces coloring
        if set([colors[0], colors[1], colors[2]]) == set(["G", "O", "W"]):
            return 0
        elif set([colors[0], colors[1], colors[2]]) == set(["B", "O", "W"]):
            return 1
        elif set([colors[0], colors[1], colors[2]]) == set(["B", "R", "W"]):
            return 2
        elif set([colors[0], colors[1], colors[2]]) == set(["G", "R", "W"]):
            return 3
        elif set([colors[0], colors[1], colors[2]]) == set(["G", "O", "Y"]):
            return 4
        elif set([colors[0], colors[1], colors[2]]) == set(["B", "O", "Y"]):
            return 5
        elif set([colors[0], colors[1], colors[2]]) == set(["B", "R", "Y"]):
            return 6
        elif set([colors[0], colors[1], colors[2]]) == set(["G", "R", "Y"]):
            return 7

    def apply_sequence(self, seq):
        # applies sequence of valid moves and returns resulting cube
        cube = self
        for s in seq:
            if s == "F":
                cube = cube.f_cw()
            elif s == "F'":
                cube = cube.f_ccw()
            elif s == "F2":
                cube = cube.f_2()
            elif s == "R":
                cube = cube.r_cw()
            elif s == "R'":
                cube = cube.r_ccw()
            elif s == "R2":
                cube = cube.r_2()
            elif s == "U":
                cube = cube.u_cw()
            elif s == "U'":
                cube = cube.u_ccw()
            elif s == "U2":
                cube = cube.u_2()
            else:
                print("Invalid sequence\n")
        return cube

    def apply_all_moves(self):
        # returns dict of cubes obtained applying all moves to current cube
        moves = {}
        moves["F"] = self.f_cw()
        moves["F'"] = self.f_ccw()
        moves["F2"] = self.f_2()
        moves["R"] = self.r_cw()
        moves["R'"] = self.r_ccw()
        moves["R2"] = self.r_2()
        moves["U"] = self.u_cw()
        moves["U'"] = self.u_ccw()
        moves["U2"] = self.u_2()
        return moves


    def f_cw(self):
        # applies F move and returns corresponding cube
        tempf = copy.deepcopy(self.faces)
        # rotate front face
        tempf['F'] = tempf['F'].reshape(2, 2)
        tempf['F'] = np.rot90(tempf['F'], 3)
        tempf['F'] = tempf['F'].flatten()
        # reassign values
        for i in range(2):
            tempf['R'][0+i*2] = self.faces['U'][2+i]
            tempf['D'][1-i] = self.faces['R'][0+i*2]
            tempf['L'][3-i*2] = self.faces['D'][1-i]
            tempf['U'][2+i] = self.faces['L'][3-i*2]
        # return new Cube
        return Cube(tempf['F'], tempf['B'], tempf['R'], tempf['L'], tempf['U'], tempf['D'])

    def f_ccw(self):
        # applies F' move and returns corresponding cube
        tempf = copy.deepcopy(self.faces)
        # rotate front face
        tempf['F'] = tempf['F'].reshape(2, 2)
        tempf['F'] = np.rot90(tempf['F'], 1)
        tempf['F'] = tempf['F'].flatten()
        # reassign values
        for i in range(2):
            tempf['U'][2+i] = self.faces['R'][0+i*2]
            tempf['R'][0+i*2] = self.faces['D'][1-i]
            tempf['D'][1-i] = self.faces['L'][3-i*2]
            tempf['L'][3-i*2] = self.faces['U'][2+i]
        # return new Cube
        return Cube(tempf['F'], tempf['B'], tempf['R'], tempf['L'], tempf['U'], tempf['D'])

    def f_2(self):
        # applies F2 move and returns corresponding cube
        tempf = copy.deepcopy(self.faces)
        # rotate front face
        tempf['F'] = tempf['F'].reshape(2, 2)
        tempf['F'] = np.rot90(tempf['F'], 2)
        tempf['F'] = tempf['F'].flatten()
        # reassign values
        for i in range(2):
            tempf['D'][1-i] = self.faces['U'][2+i]
            tempf['L'][3-i*2] = self.faces['R'][0+i*2]
            tempf['U'][2+i] = self.faces['D'][1-i]
            tempf['R'][0+i*2] = self.faces['L'][3-i*2]
        # return new Cube
        return Cube(tempf['F'], tempf['B'], tempf['R'], tempf['L'], tempf['U'], tempf['D'])

    def b_cw(self):
        # applies B move and returns corresponding cube
        tempf = copy.deepcopy(self.faces)
        # rotate front face
        tempf['B'] = tempf['B'].reshape(2, 2)
        tempf['B'] = np.rot90(tempf['B'], 3)
        tempf['B'] = tempf['B'].flatten()
        # reassign values
        for i in range(2):
            tempf['L'][0+i*2] = self.faces['U'][1-i]
            tempf['D'][2+i] = self.faces['L'][0+i*2]
            tempf['R'][3-i*2] = self.faces['D'][2+i]
            tempf['U'][1-i] = self.faces['R'][3-i*2]
        # return new Cube
        return Cube(tempf['F'], tempf['B'], tempf['R'], tempf['L'], tempf['U'], tempf['D'])

    def b_ccw(self):
        # applies B' move and returns corresponding cube
        tempf = copy.deepcopy(self.faces)
        # rotate front face
        tempf['B'] = tempf['B'].reshape(2, 2)
        tempf['B'] = np.rot90(tempf['B'], 1)
        tempf['B'] = tempf['B'].flatten()
        # reassign values
        for i in range(2):
            tempf['U'][1-i] = self.faces['L'][0+i*2]
            tempf['L'][0+i*2] = self.faces['D'][2+i]
            tempf['D'][2+i] = self.faces['R'][3-i*2]
            tempf['R'][3-i*2] = self.faces['U'][1-i]
        # return new Cube
        return Cube(tempf['F'], tempf['B'], tempf['R'], tempf['L'], tempf['U'], tempf['D'])

    def b_2(self):
        # applies B2 move and returns corresponding cube
        tempf = copy.deepcopy(self.faces)
        # rotate front face
        tempf['B'] = tempf['B'].reshape(2, 2)
        tempf['B'] = np.rot90(tempf['B'], 2)
        tempf['B'] = tempf['B'].flatten()
        # reassign values
        for i in range(2):
            tempf['D'][2+i] = self.faces['U'][1-i]
            tempf['R'][3-i*2] = self.faces['L'][0+i*2]
            tempf['U'][1-i] = self.faces['D'][2+i]
            tempf['L'][0+i*2] = self.faces['R'][3-i*2]
        # return new Cube
        return Cube(tempf['F'], tempf['B'], tempf['R'], tempf['L'], tempf['U'], tempf['D'])

    def r_cw(self):
        # applies R move and returns corresponding cube
        tempf = copy.deepcopy(self.faces)
        # rotate front face
        tempf['R'] = tempf['R'].reshape(2, 2)
        tempf['R'] = np.rot90(tempf['R'], 3)
        tempf['R'] = tempf['R'].flatten()
        # reassign values
        for i in range(2):
            tempf['B'][0+i*2] = self.faces['U'][3-i*2]
            tempf['D'][3-i*2] = self.faces['B'][0+i*2]
            tempf['F'][3-i*2] = self.faces['D'][3-i*2]
            tempf['U'][3-i*2] = self.faces['F'][3-i*2]
        # return new Cube
        return Cube(tempf['F'], tempf['B'], tempf['R'], tempf['L'], tempf['U'], tempf['D'])

    def r_ccw(self):
        # applies R' move and returns corresponding cube
        tempf = copy.deepcopy(self.faces)
        # rotate front face
        tempf['R'] = tempf['R'].reshape(2, 2)
        tempf['R'] = np.rot90(tempf['R'], 1)
        tempf['R'] = tempf['R'].flatten()
        # reassign values
        for i in range(2):
            tempf['U'][3-i*2] = self.faces['B'][0+i*2]
            tempf['B'][0+i*2] = self.faces['D'][3-i*2]
            tempf['D'][3-i*2] = self.faces['F'][3-i*2]
            tempf['F'][3-i*2] = self.faces['U'][3-i*2]
        # return new Cube
        return Cube(tempf['F'], tempf['B'], tempf['R'], tempf['L'], tempf['U'], tempf['D'])

    def r_2(self):
        # applies R2 move and returns corresponding cube
        tempf = copy.deepcopy(self.faces)
        # rotate front face
        tempf['R'] = tempf['R'].reshape(2, 2)
        tempf['R'] = np.rot90(tempf['R'], 2)
        tempf['R'] = tempf['R'].flatten()
        # reassign values
        for i in range(2):
            tempf['D'][3-i*2] = self.faces['U'][3-i*2]
            tempf['F'][3-i*2] = self.faces['B'][0+i*2]
            tempf['U'][3-i*2] = self.faces['D'][3-i*2]
            tempf['B'][0+i*2] = self.faces['F'][3-i*2]
        # return new Cube
        return Cube(tempf['F'], tempf['B'], tempf['R'], tempf['L'], tempf['U'], tempf['D'])

    def l_cw(self):
        # applies L move and returns corresponding cube
        tempf = copy.deepcopy(self.faces)
        # rotate front face
        tempf['L'] = tempf['L'].reshape(2, 2)
        tempf['L'] = np.rot90(tempf['L'], 3)
        tempf['L'] = tempf['L'].flatten()
        # reassign values
        for i in range(2):
            tempf['F'][0+i*2] = self.faces['U'][0+i*2]
            tempf['D'][0+i*2] = self.faces['F'][0+i*2]
            tempf['B'][3-i*2] = self.faces['D'][0+i*2]
            tempf['U'][0+i*2] = self.faces['B'][3-i*2]
        # return new Cube
        return Cube(tempf['F'], tempf['B'], tempf['R'], tempf['L'], tempf['U'], tempf['D'])

    def l_ccw(self):
        # applies L' move and returns corresponding cube
        tempf = copy.deepcopy(self.faces)
        # rotate front face
        tempf['L'] = tempf['L'].reshape(2, 2)
        tempf['L'] = np.rot90(tempf['L'], 1)
        tempf['L'] = tempf['L'].flatten()
        # reassign values
        for i in range(2):
            tempf['U'][0+i*2] = self.faces['F'][0+i*2]
            tempf['F'][0+i*2] = self.faces['D'][0+i*2]
            tempf['D'][0+i*2] = self.faces['B'][3-i*2]
            tempf['B'][3-i*2] = self.faces['U'][0+i*2]
        # return new Cube
        return Cube(tempf['F'], tempf['B'], tempf['R'], tempf['L'], tempf['U'], tempf['D'])

    def l_2(self):
        # applies L2 move and returns corresponding cube
        tempf = copy.deepcopy(self.faces)
        # rotate front face
        tempf['L'] = tempf['L'].reshape(2, 2)
        tempf['L'] = np.rot90(tempf['L'], 2)
        tempf['L'] = tempf['L'].flatten()
        # reassign values
        for i in range(2):
            tempf['D'][0+i*2] = self.faces['U'][0+i*2]
            tempf['B'][3-i*2] = self.faces['F'][0+i*2]
            tempf['U'][0+i*2] = self.faces['D'][0+i*2]
            tempf['F'][0+i*2] = self.faces['B'][3-i*2]
        # return new Cube
        return Cube(tempf['F'], tempf['B'], tempf['R'], tempf['L'], tempf['U'], tempf['D'])

    def u_cw(self):
        # applies U move and returns corresponding cube
        tempf = copy.deepcopy(self.faces)
        # rotate front face
        tempf['U'] = tempf['U'].reshape(2, 2)
        tempf['U'] = np.rot90(tempf['U'], 3)
        tempf['U'] = tempf['U'].flatten()
        # reassign values
        for i in range(2):
            tempf['R'][1-i] = self.faces['B'][1-i]
            tempf['F'][1-i] = self.faces['R'][1-i]
            tempf['L'][1-i] = self.faces['F'][1-i]
            tempf['B'][1-i] = self.faces['L'][1-i]
        # return new Cube
        return Cube(tempf['F'], tempf['B'], tempf['R'], tempf['L'], tempf['U'], tempf['D'])

    def u_ccw(self):
        # applies U' move and returns corresponding cube
        tempf = copy.deepcopy(self.faces)
        # rotate front face
        tempf['U'] = tempf['U'].reshape(2, 2)
        tempf['U'] = np.rot90(tempf['U'], 1)
        tempf['U'] = tempf['U'].flatten()
        # reassign values
        for i in range(2):
            tempf['B'][1-i] = self.faces['R'][1-i]
            tempf['R'][1-i] = self.faces['F'][1-i]
            tempf['F'][1-i] = self.faces['L'][1-i]
            tempf['L'][1-i] = self.faces['B'][1-i]
        
        return Cube(tempf['F'], tempf['B'], tempf['R'], tempf['L'], tempf['U'], tempf['D'])

    def u_2(self):
        # applies U2 move and returns corresponding cube
        tempf = copy.deepcopy(self.faces)
        # rotate front face
        tempf['U'] = tempf['U'].reshape(2, 2)
        tempf['U'] = np.rot90(tempf['U'], 2)
        tempf['U'] = tempf['U'].flatten()
        # reassign values
        for i in range(2):
            tempf['F'][1-i] = self.faces['B'][1-i]
            tempf['L'][1-i] = self.faces['R'][1-i]
            tempf['B'][1-i] = self.faces['F'][1-i]
            tempf['R'][1-i] = self.faces['L'][1-i]
        
        return Cube(tempf['F'], tempf['B'], tempf['R'], tempf['L'], tempf['U'], tempf['D'])

    def d_cw(self):
        # applies D move and returns corresponding cube
        tempf = copy.deepcopy(self.faces)
        # rotate front face
        tempf['D'] = tempf['D'].reshape(2, 2)
        tempf['D'] = np.rot90(tempf['D'], 3)
        tempf['D'] = tempf['D'].flatten()
        # reassign values
        for i in range(2):
            tempf['R'][2+i] = self.faces['F'][2+i]
            tempf['B'][2+i] = self.faces['R'][2+i]
            tempf['L'][2+i] = self.faces['B'][2+i]
            tempf['F'][2+i] = self.faces['L'][2+i]
        
        return Cube(tempf['F'], tempf['B'], tempf['R'], tempf['L'], tempf['U'], tempf['D'])

    def d_ccw(self):
        # applies D' move and returns corresponding cube
        tempf = copy.deepcopy(self.faces)
        # rotate front face
        tempf['D'] = tempf['D'].reshape(2, 2)
        tempf['D'] = np.rot90(tempf['D'], 1)
        tempf['D'] = tempf['D'].flatten()
        # reassign values
        for i in range(2):
            tempf['F'][2+i] = self.faces['R'][2+i]
            tempf['R'][2+i] = self.faces['B'][2+i]
            tempf['B'][2+i] = self.faces['L'][2+i]
            tempf['L'][2+i] = self.faces['F'][2+i]
        
        return Cube(tempf['F'], tempf['B'], tempf['R'], tempf['L'], tempf['U'], tempf['D'])

    def d_2(self):
        # applies D2 move and returns corresponding cube
        tempf = copy.deepcopy(self.faces)
        # rotate front face
        tempf['D'] = tempf['D'].reshape(2, 2)
        tempf['D'] = np.rot90(tempf['D'], 2)
        tempf['D'] = tempf['D'].flatten()
        # reassign values
        for i in range(2):
            tempf['B'][2+i] = self.faces['F'][2+i]
            tempf['L'][2+i] = self.faces['R'][2+i]
            tempf['F'][2+i] = self.faces['B'][2+i]
            tempf['R'][2+i] = self.faces['L'][2+i]
        
        return Cube(tempf['F'], tempf['B'], tempf['R'], tempf['L'], tempf['U'], tempf['D'])
   

class Node:
    def __init__(self, cube, g, seq):
        self.cube = cube  
        self.g = g  # path cost from root
        self.sequence = seq  # sequence of moves
        self.h = self.heu(cube)

    def heu(self, cube):
        h = 0
        for id, face in cube.faces.items():
            if id == "F":
                h += (4 - list(face).count("G"))
            if id == "B":
                h += (4 - list(face).count("B"))
            if id == "R":
                h += (4 - list(face).count("R"))
            if id == "L":
                h += (4 - list(face).count("O"))
            if id == "U":
                h += (4 - list(face).count("W"))
            if id == "D":
                h += (4 - list(face).count("Y"))
        return h/4


def cube_solver(cube):
    frontier = [Node(cube, 0, ["START"])]

    visited = []
    opposite = {"B": "F", "L": "R", "D": "U"}
    while(frontier):
        min_f = 100000
        index = None
        for i in range(len(frontier)):
            # select best node in frontier
            f = frontier[i].g + frontier[i].h
            if f < min_f:
                min_f = f
                index = i
        # remove and return node from frontier
        node = frontier.pop(index)
        if node.g <= 11:
            #print(min_f, node.g, min_f - node.g)
            if node not in visited:
                visited.append(node)
                # goal test
                if node.cube.solved:
                    return node.sequence
                # expand node and update frontier
                moves = node.cube.apply_all_moves()
                old_move = node.sequence[-1]
                for move, cube in moves.items():
                    # prune moves on same face twice in a row
                    if move[0] != old_move[0]:
                        # prune moves on opposite face
                        if old_move[0] in opposite.keys():
                            if opposite[old_move[0]] != move[0]:
                                frontier.append(Node(cube, node.g + 1, node.sequence + [move]))
                        else:
                            frontier.append(Node(cube, node.g + 1, node.sequence + [move]))

if __name__ == "__main__":

    F = np.array(["G"]*4)
    B = np.array(["B"]*4)
    R = np.array(["R"]*4)
    L = np.array(["O"]*4)
    U = np.array(["W"]*4)
    D = np.array(["Y"]*4)

    cube = Cube(F, B, R, L, U, D)

    # scrambling the cube
    while(True):
        print("Use the following symbols separated by spaces to scramble the cube:")
        print("F  F' F2 R  R' R2  U  U' U2\n")
        print("Enter sequence:", end=" ")

        # read user move sequence
        seq = str(input()).split(" ")

        # cube.print_cube()

        # apply sequence
        print("Let's Scramble cube !!!\n")
        cube = cube.apply_sequence(seq)
        # cube.print_cube()
        print("Solving.....\n")
        break

        # returns the shortest sequence to solve the cube
    solution = cube_solver(cube)

        # print out moves sequence
    string = ""
    for i in solution[1:]:
        string += (" " + i)
    print("Sequence to solve the cube: {}\n".format(string))
    for move in solution[1:]:
            # apply move
        print("Applying move:", move)
        time.sleep(1)
        cube = cube.apply_sequence([move])
    print("The cube is now solved!!!!!!\n")