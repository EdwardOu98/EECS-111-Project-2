#ifndef __TYPES_P2_H
#define __TYPES_P2_H

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sys/time.h>
#include <string>
#include <vector>
#include <unistd.h>

#define EMPTY        0
#define WOMENPRESENT 1
#define MENPRESENT   2

using namespace std;

class Person
{

	int gender; // 0: male 1: female
	std::string str_gender;
	struct timeval t_create;
	struct timeval t_start;
	struct timeval t_end;
	long time_to_stay_ms;


	unsigned long order;
	unsigned long use_order;

public:
	Person();

	void set_gender(int data);
	int get_gender(void);

	struct timeval get_create_time();

	void set_order(unsigned long data);
	unsigned long get_order(void);

	void set_use_order(unsigned long data);
	unsigned long get_use_order(void);

	void set_time(long data);
	int ready_to_leave(void);

	void start(void);
	void complete(void);
};


// Class for the restroom
// You may need to add more class member variables and functions
class Restroom {
	int status;
	int n_male;
	int n_female;
	int n_total;
	int u_male;
	int u_female;
	int u_total;

	// You need to define the data structure to
    // save the information of people using the restroom
	// You can probebly use Standard Template Library (STL) vector


public:

	Restroom(){
		status = EMPTY;
		n_female = 0;
		n_male = 0;
		n_total = 0;
		u_total = 0;
	}

	// You need to use this function to print the Restroom's status
	void print_status(void);


	// Call by reference
	// This is just an example. You can implement any function you need
	void add_person(Person& p);

	void man_wants_to_enter(Person& p);
	void woman_wants_to_enter(Person& p);
	void man_leaves(Person& p);
	void woman_leaves(Person& p);

	bool cleared_to_enter(Person& p);

};










#endif
