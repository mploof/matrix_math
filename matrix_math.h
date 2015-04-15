// matrix_math.h

#ifndef _MATRIX__MATH_h
#define _MATRIX__MATH_h

class matrix {

public:

	// Constructors, destructors, init, and memory management
	matrix();
	matrix(int, int);
	~matrix();
	void init(int, int);
	void deleteMatrix();
	void resize(int, int);

	// Set functions	
	void setValue(int, int, int);
	int	 appendRow(matrix&);
	int	 appendCol(matrix&);

	// Get functions
	int rowCount();
	int colCount();
	int getValue(int, int);
	void getRow(matrix&, int);
	void getColumn(matrix&, int);

	// Math functions
	static int		add(matrix& p_A, matrix& p_B, matrix& p_target);
	static void		add(matrix& p_A, int p_const, matrix& p_target);
	static int		subtract(matrix& p_A, matrix& p_B, matrix& p_target);
	static void		subtract(matrix& p_A, int p_const, matrix& p_target);
	static int		mult(matrix& p_A, matrix& p_B, matrix& p_target);
	static void		mult(int p_const, matrix& p_A, matrix& p_target);
	int det();

	// Print functions
	void print();
	void printRow(int);
	void printCol(int);

private:

	int**		m_matrix;						// Double point that will become a dynamically allocated 2D matrix
	int			m_rows;							// Number of rows in the matrix
	int			m_columns;						// Number of columns in the matrix
	static bool sizeMatch(matrix&, matrix&);	// Checks whether two matrix objects have matching dimensions
	int			factorize(matrix&);
	

};

#endif

