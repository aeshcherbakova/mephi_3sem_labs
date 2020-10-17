﻿#include "pch.h"
#include "TimeDiagram_static.h"

namespace lab3 {

//    #############         СТРУКТУРА СИГНАЛ          #############

	// конструктр сигнала (если не указана длительность, то берем 1)
	Signal::Signal(int state, int duration) {
		if (state != 1 && state != X && state != 0)
			throw std::invalid_argument("invalid state!");
		if (duration < 0) 
			throw std::invalid_argument("invalid duration!");

		this->state = state;
		this->duration = duration;
	};

	// оператор присваивания
	Signal& Signal::operator=(const Signal& other) noexcept {
		state = other.state;
		duration = other.duration;
		return *this;
	}

	// перегруженный оператор ++ (префиксный), добавляет 1 к длительности сигнала (состояние не меняется)
	Signal& Signal::operator ++() {
		duration++;
		return *this;
	}






	//    #############         КОНСТРУКТОРЫ          #############

	// очистка диаграммы (private) offset - начиная с какого номера элемента занулять массив
	TimeDiagram& TimeDiagram::erase(int offset) {
		for (int i = offset; i < num_of_signals; i++)
			signals[i] = Signal();   // сброс сигнала в {0, 0} конструктором по умолчанию
 		num_of_signals = 0;
		return *this;
	}

	// конструктор по умолчанию - выделяет память под массив сигналов 
	TimeDiagram::TimeDiagram() noexcept {
		// здесь память под массив сигналов выделять не нужно, она выделена при компиляции
		// инициализировать сигналы тоже не нужно - при выделении памяти работает конструктор сигнала по умолчанию
		num_of_signals = 0;
	}

	// конструктор создания диаграммы с постоянным сигналом на всю длину
	TimeDiagram::TimeDiagram(int state) {
		try { signals[0] = Signal(state, max_duration); }
		catch (std::exception& ex) { throw ex; }

		num_of_signals = 1;
	}

	// копирующий конструктор
	TimeDiagram::TimeDiagram(const TimeDiagram& TD) noexcept{
		num_of_signals = TD.num_of_signals;

		for (int i = 0; i < num_of_signals; i++)
			signals[i] = TD.signals[i];
	}

	
	TimeDiagram::TimeDiagram(const Signal arr[], int size) {
		num_of_signals = size;
		for (int i = 0; i < size; i++)
			signals[i] = arr[i];
	}


	// конструктор ввода диаграммы в виде строки ASCII символов
	TimeDiagram::TimeDiagram(std::string s) {
		num_of_signals = 0;
		int temp_duration = 0;

		for (auto ch : s) {
			if (ch != '0' && ch != '1' && ch != 'X') {
				this->erase();
				throw std::invalid_argument("incorrect input!");
			}
			
			// если в строке больше сигналов, чем выделено памяти, то обрезаем строку, исключение не выкидываем
			/*if (temp_duration == max_duration) {
				this->erase();
				throw std::out_of_range("diagram max duration exceeded");
			}  */
			if (temp_duration == max_duration) return;

			temp_duration++;
			if (ch == 'X') ch = '2';
			if (num_of_signals && signals[num_of_signals - 1].state == ch - '0')
				++signals[num_of_signals - 1];
			else signals[num_of_signals++] = { ch - '0', 1 };
		}
	}





	//    #############         ГЕТТЕРЫ И СЕТТЕРЫ          #############


	TimeDiagram& TimeDiagram::set_Signals(const Signal* arr, int size) {
		if (size <= 0 || size > max_duration)
			throw std::invalid_argument("invalid size of array");
		// если количество сигналов больше, чем выделено памяти, то кидаю исключение
		// если длительность переданного массива больше, чем выделено в диаграмме, то не выкидываю исключение, а просто обрезаю массив
		int i = 0;
		int temp_duration = 0;
		for (; temp_duration < max_duration && i < size; i++) {
			// предполагается, что в качестве аргумента передан правильный массив, тогда при присваивании try-catch не нужен
			signals[i] = arr[i];
			temp_duration += signals[i].duration;
		}
		num_of_signals = i;
		return *this;
	}

