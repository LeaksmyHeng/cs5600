/*
* HengL.CS5600.LearnC.c / 
* Program in C
* Leaksmy Heng / CS5600 / Northeastern University
* Fall 2023 / Sep 9, 2023
*
*/

#include <math.h>

/* 
*	Creating a function to convert miles per gallon to kilometers per liter.
*	Double takes up more memory but I want the precision to be correct.
*/
double mpg2kml(double miles_per_gallon) 
{
	// catching negative input for mpg and
	// catching the size of mpg in case its size is way too big that double cannot handle
	if (miles_per_gallon < 0 || 1.8*pow(10, 308) <= miles_per_gallon){
		return -1;
	}
	
	double mpg_to_kpl_rate = 0.435144; 	/* 1 mpg is equal to 0.43 kpl */
	double result;
	result = miles_per_gallon * mpg_to_kpl_rate;
	return result;
}

/* 
*	Creating a function to convert miles per gallon to liters per 100 kilometers.
*/
double mpg2lphm(double miles_per_gallon){
	// catching negative mpg or 0 cause 0 will be ZeroDivision error
	// catching the size of mpg in case its size is way too big that double cannot handle
	if (miles_per_gallon <= 0 || 1.8*pow(10, 308) <= miles_per_gallon)
	{
		return -1;
	}
	double convertion_mpg2lphm = 235.214583;
	double result;
	result = convertion_mpg2lphm / miles_per_gallon;
	return result;
}

/* 
*	Creating a function to convert miles per gallon to liters per 100 kilometers.
*/
double lph2mpg(double lphm){
	// catching negative lphm or 0 cause 0 will be ZeroDivision error
	if (lphm <= 0 || 1.8*pow(10, 308) <= lphm){
		return -1;
	}
	double convertion_lph2mpg = 235.214583;
	double result;
	result = convertion_lph2mpg / lphm;
	return result;
}

