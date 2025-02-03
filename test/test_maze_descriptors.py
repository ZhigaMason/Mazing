import pytest
from random import randint
from mazing import Maze, TILE

@pytest.mark.parametrize(
    ['kwargs'],
    [
        (dict(height=1),       ),
        (dict(width=1),        ),
        (dict(start=None),     ),
        (dict(start=(1,)),     ),
        (dict(start=(-1, 2)),  ),
        (dict(start=(1, -2)),  ),
        (dict(start=(-1, -2)), ),
        (dict(start=(10, 2)),  ),
        (dict(start=(1, 20)),  ),
        (dict(start=(10, 20)), ),
        (dict(exit=None),      ),
        (dict(exit=(1,)),      ),
        (dict(exit=(-1, 2)),   ),
        (dict(exit=(1, -2)),   ),
        (dict(exit=(-1, -2)),  ),
        (dict(exit=(10, 2)),   ),
        (dict(exit=(1, 20)),   ),
        (dict(exit=(10, 20)),  ),
        (dict(
            start=(1,1),
            exit=(1,1)
        ),                     )
    ]
)
def test_maze_init_exceptions(kwargs):
    with pytest.raises(ValueError):
        Maze(**kwargs)

def test_maze_str():
    pass

def test_maze_repr():
    pass

@pytest.mark.parametrize(
    ['w', 'h'],
    [
        (x, y) for x in [2, 5, 20] for y in [2, 5, 20]
    ]
)
def test_maze_widht_height(w, h):
    m = Maze(h, w)
    assert m.height == h
    assert m.width  == w

@pytest.mark.parametrize(
    ['start', 'exit', 'kwargs'],
    [
        ((0, 0),   (6, 7), {}),
        ((6, 2),   (8, 0), {}),
        ((93, 12), (74, 50), dict(height=100, width=100)),
        ((43, 1), (74, 0), dict(height=2, width=100)),
    ]
)
def test_maze_start_exit(start, exit, kwargs):
    m = Maze(**kwargs, start=start, exit=exit)
    assert m.start == start
    assert m.exit  == exit

    m = Maze(**kwargs)
    assert m.start == (0, 0)
    assert m.exit  == (m.width - 1, m.height - 1)
    m.start = start
    m.exit  = exit
    assert m.start == start
    assert m.exit  == exit

@pytest.mark.parametrize(
    ['gen_init', 'gen_method', 'kwargs'],
    [
        (True, True, {}),
        (False, True, {}),
        (True, False, {}),
        (False, False, {}),
        (True, True, dict(start=(2, 15), height=50)),
        (False, True, dict(width=4, height=3)),
        (True, False, dict(seed=1000)),
    ]
)
def test_maze_is_generated(gen_init, gen_method, kwargs):
    m = Maze(**kwargs, fill=gen_init)
    assert m.is_generated == gen_init
    m.is_generated = not m.is_generated
    assert m.is_generated != gen_init
    m.is_generated = True
    assert not m.clear().is_generated
    if gen_method:
        assert m.generate().is_generated

@pytest.mark.parametrize(
    ['gen_init', 'coords', 'tile', 'kwargs'],
    [
        (False, (4, 6), TILE.UP, {}),
        (True,  (7, 9), TILE.DOWN, dict(start=(1,4))),
        (False, (3, 1), TILE.LEFT, dict(exit=(5, 6))),
        (True,  (5, 4), TILE.RIGHT, dict(start=(4,7), exit=(3, 2))),
        (False, (randint(0, 99), randint(0, 99)), TILE.HOR, dict(height=100, width=100)),
        (True,  (randint(0, 99), randint(0, 99)), TILE.LEFT_UP, dict(height=100, width=100, start=(55, 99))),
        (False, (randint(0, 99), randint(0, 99)), TILE.RIGHT_DOWN, dict(height=100, width=100, start=(72, 11))),
        (False, (randint(0, 99), randint(0, 99)), TILE.LEFT_DOWN, dict(height=100, width=100, exit=(1, 83))),
        (True,  (randint(0, 99), randint(0, 99)), TILE.EMPTY, dict(height=100, width=100, exit=(10, 56))),
        (False, (randint(0, 99), randint(0, 99)), TILE.VER, dict(height=100, width=100, start=(2, 19), exit=(0, 76))),
    ]
)
def test_maze_subscipt_operator(gen_init, coords, tile, kwargs):
    m = Maze(**kwargs, fill=gen_init)
    if gen_init:
        assert m[*m.start] != TILE.WALL
        assert m[*m.exit]  != TILE.WALL
    m[*coords] = tile
    assert m[*coords] == tile
