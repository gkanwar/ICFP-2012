import array
import copy

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

class NaiveMapState(object):
    """ This is a map representation that naively makes copies
    of the Whole Damn Map on each state update """
    def __init__(self, ascii_map, n):
        self.grid = []
        for y, line in enumerate(reversed(ascii_map)):
            line = line.ljust(n) # pad spaces
            self.grid.append(array.array('c', line))
            if ROBOT in line:
                x = line.index(ROBOT)
                robot = (x, y)
        self.robot = robot

    def __getitem__(self, loc):
        (x, y) = loc
        return self.grid[y][x]

    def __setitem__(self, loc, value):
        (x, y) = loc
        self.grid[y][x] = value

    def move(self, x, y):
        self[self.robot] = EMPTY
        self[x, y] = ROBOT
        self.robot = (x, y)

    def new(self):
        return copy.deepcopy(self)

    def __contains__(self, a):
        return any(a in row for row in self.grid)

    def __str__(self):
        ascii_map = '\n'.join(''.join(line) for line in reversed(self.grid))
        return ascii_map

class MapSimulator(object):
    """ Map simulator implementation """

    def __init__(self, inp, map_cls):
        lines = inp.rstrip().splitlines()
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
        mapstate = map_cls(ascii_map, n)
        lambdas = 0
        moves = 0
        steps_underwater = 0
        self.state = (mapstate, (lambdas, moves, steps_underwater))
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
        (mapstate, (lambdas, moves, steps_underwater)) = state
        ascii_map = str(mapstate)
        ret = """\
{}

Lambdas collected: {}
Moves made: {}
Consecutive moves underwater: {}
""".format(ascii_map, lambdas, moves, steps_underwater)
        return ret

    def step(self, command, state=None, update=True, pprint=False):
        """ Executes a command, and returns the new state. """

        (mapstate, (lambdas, moves, steps_underwater)) = state or self.state
        newmapstate = mapstate.new()

        complete = False
        abort = False
        moved = True
        wait = False

        # robot movement
        (x, y) = mapstate.robot
        if command == 'L':
            (xp, yp) = (x-1, y) # yp = "y prime"
        elif command == 'R':
            (xp, yp) = (x+1, y)
        elif command == 'U':
            (xp, yp) = (x, y+1)
        elif command == 'D':
            (xp, yp) = (x, y-1)

        newloc = mapstate[xp, yp]
        moved = None

        if newloc in [EMPTY, EARTH, LAMBDA, OPEN_LIFT]:
            newmapstate.move(xp, yp)
            if newloc == LAMBDA:
                lambdas += 1
            elif newloc == OPEN_LIFT:
                complete = True
        elif command == 'R' and newloc == ROCK and mapstate[x+2, y] == EMPTY:
            newmapstate.move(xp, yp)
            newmapstate[x, y] = EMPTY
            newmapstate[x+2, y] = ROCK
        elif command == 'L' and newloc == ROCK and mapstate[x-2, y] == EMPTY:
            newmapstate.move(xp, yp)
            newmapstate[x, y] = EMPTY
            newmapstate[x-2, y] = ROCK
        else:
            moved = False

        if moved is None:
            moved = True

        if command == 'A':
            abort = True
        else:
            moves += 1

        # map update
        mapstate = newmapstate
        newmapstate = mapstate.new()
        for x in range(self.n):
            for y in range(self.m):
                if mapstate[x, y] == ROCK:
                    if y-1 >= 0 and mapstate[x, y-1] == EMPTY:
                        # rock fall
                        newmapstate[x, y] = EMPTY
                        newmapstate[x, y-1] = ROCK
                    elif y-1 >= 0 and mapstate[x, y-1] == ROCK:
                        if x+1 < self.n and mapstate[x+1, y] == EMPTY and mapstate[x+1, y-1] == EMPTY:
                            # rock slide right
                            newmapstate[x, y] = EMPTY
                            newmapstate[x+1, y-1] = ROCK
                        elif x-1 >= 0 and mapstate[x-1, y] == EMPTY and mapstate[x-1, y-1] == EMPTY:
                            # rock slide left
                            newmapstate[x, y] = EMPTY
                            newmapstate[x-1, y-1] = ROCK
                    elif y-1 >= 0 and mapstate[x, y-1] == LAMBDA:
                        if x+1 < self.n and mapstate[x+1, y] == EMPTY and mapstate[x+1, y-1] == EMPTY:
                            # rock slide right
                            newmapstate[x, y] = EMPTY
                            newmapstate[x+1, y-1] = ROCK
                elif mapstate[x, y] == CLOSED_LIFT and LAMBDA not in mapstate:
                    # all lambdas collected
                    newmapstate[x, y] = OPEN_LIFT

        # water
        water_level = self.meta['Water'] + (moves // self.meta['Flooding'] if self.meta['Flooding'] else 0)
        if mapstate.robot[0] < water_level:
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
            (x, y) = mapstate.robot
            if y+1 < self.m and newmapstate[x, y+1] == ROCK and mapstate[x, y+1] != ROCK:
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

        state = (newmapstate, (lambdas, moves, steps_underwater))
        if update:
            self.state = state

        if pprint:
            print self.pprint(state)
            if ret != CONTINUE:
                print 'Mining over, score: {}'.format(score)

        return state, (ret, score, moved)

    def transduce(self, commands, state=None, update=True, pprint=False):
        state = state or self.state
        for command in commands:
            state, (ret, score, moved) = self.step(command, state=state, update=update)
            if ret != CONTINUE:
                break

        if pprint:
            print self.pprint(state)
            if ret != CONTINUE:
                print 'Mining over, score: {}'.format(score)

        return state, (ret, score)

            
            
