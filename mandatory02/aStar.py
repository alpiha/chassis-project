class Node:  # Node has only PARENT_NODE, STATE, DEPTH
    def __init__(self, state, parent=None, depth=0, travel=0):
        self.STATE = state
        self.PARENT_NODE = parent
        self.DEPTH = depth
        self.travel = travel

    def path(self):  # Create a list of nodes from the root to this node.
        current_node = self
        path = [self]
        while current_node.PARENT_NODE:  # while current node has parent
            current_node = current_node.PARENT_NODE  # make parent the current node
            path.append(current_node)  # add current node to path
        return path

    def display(self):
        print(self)

    def __repr__(self):
        return (
            "State: "
            + str(self.STATE)
            + " - Depth: "
            + str(self.DEPTH)
            + " - Cost: "
            + str(self.travel)
        )


"""
Search the tree for the goal state and return path from initial state to goal state
"""


def TREE_SEARCH():
    fringe = []
    initial_node = Node(INITIAL_STATE)
    fringe = INSERT(initial_node, fringe)
    while fringe is not None:
        node = REMOVE_FIRST(fringe)
        if node.STATE == GOAL_STATE:
            return node.path()
        children = EXPAND(node)
        fringe = INSERT_ALL(children, fringe)
        fringe.sort(key=lambda node: node.travel)
        print("fringe: {}".format(fringe))


explored = []

"""
Expands node and gets the successors (children) of that node.
Return list of the successor nodes.
"""


def EXPAND(node):
    successors = []
    children = successor_fn(node.STATE)
    for child in children:
        if child not in explored:
            s = Node(node)  # create node for each in state list
            s.STATE = child  # e.g. result = 'F' then 'G' from list ['F', 'G']
            s.PARENT_NODE = node
            s.DEPTH = node.DEPTH + 1
            s.travel = STATE_SPACE[node.STATE][child]
            successors = INSERT(s, successors)
            explored.append(child)
    return successors


"""
Insert node in to the queue (fringe).
"""


def INSERT(node, queue):
    queue.append(node)
    return queue


"""
Insert list of nodes into the fringe
"""


def INSERT_ALL(list, queue):
    for i in list:
        queue.append(i)
    return queue


"""
Removes and returns the first element from fringe
"""


def REMOVE_FIRST(queue):
    return queue.pop(0)


"""
Successor function, mapping the nodes to its successors
"""


def successor_fn(state):  # Lookup list of successor states
    return STATE_SPACE[state]  # successor_fn( 'C' ) returns ['F', 'G']


INITIAL_STATE = "A"
GOAL_STATE = "S"


STATE_SPACE = {
    "A": {"B": 1, "I": 4},
    "B": {"A": 1, "C": 1, "F": 2},
    "C": {"B": 1, "D": 1},
    "D": {"C": 1, "E": 1},
    "E": {"D": 1, "L": 3},
    "F": {"B": 2, "G": 1},
    "G": {"F": 1},
    "H": {"K": 1, "L": 1},
    "I": {"A": 4, "J": 1, "O": 2},
    "J": {"I": 1, "M": 1, "K": 1},
    "K": {"J": 1, "H": 1, "L": 1},
    "L": {"E": 3, "K": 1, "N": 1},
    "M": {"J": 1, "N": 2, "P": 1},
    "N": {"L": 1, "M": 2, "S": 2},
    "O": {"P": 1, "Q": 1},
    "P": {"M": 1, "O": 1},
    "Q": {"O": 1, "R": 1},
    "R": {"Q": 1, "S": 2},
    "S": {"N": 2, "R": 2},
}

"""
Run tree search and display the nodes in the path to goal node
"""


def run():
    s = ""
    path = TREE_SEARCH()
    print("Solution path:")
    for node in path:
        s += node.STATE
    s = s[::-1]
    print(s)


if __name__ == "__main__":
    run()
