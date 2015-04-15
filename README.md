# matrix_math
A simple matrix class specifically for handling integer matricies. The intent of this class is to create matrix objects with the necessary functions to be used for spline interpolation through fixed points. For my particular application, this can be achieved entirely with integers, so this library does not support floating point math in the interest of increasing calculation speed.

#####Current Functions Available
- Add, subtract, multiply matricies
- Add, subtract, multiply matrix by integer
- Append vectors to matricies as rows and columns
- Extract rows and and columns from matricies to vectors
- Print a row, column, or entire matrix

#####Functions to be Added
- Find determinant for order n matrix
- Row reduction


#####Useful Spline Interpolation References
- http://www.math.ucla.edu/~baker/149.1.02w/handouts/dd_splines.pdf
- http://goo.gl/8ugmXZ
