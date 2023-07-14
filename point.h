#ifndef POINT_H
#define POINT_H


struct point
{
    double x;
    double y;
    double z;
    double r;
    double g;
    double b;
    point(){}
    point(double x1,double y1,double z1,double r1=1.0,double g1=0.0,double b1=0.0):x(x1),y(y1),z(z1),r(r1),g(g1),b(b1){}
};
#endif // POINT_H
