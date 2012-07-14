from mine import mine as maze
from array import array
import heapq
from Queue import Queue

class PriorityQueue:
    def __init__(self):
        self._list = []
    
    def top(self):
        return self._list[0]
    
    def push(self, item):
        heapq.heappush(self._list, item)
    
    def pop(self):
        return heapq.heappop(self._list)
    
    def empty(self):
        return len(self._list) == 0

def dijkstra(map):
    """Compute a score for all the points in the map"""
    numRows = map.state[0].width
    numCols = map.state[0].height
    distances = [array('d', [1e5]*numCols) for n in range(numRows)]
    (grid, (lambdas, moves, steps_underwater)) = map.state
    robot = grid.robot
    distances[robot[0]][robot[1]]=0
    states = {}
    que = PriorityQueue()
    que.push((0, robot, (map.state, []) ))
    while not que.empty():
        distance, loc, (state, steps) = que.pop()
        if loc in states:
            continue
        states[loc] = (state, steps)
        for command in ['U', 'D', 'L', 'R']:
            newState, (ret, score, moved) = map.step(command=command, state=state, update=False)
            if not moved or ret == maze.LOSE:
                continue
            newLoc = newState[0].robot
            #dist = 1
            dist = 1 if state[0][newLoc] == maze.EMPTY else 1.1
            if distances[newLoc[0]][newLoc[1]] > distances[loc[0]][loc[1]] + dist:
                distances[newLoc[0]][newLoc[1]] = distances[loc[0]][loc[1]] + dist
                que.push((distances[newLoc[0]][newLoc[1]], newLoc, (newState, steps+[command])))
    return distances, states

def greedy(map):
    numRows = map.state[0].width
    numCols = map.state[0].height
    moves = []
    while True:
        grid = map.state[0]
        scores, states = dijkstra(map)
        best = None
        bestScore = 1e5-1
        for i in range(numRows):
            for j in range(numCols):
                if grid[i, j] == maze.LAMBDA or grid[i, j] == maze.OPEN_LIFT:
                    if scores[i][j]<bestScore:
                        bestScore = scores[i][j]
                        best = states[i, j]
        if best is None:
            oldGrid = map.state[0]
            map.step('W')
            if oldGrid == map.state[0]:
                break
            moves.append('W')
        else:
            moves += best[1]
            map.state = best[0]
            waits = 0
        print map
    return ''.join(moves).rstrip('W') + 'A'

"""
def aStar(map, goal):
    numRows = map.m
    numCols = map.n
    distances = [array('d', [1e5]*numCols) for n in range(numRows)]
    (grid, robot, (lambdas, moves, steps_underwater)) = map.state
    distances[robot[0]][robot[1]]=0
    states = {}
    que = PriorityQueue()
    que.push((0, robot, map.state))
    while not que.empty():
        distance, loc, state = que.pop()
        if loc in states:
            continue
        states[loc] = state
        for command in ['U', 'D', 'L', 'R']:
            newState, (ret, score, moved) = map.step(command=command, state=state, update=False)
            if not moved or ret == maze.LOSE:
                continue
            newLoc = newState[1]
            dist = 1
            #dist = 1 if state[0][newLoc[0]][newLoc[1]] == maze.EMPTY else 1.1
            if distances[newLoc[0]][newLoc[1]] > distances[loc[0]][loc[1]] + dist:
                distances[newLoc[0]][newLoc[1]] = distances[loc[0]][loc[1]] + dist
                que.push((distances[newLoc[0]][newLoc[1]], newLoc, newState))
    return distances
"""
