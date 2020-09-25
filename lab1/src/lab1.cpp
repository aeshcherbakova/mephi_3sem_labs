#include <iostream>
#include "lab1.h"


namespace lab1 {


	// ������� ����� �������� ������� � ������� ��������� �� ������
	// 1 - ��������� �������, 0 - ������
	bool input_size(const char* msg, int& dest) {
		const char* pr = "";    // ������� ��������� �� ������
		do {
			std::cout << pr << std::endl << msg;
			pr = "You are wrong. Repeat, please!";
			if (getNum(dest) < 0) return 0;
		} while (dest < 1);
		return 1;
	}



	// ������� �������� ������� ������� ������� size
	NZItem* new_array(NZItem*& arr, int size) {
		try {
			arr = new NZItem[size];
		}
		catch (std::bad_alloc& ba) {
			std::cout << "Too big array: " << size << "items; " << ba.what() << std::endl;
			return nullptr;
		}
		return arr;
	}



	// ������� ����� ������������� ����������� �������
	Matrix* input() {
		Matrix* matr = new Matrix({ 0, 0, 0, nullptr });

		// ������� �������� ������� ������������� �������
		if (!input_size("Enter number of rows in matrix ---> ", matr->m)) return nullptr;
		if (!input_size("Enter number of columns in matrix ---> ", matr->n)) return nullptr;

		if (matr->n > 2147483647 / matr->m ||  matr->n * matr->m > 20) {
			std::cout << "If you want to input all matrix, type '1', if you want to input only not zero items, input '2' --> ";
			int ans = 0;
			const char* pr = "";
			do {
				std::cout << pr << std::endl;
				pr = "You must input only '1' or '2'";
				if (getNum(ans) < 0) return nullptr;
			} while (ans != 1 && ans != 2);

			if (ans == 1 && !input_all_matrix(matr)) return nullptr;
			else if (ans == 2 && !input_only_not_zeros(matr)) return nullptr;
		}
		else if (!input_all_matrix(matr)) return nullptr;

		return matr;
	}



	// ������� ������������ ������� ������
	void erase(Matrix *matr) {
		delete[] matr->items;
		delete matr;
	}



	// ������� ������ � ������� �������
	void output(const char* msg, Matrix* matr) {
		std::cout << std::endl << msg << std::endl;
		// std::cout.setf(std::ios::fixed);    // ����� ������ � ���������� ������� (�������� �������, �� ���� �������� ������, ��� ������ ����� �������)
		int count = 0;
		for (int i = 0; i < matr->m; ++i) {
			for (int j = 0; j < matr->n; ++j)
				if (matr->items[count].row == i && matr->items[count].column == j) {
					std::cout << matr->items[count].value << " ";
					count++;
				}
				else std::cout << "0 ";
			std::cout << std::endl;
		}
	}



	// ������ ��������� ������ � ����� ������� ��� �������� ����� ��������� �������
	// ��������� 1, ���� ��� ������, � 0, ���� �� ������� ������ / �������� �� ������
	// ���� �������� � �������, ������� �� ������� ������� ��� ���������
	int input_all_matrix(Matrix* matr) {
		// ������� �������� ������ ��� 20% ��� 50% �� ���-�� ���������
		int size = 0;
		// ���� �������� ���������� ������� ������� ������, ��� ������� ���
		if (matr->n > 2147483647 / matr->m) size = 1000000;
		else {
			int full_size = matr->n * matr->m;
			size = (full_size > 100) ? (full_size / 10) : (full_size / 2 + 1);
		}
		do {
			// �������� ������ ��� ������, ���� �������, ��������� ���������� ��������� � 2 ����
			if (!new_array(matr->items, size)) size /= 2;
		} while (!matr->items && size > 1);

		if (!matr->items) {
			std::cout << "Problems with memory allocation :(" << std::endl;
			return 0;
		}


		double temp;
		for (int i = 0; i < matr->m; i++) {
			std::cout << "Enter items for matrix line #" << (i + 1) << ":" << std::endl;
			for (int j = 0; j < matr->n; j++) {
				if (getNum(temp) < 0) {   // �������� � ������ �����
					erase(matr);
					return 0;
				}
				// ���������� ������ ������
				if (temp != 0) {
					// ����������� ������, ���� ���������� �������� ������������
					if (matr->NZ_items == size) {
						int add_items = 5;
						NZItem* new_arr = nullptr;
						do {
							new_arr = resize_array(matr->items, size, size + add_items--);
						} while (!new_arr && add_items > 0);
						if (!new_arr) {
							// �� ���������� ������ ���� ��� ���� �������, ����������
							std::cout << "Too many items :(" << std::endl;
							erase(matr);
							return 0;
						}
						matr->items = new_arr;
						size += add_items + 1;
					}

					// ������ ������� ����������, ��������� ����� ��������� �������
					matr->items[matr->NZ_items] = { i, j, temp };
					matr->NZ_items++;
				}
			}
		}

		return 1;
	}



