#include "Arduino.h"
#include "matrix_math.h"


/**********************************

	Constructors, destructors,
	init, and memory management

***********************************/

/*** Public ***/

// Default constructor
matrix::matrix(){

	m_matrix = NULL;
	m_rows = 0;
	m_columns = 0;

}

// Constructor that initializes matrix with size p_rows x p_columns
matrix::matrix(int p_rows, int p_columns){

	m_matrix = NULL;
	init(p_rows, p_columns);

}

// Constructor that initializes matrix with size p_rows x p_columns, where all elements = p_fill
matrix::matrix(int p_rows, int p_columns, int p_fill){

	m_matrix = NULL;
	init(p_rows, p_columns, p_fill);

}

// Default destructor
matrix::~matrix() {

	// Make sure matrix memory is deallocated
	// before destructing the objects
	deleteMatrix();
}

// Initialize matrix with size p_rows x p_columns
void matrix::init(int p_rows, int p_columns){

	// Make sure any existing matrix is cleared first
	deleteMatrix();
	
	m_rows = p_rows;
	m_columns = p_columns;
	m_inv_denom = 0;
	m_det_defined = false;
	
	// Allocate memory for new matrix
	m_matrix = new int*[m_rows];
	for (int i = 0; i < m_rows; i++)
		m_matrix[i] = new int[m_columns];
}

// Initialize matrix with size p_rows x p_columns, where all elements = p_fill
void matrix::init(int p_rows, int p_columns, int p_fill){

	init(p_rows, p_columns);

	// Pre-populate the matrix with the fill value
	for (byte r = 0; r < m_rows; r++){
		for (byte c = 0; c < m_columns; c++){
			m_matrix[r][c] = p_fill;
		}
	}

}

/*** Private ***/

// Deallocates the memory for the object matrix
void matrix::deleteMatrix() {

	// If the m_matrix pointer is null, then
	// don't delete it, since there isn't any
	// memory to deallocate.
	if (m_matrix == NULL)
		return;

	for (byte i = 0; i < m_columns; i++){
		delete[] m_matrix[i];
	}
	delete[] m_matrix;

	// Re-set m_matrix as null pointer
	m_matrix = NULL;
}


/*********************************

		Set Functions

**********************************/

/*** Public ***/

// Set the value of the specified matrix element. Return an error code if an invalid position is given
int matrix::setValue(int p_row, int p_column, int p_value) {
	
	// Don't allow setting value outside established matrix size
	if (p_row >= m_rows || p_column >= m_columns || p_row < 0 || p_column < 0)
		return -1;

	m_matrix[p_row][p_column] = p_value;

	// Determinant is no longer defined
	m_det_defined = false;
}

// Sets 4s down matrix diagonal with 1s on either side of 4s
void matrix::set141(){

	for (byte r = 0; r < m_rows; r++){
		for (byte c = 0; c < m_columns; c++){
			if (c == r)
				m_matrix[r][c] = 4;
			else if (c == r + 1 || c == r - 1)
				m_matrix[r][c] = 1;
			else
				m_matrix[r][c] = 0;
		}
	}

	// Determinant is no longer defined
	m_det_defined = false;

}

/*** Private ***/

// Adds the specified vector as a new row
// Returns an error code if the input is not a row vector with the correct number of columns
int matrix::appendRow(const matrix& p_row_vector) {
	
	if (p_row_vector.m_rows != 1 || p_row_vector.m_columns != this->m_columns)
		return -1;

	int new_rows = m_rows + 1;

	// Create a temporary matrix
	matrix temp(new_rows, m_columns);

	for (byte r = 0; r < new_rows; r++){
		for (byte c = 0; c < m_columns; c++){
			if (r == new_rows - 1)
				temp.m_matrix[r][c] = p_row_vector.m_matrix[0][c];
			else
				temp.m_matrix[r][c] = this->m_matrix[r][c];
		}
	}
	
	// Reinitialize the current matrix to the new size
	init(new_rows, m_columns);

	// Set the current matrix to the values in the temporary matrix
	for (byte r = 0; r < m_rows; r++){
		for (byte c = 0; c < m_columns; c++){
			m_matrix[r][c] = temp.m_matrix[r][c];
		}
	}

	// Determinant is no longer defined
	m_det_defined = false;
}

