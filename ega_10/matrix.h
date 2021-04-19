#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include <ctime>


template <typename TypeMatrix>
class Matrix
{


public:

	Matrix(const std::string fileName);
	Matrix(const Matrix<TypeMatrix>& m);
	Matrix();
	Matrix(const std::initializer_list< std::initializer_list<TypeMatrix> > in);
	~Matrix();



	template <typename T>
	friend std::ostream& operator<< (std::ostream& out, const Matrix<T>& m);


	TypeMatrix& operator() (const int line, const int col) const;


	int size() const;
	int len() const;




private:
	int height;
	int width;
	TypeMatrix** arr = nullptr;



};





template <typename TypeMatrix>
Matrix<TypeMatrix>::Matrix()
{
	std::cout << "Height = ";
	std::cin >> height;
	std::cout << "Width = ";
	std::cin >> width;
	arr = new TypeMatrix * [height];
	for (int i = 0; i < height; ++i)
	{
		arr[i] = new TypeMatrix[width];
		for (int j = 0; j < width; ++j)
		{
			std::cin >> arr[i][j];
		}
	}
}


template <typename TypeMatrix>
Matrix<TypeMatrix>::Matrix(const Matrix& m)
{
	height = m.height;
	width = m.width;

	arr = new TypeMatrix * [height];
	for (int i = 0; i < height; ++i)
	{
		arr[i] = new TypeMatrix[width];
		for (int j = 0; j < width; ++j)
		{
			arr[i][j] = m.arr[i][j];
		}
	}
}


template <typename TypeMatrix>
Matrix<TypeMatrix>::Matrix(const std::string fileName)
{
	std::ifstream iFile;
	iFile.open(fileName);
	if (!iFile.is_open())
	{
		throw("Can't open file");
	}

	iFile >> height >> width;

	arr = new TypeMatrix * [height];
	for (int i = 0; i < height; ++i)
	{
		arr[i] = new TypeMatrix[width];
		for (int j = 0; j < width; ++j)
		{
			iFile >> arr[i][j];
		}
	}

	iFile.close();
}


template <typename TypeMatrix>
Matrix<TypeMatrix>::~Matrix()
{
	if (arr != nullptr)
	{
		for (int i = 0; i < height; ++i)
		{
			delete[] arr[i];
		}
		delete[] arr;
	}
}


template <typename TypeMatrix>
std::ostream& operator<< (std::ostream& out, const Matrix< TypeMatrix >& m)
{
	for (int i = 0; i < m.height; ++i)
	{
		for (int j = 0; j < m.width; ++j)
		{
			out.width(4);
			if (j == m.width - 1)
			{
				out << m.arr[i][j];
			}
			else
			{
				out << m.arr[i][j] << " ";
			}
		}
		out << "\n";
	}
	return out;
}


template <typename TypeMatrix>
TypeMatrix& Matrix<TypeMatrix>::operator() (const int line, const int col) const
{
	if (line >= height)
	{
		throw("line >= height");
	}
	if (col >= width)
	{
		throw("col >= width");
	}
	return arr[line][col];
}


template <typename TypeMatrix>
Matrix< TypeMatrix >::Matrix(const std::initializer_list< std::initializer_list<TypeMatrix> > in)
{
	arr = new TypeMatrix * [in.size()];
	int i = 0, j = 0;
	for (auto it = in.begin(); it != in.end(); ++it)
	{
		arr[i] = new TypeMatrix[(*it).size()];
		j = 0;
		for (auto jt = (*it).begin(); jt != (*it).end(); ++jt)
		{
			arr[i][j] = *jt;
			++j;
		}
		++i;
	}
	height = i;
	width = j;
}


template <typename TypeMatrix>
int Matrix< TypeMatrix >::size() const
{
	if (width != height)
	{
		throw ("width != height");
	}
	return width;
}


void createRandMatrix(const int height, const int width, std::string fileName)
{
	std::ofstream oFile;
	oFile.open(fileName);
	if (!oFile.is_open())
	{
		throw("Can't open file");
	}

	srand(time(0));
	oFile << height << " " << width << std::endl;
	for (int i = 0; i < height; ++i)
	{
		for (int j = 0; j < width; ++j)
		{
			if (j == width - 1)
			{
				oFile << (rand() % 20);
			}
			else
			{
				oFile << (rand() % 20) << " ";
			}
		}
		oFile << std::endl;
	}
	oFile.close();
}


template < typename TypeMatrix >
int Matrix< TypeMatrix >::len() const
{
	return height;
}



