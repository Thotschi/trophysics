# Tipps und Tricks für Python

## Eigene Module schreiben und importieren

### Sloppy

Schreibe an den Anfang des Skriptes

```python
import sys
sys.path.append("PATH")
```

`sys` ist dabei ein Python-package. Für `PATH` muss man nur noch den Pfad angeben, von dem man das eigene Python-Modul einbinden will. Dieser Pfad wird dann der Liste angefügt, wo der Befehl `import` nach Modulen sucht. Anschließend kann man wie
gehabt

```python
import header_name
import header_name as abbreviation
from header_name import function1, function2, ...
from header_name import *
```

verwenden.

### Profund

`__init__`-Dateien, pyproject.toml, wheel

## kwargs (keyword arguments)
kwargs wird dazu benutzt, *optionale* Parameter an eine Funktion zu übergeben, die - wenn sie nicht übergeben werden, einen Default besitzen. kwargs sind in Python integriert.

kwargs ist die Abkürzung für „keyword arguments“. Es handelt sich um ein Dictionary, das an eine Funktion übergeben wird. Ein Dictionary wiederrum ist die Bezeichnung für die Variante einer Hash-Map, die durch Standard Python bereitgestellt wird. Diese
ist eine Sammlung von key-value-Paaren.

Der key ist ein string, mit dem man den zugehörigen value im Dictionary abrufen kann, der ein beliebiges Objekt in Python
sein kann.

In der Definition der Funktion schreibt man einfach als Argument

```python
functionname (**kwargs)
```

ein.
Die Methode
```python
kwargs.get("key", default)
```
ermöglicht das Abrufen eines value aus dem Dictionary. Falls der key nicht im Dictionary vorhanden ist, wird der Standardwert zurückgegeben. Andernfalls kann man den Wert mit 
`value = dict_name["key"]` wie aus einem gewöhnlichen Dictionary
abrufen.

Im Call der Funktion ist es dann optional, ein key-value-Paar zu übergeben. Dies geschieht mit

```python
function_name(key = value).
```

Falls kein key-value-Paar übergeben wird, wird folgend der Standardwert verwendet.
Offensichtlich ist die Reihenfolge der key-value-Paare, in der diese an die Funktion übergeben
werden, nicht von Bedeutung.

## Klassen

### Was ist das und wie funkioniert es?

Klassen sind ein sehr cooles Inbuilt-Feature von Python. Diese erlauben es im Wesentlichen, Objekte mit eigenen Operatoren und Attributen, sowie designierten Methoden zu konstruieren. Klingt kompliziert, man merkt aber schnell, wie nützlich das ist.

Eine Klasse wird definiert mit dem Keyword `class`.

Jede Klasse, die auch ein Objekt erstellt (man auch auch Klassen kreieren, die nur Makros enthalten) besitzen die `__init__()`Funktion, die ebenso built-in ist. Diese initialisiert ein Objekt der Klasse, indem es den Attributen des Objektes Werte zuweist.

Ein Beispiel könnte so aussehen: 

```python
class vec:
    def __init__(self, x: float, y: float):
        self.x = float(x)
        self.y = float(y)
```

Der Parameter `self` verweist dabei auf das aktuelle Objekt (die Instanz der Klasse). Es ist notwendig, damit Methoden auf die Attribute und andere Methoden der Instanz zugreifen können.

Daher ist er **immer** das erste Argument einer Methode. Soll die Methode einfach ein anderes Objekt derselben Klasse callen, ist es gängig, als weiteres Argument `other` zu übergeben.

Im Code können Attribute des Objektes aufgerufen und verändert werden wie folgt

```python
v = vec (0,0)
v.x = 5
print(v.x)
```

was an Strukturen in C erinnert.

Eine Instanz kann mit

```python
del v
```

gelöscht werden.

### Operatoren

Das eigentlich coole an Instanzen ist nun, dass man Operatoren wie + und * umdefinieren kann, wie diese auf Objekte der Klassen wirken sollen. Im Falle von Vektoren ginge das, indem man in der Klasse die Methode

