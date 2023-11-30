#include <iostream>
#include <windows.h>
#include <ctime>
#include <cstdlib>

using namespace std;

void output_matrix(double* matrix, int strings, int columns)
{
	for (int i = 0; i < strings; i++)
	{
		for (int j = 0; j < columns; j++)
		{
			cout << *(matrix + i * columns + j) << " ";
		}
		cout << endl;
	}
}

void fill_matrix_rand(double* matrix, int strings, int columns)
{
	srand(time(nullptr));
	for (int i = 0; i < strings; i++)
	{
		for (int j = 0; j < columns; j++)
		{
			*(matrix + i * columns + j) = ((double)rand() / (double)RAND_MAX) * 200.0 - 100.0;
		}
	}
}

void copy_matrix(double* src_matrix, int src_strings, int src_columns, double* matrix, int strings, int columns)
{
	int n = 0;
	int m = 0;
	if (strings >= src_strings)
	{
		n = src_strings;
	}
	else
	{
		n = strings;
	}
	if (columns >= src_columns)
	{
		m = src_columns;
	}
	else
	{
		m = columns;
	}
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < m; j++)
		{
			*(matrix + i * columns + j) = *(src_matrix + i * src_columns + j);
		}
	}
}

double max_matrix_value(double* matrix, int strings, int columns)
{
	double max_value = -999.9;
	for (int i = 0; i < strings; i++)
	{
		for (int j = 0; j < columns; j++)
		{
			if (*(matrix + i * columns + j) > max_value)
			{
				max_value = *(matrix + i * columns + j);
			}
		}
	}
	return max_value;
}

class MATRIX
{
private:
	double* matrix;
	int strings;
	int columns;
public:
	MATRIX()
	{
		matrix = nullptr;
		strings = 0;
		columns = 0;
	}
	MATRIX(int _strings, int _columns)
	{
		if (_strings > 0 && _columns > 0)
		{
			strings = _strings;
			columns = _columns;
			matrix = new double[strings * columns];
			fill_matrix_rand(matrix, strings, columns);
		}
		else
		{
			matrix = nullptr;
			strings = 0;
			columns = 0;
		}
	}
	MATRIX(MATRIX const& src_object)
	{
		strings = src_object.strings;
		columns = src_object.columns;
		matrix = new double[strings * columns];
		copy_matrix(src_object.matrix, src_object.strings, src_object.columns, matrix, strings, columns);
	}
	~MATRIX()
	{
		if (strings > 0 && columns > 0)
		{
			delete[]matrix;
		}
	}
	void output()
	{
		if (strings > 0 && columns > 0)
		{
			cout << "Матрица: " << endl;
			output_matrix(matrix, strings, columns);
		}
		else
		{
			cout << "[Ошибка]: матрица пуста." << endl;
		}
	}
	MATRIX& operator = (MATRIX const& object)
	{
		if (&object != this)
		{
			if (strings > 0 && columns > 0)
			{
				delete[]matrix;
			}
			strings = object.strings;
			columns = object.columns;
			matrix = new double[strings * columns];
			copy_matrix(object.matrix, object.strings, object.columns, matrix, strings, columns);
		}
		return *this;
	}
	int operator [] (const int K)
	{
		int columns_number = -1;
		double sum = 0.0;
		for (int i = 0; i < columns; i++)
		{
			for (int j = 0; j < strings; j++)
			{
				sum += *(matrix + j * columns + i);
			}
			if (sum > K)
			{
				columns_number = i;
				break;
			}
			else
			{
				sum = 0.0;
			}
		}
		return columns_number;
	}
	void operator & (const int K)
	{
		double negative_max_value = -1 * max_matrix_value(matrix, strings, columns);
		for (int i = 0; i < strings; i++)
		{
			for (int j = 0; j < columns; j++)
			{
				if ((i + j) != K)
				{
					*(matrix + i * columns + j) *= negative_max_value;
				}
			}
		}
	}
	MATRIX& operator < (MATRIX const& object)
	{
		int i = 0;
		int j = 0;
		int first_matrix_positive_values = 0;
		int second_matrix_positive_values = 0;
		for (i = 0; i < strings; i++)
		{
			for (j = 0; j < columns; j++)
			{
				if (*(matrix + i * columns + j) > 0)
				{
					first_matrix_positive_values++;
				}
			}
		}
		for (i = 0; i < object.strings; i++)
		{
			for (j = 0; j < object.columns; j++)
			{
				if (*(object.matrix + i * object.columns + j) > 0)
				{
					second_matrix_positive_values++;
				}
			}
		}
		if (first_matrix_positive_values > second_matrix_positive_values)
		{
			return *this;
		}
		else
		{
			MATRIX* result_matrix = new MATRIX(object);
			return* result_matrix;
		}
	}
	friend void output_matrix(double* matrix, int strings, int columns);
	friend void fill_matrix_rand(double* matrix, int strings, int columns);
	friend void copy_matrix(double* src_matrix, int src_strings, int src_columns, double* matrix, int strings, int columns);
	friend double max_matrix_value(double* matrix, int strings, int columns);
};

int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	MATRIX A(5, 7), B(4, 5), C(6, 6);
	cout << "Матрица A:" << endl;
	A.output();
	cout << "Матрица B:" << endl;
	B.output();
	cout << "Матрица C:" << endl;
	C.output();
	int B_2 = B[2];
	cout << "\nB[2] = " << B_2 << endl;
	int A_B_2 = A[B_2];
	cout << "\nA[B[2]] = " << A_B_2 << endl;
	MATRIX D(C);
	D & A_B_2;
	cout << "\nC & A[B[2]]:" << endl;
	D.output();
	MATRIX E = B < D;
	cout << "\nB < (C & A[B[2]]):" << endl;
	C.output();
	MATRIX F = A < E;
	cout << "\nA < B < (C & A[B[2]]):" << endl;
	F.output();
	//MATRIX RESULT = A < B < (C & A[B[2]]);
	MATRIX RESULT = A < B < D;
	RESULT.output();
	return 0;
}