	int TimeDiagram::get_Temp_Duration() const noexcept {
		int dur = 0;
		for (int i = 0; i < num_of_signals; i++)
			dur += signals[i].duration;
		return dur;
	}


//    #############         ПЕРЕГРУЖЕННЫЕ ОПЕРАТОРЫ          #############


	std::ostream& operator << (std::ostream& ostr, const TimeDiagram& td) noexcept{
		// случай пустой диаграммы
		if (!td.num_of_signals) {
			ostr << "Time diagram is empty" << std::endl;
			return ostr;
		}

		const Signal* sig_ptr = td.signals;
		std::string str_1 = "", str_0 = "", str_X = "";

		int temp_duration = 0;
		// формирование строк, отображающих сигналы
		for (int i = 0; i < td.num_of_signals; i++, sig_ptr++) {
			temp_duration += sig_ptr->duration;
			if (sig_ptr->state == 1) {
				str_1 += std::string(sig_ptr->duration, '_');
				str_0 += std::string(sig_ptr->duration, ' ');
				str_X += std::string(sig_ptr->duration, ' ');
			}
			else if (sig_ptr->state == 0) {
				str_1 += std::string(sig_ptr->duration, ' ');
				str_0 += std::string(sig_ptr->duration, '_');
				str_X += std::string(sig_ptr->duration, ' ');
			}
			else if (sig_ptr->state == X) {
				str_1 += std::string(sig_ptr->duration, ' ');
				str_0 += std::string(sig_ptr->duration, ' ');
				str_X += std::string(sig_ptr->duration, '_');
			}
		}
		
		// непосредственно вывод диаграммы
		ostr << "  ^" << std::endl;
		ostr << "  |" << std::endl;
		ostr << "  |" << std::endl;
		ostr << " 1|";
		ostr << str_1 << std::endl;
		ostr << "  |" << std::endl;
		ostr << " X|";
		ostr << str_X << std::endl;
		ostr << "  |" << std::endl;
		ostr << " 0|";
		ostr << str_0 << std::endl;
		ostr << "  |" << std::string(temp_duration + 2, '-') << '>' << std::endl;

		return ostr;
	}


	// ввод в виде строки ASCII символов, использует соответствующий конструктор
	std::istream& operator >> (std::istream& istr, TimeDiagram& td) {
		std::string input;
		if (getline(istr, input).eof())
			throw std::ios_base::failure("end of input");
		try { td = TimeDiagram(input); }
		catch (std::exception& ex) { throw ex; }
		return istr;
	}


	// оператор присваивания
	TimeDiagram& TimeDiagram::operator=(const TimeDiagram& TD)noexcept {
		if (this != &TD) {
			num_of_signals = TD.num_of_signals;
			for (int i = 0; i < num_of_signals; i++)
				signals[i] = TD.signals[i];
		}
		// в противном случае происходит самоприсваивание, ничего делать не надо
		return *this;
	}


	// оператор сложения (новая диаграмма просто пририсовывается к первой справа, если переполнение, то обрезается)
	TimeDiagram TimeDiagram::operator+(const TimeDiagram& other) const {
		if (!other.get_Num_of_Signals()) return *this;
		Signal sig_arr[max_duration];
		int i = 0, j = 0;
		for (; i < num_of_signals; i++)
			sig_arr[i] = signals[i];
		// если на стыке одинаковые сигналы, сливаем их в один
		if (i) i--;
		const Signal* other_sig = other.get_Signals();
		if (sig_arr[i].state == other_sig[0].state) {
			sig_arr[i].duration += other_sig[0].duration;
			j = 1;
		}
		for (i++; j < other.num_of_signals; i++, j++)
			sig_arr[i] = other_sig[j];

		// нам здесь не нужно считать длительность получившейся диаграммы, если она слишком большая, то будет обрезана в конструкторе
		try { TimeDiagram res(sig_arr, i); return res; }
		catch (std::exception& ex) { throw ex; }
	}


