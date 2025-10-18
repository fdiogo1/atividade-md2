/*
    Questao 1
    Aluno: Diogo Balbino Ferreira
    Matricula: 221008570
    Data: 17/10/2025
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

// === Prototipos de Funcoes ===
int encodeChar(char c);
char decodeChar(int code);
long long euclidesMDC(long long a, long long b);
long long modular_mult(long long a, long long b, long long mod); // CORRECAO: Adicionada para seguranca
long long pollardFunction(long long x, long long n);
long long pollardRho(long long n);
long long readNumber(const char *label);
long long modularInverse(long long a, long long m);
long long choosePublicE(long long phi);
long long modularPow(long long base, long long exp, long long mod, long long phi); // CORRECAO: Logica ajustada
void encryptDecrypt(long long E, long long D, long long n, long long phi);
int isPrime(long long n);


// === Pre-codificacao ===
int encodeChar(char c) {
    if (c >= 'A' && c <= 'Z') return 11 + (c - 'A');
    else if (c >= 'a' && c <= 'z') return 11 + (c - 'a');
    else if (c == ' ') return 0;
    else return -1;
}

// === Decodificacao ===
char decodeChar(int code) {
    if (code == 0) return ' ';
    else return 'A' + (code - 11);
}

// === Calculo do MDC (Algoritmo de Euclides) ===
long long euclidesMDC(long long a, long long b) {
    // A impressao foi removida desta funcao para nao poluir a saida da funcao choosePublicE
    long long original_a = a, original_b = b;
    int step = 0;
    while (b != 0) {
        long long remainder = a % b;
        a = b;
        b = remainder;
    }
    return a;
}

// === CORRECAO: Funcao de multiplicacao modular para evitar overflow ===
long long modular_mult(long long a, long long b, long long mod) {
    long long res = 0;
    a %= mod;
    while (b > 0) {
        if (b % 2 == 1) res = (res + a) % mod;
        a = (a * 2) % mod;
        b /= 2;
    }
    return res;
}

// === Funcao g(x) = (x^2 + 1) mod n ===
long long pollardFunction(long long x, long long n) {
    // CORRECAO: Usa a multiplicacao modular segura
    long long res = (modular_mult(x, x, n) + 1) % n;
    printf("     g(%lld) = (%lld^2 + 1) mod %lld = %lld\n", x, x, n, res);
    return res;
}

// === Fatoracao com metodo rho de Pollard ===
long long pollardRho(long long n) {
    if (n % 2 == 0) {
        printf("Como %lld e par, fator = 2.\n", n);
        return 2;
    }

    long long x = 2, y = 2, d = 1;
    int iteration = 0;

    printf("\n--- Iniciando Pollard Rho para N = %lld ---\n", n);

    while (d == 1) {
        iteration++;
        printf("\nIteracao %d:\n", iteration);
        x = pollardFunction(x, n);
        y = pollardFunction(pollardFunction(y, n), n);
        long long diff = llabs(x - y);
        printf("     |x - y| = %lld\n", diff);
        d = euclidesMDC(diff, n);
        printf("     mdc(|x-y|, N) = mdc(%lld, %lld) = %lld\n", diff, n, d);

        if (d == 1)
            printf("     Nenhum fator encontrado nessa iteracao.\n");
        else if (d == n) {
            printf("     Falha: mdc = n. O metodo falhou para os parametros fixos.\n");
            return -1;
        } else
            printf("     Fator encontrado: %lld\n", d);
    }
    return d;
}

// === Solicitar numero composto ===
long long readNumber(const char *label) {
    long long N;
    do {
        printf("Digite %s (produto de primos distintos, 100-9999): ", label);
        if (scanf("%lld", &N) != 1) {
            printf("Entrada invalida.\n");
            while (getchar() != '\n');
            continue;
        }
        if (N < 100 || N > 9999)
            printf("Numero fora do intervalo!\n");
        else break;
    } while (1);
    return N;
}

// === Inverso modular (Algoritmo de Euclides Estendido) ===
long long modularInverse(long long a, long long m) {
    long long m0 = m, t, q;
    long long x0 = 0, x1 = 1;

    printf("\n=== Calculando Inverso Modular de %lld mod %lld ===\n", a, m);
    if (m == 1) return 0;

    while (a > 1) {
        q = a / m;
        t = m;
        m = a % m;
        a = t;
        t = x0;
        x0 = x1 - q * x0;
        x1 = t;
    }

    if (x1 < 0) x1 += m0;

    printf("Inverso encontrado: D=%lld\n", x1);
    return x1;
}

// === Escolher expoente publico E ===
long long choosePublicE(long long phi) {
    printf("\nEscolhendo E coprimo a %lld...\n", phi);
    for (long long e = 2; e < phi; e++)
        if (euclidesMDC(e, phi) == 1) {
            printf("E = %lld e coprimo a %lld. Escolhido.\n", e, phi);
            return e;
        }
    return -1;
}

// === Exponenciacao modular com teoremas (CORRIGIDO) ===
long long modularPow(long long base, long long exp, long long mod, long long phi) {
    printf("\nCalculando %lld^%lld mod %lld\n", base, exp, mod);
    long long reducedExp = exp;

    if (base % mod == 0) return 0;

    // CORRECAO LOGICA: Apenas reduz o expoente se a condicao for satisfeita.
    if (euclidesMDC(base, mod) == 1) {
        printf("-> Aplicando Teorema de Euler (mdc(%lld, %lld) == 1)\n", base, mod);
        reducedExp = exp % phi;
        printf("-> Expoente reduzido para %lld (de %lld)\n", reducedExp, exp);
    } else {
        printf("-> Nao foi possivel aplicar Teorema de Euler (mdc != 1), usando expoente completo.\n");
    }

    long long result = 1;
    long long b = base % mod;
    long long e = reducedExp;

    while (e > 0) {
        if (e % 2 == 1)
            // CORRECAO: Usa multiplicacao modular segura
            result = modular_mult(result, b, mod);
        // CORRECAO: Usa multiplicacao modular segura
        b = modular_mult(b, b, mod);
        e /= 2;
    }

    printf("-> Resultado: %lld\n", result);
    return result;
}

// === Etapa 3: Criptografar e Descriptografar ===
void encryptDecrypt(long long E, long long D, long long n, long long phi) {
    char message[1024];
    printf("\n=== Etapa 3: Criptografia / Descriptografia ===\n");
    printf("Digite a mensagem: ");
    while (getchar() != '\n'); // Limpa o buffer do teclado
    fgets(message, sizeof(message), stdin);
    message[strcspn(message, "\n")] = 0; // Remove a nova linha do fgets

    int blocks[1024];
    char isLower[1024] = {0};
    int length = 0;

    for (int i = 0; message[i] != '\0'; i++) {
        int code = encodeChar(message[i]);
        if (code == -1) continue;
        blocks[length] = code;
        isLower[length] = (message[i] >= 'a' && message[i] <= 'z');
        length++;
    }

    long long encrypted[1024];
    printf("\n--- CRIPTOGRAFANDO ---\n");
    for (int i = 0; i < length; i++) {
        printf("\nCaractere '%c' -> Bloco M=%02d", message[i], blocks[i]);
        encrypted[i] = modularPow(blocks[i], E, n, phi);
    }

    printf("\nMensagem criptografada (blocos numericos):\n");
    for (int i = 0; i < length; i++) printf("%lld ", encrypted[i]);
    printf("\n");

    printf("\n--- DESCRIPTOGRAFANDO ---\n");
    char decrypted[1024];
    for (int i = 0; i < length; i++) {
        printf("\nBloco C=%lld", encrypted[i]);
        long long M = modularPow(encrypted[i], D, n, phi);
        char letter = decodeChar((int)M);
        if (isLower[i]) letter += ('a' - 'A'); // Converte para minuscula se necessario
        decrypted[i] = letter;
    }
    decrypted[length] = '\0';
    printf("\n\nMensagem decodificada: %s\n", decrypted);
}

// === Teste de primalidade ===
int isPrime(long long n) {
    if (n < 2) return 0;
    if (n % 2 == 0) return n == 2;
    for (long long i = 3; i * i <= n; i += 2)
        if (n % i == 0) return 0;
    return 1;
}

// === Programa principal ===
int main() {
    printf("\n=== Etapa 1: Fatoracao (Metodo Pollard-Rho) ===\n");
    printf("Cada numero deve ser produto de primos distintos.\n\n");

    long long N1 = readNumber("N1");
    long long N2 = readNumber("N2");

    long long p = pollardRho(N1);
    if (p == -1) return 1; // Encerra se a fatoracao falhou
    long long otherP = N1 / p;
    if (!isPrime(p) || !isPrime(otherP) || p == otherP) {
        printf("\nN1=%lld invalido: fatores %lld e %lld devem ser primos distintos.\n", N1, p, otherP);
        return 1;
    }

    long long q = pollardRho(N2);
    if (q == -1) return 1; // Encerra se a fatoracao falhou
    long long otherQ = N2 / q;
    if (!isPrime(q) || !isPrime(otherQ) || q == otherQ) {
        printf("\nN2=%lld invalido: fatores %lld e %lld devem ser primos distintos.\n", N2, q, otherQ);
        return 1;
    }

    printf("\n\n=== Fatores primos definidos para o RSA ===\n");
    printf("p = %lld (de N1=%lld)\n", p, N1);
    printf("q = %lld (de N2=%lld)\n", q, N2);

    // Etapa 2 - Geracao das Chaves RSA
    printf("\n=== Etapa 2: Geracao das Chaves RSA ===\n");
    long long n = p * q;
    long long phi = (p - 1) * (q - 1);
    printf("n = %lld, phi(n) = %lld\n", n, phi);

    long long E = choosePublicE(phi);
    long long D = modularInverse(E, phi);
    printf("\nChave publica: (n=%lld, E=%lld)\n", n, E);
    printf("Chave privada: (n=%lld, D=%lld)\n", n, D);

    // Etapa 3 - Criptografia / Descriptografia
    encryptDecrypt(E, D, n, phi);

    return 0;
}