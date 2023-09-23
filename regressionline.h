#ifndef REGRESSIONLINE_H
#define REGRESSIONLINE_H

#include <map>
#include <math.h>
using namespace std;

class RegressionLine
{
    public:

        typedef map<double, double> Points;

        RegressionLine(Points & points);
        virtual ~RegressionLine();

        double slope() const;
        double yIntercept() const;
        double regressionCoefficient() const;

    private:

        double slope_;
        double yIntercept_;
        double regressionCoefficient_;

};

#endif // REGRESSIONLINE_H
