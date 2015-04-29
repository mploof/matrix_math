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
	matrix(int p_rows, int p_columns);														// Creates a matrix object of size p_rows x p_columns.
	matrix(int p_rows, int p_columns, float p_fill);											// Creates a matrix object of size p_rows x p_columns and populates every element with p_fill
	~matrix();																				// Default destructor
	void init(int p_rows, int p_columns);													// Initializes matrix of size p_rows x p_columns
	void init(int p_rows, int p_columns, float p_fill);										// Initializes matrix of size p_rows x p_columns filled with p_fill

	// Set functions	
	int setValue(int p_row, int p_column, float p_value);										// Set the value of the specified matrix element. Return an error code if an invalid position is given
	int setValues(float* p_value, int p_count);												// Set matrix elements with values from 1D array
	void set141();																			// Sets 4s down matrix diagonal with 1s on either side of 4s
	int setPointConstants(float* p_value, float p_point_count);									// Sets the "C" matrix of size (p_point_count - 2) x 2
	void copy(const matrix& p_source);														// Copies source matrix into current matrix

	// Get functions
	int rowCount() const;																	// Returns the number of rows in the matrix
	int colCount() const;																	// Returns the number of columns in the matrix
	float getValue(int, int);																	// Returns the value of the specified element
	void getRow(int p_row, matrix& p_row_vector);											// Sets n x 1 input vector as specified column
	void getColumn(int p_column, matrix& p_column_vector);									// Sets 1 x n input vector as specified row

	// Math functions
	// These are all static and are called from the class, not from the objects
	static int		add(const matrix& p_A, const matrix& p_B, matrix& p_target);			// [A] +  [B]  = [target]
	static void		add(const matrix& p_A, float p_const, matrix& p_target);					// [A] +  float  = [target]
	static int		subtract(const matrix& p_A, const matrix& p_B, matrix& p_target);		// [A] -  [B]  = [target]
	static void		subtract(const matrix& p_A, float p_const, matrix& p_target);				// [A] -  float  = [target]
	static int		mult(const matrix& p_A, const matrix& p_B, matrix& p_target);			// [A] *  [B]  = [target]
	static void		mult(float p_const, const matrix& p_A, matrix& p_target);					// [A] *  float  = [target]
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
	float**		m_matrix;																	// Double pointer that will become a dynamically allocated 2D float matrix
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
	void		print(bool p_monitor, bool p_processing);									// Prints the matrix either in serial monitor or Processing format
	static int	generateID();
	int			m_nID;
};

#endif

