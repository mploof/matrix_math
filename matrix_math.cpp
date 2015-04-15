#include "Arduino.h"
#include "matrix_math.h"

/**********************************

	Constructors, destructors,
	init, and memory management

***********************************/
matrix::matrix(){

	m_matrix = NULL;
	m_rows = 0;
	m_columns = 0;

}

matrix::matrix(int p_rows, int p_columns){

	m_matrix = NULL;
	init(p_rows, p_columns);

}

matrix::~matrix() {

	// Make sure matrix memory is deallocated
	// before destructing the objects
	deleteMatrix();
}

void matrix::init(int p_rows, int p_columns){

	// Make sure any existing matrix is cleared first
	deleteMatrix();

	m_rows = p_rows;
	m_columns = p_columns;
	
	// Allocate memory for new matrix
	m_matrix = new int*[m_rows];
	for (int i = 0; i < m_rows; i++)
		m_matrix[i] = new int[m_columns];
}

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

// Public
void matrix::setValue(int p_row, int p_column, int p_value) {
	m_matrix[p_row][p_column] = p_value;
}

// Private
int matrix::appendRow(matrix& p_vector) {
	
	int new_rows = m_rows + 1;

	// Create a temporary matrix
	matrix temp(new_rows, m_columns);

	for (byte r = 0; r < new_rows; r++){
		for (byte c = 0; c < m_columns; c++){
			if (r == new_rows - 1)
				temp.m_matrix[r][c] = p_vector.m_matrix[0][c];
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
}

int matrix::appendCol(matrix& p_vector) {
	
	int new_columns = m_columns + 1;

	// Create a temporary matrix
	matrix temp(m_rows, new_columns);

	for (byte r = 0; r < m_rows; r++){
		for (byte c = 0; c < new_columns; c++){
			if (c == new_columns - 1)
				temp.m_matrix[r][c] = p_vector.m_matrix[r][0];
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
}


/*********************************

		Get Functions

**********************************/

int matrix::rowCount(){
	return m_rows;
}

int matrix::colCount(){
	return m_columns;
}

int matrix::getValue(int p_row, int p_column) {
	return m_matrix[p_row][p_column];
}

/*

Returns a column as n x 1 vector

*/
void matrix::getColumn(matrix& p_vector, int p_column) {

	// Clear any existing matrix in object
	p_vector.deleteMatrix();

	// Create m_columns x 1 vector in reference object
	p_vector.init(m_rows, 1);

	// Populate vector with selected column
	for (byte i = 0; i < m_rows; i++){
		p_vector.setValue(i, 0, m_matrix[i][p_column]);
	}
}

/*

Returns a row as 1 x n vector

*/
void matrix::getRow(matrix& p_vector, int p_row) {

	// Clear any existing matrix in object
	p_vector.deleteMatrix();

	// Create 1 x m_columns vector in reference object
	p_vector.init(1, m_columns);

	// Populate vector with selected row
	for (byte i = 0; i < m_columns; i++){
		p_vector.setValue(0, i, m_matrix[p_row][i]);
	}
}

/*********************************

		Math Functions

**********************************/

// [A]+[B], store result in target matrix
int matrix::add(matrix& p_A, matrix& p_B, matrix& p_target){

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

// [A] - const, store result in target matrix
void matrix::add(matrix& p_A, int p_const, matrix& p_target){

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

// [A]-[B], store result in target matrix
int matrix::subtract(matrix& p_A, matrix& p_B, matrix& p_target){

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

// [A] - const, store result in target matrix
void matrix::subtract(matrix& p_A, int p_const, matrix& p_target){

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

// [A]*[B], store result in target matrix
int matrix::mult(matrix& p_A, matrix& p_B, matrix& p_target){
	
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
	return 0;
}

// const * [A], store result in target matrix
void matrix::mult(int p_const, matrix& p_A, matrix& p_target){

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

// Check whether [A] and [B] have matching size in both dimensions
bool matrix::sizeMatch(matrix& p_A, matrix& p_B){
	if (p_A.m_columns == p_B.m_columns && p_A.m_rows == p_B.m_rows)
		return true;
	else
		return false;
}

int matrix::det(){



}

int matrix::factorize(matrix& p_matrix){

	for (byte i = 0; i < p_matrix.m_columns; i++){
		delay(10);
	}
}


/*********************************

		Print Functions

**********************************/

void matrix::print(){

	for (byte r = 0; r < rowCount(); r++) {
		for (byte c = 0; c < colCount(); c++) {
			Serial.print(m_matrix[r][c]);
			Serial.print("\t");
		}
		Serial.println("");
	}
	Serial.println("");

}

void matrix::printRow(int p_row){

	for (byte c = 0; c < colCount(); c++) {
		Serial.print(m_matrix[p_row][c]);
		Serial.print("\t");
	}
	Serial.println("");
	Serial.println("");
}

void matrix::printCol(int p_column){

	for (byte r = 0; r < rowCount(); r++) {
		Serial.print(m_matrix[r][p_column]);
		Serial.println("");
	}
	Serial.println("");
}