	// ������� ��� ����� ������� � ������� (row, column, value) ������ �������
	// ���������� 1, ���� ��� ������, � 0, ���� �� ������� ������ / �������� �� ������
	int input_only_not_zeros(Matrix* matr) {
		std::cout << "Input number of not zero items --> ";
		const char* pr = "";    // ������� ��������� �� ������
		do {
			std::cout << pr << std::endl;
			pr = "Number of not zero items must be bigger than 0";
			if (getNum(matr->NZ_items) < 0) return 0;
		} while (matr->NZ_items <= 0);

		// ���� �������� ��������� �������� 1000 ��������� � ������� 5*5
		if (matr->NZ_items > matr->m * matr->n) matr->NZ_items = matr->m * matr->n;
		if(!new_array(matr->items, matr->NZ_items)) return 0;     // ��������� ������

		// ���� ����� �����
		for (int i = 0; i < matr->NZ_items; i++) {
			std::cout << "Input row, column and value of not zero item #" << i + 1 << " --> ";
			NZItem* temp = matr->items + i;
			if (getNum(temp->row) < 0 || getNum(temp->column) < 0 || getNum(temp->value) < 0) {
				// �������� � ������ �����
				erase(matr);
				return 0;
			}

			// �������� �� ������, ���� ������������ �����
			if (temp->row < 0 || temp->row > matr->m - 1) {
				std::cout << "Row of item must be >= 0 and < number of rows in matrix!" << std::endl;
				i--;
				continue;
			}
			if (temp->column < 0 || temp->column > matr->n - 1) {
				std::cout << "Column of item must be >= 0 and < number of columns in matrix!" << std::endl;
				i--;
				continue;
			}
			if (!temp->value) {
				std::cout << "This item must be not zero!" << std::endl;
				i--;
				continue;
			}

			// ���������� ��������� (������� �������� �� ������� ����)
			while (temp > matr->items && temp->row < (temp - 1)->row) {
				temp = swap_left(temp);
			}
			while (temp > matr->items && temp->column < (temp - 1)->column) {
				temp = swap_left(temp);
			}
		}
		return 1;
	}