// Adds the input vector as a new column
// Returns an error code if the input is not a row vector with correct number of rows
int matrix::appendCol(const matrix& p_column_vector) {

	if (p_column_vector.m_columns != 1 || p_column_vector.m_rows != this->m_rows)
		return -1;
	
	int new_columns = m_columns + 1;

	// Create a temporary matrix
	matrix temp(m_rows, new_columns);

	for (byte r = 0; r < m_rows; r++){
		for (byte c = 0; c < new_columns; c++){
			if (c == new_columns - 1)
				temp.m_matrix[r][c] = p_column_vector.m_matrix[r][0];
			else
				temp.m_matrix[r][c] = this->m_matrix[r][c];
		}
	}
	
	// Reinitialize the current matrix to the new size
	init(m_rows, new_columns);
	
	// Set the current matrix to the values in the temporary matrix
	for (byte r = 0; r < m_rows; r++){
		for (byte c = 0; c < m_columns; c++){
			m_matrix[r][c] = temp.m_matrix[r][c];
		}
	}

	// Determinant is no longer defined
	m_det_defined = false;
}


/*********************************

		Get Functions

**********************************/

/*** Public ***/

// Returns the number of rows in the matrix
int matrix::rowCount(){
	return m_rows;
}

// Returns the number of columns in the matrix
int matrix::colCount(){
	return m_columns;
}

// Returns the value of the specified element
int matrix::getValue(int p_row, int p_column) {
	return m_matrix[p_row][p_column];
}

// Sets n x 1 input vector as specified column
void matrix::getColumn(int p_column, matrix& p_vector) {

	// If the target vector is the wrong size, reinitialize it
	if (p_vector.m_rows != 1 || p_vector.m_columns != this->m_columns)
		p_vector.init(this->m_rows, 1);

	// Populate vector with selected column
	for (byte i = 0; i < this->m_rows; i++){
		p_vector.setValue(i, 0, m_matrix[i][p_column]);
	}
}

// Sets 1 x n input vector as specified row
void matrix::getRow(int p_row, matrix& p_vector) {

	// If the target vector is the wrong size, reinitialize it
	if (p_vector.m_rows != 1 || p_vector.m_columns != this->m_columns)
		p_vector.init(1, this->m_columns);

	// Populate vector with selected row
	for (byte i = 0; i < this->m_columns; i++){
		p_vector.setValue(0, i, m_matrix[p_row][i]);
	}
}


/*********************************

		Math Functions

**********************************/

/*** Public ***/

// [A] + [B] = [target]
int matrix::add(const matrix& p_A, const matrix& p_B, matrix& p_target){

	// Return error code if matrix sizes do not match
	if (sizeMatch(p_A, p_B) == false)
		return -1;

	p_target.deleteMatrix();
	p_target.init(p_A.m_rows, p_A.m_columns);

	for (byte r = 0; r < p_target.m_rows; r++) {
		for (byte c = 0; c < p_target.m_columns; c++) {
			p_target.m_matrix[r][c] = p_A.m_matrix[r][c] + p_B.m_matrix[r][c];
		}
	}

	return 0;
}

// [A] + int = [target]
void matrix::add(const matrix& p_A, int p_const, matrix& p_target){

	// If the target matrix is not the correct size, reinitialize it
	// This also avoids reinitializing [A] if it is also being used as the target
	if (sizeMatch(p_A, p_target) == false)
		p_target.init(p_A.m_rows, p_A.m_rows);

	for (byte r = 0; r < p_target.m_rows; r++)
	{
		for (byte c = 0; c < p_target.m_columns; c++)
		{
			p_target.m_matrix[r][c] = p_target.m_matrix[r][c] + p_const;

		}
	}

}

// [A] - [B] = [target]
int matrix::subtract(const matrix& p_A, const matrix& p_B, matrix& p_target){

	// Return error code if matrix sizes do not match
	if (sizeMatch(p_A, p_B) == false)
		return -1;

	p_target.deleteMatrix();
	p_target.init(p_A.m_rows, p_A.m_columns);

	for (byte r = 0; r < p_target.m_rows; r++) {
		for (byte c = 0; c < p_target.m_columns; c++) {
			p_target.m_matrix[r][c] = p_A.m_matrix[r][c] - p_B.m_matrix[r][c];
		}
	}
	return 0;
}

// [A] - int = [target]
void matrix::subtract(const matrix& p_A, int p_const, matrix& p_target){

	// If the target matrix is not the correct size, reinitialize it
	// This also avoids reinitializing [A] if it is also being used as the target
	if (sizeMatch(p_A, p_target) == false)
		p_target.init(p_A.m_rows, p_A.m_rows);

	for (byte r = 0; r < p_target.m_rows; r++) {
		for (byte c = 0; c < p_target.m_columns; c++) {
			p_target.m_matrix[r][c] = p_A.m_matrix[r][c] - p_const;

		}
	}

}

