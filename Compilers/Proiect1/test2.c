#include <stdio.h>

int main() {
    // Declarații variabile
    int x = 10, y = 20;
    float pi = 3.14159;
    char ch = 'A';
    char str[] = "Hello, World!\n";

    /* Operații aritmetice
       pe mai multe linii */
    int sum = x + y;
    int diff = x - y;
    float result = pi * 2.0;

    // Operatori compuși
    x += 5;
    y--;

    // Comparații
    if (x == 15 && y < 20) {
        printf("Conditie adevarata\n");
    }

    // Loop
    for (int i = 0; i < 10; i++) {
        sum += i;
    }

    // Eroare intentionată: caracter ilegal
    int val = 100 @ 50;

    // String neînchis
    char* msg = "Acesta este un mesaj

    return 0;
}