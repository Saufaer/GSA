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

    //рабочие структуры данных

    struct RZ//—труктура дл€ контейнера в XRZ
    {
        double R, z;
    };
    std::map <double, RZ> XRZ;//база, содеражаща€ x, R , z 
                              //XRZ упор€дочена по возрастанию ключа х 

    struct bord
    {
        double shiftl, shiftr;
        double R, weight;

        std::map <double, RZ>::iterator num;
        std::map <double, RZ>::iterator backnum;
        std::map <double, RZ>::iterator curnum;
        std::map <double, RZ>::iterator curbacknum;
    };

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
    int procs;
    double * coeff = new double[4];

    //—труктура дл€ хранени€ результата 
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

    Pointer Serial_Search(); //последовательный решатель задачи
    Pointer PP();//параллелльный
    Pointer EV(double *leftBound, double *rightBound, vagrish::GrishaginFunction *func);
    int operations = 0;
    double time, st, fn;
    double   alltime = 0;
};

#endif
