#include "Header.h"
#include <iostream>
#include <fstream>


TAObject::TAObject(float x, float y)
{
    this->x = x;
    this->y = y;
}


TLA::TLA(float x, float y, float xc, float yc, float V, int spec) :TAObject(x, y)
{
    this->xc = xc;
    this->yc = yc;
    this->V = V;
    this->spec = spec;
    this->f = false;    
    this->landing = false;  
}

void TLA::calc(float x, float y, float xc, float yc, float V)
{
    if (x != xc) R = sqrt(pow(x - xc, 2.0f) + pow(y - yc, 2.0f));
    else R = sqrt(pow(x, 2.0f) + pow(y, 2.0f));

    if (x >= xc) { b = 0; }
    else if (x < xc && y < yc) { b = -1; }
    else if (x < xc && y > yc) { b = 1; }

    if (x - xc == 0) fi = b * acos(0.0f) * 2.0f + acos(0.0f);
    else fi = b * acos(0.0f) * 2.0f + atan((y - yc) / (x - xc));
}




TAirport::TAirport(float x, float y, float l) :TAObject(x, y)
{
    this->l = l;
    f = false;  
}


void TLA::move(float t, int a)
{
    if (a == 0) f = false;
    else if (a == 1) f = true;
}


void TAircraft::move(float t, int a)
{
    float omega;
    omega = -V / R;
    x = -(xc + R * cos(fi + omega * t)) * (a - 1) + a * (x + V * dt);
    y = -(yc + R * sin(fi + omega * t)) * (a - 1) + y * a;
}


void THelicopter::move(float t, int a)
{
    x = x - a * V * cos(fi) * dt;
    y = y - a * V * sin(fi) * dt;
}


void TAirport::Do(float t0, float tk)
{
    int TypeVhcl, a;
    float x1, y1, xc1 = 0, yc1 = 0, V1;
    float tnull = t0;
    float omega1;
    ofstream airport("airport.txt");

    cout << "Авиапарк расчитан на " << n << " ЛА" << endl;
 
    for (int i = 0; i < n; i++)
    {
        cout << "Введите тип техники: самолёт (нажмите 1) или вертолёт (нажмите 2): ";
        cin >> TypeVhcl;

        if (TypeVhcl == 1)
        {
            cout << "Самолёт: введите координаты (x, y) и скорость V: " << endl;
            random(x1, y1, xc1, yc1, V1);

            LA[i] = new TAircraft(x1, y1, xc1, yc1, V1, 1);
            LA[i]->xc = xc1 = LA[i]->x - x;
            LA[i]->yc = yc1 = LA[i]->y - y;
            LA[i]->calc(x1, y1, xc1, yc1, V1);
        }

        else if (TypeVhcl == 2)
        {
            cout << "Вертолёт: введите координаты (x, y) и скорость V: " << endl;
            random(x1, y1, xc1, yc1, V1);
    
            LA[i] = new THelicopter(x1, y1, xc1, yc1, V1, 0);
            LA[i]->xc = LA[i]->x - x;
            LA[i]->yc = LA[i]->y - y;
            LA[i]->calc(x1, y1, xc1, yc1, V1);
        }
        else
        {
            cout << "всё норм? нажмите 1 или 2" << endl;
            i--;
        }
        cout << endl;
        omega1 = -(LA[i]->V) / (LA[i]->R);
    }
    cout << endl;

    float X = x + 1.1f * l;
    a = 1;  
    int landfinish = 0; 

    while (landfinish != n) 
    {
        for (int i = 0; i < n; i++)
        {
            if (LA[i]->landing) continue;

            if (LA[i]->spec == 1) 
            {
                if (!(f && LA[i]->f) && ((LA[i]->x) < X) && (abs((LA[i]->y) - y) < (l / 50)))
                {
                    a = 0; 
                    f = true; 
                    LA[i]->f = true; 
                }
                else a = 1; 

                if (((LA[i]->x) < (x + l)) && (LA[i]->f))
                {
                    LA[i]->landing = true; 
                    cout << "Самолёт №" << i + 1 << " приземлился, время прибытия - " << tnull << endl;
                    airport << "Самолёт №" << i + 1 << " приземлился, время прибытия - " << tnull << endl;
                    a = 1;  
                    f = false; 
                    LA[f]->f = false; 
                    landfinish++;
                    continue;
                }
                else if (f == true) LA[i]->landing = false;
            }

            if (LA[i]->spec == 0) 
            {
                if (!(f && LA[i]->f))
                {
                    a = 0; 
                    f = true; 
                    LA[i]->f = true; 
                }
                else a = 1; 

                if (((LA[i]->x) < X) && (LA[i]->y < l))
                {
                    LA[i]->landing = true; //посадка завершена
                    cout << "Вертолёт №" << i + 1 << " приземлился, время прибытия - " << tnull << endl;
                    airport << "Вертолёт №" << i + 1 << " приземлился, время прибытия - " << tnull << endl;
                    a = 1;  
                    f = false; 
                    LA[f]->f = false; 
                    landfinish++;
                    continue;
                }
                else LA[i]->landing = false;
            }


            if (LA[i]->spec == 1) 
            {
                LA[i]->x = -(LA[i]->xc + LA[i]->R * cos(LA[i]->fi + omega1 * tnull)) + a * (LA[i]->x + LA[i]->V * dt);
                LA[i]->y = -(LA[i]->yc + LA[i]->R * sin(LA[i]->fi + omega1 * tnull)) + a * LA[i]->y;
            }
            if (LA[i]->spec == 0) 
            {
                LA[i]->x = abs((LA[i]->x) - abs(a * LA[i]->V * cos(LA[i]->fi) * dt));
                LA[i]->y = abs(LA[i]->y - abs(a * LA[i]->V * sin(LA[i]->fi) * dt));
                
            }
            LA[i]->xc = LA[i]->x - x;
            LA[i]->yc = LA[i]->y - y;

            tnull += dt;
        }
    }
    airport.close();
}

void random(float& x1, float& y1, float& xc1, float& yc1, float& V1)
{
    x1 = float(1 + rand() % 10000);
    y1 = float(1 + rand() % 10000);
    V1 = float(1 + rand() % 10000);
    cout << x1 << ' ' << y1 << ' ' << V1 << endl;
}