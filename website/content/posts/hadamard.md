---
title: "Преобразование Уолша — Адамара и xor-and-or-свертки"
date: 2021-09-15T17:59:03+03:00
draft: false
---

<center> <h2>Преобразование Уолша — Адамара и xor-and-or-свертки</h2> </center>
<br/>
<br/>


<!--- TODO: облачить все xor or and в ``--->

Быстрое преобразование Фурье сворачивает два массива, отправляя $a[i], b[j] \to c[i + j]$,
что соответствует тому, что $x^i \cdot x^j = x^{i + j}$.

Сейчас мы рассмотрим случаи, когда
$x^i \cdot x^j = x^{i \oplus j}$, $x^i \cdot x^j = x^{i | j}$ и $x^i \cdot x^j = x^{i \\& j}$.

Преобразование Фурье выглядело примерно таким образом: была матрица

$$M = 
\begin{pmatrix}
\omega_0^0 & \omega_0^1 & \omega_0^2 & \ldots & \omega_0^{n - 1}\\\\
\omega_1^0 & \omega_1^1 & \omega_1^2 & \ldots & \omega_1^{n - 1}\\\\
\vdots & \vdots & \vdots & \ddots & \vdots\\\\
\omega_{n - 1}^0 & \omega_{n - 1}^1 & \omega_{n - 1}^2 & \ldots & \omega_{n - 1}^{n - 1}
\end{pmatrix}
$$

Были векторы $A$ и $B$. Мы применяли матрицу $M$ к $A$ и $B$, после чего
перемножали $MA$ и $MB$ поэлементно, а затем применяли $M^{-1}$ к $MA \cdot MB$.

## xor-свертка

Здесь мы хотим сделать ровно то же самое, надо лишь поменять матрицу.

Давайте зададим матрицы $H_{2^k}$ рекурсивно.
$H_1 = 
\begin{pmatrix}
1
\end{pmatrix}
$ и
$H_{2^k} = \frac{1}{\sqrt{2}}
\begin{pmatrix}
H_{2^{k - 1}} & H_{2^{k - 1}}\\\\
H_{2^{k - 1}} & -H_{2^{k - 1}}
\end{pmatrix}
$

Корень из двух — это не очень удобное число, давайте попытаемся от него избавиться.
Определим матрицы без этого корня:
$G_1 = 
\begin{pmatrix}
1
\end{pmatrix}
$ и
$G_{2^k} =
\begin{pmatrix}
G_{2^{k - 1}} & G_{2^{k - 1}}\\\\
G_{2^{k - 1}} & -G_{2^{k - 1}}
\end{pmatrix}
$

Тогда легко заметить, что $\sqrt{n} \cdot H_n = G_n$. Мы будем считать именно $G_n$.

**Обозначение:**
$E_n$ — это матрица тождественного преобразования размера $n \times n$, то есть матрица,
у которой на главной диагонали стоят единицы, а в остальных ячейках нули.


Из-за такого рекурсивного задания сразу видно, как написать быстрое преобразование Уолша-Адамара,
то есть как посчитать $G_{2^k} \cdot A$.
На нижнем уровне рекурсии не нужно менять ничего, потому что $G_1 = E_1$,
а на других уровнях надо сначала запуститься от двух половин, а потом сделать замену
$x, y \to x + y, x - y$. Напишем код:

```cpp
void hadamard(vector<int> &a, int l, int r) {
    if (l + 1 == r) {
        return;
    }
    int mid = (r + l) / 2;
    hadamard(a, l, mid);
    hadamard(a, mid, r);
    for (int i = l; i < mid; i++) {
        int u = a[i], v = a[mid + (i - l)];
        a[i] = u + v;
        a[mid + (i - l)] = u - v;
    }
}
```

**Лемма:**    
Удивительным образом получается, что $H_{2^k}^{-1} = H_{2^k}$.


**Доказательство:**
Докажем это по индукции.
Для $H_1$ это очевидно, потому что $H_1 = E_1$.
Докажем для $H_{2^k}$, если уже известно для $H_{2^{k - 1}}$.

$$H_{2^k} \cdot H_{2^k} = 
\frac{1}{\sqrt{2}}
\begin{pmatrix}
H_{2^{k - 1}} & H_{2^{k - 1}}\\\\
H_{2^{k - 1}} & -H_{2^{k - 1}}
\end{pmatrix}
\cdot
\frac{1}{\sqrt{2}}
\begin{pmatrix}
H_{2^{k - 1}} & H_{2^{k - 1}}\\\\
H_{2^{k - 1}} & -H_{2^{k - 1}}
\end{pmatrix}
=$$

