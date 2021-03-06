---
title: "Оценка на количество делителей числа и сверхсоставные числа"
date: 2021-11-29T13:17:57+03:00
draft: false
---

<center> <h2>Оценка на количество делителей числа и сверхсоставные числа</h2> </center>
<br/>
<br/>


## Предыстория и мотивация

Иногда бывает так, что асимптотика решения задачи зависит от количества делителей числа во входе. Вы, возможно, слышали такое утверждение: <<количество делителей числа $n$ — это примерно кубический корень из $n$>>. Действительно ли у чисел может быть так много делителей?

На самом деле, это, конечно, не так, но оценка в кубический корень дает нужный порядок величин для грубых оценок на числах, с которыми мы имеем дело в реальной жизни (отличие не больше, чем в $4$ раза при $n \le 10^{15}$, и не больше, чем в $10$ раз при $n \le 10^{18}$). Давайте разберемся, сколько все таки на самом деле делителей у чисел, и как этим пользоваться, а также придумаем новую более точную оценку.


## Субполиномиальность и сверхсоставные числа

Будем обозначать количество делителей числа $n$ как $d(n)$. На самом деле $d(n)$ — это субполиномиальная величина, то есть для достаточно больших $n$ она меньше $n^{\varepsilon}$ для сколь угодно малого положительного $\varepsilon$. С доказательством можно, к примеру, ознакомиться, в [этой статье](https://codeforces.com/blog/entry/3863). То есть для ооочень больших $n$ можно оценивать $d(n)$ не только как кубический корень, но и как корень четвертой, пятой... любой степени! Однако нас не очень сильно интересуют эти теоретические оценки, потому что по настоящему они достигаются только для невероятно огромных чисел, с которыми мы не работаем. Нас интересуют более практичные оценки.

Максимальное количество делителей неразрывно связано с таким понятием как <<сверхсоставные числа>> (highly composite numbers). Это такие числа, у которых больше делителей, чем у всех меньших чисел. Первые сверхсоставные числа — это $1, 2, 4, 6, 12, 24\ldots$ Тогда, к примеру, если мы хотим понять, какое максимальное количество делителей есть у чисел, не превосходящих $1000$, можно посмотреть в список сверхсоставных чисел, найти там самое больше число, не превосходящее $1000$ (это будет $840$), и посмотреть, сколько у него делителей ($32$). На удивление сверхсоставные числа встречаются не очень часто: существует всего $156$ сверхсоставных чисел, не превосходящих $10^{18}$. К примеру, сверхсоставные числа можно найти как последовательность на OEIS: [oeis.org/A002182](https://oeis.org/A002182).

## Быстрая генерация больших сверхсоставных чисел

Для генерации сверхсоставных чисел есть [этот замечательный скрипт](https://gist.github.com/dario2994/fb4713f252ca86c1254d). По ссылке есть скрипт, способный очень быстро вычислить все сверхсоставные числа до `MAXN` (при `MAXN`$=10^{100}$ программа работает меньше секунды), а также ниже представлен список всех всех сверхсоставных чисел до $10^{18}$ с их количествами делителей и разложениями.

Давайте разберемся, как же этому коду удается так быстро находить такие большие сверхсоставные числа. Для начала придумаем парочку очевидных алгоритмов поиска сверхсоставных чисел. Давайте идти по числам по возрастанию, находить их количества делителей, и если все меньшие числа имели меньше делителей, то новое число будет сверхсоставным. Количество делителей числа можно находить аналогично проверке на простоту за $O(\sqrt n)$ (все делители разбиваются на пары вида ($x$, $\frac{n}{x}$), и один из этих делителей будет не больше корня из $n$). В частности, из этого следует самая простая оценка на количество делителей числа: $d(n) \le 2 \cdot \sqrt n$. Тогда мы найдем все сверхсоставные числа до n за $O(n \cdot \sqrt n)$, что очень долго.

Более умный способ основан на полезной идее о том, что вместо того, чтобы для каждого числа перебирать делители, мы можем действовать наоборот: для каждого числа будем перебирать, какие числа на него делятся. Эта идея похожа на идею решета Эратосфена и, как известно, работает за $O(n \log n)$, потому что числа, делящиеся на $k$ — это $k$,  $2k$, $3k$, $\ldots$ Их $\frac{n}{k}$ штук. Всего мы получим $\frac{n}{1} + \frac{n}{2} + \ldots + \frac{n}{n} = O(n \log n)$ (сумма гармонического ряда). В частности, из этого следует, что в среднем у чисел от $1$ до $n$ как раз таки очень мало делителей: $O(\log n)$. Но это все еще долго, такой алгоритм будет работать только для $n$ порядка $10^8$.

Для того, чтобы придумать эффективный алгоритм, давайте подробнее изучим нашу задачу. Давайте поймем, чему же равно количество делителей числа $n$. Пусть $n = p_1^{a_1} \cdot p_2^{a_2} \cdot \ldots \cdot p_k^{a_k}$. Тогда у $n$ есть ровно $(a_1 + 1) \cdot (a_2 + 1) \cdot \ldots \cdot (a_k + 1)$ делителей, потому что любой делитель $n$ должен состоять только из тех простых, которые есть в $n$, и при этом степени должны быть не больше, чем степени в $n$. Поэтому у нас есть $a_1 + 1$ вариантов степени $p_1$ (от $0$ до $a_1$), $a_2 + 1$ вариантов степени $p_2$ и так далее. Мы хотим, чтобы это произведение было как можно больше.

Несложно заметить, глядя на список по ссылке выше, что степени вхождения простых $2, 3, 5, 7 \ldots$ в сверхсоставные числа не возрастают. Действительно, если есть, к примеру, число $40 = 2^3 \cdot 3^0 \cdot 5^1$, то можно поменять местами степени тройки и пятерки и получить число $24 = 2^3 \cdot 3^1 \cdot 5^0$, у которого будет ровно столько же делителей, но само число будет меньше.

Собственно, ровно на этой идее и основан алгоритм выше. Он начинает с числа $1$ и постепенно генерирует все возможные числа, не превосходящие `MAXN`, у которых степени вхождения простых не возрастают. Так как сумма степеней простых не больше логарифма `MAXN`, таких невозрастающих наборов степеней будет не очень много. После чего, зная разложение, легко посчитать для каждого из них количество делителей. Однако это еще не ответ. Это лишь кандидаты на то, чтобы быть сверхсоставными. Остается отсортировать их и пройтись по порядку, выбирая все числа, у которых больше делителей, чем у всех предыдущих.

## Таблица для повседневного использования

Список сверхсоставных чисел — это, конечно, хорошо, но он не очень удобен для использования на практике. На практике у нас есть некоторое ограничение на $n$, и мы хотим узнать, какое максимальное количество делителей может быть у числа с такими ограничениями. На этот случай есть такая замечательная табличка:

| $\le N$ | $n$ | Факторизация | $d(n)$ |
| ------- | --- | ------------ | ------ |
| 20 | 12 | $2^2 \cdot 3^1$ | 6 |
| 50 | 48 | $2^4 \cdot 3^1$ | 10 |
| 100 | 60 | $2^2 \cdot 3^1 \cdot 5^1$ | 12 |
| $10^3$ | 840 | $2^3 \cdot 3^1 \cdot 5^1 \cdot 7^1$ | 32 |
| $10^4$ | 7560 | $2^3 \cdot 3^3 \cdot 5^1 \cdot 7^1$ | 64 |
| $10^5$ | 83160 | $2^3 \cdot 3^3 \cdot 5^1 \cdot 7^1 \cdot 11^1$ | 128 |
| $10^6$ | 720720 | $2^4 \cdot 3^2 \cdot 5^1 \cdot 7^1 \cdot 11^1 \cdot 13^1$ | 240 |
| $10^7$ | 8648640 | $2^6 \cdot 3^3 \cdot 5^1 \cdot 7^1 \cdot 11^1 \cdot 13^1$ | 448 |
| $10^8$ | 73513440 | $2^5 \cdot 3^3 \cdot 5^1 \cdot 7^1 \cdot 11^1 \cdot 13^1 \cdot 17^1$ | 768 |
| $10^9$ | 735134400 | $2^6 \cdot 3^3 \cdot 5^2 \cdot 7^1 \cdot 11^1 \cdot 13^1 \cdot 17^1$ | 1344 |
| $10^{11}$ | 97772875200 | $2^6 \cdot 3^3 \cdot 5^2 \cdot 7^2 \cdot 11^1 \cdot 13^1 \cdot 17^1 \cdot 19^1$ | 4032 |
| $10^{12}$ | 963761198400 | $2^6 \cdot 3^4 \cdot 5^2 \cdot 7^1 \cdot 11^1 \cdot 13^1 \cdot 17^1 \cdot 19^1 \cdot 23^1$ | 6720 |
| $10^{15}$ | 866421317361600 | $2^6 \cdot 3^4 \cdot 5^2 \cdot 7^1 \cdot 11^1 \cdot 13^1 \cdot 17^1 \cdot 19^1 \cdot 23^1 \cdot 29^1 \cdot 31^1$ | 26880 |
| $10^{18}$ | 897612484786617600 | $2^8 \cdot 3^4 \cdot 5^2 \cdot 7^2 \cdot 11^1 \cdot 13^1 \cdot 17^1 \cdot 19^1 \cdot 23^1 \cdot 29^1 \cdot 31^1 \cdot 37^1$ | 103680 |

<span style="color:gray">Исходный код таблицы в $\LaTeX$ доступен по [ссылке](https://pastebin.com/T9HzdZSD)</span>

По первому столбцу можно понять ограничения, по второму столбцу можно увидеть наименьшее число с наибольшим количеством делителей, потом его разложение на простые, и последний столбец, наконец, говорит нам то самое количество делителей. Если у вас нет этой таблицы, ее можно, к примеру, получить по двум последовательностям OEIS: [oeis.org/A066150](oeis.org/A066150), [oeis.org/A066151](http://oeis.org/A066151).

## Численный анализ оценки в кубический корень

Для точности рекомендуется пользоваться этой таблицей и не полагаться на оценку в кубический корень, однако если этой таблички нет под рукой, то эта оценка даст вам примерное представление о максимальном количестве делителей.

Чтобы лучше понимать эту оценку, давайте проанализируем графики:

![test](/images/highly_composite_plot_1.svg)


На данном графике на оси $X$ отложены десятичные логарифмы $n$ (то есть количество цифр в числе $n$), красный график — это кубический корень, а синий график — это максимальное количество делителей среди чисел, меньших $n$ (обратите внимание, что все числа по оси $Y$ умножаются на миллион). Из-за экспоненциального роста не видно, что происходит для чисел, меньших $10^{11}$, но и так видно, что для больших $n$ кубический корень уходит сильно выше, но все равно отношение не больше $10$ для обозримых чисел.

Давайте ограничим наш график на числа, меньшие $10^{12}$, чтобы посмотреть внимательнее на эту часть графика:

![test](/images/highly_composite_plot_2.svg)


Видно, что для маленьких чисел количество делителей все таки больше кубического корня, а где-то в районе между $10^{10}$ и $10^{11}$ эти графики меняются местами и уже никогда не встречаются вновь. Как я говорил раньше, сильный всплеск отношения между этими графиками происходит уже на очень больших числах, и для $n \le 10^{15}$ отношение между этими величинами не превосходит $4$.

Но если мы смотрим на такие графики, то мы видим только то, что происходит для больших чисел, потому что все маленькие величины становятся просто точкой. Для того, чтобы так не происходило, прологарифмируем также и ось $Y$:

![test](/images/highly_composite_plot_3.svg)


Теперь происходящее стало еще понятнее.

## Более точная практическая оценка

Как мы уже говорили раньше, оценка в кубический корень очень проста и дает правильный порядок величин, когда нет доступа к таблице. Кроме того, кубический корень достаточно легко вычислить даже без компьютера. Однако если у вас нет таблицы, но при этом есть компьютер, я рекомендую вам другую очень точную оценку на максимальное количество делителей: $\exp((\ln \ln n)^{1.8})$.

![test](/images/highly_composite_plot_4.svg)


Посмотрите, как удивительно точно следуют друг за другом зеленый и синий графики! До $10^{12}$ они идут настолько ровно рядом, что пропадает проблема с тем, что кубический корень достаточно сильно меньше $d(n)$ для малых $n$.

Наша новая оценка отличается от реального значения не больше, чем в $1.5$ раза для $n \le 10^{14}$, и не больше, чем в $2.5$ раза для $n \le 10^{18}$.

Еще одним удивительным фактом является то, что все три графика пересекаются примерно в одном и том же месте между $10^{11}$ и $10^{12}$.

