#pragma once

#include <string>
#include <fstream>
#include <vector>

using namespace std;

// T - the type of the records
// RS - the size of the records in the file
template<typename T, unsigned long RS>
class RecordIO {
protected:
	const char fill_char;

	fstream file;
public:
	RecordIO(string filename);
	RecordIO(string filename, ios_base::openmode mode);
	~RecordIO();

	vector<T> read_records();
	vector<T> read_records(unsigned long n);
	vector<T> read_records(unsigned long n, unsigned long pos);

	void write_records(vector<T> records);
	void write_records(vector<T> records, unsigned long pos);
};


template<typename T, unsigned long RS> RecordIO<T, RS>::RecordIO(string filename)
	: RecordIO(filename, ios_base::in | ios_base::out)
{

}
template<typename T, unsigned long RS> RecordIO<T, RS>::RecordIO(string filename, ios_base::openmode mode)
	: fill_char('0')
{
	file.open(filename, mode);
}
template<typename T, unsigned long RS> RecordIO<T, RS>::~RecordIO()
{
	file.close();
}


template<typename T, unsigned long RS> vector<T> RecordIO<T, RS>::read_records()
{
	return read_records(0);
}

template<typename T, unsigned long RS> vector<T> RecordIO<T, RS>::read_records(unsigned long n)
{
	vector<T> result;
	if (n != 0)
		result.resize(n);

	char input_str[RS + 1]; // +1 for the \n
	T input_val;

	unsigned long cur_record = 0;
	while (false == file.eof() && (cur_record < n || n == 0))
	{
		file.read(input_str, RS + 1);

		stringstream convert(input_str);
		convert >> input_val;

		if (n == 0)
			result.push_back(input_val);
		else
			result[cur_record] = input_val;

		cur_record++;
	}

	// shrink the vector for the last records in the file
	if (n != 0 && cur_record != n)
	{
		result.resize(cur_record);
	}

	return result;
}

template<typename T, unsigned long RS> vector<T> RecordIO<T, RS>::read_records(unsigned long n, unsigned long pos)
{
	auto real_pos = pos * (RS + 1); // +1 for the \n
	file.seekg(real_pos);

	return read_records(n);
}

template<typename T, unsigned long RS> void RecordIO<T, RS>::write_records(vector<T> records)
{
	for (T &r : records)
	{
		file << right << setfill(fill_char) << setw(RS) << r << endl;
	}
}

template<typename T, unsigned long RS> void RecordIO<T, RS>::write_records(vector<T> records, unsigned long pos)
{
	auto real_pos = pos * (RS + 1); // +1 for the \n
	file.seekp(real_pos);

	write_records(records);
}
