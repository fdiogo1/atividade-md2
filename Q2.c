/*
    Questao 2
    Aluno: Diogo Balbino Ferreira
    Matricula: 221008570
    Data: 17/10/2025
*/

#include <stdio.h>
#include <stdlib.h>


static void printList(const int *v, int n) {
    for (int i = 0; i < n; i++) {
        if (i) printf(" ");
        printf("%d", v[i]);
    }
    printf("\n");
}

// MDC 
static int greatestCommonDivisorVerbose(int a, int b, int *passo) {
    (void)passo; 
    int a_original = a, b_original = b;
    printf("    Calculando mdc(%d, %d):\n", a, b);
    int iter = 0;
    while (b != 0) {
        int resto = a % b;
        int quoc = a / b;
        iter++;
        printf("      [%d] %d = (%d * %d) + %d\n", iter, a, quoc, b, resto);
        a = b;
        b = resto;
    }
    printf("    Resultado: mdc(%d, %d) = %d\n", a_original, b_original, a);
    return a;
}

// MMC usando o MDC calculado acima
static int leastCommonMultipleVerbose(int a, int b, int *passo) {
    printf("Passo %d: Calculo de MMC(%d,%d) usando a*b/MDC(a,b)\n", (*passo)++, a, b);
    long long prod = (long long)a * (long long)b;
    printf("  - Produto %d * %d = %lld\n", a, b, prod);
    int g = greatestCommonDivisorVerbose(a, b, passo);
    long long l = prod / g;
    printf("  - MMC(%d,%d) = %d*%d / %d = %lld\n", a, b, a, b, g, l);
    return (int)l;
}


static void clearUntilEndOfLine(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
    }
}


static int readIntegerWithLabel(const char *label, int min, int max) {
    int valor;
    for (;;) {
        printf("%s", label);
        int ok = scanf("%d", &valor);
        if (ok != 1) {
            printf("  - Entrada invalida.  [%d,%d].\n", min, max);
            clearUntilEndOfLine();
            continue;
        }
        if (valor < min || valor > max) {
            printf("  - Valor fora do limite. Digite um inteiro no intervalo [%d,%d].\n", min, max);
            clearUntilEndOfLine();
            continue;
        }
    
        clearUntilEndOfLine();
        return valor;
    }
}

int main(void) {
    int N;
    int ciclos[10]; // limite: 1 <= N <= 10
    int passo = 1;  // numeracao das etapas

    printf("Passo %d: Leitura do numero de chaves N (1..10)\n", passo++);
    N = readIntegerWithLabel("Digite o numero de chaves N (1..10): ", 1, 10);

    printf("Passo %d: Leitura dos %d ciclos (cada Ci em 2..20)\n", passo++, N);
    for (int i = 0; i < N; i++) {
        char rotulo[64];
        snprintf(rotulo, sizeof(rotulo), "Digite C%d (2..20): ", i + 1);
        ciclos[i] = readIntegerWithLabel(rotulo, 2, 20);
    }

    printf("Passo %d: Confirmacao dos ciclos informados\n", passo++);
    printf("  - Ciclos informados: ");
    printList(ciclos, N);

    // Calculo do MMC 
    printf("Passo %d: Inicializacao do MMC parcial com C1 = %d\n", passo++, ciclos[0]);
    int mmc_total = ciclos[0];

    for (int i = 1; i < N; i++) {
        printf("Passo %d: Combinando MMC parcial (%d) com C%d = %d\n", passo++, mmc_total, i+1, ciclos[i]);
        mmc_total = leastCommonMultipleVerbose(mmc_total, ciclos[i], &passo);
        printf("  - MMC parcial apos combinar C%d = %d\n", i+1, mmc_total);
        if (mmc_total > 50) {
            printf("Passo %d: MMC parcial excede 50. Nao ha ano sincronizado no intervalo 1..50.\n", passo++);
            printf("Conclusao: Nao e possivel sincronizar as chaves no intervalo de 1 a 50 anos.\n");
            return 0;
        }
    }

    // Verificacao final e conclusao
    printf("Passo %d: Verificacao do resultado final X = %d\n", passo++, mmc_total);
    int ok = 1;
    for (int i = 0; i < N; i++) {
        int r = mmc_total % ciclos[i];
        printf("  - Verificando: %d mod %d = %d%s\n", mmc_total, ciclos[i], r, (r==0?" (ok)":" (falha)"));
        if (r != 0) ok = 0;
    }

    if (mmc_total > 0 && mmc_total <= 50 && ok) {
        printf("Conclusao: Primeiro ano sincronizado: %d\n", mmc_total);
    } else {
        printf("Conclusao: Nao e possivel sincronizar as chaves no intervalo de 1 a 50 anos.\n");
    }

    return 0;
}