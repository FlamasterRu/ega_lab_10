#include "matrix.h"
#include "code.h"
#include <vector>
#include <random>


/// Варинат 7

const int LENGTH_OF_CODE = 15;		// Длина кодировка (количество предметов)
const int Bin::Lsize = LENGTH_OF_CODE;


// Возвращает вес кодировки
int Weight(const Bin& b, const Matrix<int>& m)
{
	int sum = 0;
	for (int i = 0; i < LENGTH_OF_CODE; ++i)
	{
		sum += b[i] * m(i, 2);		// 2 это столбец весов
	}
	return sum;
}

// Возвращает ценность кодировки
int Cost(const Bin& b, const Matrix<int>& m)
{
	int sum = 0;
	for (int i = 0; i < LENGTH_OF_CODE; ++i)
	{
		sum += b[i] * m(i, 1);		// 1 это столбец цен
	}
	return sum;
}



// Случайный алгоритм генерации начальной популяции с контролем ограничений
std::vector< Bin > CreateNewPopulationRand(const int size, const Matrix<int>& m, const int bagCapacity)
{
	std::vector< Bin > Result;
	int CurrentCapacity;
	while (Result.size() < size)
	{
		Bin temp( rand() % static_cast<int>(pow(2, Bin::Lsize)) );
		if (Weight(temp, m) <= bagCapacity)
		{
			Result.push_back(temp);
		}
	}
	return Result;
}


// Жадный алгоритм генерации начальной популяции
std::vector< Bin > CreateNewPopulationGreedy(const int size, const Matrix<int>& m, const int bagCapacity)
{
	std::vector< Bin > Result;
	int CurrentCapacity, i, MaxPrice, j, NumberMax;
	while (Result.size() < size)
	{
		Bin temp;
		CurrentCapacity = 0;
		i = rand() % LENGTH_OF_CODE;
		temp[i] = 1;
		CurrentCapacity += temp[i] * m(i, 2);		// 2 - это столбец весов в матрице

		i = 1;
		while ( (i < LENGTH_OF_CODE) )
		{
			MaxPrice = -1;
			for (j = 0; j < LENGTH_OF_CODE; ++j)
			{
				if ((MaxPrice < m(j, 1)) and (temp[j] == 0) and (CurrentCapacity + m(j, 2) <= bagCapacity))		// 1 это столбец цен в матрице, 2 - это столбец весов в матрице
				{
					MaxPrice = m(j, 1);
					NumberMax = j;
				}
			}
			if (MaxPrice == -1)
			{
				break;
			}
			temp[NumberMax] = 1;
			CurrentCapacity += m(NumberMax, 2);
			++i;
		}
		Result.push_back(temp);
	}


	return Result;
}




// Одноточечный кроссовер со случайным выбором родителей
void SinglePointReproduction(const int size, const Matrix<int>& m, const int bagCapacity, std::vector< Bin >& population)
{
	if (population.size() > 40)
	{
		srand(time(0));
	}
	int p1, p2, point, n, l;
	l = population.size();
	n = population.size() * 2 + rand() % (5 * size) - size / 4;
	while (population.size() < n)
	{
		Bin temp;
		p1 = rand() % l;
		do
		{
			p2 = rand() % l;
		} while (p1 == p2);

		point = rand() % (LENGTH_OF_CODE - 1) + 1;		// от 1 до 14 <
		for (int j = 0; j < point; ++j)
		{
			temp[j] = population.at(p1)[j];
		}
		for (int j = point; j < LENGTH_OF_CODE; ++j)
		{
			temp[j] = population.at(p2)[j];
		}
		if (Weight(temp, m) <= bagCapacity)
		{
			population.push_back(temp);
		}
	}
}


