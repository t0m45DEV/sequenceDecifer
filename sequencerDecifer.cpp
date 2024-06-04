#include <iostream>
#include <cmath>
using std::cin;
using std::cout;
using std::string;


/*
    Abstract object representing the math polynomial
*/
class polynomial {

    public:
        /*
            Creates a polynomial object, with the number of coefficients and a list
            of coefficients especified

            The list of coefficients is sorted from lower power to higher, so the
            polynomial f(x) = x² - x + 2, will need the list {2, -1, 1}
            
            The third argument changes the variable letter, default is 'n'
        */
        polynomial(int numberOfCoefficients, float* coefficients, char variable = 'n') {

            _variable = variable;
            _numberOfCoefficients = numberOfCoefficients;
            _coefficients = (float*) malloc(sizeof(float) * _numberOfCoefficients);

            for (int i = 0; i < _numberOfCoefficients; i++) {
                _coefficients[i] = coefficients[i];
            }
        }

        /*
            Free the memory used for the polynomial
        */
        void destroy() {
            free(_coefficients);
        }

        /*
            Get the number of coefficients
        */
        int get_number_of_coefficients() {
            return _numberOfCoefficients;
        }

        /*
            Get the grade of the polynomial
        */
        int get_grade() {
            return _numberOfCoefficients - 1;
        }

        /*
            Get the n-th coefficient
        */
        float get_coefficient(int n) {
            return _coefficients[n];
        }

        /*
            Returns f(n), where f is the polynomial
        */
        float evaluate_in(float n) {
            float ret = 0;

            for (int i = 0; i < _numberOfCoefficients; i++) {
                ret += _coefficients[i] * pow(n, i);
            }
            return ret;
        }

        /*
            Modifies the caller polynomial, making the
            sum between it and polB
        */
        void add(polynomial polB) {

            for (int i = 0; i < _numberOfCoefficients; i++) {
                _coefficients[i] += polB.get_coefficient(i);
            }
        }

        /*
            Modifies the caller polynomial, multiplying
            every coefficient by n
        */
        void multiply_by(float n) {

            for (int i = 0; i < _numberOfCoefficients; i++) {
                _coefficients[i] = _coefficients[i] * n;
            }
        }

        /*
            Returns a string forming the polynomial
        */
        string get_printable() {
            string ret = "";

            if (_coefficients[_numberOfCoefficients - 1] < 0) {
                ret += "-";
            }

            bool hasSuccesor = false;

            for (int i = _numberOfCoefficients - 1; i > 0; i--) {
                ret += write_nth_coefficient(i);

                if (!(hasSuccesor) && (write_nth_coefficient(i) != "")) {
                    hasSuccesor = true;
                }

                if ((write_nth_coefficient(i - 1) != "") && (hasSuccesor)) {

                    if (_coefficients[i - 1] < 0) {
                        ret += " - ";
                    } else {
                        ret += " + ";
                    }
                }
            }
            ret += write_nth_coefficient(0);
            return ret;
        }

    private:
        int _numberOfCoefficients;
        float* _coefficients;
        string _variable;

        /*
            Makes a string of the n-th coefficient, without sign
        */
        string write_nth_coefficient(int n) {
        
            string ret = "";
            string component = std::to_string(abs(_coefficients[n]));

            if (_coefficients[n] > int(_coefficients[n])) {
                component += ",5";
            }

            if (_coefficients[n] != 0) {
                
                if (n == 0) {
                    ret += component;

                } else if (n == 1) {

                    if (abs(_coefficients[n]) == 1) {
                        ret += _variable;
                    } else {
                        ret += (component + _variable);
                    }
                } else {
                    string power = "^(" + std::to_string(n)  + ")";

                    if (abs(_coefficients[n]) == 1) {
                        ret += (_variable + power);
                    } else {
                        ret += (component + _variable + power);
                    }
                }
            }
            return ret;
        }
};


/*
    Final answer is printed in screen here

    It prints the polynomial given, and the first three
    evaluations

    Then includes the next number in the sequence
*/
void print_answer(polynomial pol) {
    cout << "The general formula is: " << pol.get_printable() << '\n';

    cout << "f(1) = " << pol.evaluate_in(1) << '\n';
    cout << "f(2) = " << pol.evaluate_in(2) << '\n';
    cout << "f(3) = " << pol.evaluate_in(3) << '\n';

    cout << "The next element in the sequence is f(4) = " << pol.evaluate_in(4) << '\n';
}


int main(int argc, char* argv[]) {

    // Changing the variable letter
    char polVar = 'n';

    if (argc == 5) {
        polVar = *argv[4];
    }

    // Creating the sequence in an array
    int numberOfIntegers = 3;
    float listOfIntegers[numberOfIntegers];

    for (int i = 1; i < (numberOfIntegers + 1); i++) {
        listOfIntegers[i - 1] = float(atoi(argv[i]));
    }

    /*
        Here we create three polynomials

        In pol1 we have that f(1) = 1, in pol2 f(2) = 1, and in pol3 f(3) = 1

        We will make them being equal to the value we want, and then add them

        Here the code is prepared to handle 3 numbers for the sequence, if
        we want to search for a formula for 4 or more elements, we will need
        to add the polynomials that sattisfies the formula f(i) = 1, where i
        is the index of the element

        The logic behind is the same for n elements
    */

    float case1Coefficients[3] = {0, 1, 0};     // f1(n) = n
    float case2Coefficients[3] = {-1, 1, 0};    // f2(n) = n - 1
    float case3Coefficients[3] = {2, -3, 1};    // f3(n) = n² - 3n + 2 = (n - 1)(n - 2)

    polynomial pol1 = polynomial(3, case1Coefficients, polVar);
    polynomial pol2 = polynomial(3, case2Coefficients, polVar);
    polynomial pol3 = polynomial(3, case3Coefficients, polVar);

    // Here comes the magic
    pol1.multiply_by(listOfIntegers[0]);

    pol2.multiply_by(listOfIntegers[1] - pol1.evaluate_in(2));
    pol1.add(pol2);

    pol3.multiply_by(float(listOfIntegers[2] - pol1.evaluate_in(3)) / 2);
    pol1.add(pol3);

    // Printing the final solution
    print_answer(pol1);

    // Avoiding memory leaks for every polynomial
    pol1.destroy();
    pol3.destroy();
    pol2.destroy();
    return 0;
}
