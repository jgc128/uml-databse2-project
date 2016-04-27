#pragma once

#include <string>
#include <fstream>
#include <vector>
#include <iomanip>

using namespace std;

// T - the type of the records
// RS - the size of the records in the file
template<typename T, unsigned long RS>
class RecordIO {
protected:
	const char fill_char;

	string filename;
	fstream file;

	void convert_record(char * input_str, int & result);
	void convert_record(char * input_str, long & result);
	void convert_record(char * input_str, long long & result);

public:
	RecordIO(string filename);
	RecordIO(string filename, ios_base::openmode mode);
	~RecordIO();

	vector<T> read_records();
	vector<T> read_records(unsigned long n);
	vector<T> read_records(unsigned long n, unsigned long pos);

	void write_records(const vector<T> & records);
	void write_records(const vector<T> & records, unsigned long pos);
	void write_record(const T & record);
	void write_record(const T & record, unsigned long pos);

	inline unsigned long get_file_position(unsigned long pos);
	void set_read_postion(unsigned long pos);
	void set_write_postion(unsigned long pos);

	void flush();
	void close();

	string get_filename();
};


template<typename T, unsigned long RS> RecordIO<T, RS>::RecordIO(string filename)
	: RecordIO(filename, ios_base::in | ios_base::out)
{

}
template<typename T, unsigned long RS> RecordIO<T, RS>::RecordIO(string filename, ios_base::openmode mode)
	: fill_char('0')
{
	this->filename = filename;
	file.open(this->filename, mode | ios::binary);
}
template<typename T, unsigned long RS> RecordIO<T, RS>::~RecordIO()
{
	close();
}

template<typename T, unsigned long RS> void RecordIO<T, RS>::convert_record(char * input_str, int & result)
{
	result = atoi(input_str);
}
template<typename T, unsigned long RS> void RecordIO<T, RS>::convert_record(char * input_str, long & result)
{
	result = atol(input_str);
}
template<typename T, unsigned long RS> void RecordIO<T, RS>::convert_record(char * input_str, long long & result)
{
	result = atoll(input_str);
}


template<typename T, unsigned long RS> inline unsigned long RecordIO<T, RS>::get_file_position(unsigned long pos)
{
	return pos * (RS + 1); // +1 for the \n
}
template<typename T, unsigned long RS> void RecordIO<T, RS>::set_read_postion(unsigned long pos)
{
	auto real_pos = get_file_position(pos);
	file.seekg(real_pos);
}
template<typename T, unsigned long RS> void RecordIO<T, RS>::set_write_postion(unsigned long pos)
{
	auto real_pos = get_file_position(pos);
	file.seekp(real_pos);
}

template<typename T, unsigned long RS> void RecordIO<T, RS>::flush()
{
	file.flush();
}

template<typename T, unsigned long RS> string RecordIO<T, RS>::get_filename()
{
	return filename;
}

template<typename T, unsigned long RS> void RecordIO<T, RS>::close()
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
		if (file.gcount() == 0)
			continue;

		convert_record(input_str, input_val);

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
	set_read_postion(pos);
	return read_records(n);
}

template<typename T, unsigned long RS> void RecordIO<T, RS>::write_records(const vector<T> & records)
{
	for (auto const &r : records)
	{
		file << right << setfill(fill_char) << setw(RS) << r << endl;
	}
}

template<typename T, unsigned long RS> void RecordIO<T, RS>::write_records(const vector<T> & records, unsigned long pos)
{
	set_write_postion(pos);
	write_records(records);
}

template<typename T, unsigned long RS> void RecordIO<T, RS>::write_record(const T & record)
{
	file << right << setfill(fill_char) << setw(RS) << record << endl;
}
template<typename T, unsigned long RS> void RecordIO<T, RS>::write_record(const T & record, unsigned long pos)
{
	set_write_postion(pos);
	write_record(record);
}
