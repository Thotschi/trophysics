# phydesim

Some description.

## How to write ODE

### Musts
- signature
```python
def ODE_func (t, dy, params):
```

- return value: should be numpy array

### tipps for performance


Example

```python
def ODE_func (t, y, params):
    # substitute for more intuitive names
    foo1 = params[0]
    foo2 = params[1]

    # per default C-contiguous array and more efficient than np.zeros because no values are written in memory address
    dy = np.empty(len(y), dtype=np.float64)

    # vector y has components {x, y, v_x, v_y}, dy = {v_x, v_y, a_x, a_y}
    dy[0] = y[2]
    dy[1] = y[3]
    # a_x = - G*M*x / r^3
    r = np.sqrt(y[0]**2 + y[1]**2)
    coefficient = (ph_g * m_saturn) / (r**3)
    dy[2] = - coefficient * y[0]
    dy[3] = - coefficient * y[1]
    return dy
```