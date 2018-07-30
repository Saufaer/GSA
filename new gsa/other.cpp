#include "Optim.h"
using namespace std;
Pointer global::Mapa()
{
    RZ rz;
    Pointer point;//структура - результат
    point.steps = 1;//шаги


    newX = (right + left) / 2;//первая новая точка - середина отрезка

//вставка первых трёх XRZ
    rz.z = func(left);
    rz.R = 0;
    XRZ.insert(pair<double, RZ>(left, rz));

    rz.z = func(newX);
    rz.R = 0;
    XRZ.insert(pair<double, RZ>(newX, rz));

    rz.z = func(right);
    rz.R = 0;
    XRZ.insert(pair<double, RZ>(right, rz));

    //вставка первых двух МХ
    map <double, RZ>::iterator it = ++(XRZ.begin());
    MX.insert(pair<double, double>(Calculate_M(newX, left, XRZ.at(newX).z, XRZ.at(left).z), it->first));
    ++it;
    MX.insert(pair<double, double>(Calculate_M(right, newX, XRZ.at(right).z, XRZ.at(newX).z), it->first));


    double m; //оценка константы Липшица
    double maxR;//инициализация временной макс R
    map <double, RZ>::iterator num;
    map <double, RZ>::iterator backnum;
    currentE = 1; //текущая точность решения


    while (true)
    {
        maxR = XRZ.at(left).R;
        //пересчёт характеристик
        m = Calculate_m();//поиск m

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
            if (it->first - back->first < currentE)
            {
                currentE = abs(it->first - back->first);//текущая точность
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
    point.z = func(newX);
    return point;


}