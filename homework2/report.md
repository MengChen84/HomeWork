# 41141117

作業二

### 解題說明

本題要求我們實作一個C++類別Polynomial，其抽象資料型態（ADT）和私有成員，來表示與操作多項式可以表示任意形式的多項式，
並支援輸入、輸出、加法、乘法與值代入等基本操作。與一般 vector 儲存法不同，
此題使用 struct Term 來記錄多項式中「非零項」的係數與次數，能更有效節省空間並處理稀疏多項式。


### 解題策略

1.使用 Term 結構記錄每個非零項
2.使用 termArray 動態配置一個陣列，儲存所有項
3.加法與乘法實作時，依次比較兩個多項式的項次，進行合併或乘法操作
4.利用 operator>> 讀入係數與次數、用 operator<< 格式化輸出多項式字串
5.最後實作 Eval(float f)，可代入數值計算多項式值


### 程式實作

```cpp
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
```


### 效能分析

時間複雜度
| 操作名稱         | 說明                             | 時間複雜度           |
| ------------ | ------------------------------ | --------------- |
| >> 輸入      | 輸入 $n$ 個非零項，逐一填入陣列             | $O(n)$          |
| << 輸出      | 印出每個非零項（處理正負號、次方）              | $O(n)$          |
| Add() 加法   | 雙指標合併兩個已排序的項陣列（最多 $n + m$ 項）   | $O(n + m)$      |
| Mult() 乘法  | 每項乘每項，並檢查是否需合併同次方項             | $O(n \times m)$ |
| Eval(x) 代入 | 遍歷所有項並計算 $coef \times x^{exp}$ | $O(n)$          |

空間複雜度
| 操作名稱        | 空間需求說明                           | 空間複雜度           |
| ----------- | -------------------------------- | --------------- |
| 輸入儲存        | 儲存 $n$ 個項目的 Term 陣列            | $O(n)$          |
| Add() 輸出  | 最多 $n + m$ 項結果陣列                 | $O(n + m)$      |
| Mult() 輸出 | 最壞情況每項都產生唯一次方（最多 $n \times m$ 項） | $O(n \times m)$ |
| Eval(x)   | 僅需常數空間計算總和                       | $O(1)$          |


### 測試與驗證

| 測試案例 | 輸入多項式1 | 輸入多項式2 | 加法結果 | 乘法結果 |
|----------|---------|---------|----------|----------|
| 測試一   | x³+2x+1  | x²+3x+1  | x³+x²+5x+2 | x⁵+3x⁴+3x³+7x²+5x+1 |
| 測試二   | x⁵+2x⁴+x | x⁵+x⁴+3x | 2x⁵+3x⁴+4x | x¹⁰+3x⁹+8x⁶+3x⁵     |


### 編譯與執行指令

```shell
$ g++ -std=c++17 -o main main.cpp
$ ./main.exe
```
## 申論及開發報告
# 問題與解決方案
項次未排序會導致加法與乘法錯誤：
解法：在輸入後加入排序步驟（或要求使用者依次數遞減輸入）

乘法實作容易重複儲存同次方項：
解法：在乘法中檢查並合併相同次方項（以巢狀迴圈或 map 優化）

係數為 ±1 時輸出格式需省略「1」：
解法：在 << 輸出中加入條件判斷

記憶體釋放與拷貝控制：
初步版本未實作複製建構子與拷貝賦值，未來可加入避免 shallow copy 問題

這次開發過程也提醒我，程式邏輯之外，「輸入格式友善」與「錯誤處理」在實務開發中同樣重要。
此外，多項式乘法的時間複雜度與資源需求較高，需改善效能。


