/*
������� 10
�� �������� ������ �������� ������������� ������ ����������� ������� ������������ ����� [aij], i = 1..m, j = 1..n.  
�������� m � n ������� �� �������� � �������� �� �������� ������.
������������ ����� �������, ������� � ������ ������ ������� i-�� � j-�� ��������, ��� i � ������ ������� �������� ������ �������, ������������ ����������, 
� j � ������ ���������� �������� ������ �������, ��������, ��� ����������.
�������� � ���������� ������� ������� � �������� ����� � ������������ �������������.
*/

#include <iostream>
#include "lab1.h"

using namespace lab1;

int main() {
	Matrix* matr = input();

	if (!matr) {
		std::cout << "Incorrect data :(" << std::endl;
		return 1;
	}

	// ����������� ����� �������
	Matrix* new_matr = change_items_in_matrix(matr);
	if (!new_matr) {
		std::cout << "Error in allocate memory" << std::endl;
		erase(matr);
		return 1;
	}

	// ����� �������� � ���������� ������, ���� � ������� �� �������� �������
	output("Sourced matrix:", matr);
	output("Changed matrix:", new_matr);

	
	erase(matr);
	erase(new_matr);
	return 1;
}
