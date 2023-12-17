/*
* HengL.CS5600.LearnC.c / 
* Program in C
* Leaksmy Heng / CS5600 / Northeastern University
* Fall 2023 / Sep 9, 2023
*
*/

#include <stdio.h>
#include <math.h>
#include "mpg2km.h"

int main() {
	
	double test_value1 = 0.00;			// test 0 value.
	double test_value2 = 2.95;			// test valid value.
	double test_value3 = 1589.2;		// test valid value.
	double test_value4 = -253942683.45;	// test invalid value.
	double test_value5 = -0.0000001;	// test invalid value.
	double test_value6 = 235.214;		// test valid value
	double test_value7 = 5*pow(10, 308);// test max double => overflow for mpg2kml
	int test_value8 = 2;				// use int instead of double
	
	double mpg2kml_test1 = mpg2kml(test_value1);
	double mpg2kml_test2 = mpg2kml(test_value2);
	double mpg2kml_test3 = mpg2kml(test_value3);
	double mpg2kml_test4 = mpg2kml(test_value4);
	double mpg2kml_test5 = mpg2kml(test_value5);
	double mpg2kml_test6 = mpg2kml(test_value6);
	double mpg2kml_test7 = mpg2kml(test_value7);
	double mpg2kml_test8 = mpg2kml(test_value8);
	printf("mpg2kml test1 result && expected result is: %lf == %lf\n", mpg2kml_test1, 0.000000);
	printf("mpg2kml test2 result && expected result is: %lf == %lf\n", mpg2kml_test2, 1.283675);
	printf("mpg2kml test3 result && expected result is: %lf == %lf\n", mpg2kml_test3, 691.530845);
	printf("mpg2kml test4 result && expected result is: %lf == %lf\n", mpg2kml_test4, -1.000000);
	printf("mpg2kml test5 result && expected result is: %lf == %lf\n", mpg2kml_test5, -1.000000);
	printf("mpg2kml test6 result && expected result is: %lf == %lf\n", mpg2kml_test6, 102.351961);
	printf("mpg2kml test7 result && expected result is: %lf == %lf\n", mpg2kml_test7, -1.000000);
	printf("mpg2kml test8 result && expected result is: %lf == %lf\n", mpg2kml_test8, 0.870288);
	printf("\n");
	
	double mpg2lphm_test1 = mpg2lphm(test_value1);
	double mpg2lphm_test2 = mpg2lphm(test_value2);
	double mpg2lphm_test3 = mpg2lphm(test_value3);
	double mpg2lphm_test4 = mpg2lphm(test_value4);
	double mpg2lphm_test5 = mpg2lphm(test_value5);
	double mpg2lphm_test6 = mpg2lphm(test_value6);
	double mpg2lphm_test7 = mpg2lphm(test_value7);
	double mpg2lphm_test8 = mpg2lphm(test_value8);
	printf("mpg2lphm test1 result && expected result is: %lf == %lf\n", mpg2lphm_test1, -1.000000);
	printf("mpg2lphm test2 result && expected result is: %lf == %lf\n", mpg2lphm_test2, 79.733757);
	printf("mpg2lphm test3 result && expected result is: %lf == %lf\n", mpg2lphm_test3, 0.148008);
	printf("mpg2lphm test4 result && expected result is: %lf == %lf\n", mpg2lphm_test4, -1.000000);
	printf("mpg2lphm test5 result && expected result is: %lf == %lf\n", mpg2lphm_test5, -1.000000);
	printf("mpg2lphm test6 result && expected result is: %lf == %lf\n", mpg2lphm_test6, 1.000002);
	printf("mpg2lphm test7 result && expected result is: %lf == %lf\n", mpg2lphm_test7, -1.000000);
	printf("mpg2lphm test8 result && expected result is: %lf == %lf\n", mpg2lphm_test8, 117.607292);
	printf("\n");
	
	double lph2mpg_test1 = lph2mpg(test_value1);
	double lph2mpg_test2 = lph2mpg(test_value2);
	double lph2mpg_test3 = lph2mpg(test_value3);
	double lph2mpg_test4 = lph2mpg(test_value4);
	double lph2mpg_test5 = lph2mpg(test_value5);
	double lph2mpg_test6 = lph2mpg(test_value6);
	double lph2mpg_test7 = lph2mpg(test_value7);
	double lph2mpg_test8 = lph2mpg(test_value8);
	printf("mpg2lphm test1 result && expected result is: %lf == %lf\n", lph2mpg_test1, -1.000000);
	printf("mpg2lphm test2 result && expected result is: %lf == %lf\n", lph2mpg_test2, 79.733757);
	printf("mpg2lphm test3 result && expected result is: %lf == %lf\n", lph2mpg_test3, 0.148008);
	printf("mpg2lphm test4 result && expected result is: %lf == %lf\n", lph2mpg_test4, -1.000000);
	printf("mpg2lphm test5 result && expected result is: %lf == %lf\n", lph2mpg_test5, -1.000000);
	printf("mpg2lphm test6 result && expected result is: %lf == %lf\n", lph2mpg_test6, 1.000002);
	printf("mpg2lphm test7 result && expected result is: %lf == %lf\n", lph2mpg_test7, -1.000000);
	printf("mpg2lphm test8 result && expected result is: %lf == %lf\n", lph2mpg_test8, 117.607292);
}
