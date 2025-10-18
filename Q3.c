/*
    Questao 3
    Aluno: Diogo Balbino Ferreira
    Matricula: 221008570
    Data: 17/10/2025
*/

#include <stdio.h>
#include <math.h> // Necessario para a funcao sqrt

// Funcao para potenciacao inteira
long long integerPower(int base, int exp) {
    long long result = 1;
    for (int i = 0; i < exp; i++) {
        result *= base;
    }
    return result;
}

// Funcao para encontrar a fatoracao prima de um numero N
// Retorna a quantidade de fatores primos distintos encontrados
int primeFactorization(int n, int primeFactors[], int exponents[]) {
    int factorCount = 0;
    
    // Trata todos os fatores de 2
    int power = 0;
    while (n % 2 == 0) {
        power++;
        n /= 2;
    }
    if (power > 0) {
        primeFactors[factorCount] = 2;
        exponents[factorCount] = power;
        factorCount++;
    }

    // Trata os fatores impares, iterando apenas sobre numeros impares
    for (int i = 3; i <= sqrt(n); i += 2) {
        power = 0;
        while (n % i == 0) {
            power++;
            n /= i;
        }
        if (power > 0) {
            primeFactors[factorCount] = i;
            exponents[factorCount] = power;
            factorCount++;
        }
    }

    // Se n ainda for maior que 2, o numero restante e um fator primo
    if (n > 2) {
        primeFactors[factorCount] = n;
        exponents[factorCount] = 1;
        factorCount++;
    }
    
    return factorCount;
}

// Funcao para calcular tau(N) - a quantidade de divisores
int calculateTau(int exponents[], int factorCount, int N) {
    int tau = 1;
    printf("\nCalculo intermediario de t(%d) (quantidade de divisores):\n", N);
    printf("t(%d) = ", N);
    for (int i = 0; i < factorCount; i++) {
        printf("(%d+1)", exponents[i]);
        tau *= (exponents[i] + 1);
        if (i < factorCount - 1) {
            printf(" * ");
        }
    }
    printf(" = %d\n", tau);
    return tau;
}

// Funcao para calcular sigma(N) - a soma dos divisores
long long calculateSigma(int primeFactors[], int exponents[], int factorCount, int N) {
    long long sigma = 1;
    printf("\nCalculo intermediario de o(%d) (soma dos divisores):\n", N);
    printf("o(%d) = ", N);
    for (int i = 0; i < factorCount; i++) {
        int p = primeFactors[i];
        int a = exponents[i];
        
        long long numerator = integerPower(p, a + 1) - 1;
        long long denominator = p - 1;
        long long term = numerator / denominator;
        
        printf("((%d^%d - 1)/(%d - 1))", p, a + 1, p);
        sigma *= term;
        
        if (i < factorCount - 1) {
            printf(" * ");
        }
    }
    printf(" = %lld\n", sigma);
    return sigma;
}

int main() {
    int N;
    int primeFactors[20], exponents[20];
    int factorCount;
    int tauN;
    long long sigmaN;
    double efficiencyRatio;

    printf("Digite um numero N (entre 1 e 100000): ");
    scanf("%d", &N);

    // Validacao da entrada conforme o limite da atividade (10^5)
    if (N < 1 || N > 100000) {
        printf("Erro: O valor de N deve estar no intervalo [1, 100000].\n");
        return 1;
    }
    
    // Caso especial para N=1
    if (N == 1) {
        printf("\nFatoracao Prima: O numero 1 nao possui fatores primos.\n");
        printf("\nCalculo intermediario de t(1) (quantidade de divisores):\n t(1) = 1\n");
        printf("\nCalculo intermediario de o(1) (soma dos divisores):\n o(1) = 1\n");
        printf("\nRazao de Eficiencia = o(1)/t(1) = 1/1 = 1.00\n");
        return 0;
    }

    factorCount = primeFactorization(N, primeFactors, exponents);

    printf("\nFatores primos de %d:\n", N);
    for (int i = 0; i < factorCount; i++) {
        printf("%d^%d", primeFactors[i], exponents[i]);
        if (i < factorCount - 1) {
            printf(" * ");
        }
    }
    printf("\n");

    tauN = calculateTau(exponents, factorCount, N);
    sigmaN = calculateSigma(primeFactors, exponents, factorCount, N);
    
    efficiencyRatio = (double)sigmaN / tauN;

    printf("\nResultado Final:\n");
    printf("Razao de Eficiencia = o(N)/t(N) = %lld/%d = %.2f\n", sigmaN, tauN, efficiencyRatio);

    return 0;
}