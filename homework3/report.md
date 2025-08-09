# 41141117

作業三

### 解題說明

本題要求設計並實作一個支援多項式運算的Polynomial類別，功能包含：

1.建構與輸入多項式（以係數與次方組成的項目表示）。
2.多項式加法與乘法運算。
3.多項式在特定數值下的求值。
4.多項式結果的簡化（合併相同次方、移除係數為零的項）。

資料表示：每一項以 (coefficient, exponent)的結構體表示，整體以 vector<Term>儲存，能靈活支援稀疏或稠密多項式。
運算策略：
  加法：將兩個多項式的項目合併，再透過簡化函式合併相同次方項目。
  乘法：使用雙層迴圈計算所有項目的交叉乘積，並在結束後簡化結果。
  求值：使用冪次運算 pow(x, exponent)計算每項值並加總。
效率考量：雖然簡化過程為 O(n²)，但由於項目數量通常不大，對作業範例來說可接受；若應用於大規模稀疏多項式，可改用 map<int,double> 儲存以提升效率。
易用性：透過運算子多載 (operator+, operator*, operator<<) 提供自然直觀的使用介面。


### 解題策略

1.資料結構規劃
   採用 struct Term { double coefficient; int exponent; }; 表示多項式的單一項。
   使用 vector<Term> 儲存整個多項式，保持彈性，方便動態增減項目。

2.多項式運算設計
   加法運算：直接合併兩個多項式的所有項，之後統一呼叫 simplify() 進行合併與刪除零項。
   乘法運算：雙層迴圈遍歷兩個多項式的所有項，將乘積結果放入新向量，再呼叫 simplify()。
   求值運算：遍歷項目，對每項使用 pow(x, exponent)計算後加總。

3. 簡化邏輯
   以內層迴圈或雜湊結構檢查相同次方，將係數合併。
   去除係數接近零的項目（浮點數容差 1e-9）。

4. 效能與可擴充性
   時間複雜度：加法 O(n)、乘法 O(n²)、簡化 O(n²)、求值 O(n)。
   若需處理稀疏且高次的多項式，可改用 map<int,double>以獲得更佳的簡化效率。

5. 使用者介面設計
   運算子多載 (operator+, operator*, operator<<) 讓多項式運算直覺且易讀。
   提供多種建構子支援不同輸入格式（直接輸入項目、空多項式等）。

### 程式實作

```cpp
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
```


### 效能分析

加法時間複雜度：O(n)，需遍歷兩個多項式的項目數。
乘法時間複雜度：O(n²)，需計算每一項與另一多項式所有項目的乘積。
簡化時間複雜度：O(n²)，兩兩比較以合併同次方項目。
求值時間複雜度：O(n)，需計算每項的次方與乘積。
空間複雜度：
    加法與求值：O(n)
    乘法：O(n²)（中間結果項目數量可能是兩多項式項數的乘積）
    簡化：O(n) 需要額外儲存合併後的項目列表。

### 測試與驗證

| 測試編號 | 輸入多項式 1      | 輸入多項式 2 | 測試項目 | 預期結果         |
| ---- | ------------ | ------- | ---- | ------------ |
| 測試 1 | 3x² + 2x + 1 | x² + 4  | 加法   | 4x² + 2x + 5 |
| 測試 2 | 2x + 3       | x + 1   | 乘法   | 2x² + 5x + 3 |
| 測試 3 | x² + 2x + 1  | x = 2   | 求值   | 9            |


### 編譯與執行指令

```shell
$ g++ -std=c++17 -o main main.cpp
$ ./main.exe
```
## 申論及開發報告

3.遇到的問題與解決方案
項目合併效率低：初版 simplify() 為 O(n²)，在多項式項數多時效能下降，後續可用 unordered_map 提升至 O(n)。
輸出格式化：需特別處理正負號、常數項與指數為 1 或 0 的情況，避免輸出不直觀。

4.優化方向與未來延伸
改用雜湊表結構降低合併時間複雜度。
增加多項式除法與微分功能，擴展運算能力。
增加檔案輸入/輸出功能，便於批次運算。



