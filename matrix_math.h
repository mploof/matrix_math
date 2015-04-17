// matrix_math.h

#ifndef _MATRIX__MATH_h
#define _MATRIX__MATH_h

// If using the AT90USB board, use USBSerial class instead of Serial
#if defined(__AVR_AT90USB1287__) 
#define Com USBSerial
#else
#define Com Serial
#endif

class matrix {

public:

	// Constructors, destructors, init, and memory management
	matrix();																				// Creates a matrix object of undifined size
	matrix(int p_rows, int p_columns);														// Creates a matrix object of size p_rows x p_columns 
	matrix(int p_rows, int p_columns, int p_fill);											// Creates a matrix object of size p_rows x p_columns and populates every element with p_fill
	~matrix();																				// Default destructor
	void init(int p_rows, int p_columns);													// Initializes matrix of size p_rows x p_columns
	void init(int p_rows, int p_columns, int p_fill);										// Initializes matrix of size p_rows x p_columns filled with p_fill

	// Set functions	
	int setValue(int p_row, int p_column, int p_value);										// Set the value of the specified matrix element. Return an error code if an invalid position is given
	void set141();																			// Sets 4s down matrix diagonal with 1s on either side of 4s

	// Get functions
	int rowCount();																			// Returns the number of rows in the matrix
	int colCount();																			// Returns the number of columns in the matrix
	int getValue(int, int);																	// Returns the value of the specified element
	void getRow(int p_row, matrix& p_row_vector);											// Sets n x 1 input vector as specified column
	void getColumn(int p_column, matrix& p_column_vector);									// Sets 1 x n input vector as specified row

	// Math functions
	// These are all static and are called from the class, not from the objects
	static int		add(const matrix& p_A, const matrix& p_B, matrix& p_target);			// [A] +  [B]  = [target]
	static void		add(const matrix& p_A, int p_const, matrix& p_target);					// [A] +  int  = [target]
	static int		subtract(const matrix& p_A, const matrix& p_B, matrix& p_target);		// [A] -  [B]  = [target]
	static void		subtract(const matrix& p_A, int p_const, matrix& p_target);				// [A] -  int  = [target]
	static int		mult(const matrix& p_A, const matrix& p_B, matrix& p_target);			// [A] *  [B]  = [target]
	static void		mult(int p_const, const matrix& p_A, matrix& p_target);					// [A] *  int  = [target]
	static int		determinant(const matrix& p_matrix);									// det[A]
	static void		transpose(const matrix& p_matrix, matrix& p_target);					// transpose[A] = [target]
	static int		inverse(const matrix& p_matrix, matrix& p_target);

	// Print functions
	void print(String);																		// Prints the entire matrix with name header
	void print();																			// Prints the entire matrix
	void printRow(int);																		// Prints specified row
	void printCol(int);																		// Prints specified column

private:

	// Variables
	int**		m_matrix;																	// Double pointer that will become a dynamically allocated 2D matrix
	int			m_rows;																		// Number of rows in the matrix
	int			m_columns;																	// Number of columns in the matrix
	int			m_inv_denom;																// Denominator of inverse matrix element values
	int			m_det;																		// Determinant of the matrix
	bool		m_det_defined;																// Flag indicating whether the determinant has been calculated for the matrix yet

	// Functions
	void		deleteMatrix();																// Clears the memory allocated for the matrix object and points it to NULL
	static bool sizeMatch(const matrix&, const matrix&);									// Checks whether two matrix objects have matching dimensions
	bool		sizeMatch(const matrix&) const;												// Checks whether current matrix object and target have matching dimensions
	bool		isSquare() const;															// Checks whether matrix is square, returns true/false
	int			appendRow(const matrix& p_row_vector);										// Adds the specified vector as a new row
	int			appendCol(const matrix& p_column_vector);									// Adds the input vector as a new column
	void		divideScalar(int);															// Divides the matrix by a scalar
	int			transposeInPlace();															// Transposes a matrix in place. Can only be performed on square matricies
	int			cofactorMatrix(matrix& p_target) const;										// Creates cofactor matrix in target object
};

#endif

