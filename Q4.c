/*
    Questao 4
    Aluno: Diogo Balbino Ferreira
    Matricula: 221008570
    Data: 17/10/2025
*/

#include <stdio.h>
#ifdef _WIN32
#include <windows.h>
#endif

// Calcula o MDC e exibe os passos do processo.
int mdcComPassos(int a, int temp) {
    int r;
    // O loop continua ate o resto da divisao ser zero.
    while (temp != 0) {
        r = a % temp;
        printf("Algoritmo de Euclides: %d mod %d = %d\n", a, temp, r);
        a = temp;
        temp = r; // Atualiza os valores para a proxima iteracao.
    }
    return a;
}

// Calcula o inverso modular de 'a' em relacao ao modulo 'm'.
int inversoModular(int a, int m) {
    int m0 = m, t, q;
    int x0 = 0;
    int x1 = 1;
    int A = a, B = m;

    // Um pre-requisito para o inverso existir e que mdc(a, m) seja 1.
    if (mdcComPassos(a, m) != 1) {
        printf("Nao existe inverso de %d modulo %d.\n", a, m);
        return -1; // Retorna -1 para indicar erro.
    }

    // Nucleo do Algoritmo de Euclides Estendido.
    while (m != 0) {
        q = a / m;
        t = m;      m = a % m;      a = t;
        t = x0;     x0 = x1 - q * x0;   x1 = t;
    }

    // Garante que o resultado final seja um numero positivo.
    if (x1 < 0)
        x1 += m0;

    printf("\nSubstituindo, temos que o inverso de %d em %d é %d.\n\n", A, B, x1);
    return x1;
}

// Calcula (base^exp) % mod de forma otimizada.
int powMod(int base, int exp, int mod) {
    long long res = 1; // O resultado comeca em 1.
    long long b = base % mod;
    while (exp > 0) {
        // Se o expoente atual e impar, multiplica o resultado pela base.
        if (exp % 2 == 1)
            res = (res * b) % mod;
        
        b = (b * b) % mod; // Eleva a base ao quadrado.
        exp >>= 1;         // Divide o expoente por 2.
    }
    return (int)res;
}

int main() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8); // Para acentuacao no Windows.
#endif

    int H, G, Zn, x, n1;

    // Leitura dos dados de entrada.
    printf("Insira H: ");
    scanf("%d", &H);
    printf("Insira G: ");
    scanf("%d", &G);
    printf("Insira Zn: ");
    scanf("%d", &Zn);
    printf("Insira x: ");
    scanf("%d", &x);
    printf("Insira n1: ");
    scanf("%d", &n1);
    printf("\n");

    // Etapa 1: Calcular a divisao modular (H / G).
    int inverso = inversoModular(G, Zn);
    if (inverso == -1) return 0; // Encerra se o inverso nao existir.
    
    // O resultado da divisao e 'a'.
    int a = (H * inverso) % Zn;

    printf("Fazendo a multiplicacao modular: %d * %d mod %d = %d\n", H, inverso, Zn, a);
    printf("  Sendo %d o inverso de %d.\n", inverso, G);

    // Etapa 2: Calcular a exponenciacao modular de 'a'.
    int resultado = powMod(a, x, n1);
    printf("Valor final da congruencia: %d\n", resultado);

    return 0;
}

/*

Para H = 7, G = 3, Zn = 11, x = 10 e n1 = 13
O resultado final será = 4

PARTE 2: V ou F

(V) O algoritmo de Euclides estendido é utilizado para calcular o inverso modular de um número.
(F) Se mdc(G, Zn) ≠ 1, o programa ainda consegue encontrar o inverso de G em Zn.
(V) A operação (H * inverso) % Zn representa a divisão modular de H por G.
(V) Se n1 for primo, o código aplica o Pequeno Teorema de Fermat para simplificar o cálculo de a^x mod n1.
(F) A função powMod implementa o cálculo de potência modular utilizando multiplicações diretas sem otimização.
(V) Quando o resultado do inverso é negativo, o código ajusta o valor somando o módulo m0.
(V) O cálculo de fi(n1) (função totiente de Euler) é utilizado apenas quando n1 não é primo.

*/