#include <list>     // doubly linked list
#include <string>
#include <fstream>
#include <stdexcept>

#include "Repair.hpp"

using std::string;
using std::size_t;
using std::ifstream;


class RepairList {

	// Friend functions: read and write
	friend std::ostream & operator<<( std::ostream & stream, const RepairList & repairlist );
	friend std::istream & operator>>( std::istream & stream, RepairList & repairlist );

private:

	// Data members
	std::list<Repair> _dailyList;
	std::list<Repair>::iterator _nowServicing;

public:

	
	RepairList() = default;  // default constructor

	RepairList( const RepairList & other ) = default; // copy constructor
	RepairList( RepairList && other ) = default; // move constructor
	RepairList & operator=(const RepairList& rhs ) = default; // assignment operator

	~RepairList() = default; // destructor

	void loadAdvanceList(const std::string& filename); // read from a file

	
	std::size_t size() const;
	// returns the number of repair requests in the list

	Repair currRepair() const; 
	// returns the current repair request

	void next(); 
	//go to the next repair request in the list, if any

	void addToList(const Repair& newRequest); 
	// append a new repair request to the end of the list

	void insertLoyal(const Repair& newRequest); 
	// insert a new repair request after the current position of the list

	void insertPriority(const Repair& newRequest);
	// insert a new repair request before the current position of the list

	string numberInRepairList(size_t number); 
	// returns the vehicle that was serviced at given number in the list

	double maxCost(); 
	// returns the maximum cost among all repairs



};



std::ostream& operator<<(std::ostream& stream, const RepairList& repairlist)
{
	for (const auto& request : repairlist._dailyList)   stream << '\n' << std::setw(5) << request;

	return stream;
}


std::istream& operator>>(std::istream& stream, RepairList& repairlist)
{
	Repair request;

	while (stream) {
		stream >> request;
		repairlist.addToList(std::move(request));
	}

	return stream;
}

// read the list of advanced repair requests from the specified file: ALREADY COMPLETED
void RepairList::loadAdvanceList(const std::string& filename)
{
	std::ifstream inFile(filename);
	Repair newRequest;

	if (inFile.is_open())
	{
		// Read each line
		while (inFile >> newRequest) {
			addToList(newRequest);
		}

		inFile.close();  // file closes
		_nowServicing = _dailyList.begin();
	}
	else
	{
		throw std::invalid_argument("Could not open " + filename);
	}
}



// return the size of the list

std::size_t RepairList::size() const
{
	return _dailyList.size();
}


// return the current repair
Repair RepairList::currRepair() const
{
	return * _nowServicing;

}


// the current repair has been serviced so move the iterator to the next request
void RepairList::next()
{
	++_nowServicing;
}



// add a repair request to the end of current list
void RepairList::addToList(const Repair& newRequest)
{
	_dailyList.push_back(newRequest);
}


// insert a new repair request coming from a loyal customer
// right after the current position but do not make changes to the current position
void RepairList::insertLoyal(const Repair& newRequest)
{
	++_nowServicing;
	_nowServicing = _dailyList.insert(_nowServicing, newRequest);
	--_nowServicing;
}


// insert a new repair request before the current position of the list
void RepairList::insertPriority(const Repair& newRequest)
{
	_nowServicing = _dailyList.insert(_nowServicing, newRequest);
}


// returns the vehicle that was serviced at given number in the list
string RepairList::numberInRepairList(size_t number)
{
	auto _nowServicing = _dailyList.begin();
	advance(_nowServicing, number-1); //advance finds element at number location
	return  _nowServicing->vehicle();
}


// returns the maximum cost among all repairs
double RepairList::maxCost()
{
	double maximum = 0.0;
	for(_nowServicing = _dailyList.begin(); _nowServicing != _dailyList.end(); ++_nowServicing){
		if(_nowServicing->cost() > maximum){
			maximum = _nowServicing->cost();
		}
	}
	return maximum;
}