$$ = \frac{1}{2}
\begin{pmatrix}
2H_{2^{k - 1}}^2 & 0\\\\
0 & 2H_{2^{k - 1}}^2
\end{pmatrix} = \frac{1}{2}
\begin{pmatrix}
2E_{2^{k - 1}} & 0\\\\
0 & 2E_{2^{k - 1}}
\end{pmatrix}
$$


Мы получили, что $H_n \cdot H_n = E_n$.
Но мы знаем, что $H_n = \frac{G_n}{\sqrt{n}}$,
поэтому $G_n \cdot G_n = n \cdot E_n$.
Так что вместо того, чтобы домножать на $H$, можно домножать на $G$,
но в конце просто поделить на $n$ после обратного преобразования. Таким образом мы научились делать xor-умножение многочленов
без использования вещественных или комплексных чисел:

```cpp
// we assume that a.size() == b.size() == 2^k
vector<int> xormult(vector<int> a, vector<int> b) {
    hadamard(a, 0, (int)a.size());
    hadamard(b, 0, (int)b.size());
    for (size_t i = 0; i < a.size(); i++) {
        a[i] *= b[i];
    }
    hadamard(a, 0, a.size()); // inverse convolution is the same as direct
    for (size_t i = 0; i < a.size(); i++) {
        a[i] /= (int)a.size();
    }
    return a;
}
```

Если вы внимательно следили за происходящим, то могли заметить, что я вас немного обманул.
Я доказал кучу всего, но никак не объяснил, почему все это даст нам xor-свертку.

Обозначим xor-свертку за $\\$$, то есть

$$(a_0, a_1, \ldots, a_n) \\$ (b_0, b_1, \ldots, b_n) = 
(\sum \limits_{k = 0}^{n} a_k \cdot b_{0 \oplus k}, \sum \limits_{k = 0}^{n} a_k \cdot b_{1 \oplus k},
\ldots, \sum \limits_{k = 0}^{n} a_k \cdot b_{n \oplus k})$$

Тогда нам необходимо доказать, что $(Ga) \cdot (Gb) = G (a \\$ b)$.
Для $G_1$ это очевидно, потому что свертка — это просто умножение чисел.
Для $G_2$ необходимо написать условия:

$$\begin{pmatrix}
a_0\\\\
a_1
\end{pmatrix}
\\$ 
\begin{pmatrix}
b_0\\\\
b_1
\end{pmatrix} =
\begin{pmatrix}
a_0 \cdot b_0 + a_1 \cdot b_1\\\\
a_0 \cdot b_1 + a_1 \cdot b_0
\end{pmatrix}
$$

$$G_2a = 
\begin{pmatrix}
a_0 + a_1\\\\
a_0 - a_1
\end{pmatrix}
$$

$$G_2b = 
\begin{pmatrix}
b_0 + b_1\\\\
b_0 - b_1
\end{pmatrix}
$$

$$(G_2a) \cdot (G_2b) = 
\begin{pmatrix}
(a_0 + a_1) \cdot (b_0 + b_1)\\\\
(a_0 - a_1) \cdot (b_0 - b_1)
\end{pmatrix}
 = G (a \\$ b)$$

 Для бóльших размерностей все доказывается по индукции.
Таким образом можно было бы изначально искать матрицу $G$,
предположив, что $G_2 = 
\begin{pmatrix}
c_0 & c_1 \\\\
c_2 & c_3
\end{pmatrix}
$, решить систему уравнений.

## and-свертка

and-свертка делается аналогично, надо только поменять матрицу.
$T_1 = \begin{pmatrix}
1\\\\
\end{pmatrix}
$,
$
T_{2^k} =  \begin{pmatrix}
T_{2^{k - 1}} & T_{2^{k - 1}}\\\\
0 & T_{2^{k - 1}}
\end{pmatrix}
$.

К сожалению, в отличие от xor-свертки, здесь обратная матрица не совпадает с прямой,
но зато нет корней из двойки, поэтому не придется в конце делить на $n$.