	// копирование диаграммы mult раз (если переполнение, то обрезается, исключения не выкидываются)
	TimeDiagram& TimeDiagram::operator*=(int mult) {
		if (mult < 1) throw std::invalid_argument("invalid multiplier");
		for (int cnt = 1; cnt < mult; cnt++)
			*this = *this + *this;
		return *this;
	}


	// замена диаграммы начиная с времени offset на другую диаграмму
	TimeDiagram& TimeDiagram::operator()(const TimeDiagram& other, int offset) {
		int temp_duration = this->get_Temp_Duration();
		if (offset < 0 || offset > temp_duration)
			throw std::invalid_argument("invalid offset");

		int difference = temp_duration - offset;
		for (int i = num_of_signals; i >= 0 && difference > 0; i--)
			if (signals[i].duration <= difference) {
				difference -= signals[i].duration;
				signals[i] = Signal();
			}
			else {
				signals[i].duration -= difference;
				break;
			}
		
		*this = *this + other;
		return *this;
	}



	// сдвиг диаграммы на shift влево
	// кусок длительностью shift слева просто исчезает, справа добавляются нули
	TimeDiagram& TimeDiagram::operator<<=(int shift) {
		if (shift <= 0)
			throw std::invalid_argument("shift must be bigger than zero");

		// сначала идем слева направо и считаем, сколько сигналов надо будет удалить
		int temp_shift = 0, num_to_delete = 0;
		while (temp_shift < shift && signals[num_to_delete].duration) {
			temp_shift += signals[num_to_delete].duration;
			num_to_delete++;
		}

		int difference = temp_shift - shift;  
		// если разница ноль, то сдвигаемся на num_to_delete и больше не паримся
		// если разница больше нуля, то сдвигаем на (num_to_delete - 1), а последний оставшийся укорачиваем
		// длительность последнего сигнала после укорачивания будет равна именно difference

		if (difference < 0) return this->erase();
		if (difference) {
			num_to_delete--;
			signals[num_to_delete].duration = difference;
		}
		for (int i = 0; i < num_of_signals; i++)
			// тернарный оператор не работает, видимо, его тоже надо переопределять, но мне лень разбираться
			if (i + num_to_delete >= max_duration) 
				signals[i] = Signal();
			else
				signals[i] = signals[i + num_to_delete];
		num_of_signals -= num_to_delete;
		return *this;
	}
	

	// сдвиг диаграммы на shift вправо
	// как я поняла, образовавшееся пустое место слева надо просто заполнить нулями
	TimeDiagram& TimeDiagram::operator>>=(int shift) {
		if (shift <= 0) throw std::invalid_argument("shift must be bigger than zero");
		if (!num_of_signals) return *this;

		int temp_duration = shift;
		int new_num_of_signals = num_of_signals;
		int i = 0;

		for (; i < num_of_signals && temp_duration <= max_duration; i++)
			temp_duration += signals[i].duration;
		// если переполнилась длительность, укорачиваем/зануляем текущий элемент и зануляем все следующие
		if (temp_duration >= max_duration) {
			new_num_of_signals = i;
			signals[i - 1].duration -= (temp_duration - max_duration);
			// зануляем все последующие сигналы
			this->erase(i);
		}

		// просто сдвиг вправо на 1 элемент
		num_of_signals = new_num_of_signals;
		for (int i = num_of_signals; i > 0; i--)
			signals[i] = signals[i - 1];
		num_of_signals++;

		try { signals[0] = Signal(0, shift); }
		catch (std::exception& ex) { throw ex; }

		return *this;
	}

}