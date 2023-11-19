/*
Program: Prime Race
Author: Phillip McCullough
Date: 11/16/2023
C/Cpp: 17
-------10--------20--------30--------40--------50--------60--------70--------80

Prime Race finds all the prime numbers in a set range repeatedly for about five 
seconds. 

Data for validating our results, the number of primes to be found under some
limit, such as 168 primes under 1000.

Range: Number of primes.
10: 4
100: 25
1000: 168
10000: 1229
100000: 9592
1000000: 78498
10000000: 664579
100000000: 5761455
1000000000: 50847534
10000000000: 455052511

I must give accreditations to Pol Marcet and Michael Barber.
My entry for the Dave's Garage programming drag race was influenced by,
// ---------------------------------------------------------------------------
// PrimeCPP.cpp : Pol Marcet's Modified version of Dave's Garage Prime Sieve
// Some great ideas taken from Rust's implementation from Michael Barber
// @mike-barber https://www.github.com/mike-barber (bit-storage-rotate)
// ---------------------------------------------------------------------------
*/
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <chrono>
#include <cmath>
using namespace std;
using namespace std::chrono;

const map<const long long, const int> validValues = {
	{ 10LL, 4 },
	{ 100LL, 25 },
	{ 1000LL, 168 },
	{ 10000LL, 1229 },
	{ 100000LL, 9592 },
	{ 1000000LL, 78498 },
	{ 10000000LL, 664579 },
	{ 100000000LL, 5761455 },
	{ 1000000000LL, 50847534 },
	{ 10000000000LL, 455052511 }
};

void greeting();
int menu();
long long getRangeMax();
void primeBenckmark();
string numberCommaFormat(double number);

int main()
{
	greeting();

	const int RUN_BENCKMARK = 1;
	int option;
	do
	{
		option = menu();
		switch (option)
		{
		case RUN_BENCKMARK: primeBenckmark();
		}
	} while (option == RUN_BENCKMARK);

	return 0;
} // --------------------------------------------------------------------------

void greeting()
{
	cout << "\n"
		" Prime Race finds the number of primes from zero to a given value.\n"
		" The program will display the number of passes performed in about\n"
		" five seconds and the actual test runtime.\n";
} // --------------------------------------------------------------------------

int menu()
{
	const int MIN_OPTION = 1;
	const int MAX_OPTION = 2;
	int option;
	cout << "\n"
		" 1) Run Benchmark\n"
		" 2) Exit\n\n"
		" Enter your choice: ";
	cin >> option;
	while (cin.fail() || option < MIN_OPTION || option > MAX_OPTION)
	{
		if (cin.fail())
		{
			cin.clear();
			cin.ignore();
		}
		cout << "\n Invalid Value!\n"
			" Enter the number of the option you would like to choose: ";
		cin >> option;
	}
	cin.ignore();
	return option;
} // --------------------------------------------------------------------------

long long getRangeMax()
{
	long long upperLimit;
	
	cout << "\n"
		" You may choose the maximum value for the range by factors of ten.\n"
		" Example: 10, 100, 1000, etc. up to 10 billion.\n"
		" Choose the range limit: ";
	cin >> upperLimit;
	while (cin.fail() || validValues.find(upperLimit) == validValues.end())
	{
		if (cin.fail())
		{
			cin.clear();
			cin.ignore();
		}
		cout << "\n Invalid Value: Choose the range limit: ";
		cin >> upperLimit;
	}
	cin.ignore();
	return upperLimit;
} // --------------------------------------------------------------------------

/*
The prime benchmark function works by checking prime factors. If a number does 
not divie evenly by a known prime number it is the next prime number/factor. 
*/
void primeBenckmark()
{
	long long rangeMax = getRangeMax();
	
	cout << "\n Benchmark In Progress. ";

	vector<int> primes;
	bool isPrime;
	int primesPosition;
	int primeFactor;
	int checkLimit;
	int checkNumber;

	int passes = 0;
	const int RUN_DURATION = 5;
	steady_clock::time_point runTime, startTime = steady_clock::now();

	while (duration_cast<seconds>(runTime - startTime).count() < RUN_DURATION)
	{
		primes = { 2, 3 }; // Setting the first prime factors.
		checkNumber = 5; // The first prime number added to primes.
		while (checkNumber < rangeMax)
		{
			isPrime = true;
			primesPosition = 1;
			primeFactor = primes[primesPosition];
			checkLimit = static_cast<int>(sqrt(checkNumber));

			while (primeFactor <= checkLimit)
			{
				if (checkNumber % primeFactor == 0)
				{
					isPrime = false;
					break;
				}
				primesPosition += 1;
				primeFactor = primes[primesPosition];
			}

			if (isPrime) primes.push_back(checkNumber);
			checkNumber += 2;
		}
		runTime = steady_clock::now();
		passes += 1;
	}

	int primesCount = primes.size();
	if (validValues.find(rangeMax)->second == primesCount)
		cout << "\n\n Test Successful!\n";
	else cout << "\n\n Test Failure!\n";

	const double ONE_MICROSECOND = 0.000001;
	double testDuration = static_cast<double>
		(duration_cast<microseconds>
			(runTime - startTime).count() * ONE_MICROSECOND);
	
	cout << "\n Passes: " << numberCommaFormat(passes) << 
		"\n Runtime: " << testDuration << " seconds\n"
		" There are " << numberCommaFormat(primesCount) 
		<< " prime numbers in the range: 0 - " << numberCommaFormat(rangeMax) 
		<< "\n";

	string yes_no;
	std::cout << "\n Would you like to view the prime numbers? [yes/no]: ";
	getline(cin, yes_no);
	if (yes_no[0] == 'Y' || yes_no[0] == 'y')
	{
		const int DISPLAY_SET_SIZE = 10;
		cout << '\n';
		for (int i = 0; i < primesCount; i++)
		{
			cout << ' ' << numberCommaFormat(primes[i]) << '\n';
			if (primesCount >= DISPLAY_SET_SIZE && 
				(i + 1) % DISPLAY_SET_SIZE == 0)
			{ // FYI, hold down enter/return to see large sets faster!
				string choice;
				cout << "\n Press enter/return to see the next " 
					<< DISPLAY_SET_SIZE << " primes or x to exit: ";
				getline(cin, choice);
				if (choice[0] == 'x' || choice[0] == 'X') break;
				cout << '\n';
			}
		}
		cout << "\n View prime numbers complete.\n";
	}
} // --------------------------------------------------------------------------

string numberCommaFormat(double number)
{
	string numberString = to_string(number);
	int decimal = numberString.find('.');

	if (decimal > 3)
		for (int i = decimal - 3; i > 0; i -= 3)
			numberString.insert(i, ",");

	decimal = numberString.find('.');
	return numberString.substr(0, decimal);
} // --------------------------------------------------------------------------