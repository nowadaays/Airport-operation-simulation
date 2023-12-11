#ifndef Header_H    
#define Header_H

#include <math.h>
using namespace std;

const int n = 7;
const float dt = 0.01f;


class TAObject
{

public:
    float x, y;
    bool f;

    
    TAObject() {}; 
    TAObject(float x, float y); 
    ~TAObject() {}; 
};




class TLA : public TAObject 
{
public:
    int b;
    float xc, yc, fi, R, V;
    bool landing;
    int spec;   

    void calc(float x, float y, float xc, float yc, float V);
    TLA() {};
    TLA(float x, float y, float xc, float yc, float V, int spec); 
    virtual void move(float t, int a); 
};




class TAircraft : public TLA
{
public:
    TAircraft(float x, float y, float xc, float yc, float V, int spec) :TLA(x, y, xc, yc, V, spec) {};
    virtual void move(float t, int a);
};




class THelicopter : public TLA
{
public:
    THelicopter(float x, float y, float xc, float yc, float V, int spec) :TLA(x, y, xc, yc, V, spec) {};
    virtual void move(float t, int a);
};




class TAirport : public TAObject
{
public:
    float l;
    TLA* LA[n];

    TAirport() { x = 0; y = 0; l = 1; };
    TAirport(float x, float y, float l);
    void Do(float t0, float tk);
    ~TAirport()
    {
        delete[] LA;
    }

};

void random(float& x1, float& y1, float& xc1, float& yc1, float& V1);

#endif

