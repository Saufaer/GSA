#include "Optim.h"
using namespace std;
global::Pointer global::Serial_Search()
{
    RZ rz;//контейнер
    Pointer point;//структура - результат
    point.steps = 1;//шаги
    currentE = right - left; //текуща€ точность решени€

    //--------------------------------------------перва€ итераци€ алгоритма
    newX = (right + left) / 2;//перва€ нова€ точка - середина отрезка
    //вставка первых трЄх XRZ
    rz.z = func(left);
    rz.R = 0;
    XRZ.insert(pair<double, RZ>(left, rz));

    rz.z = func(newX);
    rz.R = 0;
    XRZ.insert(pair<double, RZ>(newX, rz));

    rz.z = func(right);
    rz.R = 0;
    XRZ.insert(pair<double, RZ>(right, rz));

    //поиск макс ћ среди двух
    double rightM = Calculate_M(right, newX, XRZ.at(right).z, XRZ.at(newX).z);

    maxM = Calculate_M(newX, left, XRZ.at(newX).z, XRZ.at(left).z);;
    if (maxM < rightM)
    {
        maxM = rightM;
    }
 
    double m; //оценка константы Ћипшица
    double maxR;//инициализаци€ временной макс R

    map <double, RZ>::iterator num;//итератор дл€ интервала с макс R
    map <double, RZ>::iterator backnum;//итератор на его левую границу

    double itcurE;//точность на текущем интервале

    //------------------------------------------------основной цикл алгоритма
    while (true)
    {
        maxR = XRZ.at(newX).R;
        //пересчЄт характеристик
        m = Calculate_m();//поиск m
        for (map <double, RZ>::iterator it = ++(XRZ.begin()), back = XRZ.begin() // ќпределение интервала с максимальным R
            ; it != XRZ.end();
            ++back, ++it
            )
        {
            (it)->second.R = Calculate_R(m, it, back);//подсчЄт R в текущем интервале

            if (XRZ.at(it->first).R > maxR)
            {
                maxR = XRZ.at(it->first).R;
                num = it;//запоминаю номер интервала (индекс его правой границы)
                backnum = back;
            }

            itcurE = it->first - back->first;//точность на текущем итервале
            if (itcurE < currentE)
            {
                currentE = itcurE;//точность решени€
            }
        }
        if (currentE <= E)//выход по точности
        {
            break;
        }

        newX = Calculate_X(m, num, backnum);//нахожу следующую точку испытани€ в найденном интервале

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