Обратные матрицы:
$T_1^{-1} = \begin{pmatrix}
1\\\\
\end{pmatrix}
$,
$
T_{2^k}^{-1} =  \begin{pmatrix}
T_{2^{k - 1}}^{-1} & -T_{2^{k - 1}}^{-1}\\\\
0 & T_{2^{k - 1}}^{-1}
\end{pmatrix}
$.

Из-за рекуррентного задания матриц, мы опять же легко можем написать алгоритм:

```cpp
void and_convolution(vector<int> &a, int l, int r) {
    if (l + 1 == r) {
        return;
    }
    int mid = (r + l) / 2;
    and_convolution(a, l, mid);
    and_convolution(a, mid, r);
    for (int i = l; i < mid; i++) {
        a[i] += a[mid + (i - l)];
    }
}

void rev_and_convolution(vector<int> &a, int l, int r) {
    if (l + 1 == r) {
        return;
    }
    int mid = (r + l) / 2;
    rev_and_convolution(a, l, mid);
    rev_and_convolution(a, mid, r);
    for (int i = l; i < mid; i++) {
        a[i] -= a[mid + (i - l)];
    }
}

vector<int> andmult(vector<int> a, vector<int> b) {
    and_convolution(a, 0, a.size());
    and_convolution(b, 0, b.size());
    for (size_t i = 0; i < a.size(); i++) {
        a[i] *= b[i];
    }
    rev_and_convolution(a, 0, a.size());
    return a;
}
```

Проверка правильности выбранной матрицы и ее обратной остается читателю в качестве упражнения.


## or-свертка

Все аналогично.

$Q_1 = \begin{pmatrix}
1\\\\
\end{pmatrix}
$,
$
Q_{2^k} =  \begin{pmatrix}
Q_{2^{k - 1}} & 0\\\\
Q_{2^{K - 1}} & Q_{2^{k - 1}}
\end{pmatrix}
$.


Обратные матрицы:

$Q_1^{-1} = \begin{pmatrix}
1\\\\
\end{pmatrix}
$,
$
Q_{2^k}^{-1} =  \begin{pmatrix}
Q_{2^{k - 1}}^{-1} & 0\\\\
-Q_{2^{K - 1}}^{-1} & Q_{2^{k - 1}}^{-1}
\end{pmatrix}
$.


Из-за рекуррентного задания матриц, мы опять же легко можем написать алгоритм:

```cpp
void or_convolution(vector<int> &a, int l, int r) {
    if (l + 1 == r) {
        return;
    }
    int mid = (r + l) / 2;
    or_convolution(a, l, mid);
    or_convolution(a, mid, r);
    for (int i = l; i < mid; i++) {
        a[mid + (i - l)] += a[i];
    }
}

void rev_or_convolution(vector<int> &a, int l, int r) {
    if (l + 1 == r) {
        return;
    }
    int mid = (r + l) / 2;
    rev_or_convolution(a, l, mid);
    rev_or_convolution(a, mid, r);
    for (int i = l; i < mid; i++) {
        a[mid + (i - l)] -= a[i];
    }
}

vector<int> ormult(vector<int> a, vector<int> b) {
    or_convolution(a, 0, a.size());
    or_convolution(b, 0, b.size());
    for (size_t i = 0; i < a.size(); i++) {
        a[i] *= b[i];
    }
    rev_or_convolution(a, 0, a.size());
    return a;
}

```



Проверка правильности выбранной матрицы и ее обратной остается читателю в качестве упражнения.

**Замечание:**
Заметим, что $a | b = \overline{\overline{a} \\& \overline{b}}$, где $\overline{x}$ — замена всех единичных битов числа
на нулевые, а нулевых на единичные. Поэтому or-свертку можно написать через $and$-свертку (и наоборот).



## Оптимизации

Убрать рекурсию здесь проще, чем в FFT. Рекурсия головная, поэтому раскрывается очевидным образом.
Также прямые и обратные отображения очень похожи, так что их можно объединить в одну функцию.

```cpp
void hadamard(vector<int> &a) {
    for (size_t len = 1; len < a.size(); len *= 2) {
        for (size_t i = 0; i < a.size(); i += 2 * len) {
            for (size_t j = 0; j < len; j++) {
                int u = a[i + j], v = a[i + len + j];
                a[i + j] = u + v;
                a[i + len + j] = u - v;
            }
        }
    }
}
```