// Однородный кроссовер с наиболее приспособленными родителями
void HomogeneousReproduction(const int size, const Matrix<int>& m, const int bagCapacity, std::vector< Bin >& population)
{
	if (population.size() > 40)
	{
		srand(time(0));
	}
	Bin Best1, Best2;
	int max = -1;
	for (int i = 0; i < population.size(); ++i)
	{
		if (max < Cost(population[i], m))
		{
			max = Cost(population[i], m);
			Best1 = population[i];
		}
	}
	max = -1;
	for (int i = 0; i < population.size(); ++i)
	{
		if ( (max < Cost(population[i], m)) and (population[i] != Best1) )
		{
			max = Cost(population[i], m);
			Best2 = population[i];
		}
	}
	int n;
	n = population.size() * 2 + rand() % (5 * size) - size / 4;
	while (population.size() < n)
	{
		Bin temp;
		for (int i = 0; i < LENGTH_OF_CODE; ++i)
		{
			max = rand() % 2;
			if (max == 0)
			{
				temp[i] = Best1[i];
			}
			else
			{
				temp[i] = Best2[i];
			}
		}
		if (Weight(temp, m) <= bagCapacity)
		{
			population.push_back(temp);
		}
	}
}


// Мутация Сальтация (где два случайных бита меняются местами)
void MutationSaltation(const int size, const Matrix<int>& m, const int bagCapacity, std::vector< Bin >& population)
{
	if (population.size() > 40)
	{
		srand(time(0));
	}
	int p, g1, g2;
	Bin temp;
	for (int i = 0; i < population.size(); ++i)
	{
		p = rand() % 5;		//  мутация с вероятностью 0.2
		if (p == 0)
		{
			g1 = rand() % LENGTH_OF_CODE;
			do
			{
				g2 = rand() % LENGTH_OF_CODE;
			} while (g1 == g2);
			temp = population[i];
			p = temp[g1];
			temp[g1] = temp[g2];
			temp[g2] = p;
			if (Weight(temp, m) <= bagCapacity)
			{
				population.push_back(temp);
			}
		}
	}
}


// Мутация Дополнение (где каждый бит меняется на противоположенный)
void MutationAddition(const int size, const Matrix<int>& m, const int bagCapacity, std::vector< Bin >& population)
{
	if (population.size() > 40)
	{
		srand(time(0));
	}
	int p;
	Bin temp;
	for (int i = 0; i < population.size(); ++i)
	{
		p = rand() % 10;		// мутация с вероятностью 0.1
		if (p == 0)
		{
			temp = population[i];
			for (int j = 0; j < LENGTH_OF_CODE; ++j)
			{
				if (temp[j] == 0)
				{
					temp[j] = 1;
				}
				else
				{
					temp[j] = 0;
				}
			}
			if (Weight(temp, m) <= bagCapacity)
			{
				population.push_back(temp);
			}
		}

	}
}


// Оператор селекции. Стратегия перекрывающихся поколений. Элитарная. B-турнир
std::vector< Bin > SelectionBTournament(const int size, const Matrix<int>& m, const int bagCapacity, std::vector< Bin >& population)
{
	if (population.size() > 40)
	{
		srand(time(0));
	}
	std::vector< Bin > NextGen;
	Bin temp;
	int max = -1;
	for (int i = 0; i < population.size(); ++i)
	{
		if (max < Cost(population[i], m))
		{
			max = Cost(population[i], m);
			temp = population[i];
		}
	}
	NextGen.push_back(temp);		// лучший проходит сразу

	int b, i, num;
	b = 2;			// размер турнира
	std::vector< Bin > B;
	i = 0;
	for (int j = 1; j < size; ++j)
	{
		B.clear();
		while (B.size() < b)
		{
			num = rand() % size;
			if (num == 0)
			{
				B.push_back(population[i % population.size()]);
			}
			++i;
		}
		max = -1;
		for (int k = 0; k < b; ++k)
		{
			if (max < Cost(B[k], m))
			{
				max = Cost(B[k], m);
				temp = B[k];
			}
		}
		NextGen.push_back(temp);
	}
	return NextGen;
}


