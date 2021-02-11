#include "eoscalculator.h"
#include "math.h"

#include <QDebug>


eoscalculator::eoscalculator(QObject *parent) : QObject(parent)
{

}

QList<double> eoscalculator::calculateV(double T, double P, double a, double b)
{
    QList<double> * solutions = new QList<double>();
    coeff_a = P * sqrt(T);
    coeff_b = - R * T * sqrt(T);
    coeff_c = a - R * T * sqrt(T) * b - P * pow(b, 2) * sqrt(T);
    coeff_d = - a * b;

    //rijesi kubnu jednadzbu - Cardanove formule
    p = (3 * coeff_a * coeff_c - pow(coeff_b, 2))/(3 * pow(coeff_a, 2));
    q = (2 * pow(coeff_b, 3) - 9 * coeff_a * coeff_b * coeff_c + 27 * pow(coeff_a, 2) * coeff_d)/(27 * pow(coeff_a, 3));

    double condition = pow(q / 2, 2) + pow(p / 3, 3);

    if(condition > 0) //jednadzba ima jedinstveno realno rj
    {
       // qDebug() << " > 0";
        double solution = cbrt(- q/2 + sqrt(condition)) + cbrt(-q/2 - sqrt(condition)) - coeff_b/(3 * coeff_a);
        solutions->append(solution);
    }
    else if(condition < 0) //tri realna korijena
    {
       // qDebug() << " < 0";
        double r = sqrt(pow(-p/3, 3));
        double y = - q/(2 * r);
        double phi = acos(y);
        double solution_1 = 2 * cbrt(r) * cos(phi/3) - coeff_b/(3 * coeff_a); //k = 0
        double solution_2 = 2 * cbrt(r) * cos((phi + 2 * M_PI)/3) - coeff_b/(3 * coeff_a); //k = 1
        double solution_3 = 2 * cbrt(r) * cos((phi + 4 * M_PI)/3) - coeff_b/(3 * coeff_a); //k = 2
        solutions->append(solution_1);
        solutions->append(solution_2);
        solutions->append(solution_3);
    }
    else{ // tri realna korijena gdje je jedan dvostruk
       // qDebug() << " == 0";
        double solution_1 = 2 * cbrt(-q/2) - coeff_b/(3*coeff_a);
        double solution_2_3 = cbrt(q/2) - coeff_b/(3*coeff_a);
        solutions->append(solution_1);
        solutions->append(solution_2_3);
        solutions->append(solution_2_3);
    }
	
	return *solutions;
}
