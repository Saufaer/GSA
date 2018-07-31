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

    //рабочие структуры данных
    
    struct RZ//—труктура дл€ контейнера в XRZ
    {
        double R, z;
    };
    std::map <double, RZ> XRZ;//база, содеражаща€ x, R , z 
                              //XRZ упор€дочена по возрастанию ключа х 
                             

    //измен€емые переменные
    double currentE;//текуща€ точность решени€

    double newX;//нова€ точка х
    double newXback;//точка х передшествующа€ newX

    double newM;//ћ на новом интервале
    double maxM;//максимальна€ ћ в текущей области

    double xl, xr, zl, zr;//переменные дл€ работы с итераторами

    //целева€ функци€
    double func(const double x);

    //подсчЄт M на заданном интервале
    double Calculate_M(double xr, double xl, double zr, double zl);

    //  оценка константы Ћипшица
    double Calculate_m();

    //подсчЄт новой точки испытани€
    double Calculate_X(double m, std::map <double, RZ>::iterator num, std::map <double, RZ>::iterator backnum);

    //подсчЄт характеристики на заданном интервале
    double Calculate_R(double m, std::map <double, RZ>::iterator it, std::map <double, RZ>::iterator back);


public:
    //входные параметры
    double left, right;//стартовые границы области
    double r;//параметр метода
    double E;//граница выхода по точности
    int Nmax;//граница выхода по числу шагов

    double * coeff = new double[4];

    //—труктура дл€ хранени€ результата 
    struct Pointer
    {
        double x, z;
        int steps;
    };

    //последовательный решатель задачи
    Pointer Serial_Search();
};

#endif
