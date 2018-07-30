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

//Структура для хранения результата 
struct Pointer
{
    double x, z;
    int steps;
};


struct RZ
{
    double R, z;

};

class global
{
private:

public:
    std::map <double, RZ> XRZ;
    std::map <double, double> MX;
    double left, right;
    double r;
    double E;
    int Nmax;

    double currentE = 0;
    double * coeff = new double[4];

    double newX;

    double func(const double x)
    {
        //return sin(x) + sin((10. * x) / 3.);
        //return (3 * x - 14)*sin(1.8 * x);
        //return -x + sin(3 * x) - 1;
        return coeff[0] * sin(coeff[1] * x) + coeff[2] * cos(coeff[3] * x);
    };
    Pointer Mapa();


    /////////////////////////////////////////////////////////////////////////////////////////////
    double xl;
    double xr;

    double zl;
    double zr;

    double newM;
    double newXback;
     double Calculate_M(double xr, double xl, double zr, double zl)
     {
     return abs(zr - zl) / (xr - xl);
     }
    //  оценка константы Липшица
    double Calculate_m()
    {
        //подсчёт M
       std::map <double, RZ>::iterator YYback = --XRZ.find(newX);//поиск итератора перед newX в XRZ
       std::map <double, RZ>::iterator YY = XRZ.find(newX);

        newXback = YYback->first;//значение х перед newX

       
        zr = YY->second.z;
        zl = YYback->second.z;

        newM = Calculate_M(newX, newXback, zr, zl);//подсчёт нового М

        MX.insert(std::pair<double, double>(newM, newX));//занесение М в базу (максимальный в конце)
        //TODO придумать как можно использовать контейнер х

       double M ;     
       M = MX.rbegin()->first;//получение макс М по ключу
        if (M > 0)
        {
            return r * M;
        }
        else //M==0
        {
            return 1.0;
        }
    }
    double Calculate_X(double m, std::map <double, RZ>::iterator num, std::map <double, RZ>::iterator backnum)
    {
        xr = num->first;
        xl = backnum->first;

        zr = num->second.z;
        zl = backnum->second.z;

        return 0.5 * (xr + xl) - (zr -zl) / (2.0 * m);
    }
    double Calculate_R(double m, std::map <double, RZ>::iterator it, std::map <double, RZ>::iterator back)
    {
        xr = it->first;
        xl = back->first;

        zr = it->second.z;
        zl = back->second.z;
        return m * (xr - xl) + (((zr - zl) *
            (zr - zl)) / (m * (xr - xl))) -
            2 * (zr + zl);
    }
};

#endif
