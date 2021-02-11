#ifndef EOSCALCULATOR_H
#define EOSCALCULATOR_H

#include <QObject>

class eoscalculator : public QObject
{
    Q_OBJECT
public:
    explicit eoscalculator(QObject *parent = nullptr);
    Q_INVOKABLE QList<double> calculateV(double, double, double, double);
private:
    double coeff_a, coeff_b, coeff_c, coeff_d;
    double R = 8.314462;
    double p, q;

signals:

};

#endif // EOSCALCULATOR_H
