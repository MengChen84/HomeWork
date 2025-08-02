#include <iostream>
#include <cmath>
#include <iomanip>
#include <chrono>
using namespace std;

// === Term 結構 ===
struct Term {
    float coef; // 係數
    int exp;    // 次數
};

// === Polynomial 類別 ===
class Polynomial {
private:
    Term* termArray; // 儲存非零項的陣列
    int capacity;     // termArray 的大小（可容納多少項）
    int terms;        // 實際非零項數

    // 動態擴充容量（若需要）
    void Resize(int newCapacity) {
        Term* newArray = new Term[newCapacity];
        for (int i = 0; i < terms; ++i) {
            newArray[i] = termArray[i];
        }
        delete[] termArray;
        termArray = newArray;
        capacity = newCapacity;
    }

public:
    // === 建構子 ===
    Polynomial(int cap = 10) {
        capacity = cap;
        termArray = new Term[capacity];
        terms = 0;
    }

    // === 解構子 ===
    ~Polynomial() {
        delete[] termArray;
    }

    // === 加項函式（由使用者輸入用）===
    void NewTerm(float coef, int exp) {
        if (terms == capacity) Resize(capacity * 2);
        termArray[terms].coef = coef;
        termArray[terms].exp = exp;
        ++terms;
    }

    // === 運算子 >> 輸入 ===
    friend istream& operator>>(istream& in, Polynomial& p) {
        cout << "請輸入非零項數：";
        in >> p.terms;
        p.capacity = p.terms;
        delete[] p.termArray;
        p.termArray = new Term[p.capacity];
        for (int i = 0; i < p.terms; ++i) {
            cout << "請輸入第 " << i + 1 << " 項 (係數 次數)：";
            in >> p.termArray[i].coef >> p.termArray[i].exp;
        }
        return in;
    }

    // === 運算子 << 輸出 ===
    friend ostream& operator<<(ostream& out, const Polynomial& p) {
        if (p.terms == 0) {
            out << "0";
            return out;
        }

        for (int i = 0; i < p.terms; ++i) {
            float c = p.termArray[i].coef;
            int e = p.termArray[i].exp;

            if (i > 0 && c > 0) out << " + ";
            else if (c < 0) out << " - ";

            float absC = abs(c);
            if (absC != 1 || e == 0)
                out << absC;

            if (e > 0) {
                out << "x";
                if (e > 1) out << "^" << e;
            }
        }
        return out;
    }

    // === 多項式加法 ===
    Polynomial Add(const Polynomial& b) {
        Polynomial result(terms + b.terms);
        int i = 0, j = 0;

        while (i < terms && j < b.terms) {
            if (termArray[i].exp == b.termArray[j].exp) {
                float sum = termArray[i].coef + b.termArray[j].coef;
                if (sum != 0)
                    result.NewTerm(sum, termArray[i].exp);
                i++; j++;
            } else if (termArray[i].exp > b.termArray[j].exp) {
                result.NewTerm(termArray[i].coef, termArray[i].exp);
                i++;
            } else {
                result.NewTerm(b.termArray[j].coef, b.termArray[j].exp);
                j++;
            }
        }

        // 尾巴剩下的項
        while (i < terms)
            result.NewTerm(termArray[i].coef, termArray[i].exp), i++;
        while (j < b.terms)
            result.NewTerm(b.termArray[j].coef, b.termArray[j].exp), j++;

        return result;
    }

    // === 多項式乘法 ===
    Polynomial Mult(const Polynomial& b) {
        Polynomial result;

        for (int i = 0; i < terms; ++i) {
            for (int j = 0; j < b.terms; ++j) {
                float c = termArray[i].coef * b.termArray[j].coef;
                int e = termArray[i].exp + b.termArray[j].exp;

                // 檢查是否已存在同次方
                bool merged = false;
                for (int k = 0; k < result.terms; ++k) {
                    if (result.termArray[k].exp == e) {
                        result.termArray[k].coef += c;
                        merged = true;
                        break;
                    }
                }
                if (!merged)
                    result.NewTerm(c, e);
            }
        }
        return result;
    }

    // === 多項式代入運算 ===
    float Eval(float x) {
        float sum = 0;
        for (int i = 0; i < terms; ++i) {
            sum += termArray[i].coef * pow(x, termArray[i].exp);
        }
        return sum;
    }
};
int main() {
    Polynomial A, B;

    cout << "=== 輸入多項式 A ===\n";
    cin >> A;

    cout << "=== 輸入多項式 B ===\n";
    cin >> B;

    cout << "\n多項式 A: " << A << endl;
    cout << "多項式 B: " << B << endl;

    Polynomial C = A.Add(B);
    Polynomial D = A.Mult(B);

    cout << "\nA + B = " << C << endl;
    cout << "A * B = " << D << endl;

    float x;
    cout << "\n請輸入一個數字 x：";
    cin >> x;
    cout << "A(x) = " << A.Eval(x) << endl;
    cout << "B(x) = " << B.Eval(x) << endl;

    return 0;
}

