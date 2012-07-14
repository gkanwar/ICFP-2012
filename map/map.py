import array

ROBOT = 'R'
WALL = '#'
ROCK = '*'
LAMBDA = '\\'
CLOSED_LIFT = 'L'
OPEN_LIFT = 'O'
EARTH = '.'
EMPTY = ' '

WIN = 1
LOSE = 2
ABORT = 3
CONTINUE = 0

class NaiveMap:
    """ This is a map simulator implementation that naively makes copies
    of the Whole Damn Map on each step """

    def __init__(self, inp):
        lines = inp.strip().splitlines()
        if '' in lines: # blank line
            i = lines.index('')
            ascii_map = lines[:i]
            metadata = lines[i+1:]
        else:
            ascii_map = lines
            metadata = []
        # parse map
        m = len(ascii_map)
        n = max(len(line) for line in ascii_map)
        grid = []
        for y, line in enumerate(reversed(ascii_map)):
            line = line.ljust(n) # pad spaces
            grid.append(array.array('c', line))
            if ROBOT in line:
                x = line.index(ROBOT)
                robot = (y, x)
        lambdas = 0
        moves = 0
        steps_underwater = 0
        self.state = (grid, robot, (lambdas, moves, steps_underwater))
        self.m = m
        self.n = n
        # parse metadata
        self.meta = {
            'Water': 0,
            'Flooding': 0,
            'Waterproof': 10,
            }
        for line in metadata:
            [key, value] = line.split()
            self.meta[key] = int(value)

    def __str__(self):
        return self.pprint(self.state)

    def pprint(self, state):
        (grid, robot, (lambdas, moves, steps_underwater)) = state
        ascii_map = '\n'.join(''.join(line) for line in reversed(grid))
        ret = """\
{}

Lambdas collected: {}
Moves made: {}
Consecutive moves underwater: {}
""".format(ascii_map, lambdas, moves, steps_underwater)
        return ret

    def step(self, command, state=None, update=True, pprint=False):
        """ Executes a command, and returns the new state. """

        (grid, robot, (lambdas, moves, steps_underwater)) = state or self.state
        newgrid = [row[:] for row in grid] # make a copy

        complete = False
        abort = False

        # robot movement
        (y, x) = robot
        if command == 'L':
            (yp, xp) = (y, x-1) # yp = "y prime"
        elif command == 'R':
            (yp, xp) = (y, x+1)
        elif command == 'U':
            (yp, xp) = (y+1, x)
        elif command == 'D':
            (yp, xp) = (y-1, x)
        newloc = grid[yp][xp]

        if newloc in [EMPTY, EARTH, LAMBDA, OPEN_LIFT]:
            robot = (yp, xp)
            newgrid[yp][xp] = ROBOT
            newgrid[y][x] = EMPTY
            if newloc == LAMBDA:
                lambdas += 1
            elif newloc == OPEN_LIFT:
                complete = True
        elif command == 'R' and newloc == ROCK and grid[y][x+2] == EMPTY:
            robot = (yp, xp)
            newgrid[yp][xp] = ROBOT
            newgrid[y][x] = EMPTY
            newgrid[y][x+2] = ROCK
        elif command == 'L' and newloc == ROCK and grid[y][x-2] == EMPTY:
            robot = (yp, xp)
            newgrid[yp][xp] = ROBOT
            newgrid[y][x] = EMPTY
            newgrid[y][x-2] = ROCK

        if command == 'A':
            abort = True
        else:
            moves += 1

        # map update
        grid = newgrid
        newgrid = [row[:] for row in grid]
        for x in range(self.n):
            for y in range(self.m):
                if grid[y][x] == ROCK:
                    if y-1 >= 0 and grid[y-1][x] == EMPTY:
                        # rock fall
                        newgrid[y][x] = EMPTY
                        newgrid[y-1][x] = ROCK
                    elif y-1 >= 0 and grid[y-1][x] == ROCK:
                        if x+1 < self.n and grid[y][x+1] == EMPTY and grid[y-1][x+1] == EMPTY:
                            # rock slide right
                            newgrid[y][x] = EMPTY
                            newgrid[y-1][x+1] = ROCK
                        elif x-1 >= 0 and grid[y][x-1] == EMPTY and grid[y-1][x-1] == EMPTY:
                            # rock slide left
                            newgrid[y][x] = EMPTY
                            newgrid[y-1][x-1] = ROCK
                    elif y-1 >= 0 and grid[y-1][x] == LAMBDA:
                        if x+1 < self.n and grid[y][x+1] == EMPTY and grid[y-1][x+1] == EMPTY:
                            # rock slide right
                            newgrid[y][x] = EMPTY
                            newgrid[y-1][x+1] = ROCK
                elif grid[y][x] == CLOSED_LIFT and not any(LAMBDA in row for row in grid):
                    # all lambdas collected
                    newgrid[y][x] = OPEN_LIFT

        # water
        water_level = self.meta['Water'] + (moves // self.meta['Flooding'] if self.meta['Flooding'] else 0)
        if robot[0] < water_level:
            steps_underwater += 1
        else:
            steps_underwater = 0

        # ending conditions
        ret = None
        if complete:
            ret = WIN
        elif abort:
            ret = ABORT
        elif steps_underwater > self.meta['Waterproof']:
            ret = LOSE
        else:
            (y, x) = robot
            if y+1 < self.m and newgrid[y+1][x] == ROCK and grid[y+1][x] != ROCK:
                ret = LOSE

        # scoring
        if ret is not None:
            score = lambdas*25 - moves
            if ret is ABORT:
                score += lambdas*25
            elif ret is WIN:
                score += lambdas*50
        else:
            ret = CONTINUE
            score = None

        state = (newgrid, robot, (lambdas, moves, steps_underwater))
        if update:
            self.state = state

        if pprint:
            print self.pprint(state)
            if ret != CONTINUE:
                print 'Mining over, score: {}'.format(score)

        return state, (ret, score)

    def transduce(self, commands, state=None, update=True, pprint=False):
        state = state or self.state
        for command in commands:
            state, (ret, score) = self.step(command, state=state, update=update)
            if ret != CONTINUE:
                break

        if pprint:
            print self.pprint(state)
            if ret != CONTINUE:
                print 'Mining over, score: {}'.format(score)

        return state, (ret, score)

            
            
