using std::invalid_argument;
using std::out_of_range;
using std::logic_error;
using std::cerr;

template<typename Key>
class Array {
public:
	Array() : buffer(0), buffer_size(0), real_size(0) {}
	~Array() { delete[] buffer; }

	bool is_correct(int index) {
		if (index >= 0 && index < real_size)
			return true;
		else
			return false;
	}

	Key& operator[] (int index) {
		try {
			if (!is_correct(index)) {
				throw out_of_range("Index exceeds range");
			}
		}
		catch (out_of_range& err) {
			cerr << err.what() << "\n";
			exit(1);
		}
		return buffer[index];
	};

	void add(Key element) {
		if (real_size == buffer_size)
			grow();
		buffer[real_size] = element;
		real_size++;
	};

	void swap(int pos1, int pos2) {
		try {
			if (!is_correct(pos1) || !is_correct(pos2))
				throw out_of_range("Index exceeds range");
		}
		catch (out_of_range& err) {
			cerr << err.what() << "\n";
			exit(1);
		}
		Key temp = buffer[pos1];
		buffer[pos1] = buffer[pos2];
		buffer[pos2] = temp;
	}
	void pop_back() {
		try {
			if (real_size == 0) {
				throw logic_error("Can't delete from empty vector");
			}
		}
		catch (logic_error& err) {
			cerr << err.what() << "\n";
			exit(1);
		}
		if (real_size * 4 < buffer_size) {
			grow_down();
		}
		real_size--;
	}

	int& size() {
		return real_size;
	}

private:
	Key* buffer;
	int buffer_size;
	int real_size;

	void grow() {
		int new_buffer_size = std::max(2 * buffer_size, 1);
		Key* new_buffer = new Key[new_buffer_size];
		for (int i = 0; i < real_size; ++i)
			new_buffer[i] = buffer[i];
		delete[] buffer;
		buffer = new_buffer;
		buffer_size = new_buffer_size;
	};

	void grow_down() {
		int new_buffer_size = std::max(buffer_size / 4, 1);
		Key* new_buffer = new Key[new_buffer_size];
		for (int i = 0; i < real_size; ++i)
			new_buffer[i] = buffer[i];
		delete[] buffer;
		buffer = new_buffer;
		buffer_size = new_buffer_size;
	};
};
