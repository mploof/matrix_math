// matrix_math.h

#ifndef _MATRIX__MATH_h
#define _MATRIX__MATH_h

class matrix {

public:

	// Constructors, destructors, init, and memory management
	matrix();											// Creates a matrix object of undifined size
	matrix(int p_rows, int p_columns);					// Creates a matrix object of size p_rows x p_columns 
	matrix(int p_rows, int p_columns, int p_fill);		// Creates a matrix object of size p_rows x p_columns and populates every element with p_fill
	~matrix();											// Default destructor
	void init(int p_rows, int p_columns);				// Initializes matrix of size p_rows x p_columns
	void init(int p_rows, int p_columns, int p_fill);	// Initializes matrix of size p_rows x p_columns filled with p_fill

	// Set functions	
	int setValue(int p_row, int p_column, int p_value);
	int	appendRow(matrix& p_row_vector);
	int	appendCol(matrix& p_row_column);

	// Get functions
	int rowCount();
	int colCount();
	int getValue(int, int);
	void getRow(matrix&, int);
	void getColumn(matrix&, int);

	// Math functions
	// These are all static and are called from the class, not from the objects
	static int		add(matrix& p_A, matrix& p_B, matrix& p_target);
	static void		add(matrix& p_A, int p_const, matrix& p_target);
	static int		subtract(matrix& p_A, matrix& p_B, matrix& p_target);
	static void		subtract(matrix& p_A, int p_const, matrix& p_target);
	static int		mult(matrix& p_A, matrix& p_B, matrix& p_target);
	static void		mult(int p_const, matrix& p_A, matrix& p_target);
	static int		determinant(matrix& p_matrix);

	// Print functions
	void print();
	void printRow(int);
	void printCol(int);

private:

	int**		m_matrix;						// Double pointer that will become a dynamically allocated 2D matrix
	int			m_rows;							// Number of rows in the matrix
	int			m_columns;						// Number of columns in the matrix
	static bool sizeMatch(matrix&, matrix&);	// Checks whether two matrix objects have matching dimensions
	void		deleteMatrix();					// Clears the memory allocated for the matrix object and points it to NULL
	

};

#endif

