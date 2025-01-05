#include <iostream>
#include <vector>
#include <cmath>
#include <functional>
#include <fstream>

using namespace std;

//contants
const double E0 = 10.00;
const double tau0 = 4.00;

//strain fucntion
const double maxStrain = 0.1;
const double strainRate = 0.01;
const double t1 = maxStrain / strainRate;

double strain(double t) {
    if(t <= t1) {
        return strainRate * t;
    } else {
        return maxStrain - (t - t1) * strainRate;
    }
}

//strain rate fucntion
double strainRateFunction(double t) {
    double dt = 1e-4;
    return ((strain(t+dt) - strain(t))/ dt); 
}

//Relaxation function
double Er(double t, double tau) {
    return E0 * exp(-(t-tau)/tau0);
}

// intgrand fucntion
double integrand(double tau, double t) {
    return Er (t, tau) * strainRateFunction(tau);
}

// Nuermcial integration (trapezoidal rule)
double integrate(double t, const function<double(double)> &func, int numSteps = 1000)  {
    double a = 0;
    double b = t;
    double h = ((b-a)/numSteps);
    double result = 0.5 * (func(a) + func(b));
    for(int i = 1 ; i < numSteps; i++ ) {
        double x = a + i * h;
        result += func(x);
    }
    return result * h;
}

int main() {
    int N = 1000;
    double T  = 20.00;
    double dt = T / N;

    vector<double> time(N);
    vector<double> strainValues(N);
    vector<double> stressValues(N);

    //Time
    for (int i=0; i < N; i++) {
        time[i] = i * dt;
    }
    
    //Calculate the stress and strain
    for(int i = 0; i < N;i++) {
        double t = time[i];
        strainValues[i] = strain(t);
        stressValues[i] = integrate(t, [t](double tau) {return integrand(tau, t);});
    }

    //output results to a file
    ofstream outFile("output.csv");
    outFile << "Time,Strain,Stress\n";
    for (int i = 0; i < N; i++) {
        outFile <<time[i] << "," << strainValues[i] << "," << stressValues[i] <<endl;
    }
    outFile.close();
    cout << "compeleted!";
    return 0;
}