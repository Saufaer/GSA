#pragma once
#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#include <stdio.h>
#include <vector>
#include <queue>
#include <list>
#include <omp.h>
#include <map>
#include "grishagin\include\grishagin_function.hpp"
#include "evolv.h"
class global
{
private:

    //������� ��������� ������

    struct RZ//��������� ��� ���������� � XRZ
    {
        double R, z;
    };
    std::map <double, RZ> XRZ;//����, ����������� x, R , z 
                              //XRZ ����������� �� ����������� ����� � 

    struct bord
    {
        double shiftl, shiftr;
        double R, weight;

        std::map <double, RZ>::iterator num;
        std::map <double, RZ>::iterator backnum;
        std::map <double, RZ>::iterator curnum;
        std::map <double, RZ>::iterator curbacknum;
    };

    //���������� ����������
    double currentE;//������� �������� �������

    double newX;//����� ����� �
    double newXback;//����� � ��������������� newX

    double newM;//� �� ����� ���������
    double maxM;//������������ � � ������� �������

    double xl, xr, zl, zr;//���������� ��� ������ � �����������

                          //������� �������
    double func(const double x);

    //������� M �� �������� ���������
    double Calculate_M(double xr, double xl, double zr, double zl);

    //  ������ ��������� �������
    double Calculate_m();

    //������� ����� ����� ���������
    double Calculate_X(double m, std::map <double, RZ>::iterator num, std::map <double, RZ>::iterator backnum);

    //������� �������������� �� �������� ���������
    double Calculate_R(double m, std::map <double, RZ>::iterator it, std::map <double, RZ>::iterator back);


public:
    //������� ���������
    double left, right;//��������� ������� �������
    double r;//�������� ������
    double E;//������� ������ �� ��������
    int Nmax;//������� ������ �� ����� �����
    int procs;
    double * coeff = new double[4];

    //��������� ��� �������� ���������� 
    struct Pointer
    {
        double x, y, z;
        int steps;
    };
    inline double Func(const double *_y, vagrish::GrishaginFunction *func)
    {
        // _y[1] = y ; _y[0] = x;
        //return 2 * cos(_y[1]) + 3 * sin(_y[0]); //y[-5,0] x[-4,2] examin ~= (x_1.6 , y_-3.1)

        //auto *func = new vagrish::GrishaginFunction();
        //func->SetFunctionNumber(num);
        //std::shared_ptr<IGOProblem<double>> problem;
        //problem = std::shared_ptr<IGOProblem<double>>(func);

        //double leftBound[2], rightBound[2];
        //problem->GetBounds(leftBound, rightBound);
        return func->Calculate(_y);

    }

    Pointer Serial_Search(); //���������������� �������� ������
    Pointer PP();//�������������
    Pointer EV(double *leftBound, double *rightBound, vagrish::GrishaginFunction *func);
    int operations = 0;
    double time, st, fn;
    double   alltime = 0;
};

#endif
