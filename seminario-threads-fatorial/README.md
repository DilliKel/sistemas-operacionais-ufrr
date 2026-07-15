# Seminário — Threads e Cálculo de Fatorial

Seminário de Sistemas Operacionais sobre **multithreading com pthreads**, usando o cálculo do fatorial de números grandes como estudo de caso para comparar execução sequencial vs. paralela.

**Autores:** Kelvin Araújo Ferreira e Natália Almada
**Instituição:** UFRR — Universidade Federal de Roraima

## Sobre

O fatorial de números grandes (ex.: 100.000!) estoura os tipos numéricos nativos (`int`, `long`, `double`), então o resultado precisa ser representado como um vetor de dígitos, com a multiplicação feita "na mão" — algarismo por algarismo, com *carry* — exatamente como se faz multiplicação de números grandes no papel. Esse problema é usado como base para demonstrar a criação, sincronização e junção (`join`) de threads em C com a biblioteca `pthread`.

## Código

| Arquivo | Descrição |
|---|---|
| [`codigo/sem-thread-fatorial.c`](./codigo/sem-thread-fatorial.c) | Calcula o fatorial de forma sequencial (sem threads). Representa o resultado como um vetor de dígitos (`buffer[]`) e implementa a multiplicação grande manualmente. Serve de baseline de comparação. |
| [`codigo/thread-fatorial.c`](./codigo/thread-fatorial.c) | Mesma lógica, mas paralelizada com `pthread`: divide o intervalo de multiplicações (2 até N) em blocos de 1000 entre várias threads, sincronizadas por um `pthread_mutex_t`. |
| [`codigo/zeros-fatorial.c`](./codigo/zeros-fatorial.c) | Programa auxiliar que calcula o fatorial (via `double` e recursão, com limite de precisão) e conta quantos zeros à direita o resultado tem — equivalente a contar quantos fatores de 5 existem em 1×2×...×N. |

### ⚠️ Nota técnica sobre `thread-fatorial.c`

O `pthread_mutex_lock` na função `threadCalc` engloba o laço inteiro de cada thread, então, na prática, apenas uma thread executa a região crítica por vez — o cálculo acaba sendo serializado, sem ganho real de paralelismo. Isso é esperado: a multiplicação do fatorial é inerentemente sequencial (cada passo depende do resultado do passo anterior), então o objetivo do código é didático — demonstrar a mecânica de criação de threads, mutex e `join` — não obter um fatorial mais rápido.

## Capturas e demonstração

- [`capturas/esquema-whimsical.png`](./capturas/esquema-whimsical.png) — esquema da lógica do algoritmo/arquitetura de threads, feito no Whimsical.
- [`capturas/teste-com-threads.png`](./capturas/teste-com-threads.png) e [`capturas/teste-fatorial-com-threads.png`](./capturas/teste-fatorial-com-threads.png) — execução da versão com threads.
- [`capturas/teste-zeros-fatorial.png`](./capturas/teste-zeros-fatorial.png) — execução do `zeros-fatorial.c`.
- [`capturas/teste-sem-thread-100000-fatorial.mp4`](./capturas/teste-sem-thread-100000-fatorial.mp4) — vídeo de demonstração do cálculo de 100.000! na versão sem threads.

## Slides

[`slides-seminario.pdf`](./slides-seminario.pdf) — apresentação usada no seminário.

## Como compilar e executar

```bash
# Versão sem threads
gcc codigo/sem-thread-fatorial.c -o sem-thread-fatorial
./sem-thread-fatorial

# Versão com threads (requer linkar a pthread)
gcc codigo/thread-fatorial.c -o thread-fatorial -lpthread
./thread-fatorial

# Contador de zeros do fatorial
gcc codigo/zeros-fatorial.c -o zeros-fatorial -lm
./zeros-fatorial
```
