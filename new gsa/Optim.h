#pragma once

#pragma once
#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#include <stdio.h>
#include <vector>
#include <queue>
#include <list>
#include <omp.h>
#include <map>

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

    double * coeff = new double[4];

    //��������� ��� �������� ���������� 
    struct Pointer
    {
        double x, z;
        int steps;
    };

    //���������������� �������� ������
    Pointer Serial_Search();
};

#endif