	// ������� �������� ����� �������:
	// ������� � ������ ������ ������� i-�� � j-�� ��������, ��� i � ������ ������� �������� ������ �������, ������������ ����������, 
	// � j � ������ ���������� �������� ������ �������, ��������, ��� ����������.
	Matrix* change_items_in_matrix(Matrix* matr) {

		// ������� ������ ����� �������, ������, ����� ������ �� ���������
		Matrix* new_matr = new Matrix({ matr->m, matr->n, matr->NZ_items, nullptr });
		if(!new_array(new_matr->items, new_matr->NZ_items)) return nullptr;

		// ����������, �������� ��������
		// ����������� ���������� ����� 2 ����� - I � J ������ ��������, � ����� 2 ���������
		// ��������� �� ��� ������, ������� ���� �������� �������. ���� ����, �� ��������� ������, ���� �� ����, �� ��������� �������
		int I = -1, J = -1;
		NZItem* I_ptr = nullptr, * J_ptr = nullptr;
		NZItem* new_matr_ptr = new_matr->items;
		NZItem* ptr = matr->items;            // ���������, ������� ����� ������ �� ������� 
		int start_line_offset = 0;   // ����� ��������� ������������ ������ ������� (������ ������ � ������ ������)
		int end_line_offset = 0;     // ����� ��������� ������������ ������ ������� (��������� ������ � ������ ������)

		for (int i = 0; i < matr->m; ++i) {
			// ���� ����������� ��� ��� ������, � ������� �� ���������, ��������� ����
			if (start_line_offset == matr->NZ_items) break;
			ptr = matr->items + start_line_offset;
			if (ptr->row != i) continue;     // ���� � ������ ������ ��� �������, ��������� �� ���������
			new_matr_ptr = new_matr->items + start_line_offset;

			// �� ���� ������� ����� ���� ����������, ��� � ������ ���� ��������� �������
			I_ptr = find_I_item_in_a_row(matr, ptr, i, I);

			// ��������� ��������� �� ������ � ��������� ������ ������
			while (ptr < matr->items + matr->NZ_items - 1 && (ptr + 1)->row == i) ptr++;
			end_line_offset = ptr - matr->items;    // ���������� ����� ���������� ������ � ������ ������
			// ������ ptr ��������� �� ��������� ������ � ������, �� ������ ������ J

			J_ptr = find_J_item_in_a_row(matr, ptr, i, J);

			//std::cout << std::endl << "row = " << i << " I = " << I << " J = " << J << std::endl;
			
			// �������� �� ������� ������� � ����� � ������ ������� ����
			// �������� ��� �� start_ptr �� next_row_ptr 
			// 4 ��������, ��� �������� ������ �������:

			// (1) ���� ���� �� I ��� J �� ����������, ����� ���������� � ������ �� ������
			if (I == -1 || J == -1) 
				simple_copy(matr->items, new_matr->items, start_line_offset, end_line_offset);

			// (2) ������� - ���� ����, ������ �� ����, ���� �������� (����� ������� �����������)
			else if (I_ptr && !J_ptr || J_ptr && !I_ptr) {
				// ������� �����������, ��� ����, � ��� �� ����
				NZItem* not_zero = nullptr;
				int zero_column = -1;
				if (I_ptr) { not_zero = I_ptr; zero_column = J; }
				else { not_zero = J_ptr; zero_column = I; }

				ptr = matr->items + start_line_offset;
				//new_matr_ptr = new_matr->items + temp_line_offset;
				int flag = false;    // ���� = 0, ���� ������ �� ���������, = 1 ����� �����������
				for (int k = start_line_offset; k <= end_line_offset; k++, ptr++, new_matr_ptr++) {

					// ������ ���� �������� �� ����� ���� - ������� ��������� ����� �� ���� ������
					// ���� ������ ������: ptr - ��������� � ������� / ��������� � ������ / ��������� ����� ���, ���� ���� �������� ���
					if (!flag && (k == end_line_offset || (ptr + 1)->column >= zero_column || (ptr + 1)->row != i)) {
						// ������� �������� ������� ��������, ���� ��� �� not_zero
						if (ptr->column != not_zero->column) {
							*new_matr_ptr = { i, ptr->column, ptr->value };
							new_matr_ptr++;
						}

						// ����� ���� ��������� not_zero
						*new_matr_ptr = { i, zero_column, not_zero->value };
						flag = true;   // ����� �������� �� �������� ��������� ���
					}
					// ������, ������� ����������, ���������� �� ����
					else if (ptr->column == not_zero->column)
						new_matr_ptr--;
					// ���� �� ������ �� I � J, ����� �������� ������� ��������
					else *new_matr_ptr = { i, ptr->column, ptr->value };
				}
			}

			else {
				// (3) ���� ��� �� ����, ������ ������� �������� � ������� � �������� �������, �������� � �����, ����� ������ � �������� �������
				// ������, ���������� ���, ��� � ��� ��������� ��������� �� ��� ������, ��� �� ����� ������ �� ������
				if (I_ptr && J_ptr) swap(I_ptr->value, J_ptr->value);

				
				// (3) ���� ��� ����, �� ������ �� ������
				simple_copy(matr->items, new_matr->items, start_line_offset, end_line_offset);

				// ���������� I � J � �������� ������� �� �����
				if (I_ptr && J_ptr) swap(I_ptr->value, J_ptr->value);
			}
			
			start_line_offset = ++end_line_offset;
		}

		return new_matr;
	}



