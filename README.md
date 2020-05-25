# Zsqrt2-matrix-manipulator

This lets you perform gaussian transformations on matrix over ring Z[sqrt(2)] in a console-like environment.

# Getting started
After you compile the source, place file 'matrix.txt' in the same directory. Type your matrix as provided in the example below.
```
matrix.txt:     Actual matrix: 
0 -1 0 0        -sqrt(2)       0  
4 0 -2 3            4     3sqrt(2)-2
```
Start the executable from your command prompt. If you've typed your matrix properly, you'll see this:
```
(i) *** Z[<2>] Matrix manipulator, 25 may 2020 ***
(i) Reading matrix from matrix.txt...
(i) Success
(    -<2>)        0
       4   3<2>-2
Command:
```
Otherwise you'll get an error message telling you what you did wrong.
Notice that the first element of the matrix in the example is included in brackets `( )`. This is a useful feature -- see Denotions.

# This is so confusing, can I learn from an example?!
Sure, go ahead! The file example.txt is a listing of how I calculated SNF of a 4x3 matrix using this script.

# Denotions
All rows and columns are numerated from 1, not 0.

`<2>` means sqrt(2),

`( )` is notion for the non-zero element with the least norm, see example above; it is needed for calculating SNF.

`# #` is notion for ignored elements, see `ignore` in commands

# Commands
`addrow, addcol, perrow, percol, mulrow, mulcol, ignore, undo`
## addrow, addcol
`addrow i j a b` adds row `j` multiplied by `a+b*sqrt(2)` to row `i`
`addrow i j` adds row `j` multiplied by 1 to row `i`.
## perrow, percol
`perrow i j` swaps rows `i` and `j`.
## mulrow, mulcol
`mulrow i a b` multiplies row `i` by `a+b*sqrt(2)`. It gives a warning when `a+b*sqrt(2)` is not invertible 
-- multiplication by non-invertibles is not allowed when calculating Smith Normal Form.
## ignore
`ignore i j` -- ignores/unignores element on row `i`, column `j`, see below:

When an element is `ignore`'d, it is not counted when the program searches for the non-zero element with the least norm (see Denotions, `( )`). 
If an element is ignored, it has `# #` around it. To 'unignore' an element, type `ignore i j` again.

**Warning**: `ignore` must be undone by `ignore`, and not `undo`!

## undo
`undo` reverts the last action performed on the matrix (except for `ignore`, see section above). 
The program remembers 10 consequent states of the matrix.
