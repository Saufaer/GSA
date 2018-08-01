#include "Optim.h"
using namespace std;
global::Pointer global::Serial_Search()
{
    RZ rz;//контейнер
    Pointer point;//структура - результат
    point.steps = 0;//шаги
    currentE = right - left; //текущая точность решения
                             //--------------------------------------------первая итерация алгоритма

    rz.z = func(left);
    rz.R = 0;
    XRZ.insert(pair<double, RZ>(left, rz));
    rz.z = func(right);
    rz.R = 0;
    XRZ.insert(pair<double, RZ>(right, rz));

    maxM = Calculate_M(right, left, XRZ.at(right).z, XRZ.at(left).z);
    newX = right;

    double m = Calculate_m(); //оценка константы Липшица

    map <double, RZ>::iterator num = ++(XRZ.begin());//итератор для интервала с макс R
    map <double, RZ>::iterator backnum = (XRZ.begin());//итератор на его левую границу
    double maxR = Calculate_R(m, num, backnum);;//инициализация временной макс R

    double itcurE;//точность на текущем интервале

                  //------------------------------------------------основной цикл алгоритма
    while (true)
    {
        maxR = (++(XRZ.begin()))->second.R;
        //пересчёт характеристик
        m = Calculate_m();//поиск m

        num = ++(XRZ.begin());
        backnum = (XRZ.begin());

        for (map <double, RZ>::iterator it = ++(XRZ.begin()), back = XRZ.begin() // Определение интервала с максимальным R
            ; it != XRZ.end();
            ++back, ++it
            )
        {
            (it)->second.R = Calculate_R(m, it, back);//подсчёт R в текущем интервале

            if (XRZ.at(it->first).R > maxR)
            {
                maxR = XRZ.at(it->first).R;
                num = it;//запоминаю номер интервала (индекс его правой границы)
                backnum = back;

            }

            itcurE = it->first - back->first;//точность на текущем итервале
            if (itcurE < currentE)
            {
                currentE = itcurE;//точность решения
            }
        }
        if (currentE <= E)//выход по точности
        {
            break;
        }

        newX = Calculate_X(m, num, backnum);//нахожу следующую точку испытания в найденном интервале

                                            //заноc x и z в базу
        rz.R = 0;
        rz.z = func(newX);
        XRZ.insert(pair<double, RZ>(newX, rz));

        point.steps++;

        if (point.steps == Nmax)//выход по числу шагов
        {
            break;
        }
    }

    point.x = newX;
    point.z = rz.z;
    return point;


}