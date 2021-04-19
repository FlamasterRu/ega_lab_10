#pragma once
#include <string>
#include <cmath>


extern const int LENGTH_OF_CODE;


class Bin
{
private:
	bool* arr;
	int numberD;

public:

	static const int Lsize;		// длина кодировки

	Bin()
	{
		numberD = 0;
		arr = new bool[Lsize];
		for (int i = 0; i < Lsize; ++i)
		{
			arr[i] = 0;
		}
	}


	Bin(const Bin& n) : numberD(n.numberD)
	{
		arr = new bool[Lsize];
		for (int i = 0; i < Lsize; ++i)
		{
			arr[i] = n.arr[i];
		}
	}


	Bin(const int n) : numberD(n)
	{
		int i, temp = numberD;
		arr = new bool[Lsize];
		for (i = Lsize - 1; i >= 0; --i)
		{
			arr[i] = temp % 2;
			temp /= 2;
		}
	}

	~Bin()
	{
		delete[] arr;
	}


	void operator= (const Bin& right)
	{
		numberD = right.numberD;
		for (int i = 0; i < Lsize; ++i)
		{
			arr[i] = right.arr[i];
		}
	}


	bool operator== (const Bin& right) const
	{
		return numberD == right.numberD;
	}

	bool operator!= (const Bin& right) const
	{
		return !( (*this) == right );
	}

	bool& operator[] (const int num)
	{
		if (num >= Lsize)
		{
			throw ("num >= Lsize");
		}
		return arr[num];
	}


	bool operator[] (const int num) const
	{
		if (num >= Lsize)
		{
			throw ("num >= Lsize");
		}
		return arr[num];
	}

	friend bool operator< (const Bin& left, const Bin& right);

	friend std::ostream& operator<< (std::ostream& out, const Bin& m);
};

bool operator< (const Bin& left, const Bin& right)
{
	return left.numberD < right.numberD ? 1 : 0;
}



std::ostream& operator<< (std::ostream& out, const Bin& m)
{
	for (int i = 0; i < Bin::Lsize; ++i)
	{
		out << m[i];
	}
	return out;
}