```python
def __add__(self,other):
    return vec(self.x + other.x, self.y + other.y)
```

hinzufügt. Hier folgt eine unvollständige Liste mit Operatoren, die auf diese Weise umdefiniert werden können:

- `__add__` --> objekt1 + object2
- `__sub__` --> object1 - object2
- `__mul__` --> objekt1 * object2
- `__truediv__` --> objekt1 / object2
- `__floordiv__` --> objekt1 // objekt2

Objekt 1 und 2 können auch verschiedenen Klassen angehören. So könnte 

```python
def __mul__(self, a):
    a = float(a)
    if isinstance(a, (int, float)):
        return Vec(self.x * a, self.y * a)
    raise TypeError("Multiplication only supported with scalars.")
```

auch die Skalarmmultiplikation definieren.

### die __str__() Methode

Die `__str__()`-Methode steuert, was zurückgegeben werden soll, wenn die Instanz als String repräsentiert werden soll. Also z.B.

```python
# in Definition der Klasse:
    def __str__(self):
        return f"vec_x: {self.x}, vec_y: {self.y}"

v = vec(3,1)
print(v)
# --> vec_x: 3, vec_y: 1
```

## Datei nur dann erstellen, wenn sie noch nicht existiert. Sonst aufrufen

Untenstehender Code-Block ist eine Vorlage dafür, eine Datei zu
erstellen, wenn sie noch nicht existiert und sie lediglich zu
öffnen, wenn sie schon existiert.

```python
import os
import errno
import pandas as pd


flags = os.O_CREAT | os.O_EXCL | os.O_WRONLY
# give name of file you want to open/create
filename = "testname"

try:
    file_handle = os.open(filename, flags)
except OSError as e:
    if e.errno == errno.EEXIST:  # Failed as file already exists.
        # code block that edits existing file, pass as place holder
        pass
    else:  # Something unexpected went wrong so reraise the exception.
        raise
else:  # No exception, so the file must have been created successfully.
    # Using `os.fdopen` converts the handle to an open file object
    with os.fdopen(file_handle, 'w') as file_obj:
        # Write text to the file.
        file_obj.write("Hello World!\n")
```

## Python-Module kompilieren

Es gibt die Möglichkeit, Python-Module zur shared library oder Programme zu einer binary zu kompilieren; entweder um sie effizienter zu machen oder den Source-Code zu verstecken. Je nach Anwendungsfall eignen sich verschiedene Methoden, das zu tun. Hier betrachte ich nur diee Kompilierung mit nuitka und Cython.

### Nuitka

Die Kompilierung mit Nuitka dauert zwar sehr lange, bietet allerdings auch hohen Schutz (keineBytecode-Zugänglichkeit).
Nuitka kann man über homebrew oder mit pip installieren.

Die Documentation für Use Cases findet man unter https://nuitka.net/user-documentation/use-cases.html

Der Befehl, um ein Modul zu kompilieren, lautet beispielsweise

```bash
python -m nuitka --module my_module.py
```

Daraus erhält man eine .so-Datei (shared library).

Anschließend kann man diese .so-Datei als Modul wie zu Beginn dieser Wiki-Seite erklärt importieren und benutzen.

#### Externe Pakete in der kompilierten Library

Wenn man in die Datei `my_module.py` andere Packages importiert, wird die Abhängigkeit in der shared library von diesen Packages gespeichert und die Module werden erst imporiert, wenn man das Hauptskript, wie die shared library importiert wird, ausführt.

In Nuitka gibt es auch die Möglichkeit, mit der Flag `--include-package=some_package` oder `--include-module=some_module` die Module direkt zu importieren. Im Gegensatz zu cython werden allerdings die Abhängigkeiten nicht mitinstalliert, was den Kompilierungsprozess lästig macht, da man diese Konfiguration hinzufügen muss. Ein Guide findet ihr hier: https://nuitka.net/user-documentation/nuitka-package-config.html

### Cython

Noch cooler als eine Python-Library zu kompilieren, ist es, ein Python-Modul in C zu implementieren, wodurch man z.B. native C-Bibliotheken in Python verwenden kann.


