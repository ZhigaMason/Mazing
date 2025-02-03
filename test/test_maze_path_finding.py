import pytest
from random import randint
from heapq import heappop, heappush
from mazing import Maze, TILE
from dataclasses import dataclass, field
from typing import Tuple

@dataclass(order=True)
class PrioritizedTuple:
    priority: int
    tpl: Tuple=field(compare=False)


def l1_dist(a, b):
    return abs(a[0] - b[0]) + abs(a[1] - b[1])

def neighbors(coords, maze):
    val = maze[*coords]._value
    if val & TILE.UP._value:
        yield (coords[0], coords[1] - 1)
    if val & TILE.DOWN._value:
        yield (coords[0], coords[1] + 1)
    if val & TILE.RIGHT._value:
        yield (coords[0] + 1, coords[1])
    if val & TILE.LEFT._value:
        yield (coords[0] - 1, coords[1])

@pytest.mark.parametrize(
    ['kwargs', 'path_exists'],
    [
        ({}, False),
        (dict(fill=True), True),
        (dict(height=100, width=100), False),
        (dict(height=100, width=100, fill=True), True),
        (dict(height=100, width=100, start=(49,49), exit=(50,50)), False),
        (dict(height=100, width=100, start=(49,49), exit=(50,50), fill=True), True),
        (dict(height=1000, width=1000, fill=True), True),
        (dict(height=1000, width=1000, fill=True, seed=randint(0, 100)), True),
        (dict(height=1000, width=1000, fill=True, seed=randint(0, 100)), True),
    ]
)
def test_path_existence(kwargs, path_exists):
    m = Maze(**kwargs)
    heap = [PrioritizedTuple(0, m.start)]
    distance = {m.start:0}

    while heap:
        curr = heappop(heap)
        for n in neighbors(curr.tpl, m):
            if n == m.exit:
                return
            if n not in distance:
                distance.update({n: distance[curr.tpl] + 1})
                heappush(
                    heap,
                    PrioritizedTuple(distance[curr.tpl] + 1 + l1_dist(n, m.exit), n)
                )
    if path_exists:
        assert False, "Path from start to exit does not exist, though it should"

