#include <iostream>
#include <vector>
#include <cmath>
#include <chrono>
using namespace std;
using namespace chrono;

struct Term {
    double coefficient;
    int exponent;
};

class Polynomial {
private:
    vector<Term> terms;
    void simplify() {
        vector<Term> simplified;
        for (auto &t : terms) {
            bool merged = false;
            for (auto &s : simplified) {
                if (s.exponent == t.exponent) {
                    s.coefficient += t.coefficient;
                    merged = true;
                    break;
                }
            }
            if (!merged) simplified.push_back(t);
        }
        terms.clear();
        for (auto &s : simplified) {
            if (fabs(s.coefficient) > 1e-9) {
                terms.push_back(s);
            }
        }
    }
public:
    Polynomial() = default;
    Polynomial(const vector<Term>& terms) : terms(terms) { simplify(); }
    Polynomial operator+(const Polynomial& other) const {
        vector<Term> result = terms;
        result.insert(result.end(), other.terms.begin(), other.terms.end());
        Polynomial sum(result);
        sum.simplify();
        return sum;
    }
    Polynomial operator*(const Polynomial& other) const {
        vector<Term> result;
        for (auto &a : terms) {
            for (auto &b : other.terms) {
                result.push_back({a.coefficient * b.coefficient, a.exponent + b.exponent});
            }
        }
        Polynomial product(result);
        product.simplify();
        return product;
    }
    double evaluate(double x) const {
        double sum = 0;
        for (auto &t : terms) {
            sum += t.coefficient * pow(x, t.exponent);
        }
        return sum;
    }
    friend ostream& operator<<(ostream& os, const Polynomial& poly) {
        for (size_t i = 0; i < poly.terms.size(); i++) {
            if (i > 0 && poly.terms[i].coefficient > 0) os << "+";
            os << poly.terms[i].coefficient;
            if (poly.terms[i].exponent > 0) {
                os << "x";
                if (poly.terms[i].exponent > 1) os << "^" << poly.terms[i].exponent;
            }
            os << " ";
        }
        return os;
    }
};

int main() {
    Polynomial p1({{3,2},{2,1},{-5,0}});
    Polynomial p2({{1,1},{4,0}});

    cout << "P1: " << p1 << "\n";
    cout << "P2: " << p2 << "\n";

    Polynomial sum = p1 + p2;
    Polynomial product = p1 * p2;

    cout << "P1 + P2 = " << sum << "\n";
    cout << "P1 * P2 = " << product << "\n";
    cout << "P1 evaluated at x=2: " << p1.evaluate(2) << "\n";

    auto start = high_resolution_clock::now();
    Polynomial largeP1({{1,1000}}), largeP2({{1,1000}});
    Polynomial res = largeP1 * largeP2;
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(end - start);
    cout << "Multiplication took: " << duration.count() << " ms\n";
}