```cpp
void and_convolution(vector<int>& a, bool inv) {
    for (size_t len = 1; len < a.size(); len *= 2) {
        for (size_t i = 0; i < a.size(); i += 2 * len) {
            for (size_t j = 0; j < len; j++) {
                if (inv) {
                    a[i + j] -= a[i + len + j];
                } else {
                    a[i + j] += a[i + len + j];
                }
            }
        }
    }
}
```

```cpp
void or_convolution(vector<int>& a, bool inv) {
    for (size_t len = 1; len < a.size(); len *= 2) {
        for (size_t i = 0; i < a.size(); i += 2 * len) {
            for (int j = 0; j < len; j++) {
                if (inv) {
                    a[i + len + j] -= a[i + j];
                } else {
                    a[i + len + j] += a[i + j];
                }
            }
        }
    }
}
```

## Альтернативный взгляд на and-or-свертки

Представленные выше алгебраические рассуждения на самом деле необходимы только для xor-свертки. Для and- и or-сверток можно обойтись без этого. Давайте докажем иначе, что or-свертка будет выглядеть так.

Для and-свертки это будет аналогично из-за замечания выше об их эквивалентности, так как операции and и or отличаются заменой нулей на единицы, а единиц на нули.

Для альтернативного доказательства работы алгоритма or-свертки давайте введем понятие массива сумм по подмножествам (эта техника также известна как SOS-DP (sum over subsets)).

**Определение:**
Пусть дан массив $A$. Давайте назовем массивом сумм по подмножествам массива $A$ такой массив $B$, что

$$ B_i = \sum_{j : j \subset i} A_j$$

При этом условие $j \subset i$ в данном случае воспринимается как содержание подмножеств единичных битов. То есть если бы мы воспринимали числа как маски. На языке битовых операций это условие можно записать как `i | j = i`, то есть $j$ — это число $i$, в котором занулили некоторые единичные биты.


Это называется массивом сумм по подмножествам именно потому, что если все индексы лежат в промежутке $[0, 2^k)$, то их стоит воспринимать как маски подмножеств множества $\{\, 0, 1, 2, \ldots, k - 1 \,\}$.

Давайте покажем, что or-свертка как раз таки насчитывает массив сумм по подмножествам.

Давайте считать динамику по битам. `dp[mask][ind]` — это сумма по тем подмножествам маски $mask$, в которых изменялись только первые $ind$ битов. Если $ind = 0$, то мы не меняли никаких битов, и это просто $A_{mask}$. Далее при переходе от $ind - 1$ к $ind$ нужно, возможно, поменять текущий бит, либо не менять его. То есть если бит $ind$ в $mask$ равен нулю, то мы ничего поменять не можем:

`dp[mask][ind] = dp[mask][ind - 1]`

А если он равен единице, то мы его можем занулить:

`dp[mask][ind] = dp[mask][ind - 1] + dp[mask ^ (1 << ind)][ind - 1]`

Теперь если мы уберем вторую координату динамики и будем насчитывать ее in-place, то как раз таки получим or-свертку. Алгоритм перебирает бит, после чего рассматривает все пары чисел с битом и без и прибавляет меньшее к большему. Соответственно, обратная or-свертка — это обратное преобразование: по массиву сумм по подмножествам получаем изначальный массив.

Тогда остается заметить, что с точки зрения массивов сумм по подмножествам or-свертка — это просто поэлементное умножение, потому что если `or` двух масок — это подмаска $mask$, то это равносильно просто тому, что обе эти маски — это подмаски $mask$.
Что и требовалось доказать.

Из такого объяснения вытекает немного другой способ написания той же or-свертки, который может показаться вам более простым:

```cpp
void or_convolution(vector<int>& a, bool inv) {
    for (size_t len = 1; len < a.size(); len *= 2) {
        for (size_t i = 0; i < a.size(); i++) {
            if (i & len == 0) {
                if (inv) {
                    a[i ^ len] -= a[i];
                } else {
                    a[i ^ len] += a[i];
                }
            }
        }
    }
}
```


## Задачи для практики


 
- [https://csacademy.com/contest/archive/task/random\_nim\_generator/](https://csacademy.com/contest/archive/task/random_nim_generator/)

- [https://csacademy.com/contest/archive/task/and-closure/](https://csacademy.com/contest/archive/task/and-closure/)

- [https://csacademy.com/contest/archive/task/token-grid/](https://csacademy.com/contest/archive/task/token-grid/)

- [https://www.codechef.com/NOV19A/problems/MDSWIN](https://www.codechef.com/NOV19A/problems/MDSWIN)   


