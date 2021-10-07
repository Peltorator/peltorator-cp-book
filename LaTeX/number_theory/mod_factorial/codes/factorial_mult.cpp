\begin{code}
int factorial_power(int n, int p) {
    int power = 0;
    while (n > 0) {
        n /= p;
        power += n;
    }
    return power;
}
\end{code}