// Оператор селекции. Стратегия перекрывающихся поколений. Элитарная. Универсальный выбор
std::vector< Bin > SelectionUniversaChoice(const int size, const Matrix<int>& m, const int bagCapacity, std::vector< Bin >& population)
{
	if (population.size() > 40)
	{
		srand(time(0));
	}
	std::vector< Bin > NextGen;
	Bin temp;
	int max = -1;
	for (int i = 0; i < population.size(); ++i)
	{
		if (max < Cost(population[i], m))
		{
			max = Cost(population[i], m);
			temp = population[i];
		}
	}
	NextGen.push_back(temp);		// лучший проходит сразу


	std::vector< double > RandomVariables;
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> dis(0.0, 1.0);		// равномерное распределение
	for (int i = 0; i < size; ++i)
	{
		RandomVariables.push_back(dis(gen));
	}

	int denominator = 0;
	for (int i = 0; i < population.size(); ++i)
	{
		denominator += Cost(population[i], m);
	}
	double x;
	int i;
	for (int k = 0; k < size; ++k)
	{
		x = 0;
		for (i = 0; i < population.size(); ++i)
		{
			x += static_cast< double >(Cost(population[i], m)) / static_cast<double>(denominator);
			if (x >= RandomVariables[k])
			{
				break;
			}
		}
		NextGen.push_back(population[i]);
	}
	return NextGen;
}




