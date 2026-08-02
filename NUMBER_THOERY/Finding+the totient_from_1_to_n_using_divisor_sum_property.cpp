void phi_1_to_n(int n) {
    vector<int> phi(n + 1);
    phi[0] = 0;
    phi[1] = 1;
 //  phi[x] = x - Σ(phi(proper divisors of x))
    for (int i = 2; i <= n; i++)
        phi[i] = i - 1;//one sabka devisor isliyr phle hi minus kr diya

    for (int i = 2; i <= n; i++)
        for (int j = 2 * i; j <= n; j += i)//i jitna ko bhi devisor hai sabse minu kr do
              phi[j] -= phi[i];
}

/*
