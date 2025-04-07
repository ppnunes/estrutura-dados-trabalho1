#include "processo.h"

int main()
{
    int N = 0;

    Processo* X = abreArquivo ("processo_043_202409032338.csv", &N);
    printf("%d\n", X[0].id);
    printf("%d\n", X[N-1].id);

    return 0;
}
