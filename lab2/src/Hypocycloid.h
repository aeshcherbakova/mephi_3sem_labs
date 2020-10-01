#ifndef _HYPOCYCLOID_
#define _HYPOCYCLOID_

#include <iostream>
#include <math.h>
#include <limits>

namespace lab2 {

	const char* exceptions[];   //������ ��������������� ��������� �� �������
	const char* menu[];         //����
	extern const int NMsgs;     //����� ������ ������������ �� ����


	// ��������� Point ������ ���������� ����� �� ���������
	struct Point {
		double x, y; // ���������� ����� �� ���������
		Point(double x0 = 0, double y0 = 0) :x(x0), y(y0) {}   // ������ �����������, ������ ����� ������ �� ����

		// ���������� ���������, ����� ������ ������ � ������ (public �� ���������)
		friend bool operator==(const Point& a, const Point& b) {
			return (a.x == b.x && a.y == b.y);
		}
	};


	class Hypocycloid {

	private:
		double r_ex, r_in;   // ������� �������/������������ (external) � ����������/������������ (internal) �����������
		double d;            // ���������� �� �����, ����������� ������������, �� ������ ������������� �����

	public:
		// ������������
		Hypocycloid(double k = 3);  // ������ �����������, ������������ ������������� k - ���������� �������� ������� � �����������
		Hypocycloid(double R, double r);
		Hypocycloid(double R, double r, double d);

		// �������
		Hypocycloid& setR_EX(double R);
		Hypocycloid& setR_IN(double r);        
		Hypocycloid& setD(double d) { Hypocycloid::d = d; return *this; }    // ����� ��� ��� �����������

		// �������
		double getR_EX() const { return r_ex; }
		double getR_IN() const { return r_in; }
		double getD() const { return d; }

		// ������ ������
		const char* type() const;
		// ������� ���������� ������� �������������� ������������ �����������
		// ������������ ������� �������� � ��������� "�����" - ������� �������, � ����� ����������
		Point limiting_radii() const { return { abs(r_ex - r_in + d), abs(r_ex - r_in - d) }; }  
		Point point_from_angle(double alpha) const;
		double curvature_radius(double alpha) const;
		double area(double alpha) const;

	};  // class Hypocycloid


	// ������ ������� ����� ������ �������� ������������� ����
	template <class T>
	int getNum(T& a, std::istream& c = std::cin) {
		do {
			c >> a;
			if (c.eof()) return 0;     // ��������� ����� �����
			if (!c.good()) {          // ���������� ������ �����     
				c.clear();
				c.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			} else return 1;
		} while (1);
	}


	// ������� ������ �� ����� ���������� ������������ (������� �� ������� �� ����)
	void print_parameters(const Hypocycloid& h, std::ostream& c);


	// ������ ���������� �������
	int (*fptr[])(const Hypocycloid&, std::istream&, std::ostream&);

	int D_find_point(const Hypocycloid&, std::istream & = std::cin, std::ostream & = std::cout),
		D_curvature_radius(const Hypocycloid&, std::istream & = std::cin, std::ostream & = std::cout),
		D_sectorial_area(const Hypocycloid&, std::istream & = std::cin, std::ostream & = std::cout);

	// ������ ���������� �������
	int dialog(std::istream& istr = std::cin, std::ostream& ostr = std::cout);
	int D_change_params(Hypocycloid&, std::istream & = std::cin, std::ostream & = std::cout);
	int D_input_angle(double&, std::istream& = std::cin, std::ostream& = std::cout);
}

#endif