// [A] * [B] = [target]
int matrix::mult(const matrix& p_A, const matrix& p_B, matrix& p_target){
	
	// Return error code if [A] column count does not match [B] row count
	if (p_A.m_rows != p_B.m_columns)
		return -1;

	// If the target matrix is not the correct size, reinitialize it
	if (p_target.m_rows != p_A.m_rows || p_target.m_columns != p_B.m_columns)
		p_target.init(p_A.m_rows, p_B.m_columns);

	int product = 0;
	int target_r = 0;
	int target_c = 0;

	for (byte i = 0; i < p_target.m_rows; i++) {
		for (byte j = 0; j < p_target.m_columns; j++) {
			int product = 0;
			for (byte k = 0; k < p_A.m_columns; k++) {
				product += p_A.m_matrix[i][k] * p_B.m_matrix[k][j];
			}
			p_target.m_matrix[i][j] = product;
		}
	}
	
	// If the [B] matrix is an inverse, divide the target by [B] inverse denominator
	if (p_B.m_inv_denom != 0)
		p_target.divideScalar(p_B.m_inv_denom);

	return 0;
}

// [A] * int = [target]
void matrix::mult(int p_const, const matrix& p_A, matrix& p_target){

	// If the target matrix is not the correct size, reinitialize it
	// This also avoids reinitializing [A] if it is also being used as the target
	if (sizeMatch(p_A, p_target) == false)
		p_target.init(p_A.m_rows, p_A.m_rows);

	for (byte r = 0; r < p_target.m_rows; r++){
		for (byte c = 0; c < p_target.m_columns; c++) {
			p_target.m_matrix[r][c] = p_A.m_matrix[r][c] * p_const;
		}
	}

}

// Checks whether matrix is square, returns true/false
bool matrix::isSquare() const {
	if (m_rows == m_columns)
		return true;
	else
		return false;
}

// det[A] (Finds determinant of 2x2 matrix, factorizes and recurses for larger matricies)
int matrix::determinant(const matrix& p_matrix){

	// Don't re-calculate if it's already been done
	if (p_matrix.m_det_defined)
		return p_matrix.m_det;

	int det = 0;	// Determinant value
	int n = 0;		// Matrix order

	// Is the matrix square? If not we cannot find the determinant, so bail
	if (p_matrix.isSquare() == false)
		return -10000;
	else
		n = p_matrix.m_rows;

	// If the matrix is larger than order 7, bail, since it will take too long to calculate
	if (n > 7)
		return -20000;

	// Is the input a 1x1 matrix? If yes, then determinant = that single value (And why are you trying to find the determinant?)
	if (n == 1)
		return p_matrix.m_matrix[0][0];

	// Is the input a 2x2 matrix? If yes, then find the determinant
	if (n == 2) {
		det = (p_matrix.m_matrix[0][0] * p_matrix.m_matrix[1][1]) - (p_matrix.m_matrix[0][1] * p_matrix.m_matrix[1][0]);
		return det;
	}
	
	// If not, find the ijth minors of the matrix until 2 x 2 minors are found
	else {
		// Create n submatricies of order n - 1 and factorize them
		for (byte i = 0; i < n; i++){

			// Save the row 0 value of the current column as the coefficient of the minor
			int minor_coeff = p_matrix.m_matrix[0][i];

			// Create minor of order n - 1
			matrix minor(n - 1, n - 1);

			// Create a submatrix column counter
			byte c_sub = 0;

			// Populate the submatrix
			for (byte c = 0; c < n; c++){

				// If the current input matrix column is that of the current coefficient, skip this iteration
				if (c == i)
					continue;

				// Populate the current column, j, of the submatrix
				for (byte r = 1; r < n; r++){
					byte r_sub = r - 1;
					minor.m_matrix[r_sub][c_sub] = p_matrix.m_matrix[r][c];
				}

				// Increment to the next submatrix column
				c_sub++;
			}

			// Reset the sub matrix column counter
			c_sub = 0;

			// Recurse function. If i is even, add the result to the current determinant value, other wise subtract it
			if (i % 2 == 0)
				det += minor_coeff * determinant(minor);
			else
				det -= minor_coeff * determinant(minor);
		}

		return det;
	}
}

// transpose[A] = [target]
void matrix::transpose(const matrix& p_matrix, matrix& p_target) {
	
	// If the target isn't the correct size, reinitialize it
	if (p_target.m_rows != p_matrix.m_columns || p_target.m_columns != p_matrix.m_rows)
		p_target.init(p_matrix.m_columns, p_matrix.m_rows);

	for (byte r = 0; r < p_matrix.m_rows; r++) {
		for (byte c = 0; c < p_matrix.m_columns; c++) {
			p_target.m_matrix[c][r] = p_matrix.m_matrix[r][c];
		}
	}
}

