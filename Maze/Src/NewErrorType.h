#pragma once
//#include <stdexcept>
#include <iostream>
using namespace std; 

#define CHECK(val) checkTestResult(val) 

enum ERROR_TYPE
{
	ERROR_									= 0,
	SUCCESS_								= 1,

	ERROR_OUT_OF_RANGE						= 100,
	ERROR_WRONG_TILE						= 101, 
	ERROR_ALREADY_STEPPED_TILE				= 102, 

	ERROR_DIFFERERNT_VALUE					= 200, 

	ERROR_WRONG_MAP							= 300, 
};

int checkTestResult(int error_type)
{
	int ret = static_cast<int>(ERROR_TYPE::ERROR_);
	if (error_type == 1) ret = static_cast<int>(ERROR_TYPE::SUCCESS_); 
	else throw error_type;

	return ret; 
}

void catchFunction(int error_type, int error_type_detail, const char message[])
{
	cout << message << endl;
	if (error_type_detail) throw error_type_detail;
	else throw error_type;
}