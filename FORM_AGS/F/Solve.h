//#pragma once
//#ifndef __GLOBAL_H__
//#define __GLOBAL_H__
//
//#include <stdio.h>
//#include <vector>
//#include <queue>
//
//#include <omp.h>
//
//
////Структура для хранения результата 
//struct Pointer
//{
//	double x, z;
//	int steps;
//};
//
////Структура для процесса поиска
// struct ChX {
//	double x;
//	double R;
//};
//
//
//class global
//{
//private:
//	
//public:
//
//
//	
//	double left, right;					
//	double r;							
//	double E;							
//	int Nmax;							
//	int S = 0;							
//	double currentE = 0;				
//	double * coeff = new double[4];
//
//
//	 double func(const double x, double *coeff)
//	{
//		//return sin(x) + sin((10. * x) / 3.);
//		//return (3 * x - 14)*sin(1.8 * x);
//		//return -x + sin(3 * x) - 1;
//		return coeff[0] * sin(coeff[1] * x) + coeff[2] * cos(coeff[3] * x);
//	};
//
//
//	//сортировка для структуры ChX
//	struct sort_class_x
//	{
//		bool operator() (ChX left, ChX right)
//		{
//			return (left.x < right.x);
//		}
//	} obj;
//
//	Pointer Simple(std::vector<ChX> &RX, double left, double right, double * coeff, int Nmax, double E);
//	Pointer Piavsky(std::vector<ChX> &RX, double left, double right, double * coeff,  double r, int Nmax, double E);
//	Pointer Strongin(std::vector<ChX> &RX, double left, double right, double * coeff, double r, int Nmax, double E);
//
//	double MCh(std::vector<ChX> &RX, double * coeff, int i)
//	{
//		return abs(func(RX[i].x, coeff) - func(RX[i - 1].x, coeff)) / (RX[i].x - RX[i - 1].x);
//	}
//	//  оценка константы Липшица
//	double CalculateM(double r, std::vector<ChX> &RX, double * coeff)
//	{
//		double M = MCh(RX, coeff, 1);
//		for (int i = 1; i < RX.size(); i++)
//		{
//			double currentM = MCh(RX, coeff, i);
//			if (currentM > M)
//				M = currentM;
//		}
//		if (M > 0)
//		{
//			return r * M;
//		}
//		else //M==0
//		{
//			return 1.0;
//		}
//	}
//	double X(std::vector<ChX> &RX, double m, double * coeff,int i)
//	{
//		return 0.5 * (RX[i].x + RX[i-1].x) - (func(RX[i].x, coeff) - func(RX[i-1].x, coeff)) / (2.0 * m);	       
//	}
//	double XSimple(std::vector<ChX> &RX, double * coeff, int i)
//	{
//		return 0.5 * (RX[i].x + RX[i - 1].x);
//	}
//	double RSimple(std::vector<ChX> &RX,int i)
//	{
//		return (RX[i].x - RX[i-1].x);
//	}
//	double RPiavsky(std::vector<ChX> &RX,double m,int i)
//	{
//		return 0.5 * m * (RX[i].x - RX[i-1].x) - (func(RX[i].x, coeff) + func(RX[i-1].x, coeff)) / 2.0;
//	}
//	double RStrongin(std::vector<ChX> &RX, double m, int i)
//	{
//		return m * (RX[i].x - RX[i - 1].x) + (((func(RX[i].x, coeff) - func(RX[i - 1].x, coeff)) *
//			(func(RX[i].x, coeff) - func(RX[i - 1].x, coeff))) / (m * (RX[i].x - RX[i - 1].x))) -
//			2 * (func(RX[i].x, coeff) + func(RX[i - 1].x, coeff));
//	}
//};
//
//#endif
