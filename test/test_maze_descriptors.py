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
    ['gen_init', 'gen_method', 'coords', 'tile', 'kwargs'],
    [
        (False, False, (4, 6), TILE.UP, {}),
        (True,  False, (7, 9), TILE.DOWN, dict(start=(1,4))),
        (False, True,  (3, 1), TILE.LEFT, dict(exit=(5, 6))),
        (True,  True,  (5, 4), TILE.RIGHT, dict(start=(4,7), exit=(3, 2))),
        (False, False, (randint(0, 99), randint(0, 99)), TILE.HOR, dict(height=100, width=100)),
        (True,  False, (randint(0, 99), randint(0, 99)), TILE.LEFT_UP, dict(height=100, width=100, start=(55, 99))),
        (False, True,  (randint(0, 99), randint(0, 99)), TILE.RIGHT_DOWN, dict(height=100, width=100, start=(72, 11))),
        (False, False, (randint(0, 99), randint(0, 99)), TILE.LEFT_DOWN, dict(height=100, width=100, exit=(1, 83))),
        (True,  False, (randint(0, 99), randint(0, 99)), TILE.EMPTY, dict(height=100, width=100, exit=(10, 56))),
        (False, False, (randint(0, 99), randint(0, 99)), TILE.VER, dict(height=100, width=100, start=(2, 19), exit=(0, 76))),
    ]
)
def test_maze_subscipt_operator(gen_init, gen_method, coords, tile, kwargs):
    m = Maze(**kwargs, fill=gen_init)
    if gen_init:
        assert m[*m.start] != TILE.WALL
        assert m[*m.exit]  != TILE.WALL
    m.clear()
    assert m[*m.start] == TILE.WALL
    assert m[*m.exit]  == TILE.WALL
    if gen_method:
        m.generate()
        assert m[*m.start] != TILE.WALL
        assert m[*m.exit]  != TILE.WALL
    m[*coords] = tile
    assert m[*coords] == tile

@pytest.mark.parametrize(
    ['itr', 'kwargs', '_repr', '_str'],
    [
        (
            [
                ((0,0), TILE.UP), ((0,1), TILE.DOWN), ((1, 0), TILE.LEFT),((1,1), TILE.RIGHT)
            ],
            dict(height=2, width=2),
            'S╸\n╻E\n', '# ####\n#S#  #\n######\n######\n# ##E \n# ####\n'
        ),
        (
            [
                ((0,0), TILE.UP), ((0,1), TILE.DOWN), ((1, 0), TILE.LEFT),((1,1), TILE.RIGHT)
            ],
            dict(height=2, width=2, start=(1,0), exit=(0,1)),
            '╹S\nE╺\n', '# ####\n# # S#\n######\n######\n#E##  \n# ####\n'
        ),
        (
            [
                ((0, 0), TILE.LEFT_UP), ((0, 1), TILE.LEFT_DOWN), ((1, 0), TILE.RIGHT_UP), ((1,1), TILE.RIGHT_DOWN),
            ],
            dict(height=2, width=2, start=(1,0), exit=(0,1)),
            '┛S\nE┏\n', '# ## #\n  ##S \n######\n######\n E##  \n# ## #\n'
        ),
        (
            [
                ((0,0), TILE.UP), ((0,1), TILE.DOWN), ((0, 2), TILE.LEFT),((0,3), TILE.RIGHT),
                ((1, 0), TILE.LEFT_UP), ((1, 1), TILE.LEFT_DOWN), ((1, 2), TILE.RIGHT_UP), ((1,3), TILE.RIGHT_DOWN),
                ((2, 0), TILE.HOR), ((2, 1), TILE.VER), ((2, 2), TILE.EMPTY), ((2,3), TILE.WALL),
                ((3, 0), TILE.WALL_UP), ((3, 1), TILE.WALL_LEFT), ((3, 2), TILE.WALL_RIGHT), ((3,3), TILE.WALL_DOWN),
            ],
            dict(height=4, width=4),
            'S┛━┳\n╻┓┃┣\n╸┗╋┫\n╺┏ E\n',
            '# ## #######\n#S#  #      \n########## #\n####### ## #\n# #  ## ##  \n# ## ## ## #\n#### ## ## #\n  ##       #\n####### ## #\n########## #\n#  #  ### E \n#### #######\n'
        )
    ]
)
def test_maze_str(itr, kwargs, _repr, _str):
    m = Maze(**kwargs)
    for (x,y),tile in itr:
        m[x, y] = tile
    assert repr(m) == _repr
    assert str(m)  == _str
