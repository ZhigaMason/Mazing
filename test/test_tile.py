import pytest
from mazing import TILE

@pytest.mark.parametrize(
    ['tile', '_str'],
    zip(
        [
            TILE.UP, TILE.DOWN, TILE.LEFT, TILE.RIGHT,
            TILE.HOR, TILE.VER, TILE.LEFT_UP, TILE.LEFT_DOWN,
            TILE.RIGHT_UP, TILE.RIGHT_DOWN, TILE.WALL_LEFT, TILE.WALL_RIGHT,
            TILE.WALL_UP, TILE.WALL_DOWN, TILE.EMPTY, TILE.DOWN
        ],
        [
            '╹', '╻', '╸', '╺',
            '━', '┃', '┛', '┓',
            '┗', '┏', '┣', '┫',
            '┳', '┻', '╋', '╻'
        ]
    )
)
def test_tile_to_string(tile, _str):
    assert(str(tile) == _str)

@pytest.mark.parametrize(
    ['target', 'srcs'],
    [
        (TILE.EMPTY,      [TILE.UP, TILE.DOWN, TILE.LEFT, TILE.RIGHT]),
        (TILE.VER,        [TILE.UP, TILE.DOWN]),
        (TILE.HOR,        [TILE.LEFT, TILE.RIGHT]),
        (TILE.LEFT_UP,    [TILE.LEFT, TILE.UP]),
        (TILE.LEFT_DOWN,  [TILE.LEFT, TILE.DOWN]),
        (TILE.RIGHT_UP,   [TILE.UP, TILE.RIGHT]),
        (TILE.RIGHT_DOWN, [TILE.DOWN, TILE.RIGHT]),
        (TILE.WALL_LEFT,  [TILE.UP, TILE.DOWN, TILE.RIGHT]),
        (TILE.WALL_RIGHT, [TILE.UP, TILE.DOWN, TILE.LEFT]),
        (TILE.WALL_UP,    [TILE.DOWN, TILE.LEFT, TILE.RIGHT]),
        (TILE.WALL_DOWN,  [TILE.UP, TILE.LEFT, TILE.RIGHT]),
        (TILE.WALL,       [])
    ]
)
def test_tile_values(target, srcs):
    a = 0
    for src in srcs:
        a |= src._value
    assert target._value == a

def test_tile_no_instantiation():
    with pytest.raises(TypeError, match="cannot create '.*TILE' instances"):
        TILE()