// [target] = [A]^-1
int matrix::inverse(const matrix& p_matrix, matrix& p_target){
	
	// Only try to invert square matricies. Return an error code if necessary
	if (p_matrix.isSquare() == false)
		return -1;

	int err = 0;

	// Create the cofactor matrix and transpose it
	p_matrix.cofactorMatrix(p_target);
	p_target.transposeInPlace();

	// Save the determinant of the original matrix as the inverse element denominator 
	// Use the precalculated determinant, if available
	if (p_matrix.m_det_defined)
		p_target.m_inv_denom = p_matrix.m_det;
	// Otherwise, calculate the determinant
	else
		p_target.m_inv_denom = determinant(p_matrix);

	return 0;
}

/*** Private ***/

// Transposes a matrix in place. Can only be performed on square matricies
int matrix::transposeInPlace(){
	
	// If the matrix is not square, return an error code
	if (isSquare() == false)
		return -1;

	int temp = 0;

	for (byte r = 1; r < m_rows; r++) {
		for (byte c = 0; c < r; c++) {
			temp = m_matrix[r][c];
			m_matrix[r][c] = m_matrix[c][r];
			m_matrix[c][r] = temp;
		}
	}
	
	// Determinant is no longer defined
	m_det_defined = false;

	return 0;
}

// Creates cofactor matrix in target object
int matrix::cofactorMatrix(matrix& p_target) const {

	// If the input matrix isn't square, bail and report error code
	if (isSquare() == false)
		return -1;

	// Set the matrix order
	int n = this->m_rows;

	// If the target isn't the correct size, reinitialize it
	if (sizeMatch(p_target) == false)
		p_target.init(n, n);

	int det = 0;

	// Hold the adjoint matricies
	matrix c(n - 1, n - 1);

	int i1 = 0;
	int j1 = 0;
	for (byte j = 0; j < n; j++) {
		for (byte i = 0; i < n; i++) {

			/* Form the adjoint a_ij */
			i1 = 0;
			for (byte ii = 0; ii<n; ii++) {
				if (ii == i)
					continue;
				j1 = 0;
				for (byte jj = 0; jj<n; jj++) {
					if (jj == j)
						continue;
					c.m_matrix[i1][j1] = this->m_matrix[ii][jj];
					j1++;
				}
				i1++;
			}

			/* Calculate the determinate */
			det = matrix::determinant(c);

			/* Fill in the elements of the cofactor */
			if ((i + j) % 2 == 0)
				p_target.m_matrix[i][j] = det;
			else
				p_target.m_matrix[i][j] = -det;
		}
	}
}

// Divides every matrix element by scalar p_input
void matrix::divideScalar(int p_input){

	// Determinant is no longer defined
	m_det_defined = false;

	for (byte r = 0; r < m_rows; r++) {
		for (byte c = 0; c < m_columns; c++) {
			m_matrix[r][c] = m_matrix[r][c] / p_input;
		}
	}

}

// Checks whether current matrix object and target have matching dimensions
bool matrix::sizeMatch(const matrix& p_B) const {
	if (this->m_columns == p_B.m_columns && this->m_rows == p_B.m_rows)
		return true;
	else
		return false;
}

// Check whether [A] and [B] have matching size in both dimensions
bool matrix::sizeMatch(const matrix& p_A, const matrix& p_B){
	if (p_A.m_columns == p_B.m_columns && p_A.m_rows == p_B.m_rows)
		return true;
	else
		return false;
}

/*********************************

		Print Functions

**********************************/

/*** Public ***/

// Prints the entire matrix with name header
void matrix::print(String p_name){
	Com.println("");
	Com.println(p_name);
	matrix::print();
}

// Prints the entire matrix
void matrix::print(){
	for (byte r = 0; r < rowCount(); r++) {
		for (byte c = 0; c < colCount(); c++) {
			
			// If this element is positive, print an extra space to align it with negative values
			if (m_matrix[r][c] >= 0)
				Com.print(" ");

			// Print the element
			Com.print(m_matrix[r][c]);

			// If this matrix is an inverse, print its denominator value
			if (m_inv_denom != 0){
				Com.print("/");
				Com.print(m_inv_denom);
			}

			// Print a tab for spacing
			Com.print("\t");
		}
		// Move to the next row
		Com.println("");
	}
}

// Prints specified row
void matrix::printRow(int p_row){

	for (byte c = 0; c < colCount(); c++) {
		Com.print(m_matrix[p_row][c]);
		Com.print("\t");
	}
	Com.println("");
}

// Prints specified column
void matrix::printCol(int p_column){

	for (byte r = 0; r < rowCount(); r++) {
		Com.print(m_matrix[r][p_column]);
		Com.println("");
	}
	Com.println("");
}