	// ����: �� ����� ���������� ������ ������� ������ ������, ���������� ������������� ������ ������ ������ � ���� ��� ������� (�� ���� ������� �� ������)
	// ������������� ������, ���� � ��� �� ���������� ������
	// ���������� ���, ��� ������ ������� ������� ����� �������������, ��� ��� ����������� � ������ - ����� �������
	// ��� ����: ��� ��� ���������� ������ ������ � ���������, ��� �� ���� ������������� ��� ������������� ������
	// ����� ����� ����� ����, ������ ���� ��� ������ ������� �� ������ ��������� ����� (�� ��� ���� ��, �� ������� �����������)



	// ������� ������� ����� I ������� �������� (������, ������� ������, ��� ����������)
	// ���� ��� ����, ���������� ������� ���������, ���� �� ����, �� ����������� ���������
	NZItem* find_I_item_in_a_row(Matrix* matr, NZItem *&ptr, int temp_row, int& I) {
		// ����� ������� ���� ������ ������� �����������
		while (ptr < matr->items + matr->NZ_items && ptr->row == temp_row) {
			int column = ptr->column;
			// ���� ptr �� ������� �����, ���������� �������� � ����� �������
			if (column != 0) {
				NZItem* prev = (ptr == matr->items) ? nullptr : (ptr - 1);
				double left_value = (prev && prev->row == temp_row && prev->column == column - 1) ? prev->value : 0;

				// �������, ��� ������, � ������ I, ���� �������
				if (ptr->value > left_value) {
					I = column;
					return ptr;
				}
			}

			// ���� ptr �� ������� ������, ���������� �������� � ������ �������
			if (column != matr->n - 1) {
				NZItem* next = (ptr == matr->items + matr->NZ_items) ? nullptr : (ptr + 1);
				double right_value = (next && next->row == temp_row && next->column == column + 1) ? next->value : 0;

				// �������, ��� ������, � ������ I
				if (right_value > ptr->value) {
					I = column + 1;
					return (right_value ? next : nullptr);
				}
			}
			ptr++;
		}
		ptr--;
		I = -1;
		return nullptr;  // ���� �� ������� ������ �����
	}



	// ������� ������� ����� J ������� �������� (���������, ������� ������, ��� ����������)
	// ���� ��� ����, ���������� ������� ���������, ���� �� ����, �� ����������� ���������
	NZItem* find_J_item_in_a_row(Matrix* matr, NZItem*& ptr, int temp_row, int& J) {
		for (; ptr >= matr->items && ptr->row == temp_row; ptr--) {
			int column = ptr->column;

			// ���� ptr �� ������� ������, ���������� �������� � ������ �������
			if (column != matr->n - 1) {
				NZItem* next = (ptr == matr->items + matr->NZ_items - 1) ? nullptr : (ptr + 1);
				double right_value = (next && next->row == temp_row && next->column == column + 1) ? next->value : 0;

				// �������, ��� ������, � ������ J
				if (right_value < ptr->value) {
					J = column + 1;
					return (right_value ? next : nullptr);
				}
			}

			// ���� ptr �� ������� �����, ���������� �������� � ����� �������
			if (column != 0) {
				NZItem* prev = (ptr == matr->items) ? nullptr : (ptr - 1);
				double left_value = (prev && prev->row == temp_row && prev->column == column - 1) ? prev->value : 0;

				// �������, ��� ������, � ������ I, ���� �������
				if (ptr->value < left_value) {
					J = column;
					return ptr;
				}
			}
		}
		J = -1;
		return nullptr;
	}



	// ������� ����������� ���������� ������� �� ������� ������� � ����� ��� ���������
	void simple_copy(NZItem *old_array, NZItem* new_array, int start_offset, int finish_offset) {
		for (int i = start_offset; i <= finish_offset; i++) {
			NZItem* temp = old_array + i;
			new_array[i] = { temp->row, temp->column, temp->value };
		}
	}
}
