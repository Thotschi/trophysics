# Tipps und Tricks für C

## Header einbinden

Header in C bestehen meist aus einer Header-Datei mit Endung `.h` und einer `.c`-Datei. In der eigentlichen Header-Datei sind Definitionen und Deklarationen. Die wird im Programm mit

```C
#import "PATH/header.h"
```

eingebunden. Die zugehörige `.c`-Datei wird zu einer `.o`-Datei (Object Datei) kompiliert:

```bash
gcc -c c-library.c -o c-library.o
```

 und anschließend bei der Kompilierung des Hauptprogrammes gelinkt:

```bash
gcc -o test main.c c-library.o
```

Gegebenenfalls muss man noch andere Bibliotheken linken wie die Lapacke-Bibliothek.


Wie genau das alles funktioniert, kann man sehr gut in der *makefile* nachvollziehen.

## Shared Libraries mit GCC

Mit diesem Guide kann man shared Libraries, die den Namen libfoo.so oder libfoo.a tragen, linken mit

```bash
gcc -Wall -o test main.c -lfoo
```

wenn man Adminrechte hat. Ansonsten wird es minimal umständlicher. Das ist aber immer noch wesentlich kürzer, als der in der makefile in templates beschriebene Weg, und ziemlich elegant.

Auf diese Weise kann man auch spontan Libraries verlinken, ohne sich einen großen Block in die makefile zu bauen.

Statt die notwendigen Schritte hier zu beschreiben, verlinke ich einfach eine Quelle, da ich es besser nicht schreiben könnte.

https://www.cprogramming.com/tutorial/shared-libraries-linux-gcc.html

**Bemerkung**: sehr wichtig!!! Damit Linux die .so-Datei als ELF-Datei erkennt und lesen kann, müssen die einzelnen .c-Dateien,
die zur .so-Datei gelinkt werden, mit der Flag `-fPIC` kompiliert werden!!!

**Bemerkung**: Auf MacOS heißt die Variable, in welcher man den Pfad, in dem die Bibliothek liegt, angibt: `DYLD_LIBRARY_PATH`.

## Warning: unused parameter

Man kann eine Funktion mit einer Variable deklarieren, die aber von der Funktion nicht verwendet wird. Das kann sinnvoll sein, um z.B. eine Typendefinition des Funktionszeigers zu erfüllen, wenn die Funktion als Argument übergeben wird. Bei einem ungenutzten Argument in einer Funktion wird vom Compiler folgende Warnung ausgegeben:

```bash
warning: unused parameter ‘fooparam’ [-Wunused-parameter]
```

Diese kann man umgehen, in dem in der Funktionsdeklaration bei der entsprechenden Variable, hier: `fooparam` vom Typ `footype`, folgenden Cast macht:

```C
footype __attribute__((unused)) fooparam
```

im Gegensatz zu 

```C
footype fooparam
```

was zur besagten Warnung führen würde.


## Funktion mit optionalen Argumenten, die - wenn sie nicht übergeben werden - einen default-Wert haben

hier für eine Funktion mit Rückgabewert double. Funktioniert natürlich auch für void-Funktionen usw.

```c
// Struktur mit Argumenten von f
typedef struct {
  double a;
  int b;
} f_args;

// ersetzt call der Funktion f durch initializer-Funktion var_f und castet Argumente in die Struktur f_args
#define f(...) var_f((f_args){__VA_ARGS__})

// wenn Argumente von var_f in Struktur nicht belegt sind, werden die default-Werte eingesetzt und base-Funktion ausgeführt
double var_f(f_args in){
  double a_out = in.a ? in.a : 3.142;
  int b_out = in.b ? in.b : 8;
  return f_base(a_out, b_out);
}

// eigentliche Funktion f, die das tut, was man will, wenn man f(a, b) eingibt
double f_base(double a, int b) {
  return a * b;
}
```

Mögliche Calls der Funktion sind dann:

```c
f(3, 10);     // a=3, b=10
f(.a=2.3, 1); // a=2.3, b=1
f(2);         // a=2, b=8
f(.b=5);      // a=3.142, b=5
```

## String Literal

Wenn man einen Character-Pointer mit einen String initialisiert:

```C
  char *test_str = "Test";
```

wird der String vom Compiler in ein konstantes Character-Array geschrieben:

```C
const char string_literal[] = "Test";
```

und der Pointer auf den ersten Eintrag des Strings
in den Pointer `test_str*` geschrieben. Da das String Literal aber ein konstantes Aray ist, kann man keine Buchstaben ändern durch

```C
test_str[2] = 'x';
// führt zu Error
```


## Strukturen mit malloc allokieren

Man kann auch mit malloc Strukturen allokieren und damit ihren Speicher dynamisch verwalten. Dabei ist C überraschend flexibel und denkt unerwartet viel für einen mit. Strukturen dynamisch zu verwalten, ist z.B. essenziell für Hashmaps, in denen ein Eintrag der Hashmap aus einem key-value-Paar besteht, das als Struktur formalisiert wird. Da man in Hashmaps Einträge hinzufügen, aber auch wieder löschen will, muss man den verwendeten Speicher der key-value-Paare entsprechend dynamisch verwalten können.

Unterhalb betrachten wir das ganze am Beispiel einer Bankkontos. Die Signatur der Struktur ist folgende:

```C
typedef struct {
  char *name;
  double balance;
  char iban[];     // String auf Stack
} account;
```

Die Einträge der Struktur heißen Member. Weil wir ja eigentlich die Struktur dynamisch allokieren wollen, ist es paradox, den String `iban` auf dem Stack zu speichern. Es geht, ist aber eben kontraproduktiv. Besser ist es, den Pointer auf das Charakter-Array als Member einzuführen, wie für die `name` gezeigt.

Die Speicherzuweisung in der `main`-Funktion sieht dann wie folgt aus:

```C
  // Allokation auf Heap
  account *client1 = (account *) malloc(sizeof(account));
  // Zuweisung Werte
  client1->balance = 1037.81;
```
Der `->` Operator ist dabei eine Kurzschreibweise für

 ```C
(*client1).balance == client1->balance
 ```

wobei `*` der Dereferenzierungsoperator ist. Man beachte, dass C `account*` direkt als Pointer auf die Members der Struktur erkennt und selbstständig den benötigten Speicherplatz durch `sizeof(account)` berechnen kann.

Weiter geht es dann mit

```C
  /* Pointer des Strings wird zu Pointer eines Strings mit 15 Charaktern gecasted.
   * Muss nicht gefreed werden! */
  client1->name = (char *) malloc(15 * sizeof(char));
  strcpy(client1->name, "Erika Mustermann");

  printf("%s has %g € in her account.\n", client1->name, client1->balance);
  free(client1);
```

Dafür kann man natürlich auf dem Stack eine Struktur initialisieren durch

```C
struct account client2 = {"Max Mustermann", 593.12, "DE31415926535897932384"}
```

Hier wird die IBAN aber als String-Literal gespeichert.