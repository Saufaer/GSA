//#include <stdio.h>
//#include <math.h>
//#include <stdlib.h>
//
//#include <omp.h>
//#include "global.h"
//
//int main(int argv, char* argc[])
//{
//	system("color 3F");
//	system("title AGS");
//	global test1(0, 8, 2), test2(0, 8, 2), test3(0, 8, 2);
//	double eps = 0.00001;
//	int n_max = 1000;
//	test1.Optimize(0, 8, n_max, eps);
//	printf("\n Threads = [ %i ]", 1);
//	printf("\n Min X = [ %.15lf ]", test1.GetSolutionX());
//	printf("\n Min Z = [ %.15lf ]     [ N_max = %i , Eps = %.8lf ]\n Step count = [ %i ]\n", test1.GetSolutionZ(), n_max, eps, test1.GetSteps());
//	printf("\n Time = [ %lf ] sec\n", test1.Time());
//	printf("\n * * * * * * * * * * * * * * * * * * \n");
//	/*eps = 0.0000001;
//	n_max = 1000000;
//	test2.Optimize(0, 8, n_max, eps);
//	printf("\n Min X = [ %.15lf ]", test2.GetSolutionX());
//	printf("\n Min Z = [ %.15lf ]     [ N_max = %i , Eps = %.8lf ]\n Step count = [ %i ]\n", test2.GetSolutionZ(), n_max, eps, test2.GetSteps());
//	printf("\n Time = [ %lf ] sec\n", test2.Time());
//	printf("\n * * * * * * * * * * * * * * * * * * \n");*/
//	test3.Optimize(0, 8, n_max, eps, omp_get_max_threads());
//	printf("\n Threads = [ %i ]", omp_get_max_threads());
//	printf("\n Min X = [ %.15lf ]", test3.GetSolutionX());
//	printf("\n Min Z = [ %.15lf ]     [ N_max = %i , Eps = %.8lf ]\n Step count = [ %i ]\n", test3.GetSolutionZ(), n_max, eps, test3.GetSteps());
//	printf("\n Time = [ %lf ] sec\n", test3.Time());
//	printf("\n * * * * * * * * * * * * * * * * * * \n");
//	system("pause");
//	return 0;
//}