int main()
{
	setlocale(LC_ALL, "rus");
	srand(time(0));
	
	Matrix<int> Input = {
		{1, 11, 6},
		{2, 1, 20},
		{3, 17, 16},
		{4, 8, 17},
		{5, 9, 19},
		{6, 6, 12},
		{7, 26, 12},
		{8, 29, 4},
		{9, 22, 19},
		{10, 24, 25},
		{11, 30, 30},
		{12, 17, 29},
		{13, 30, 17},
		{14, 21, 3},
		{15, 20, 10}
	};

	int BagCapacity = 95;
	int Answer[5] = {0,0,0,0,0}, PopulationSize, NumberOfIterations;

	//std::cout << Input << std::endl;

	std::cout << "Введите размер популяции: ";
	std::cin >> PopulationSize;
	std::cout << "Введите количество итераций алгоритма: ";
	std::cin >> NumberOfIterations;
	std::cout << std::endl;
	std::vector<Bin> Population;


	

	////////////////////////////////////////////////////////////////////////
	///// диалог выбора других операторов
	
	std::cout << "Алгоритм по умолчанию: " << std::endl;
	
	while (true)
	{
		if (Answer[1] == 0)		// алгоритм создания начальной популяции
		{
			std::cout << "1. Формирование новой популяции: Случайный алгоритм с контролем ограничений" << std::endl;
		}
		else
		{
			std::cout << "1. Формирование новой популяции: Жадный алгоритм" << std::endl;
		}
		if (Answer[2] == 0)
		{
			std::cout << "2. Оператор кроссовера: Одноточечный кроссовер со случайным выбором родителей" << std::endl;
		}
		else
		{
			std::cout << "2. Оператор кроссовера: Однородный кроссовер с наиболее приспособленными родителями" << std::endl;
		}
		if (Answer[3] == 0)
		{
			std::cout << "3. Оператор мутации: Сальтация (два случайных бита меняются местами)" << std::endl;
		}
		else
		{
			std::cout << "3. Оператор мутации: Дополнение (каждый бит меняется на противоположенный)" << std::endl;
		}
		if (Answer[4] == 0)
		{
			std::cout << "4. Оператор селекции: Стратегия перекрывающихся поколений. Элитарная. B-турнир" << std::endl;
		}
		else
		{
			std::cout << "4. Оператор селекции: Стратегия перекрывающихся поколений. Элитарная. Универсальный выбор" << std::endl;
		}
		// остальные операторы


		std::cout << "Изменить алгоритм (выбрать другие операторы). 0 или 1?  ";
		std::cin >> Answer[0];
		std::cout << std::endl;
		if (Answer[0] == 0)
		{
			break;
		}

		std::cout << "Пункт алгоритма: ";
		std::cin >> Answer[0];
		switch (Answer[0])
		{
		case 1:
			std::cout << "0 - Случайный алгоритм с контролем ограничений" << std::endl;
			std::cout << "1 - Жадный алгоритм" << std::endl;
			std::cout << "Варинат: ";
			std::cin >> Answer[1];
			break;
		case 2:
			std::cout << "0 - Одноточечный кроссовер со случайным выбором родителей" << std::endl;
			std::cout << "1 - Однородный кроссовер с наиболее приспособленными родителями" << std::endl;
			std::cout << "Вариант: ";
			std::cin >> Answer[2];
			break;
		case 3:
			std::cout << "0 - Сальтация (два случайных бита меняются местами)" << std::endl;
			std::cout << "1 - Дополнение (каждый бит меняется на противоположенный)" << std::endl;
			std::cout << "Вариант: ";
			std::cin >> Answer[3];
			break;
		case 4:
			std::cout << "0 - B-турнир. Стратегия перекрывающихся поколений. Элитарная." << std::endl;
			std::cout << "1 - Универсальный выбор. Стратегия перекрывающихся поколений. Элитарная." << std::endl;
			std::cout << "Вариант: ";
			std::cin >> Answer[4];
			break;
		default:
			break;
		}
		std::cout << std::endl;
	}
	////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////
	



	if (Answer[1] == 0)		// создание начальной популяции
	{
		Population = CreateNewPopulationRand(PopulationSize, Input, BagCapacity);
	}
	else
	{
		Population = CreateNewPopulationGreedy(PopulationSize, Input, BagCapacity);
	}



	int MaxCost = 0;
	Bin CodeMaxCost;

	std::cout << "Поколение 0:" << std::endl;
	for (auto it = Population.begin(); it != Population.end(); ++it)
	{
		std::cout << (*it) << ". Вес: " << Weight((*it), Input) << ". Ценность: " << Cost((*it), Input) << std::endl;
		if (MaxCost < Cost((*it), Input))
		{
			MaxCost = Cost((*it), Input);
			CodeMaxCost = (*it);
		}
	}
	std::cout << "Лучшая особь: " << CodeMaxCost << ". Ценность: " << Cost(CodeMaxCost, Input) << std::endl << std::endl;



	for (int i = 0; i < NumberOfIterations; ++i)		// основной цикл алгоритма
	{
		// Воспроизводство
		if (Answer[2] == 0)
		{
			SinglePointReproduction(PopulationSize, Input, BagCapacity, Population);
		}
		else
		{
			HomogeneousReproduction(PopulationSize, Input, BagCapacity, Population);
		}

		// Мутация
		if (Answer[3] == 0)
		{
			MutationSaltation(PopulationSize, Input, BagCapacity, Population);
		}
		else
		{
			MutationAddition(PopulationSize, Input, BagCapacity, Population);
		}

		// Отбор
		if (Answer[4] == 0)
		{
			Population = SelectionBTournament(PopulationSize, Input, BagCapacity, Population);
		}
		else
		{
			Population = SelectionUniversaChoice(PopulationSize, Input, BagCapacity, Population);
		}



		std::cout << "Поколение " << i+1 << ": " << std::endl;
		for (auto it = Population.begin(); it != Population.end(); ++it)
		{
			std::cout << (*it) << ". Вес: " << Weight((*it), Input) << ". Ценность: " << Cost((*it), Input) << std::endl;
			if (MaxCost < Cost((*it), Input))
			{
				MaxCost = Cost((*it), Input);
				CodeMaxCost = (*it);
			}
		}
		std::cout << "Лучшая особь: " << CodeMaxCost << ". Ценность: " << Cost(CodeMaxCost, Input) << std::endl << std::endl;

	}

	std::cout << "Итог работы: " << CodeMaxCost << ". Ценность: " << Cost(CodeMaxCost, Input) << std::endl << std::endl;
}
