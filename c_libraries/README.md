# C_Header

In diesem Ordner befindet sich in Gesamtheit eine C-Bibliothek mit Headern. Diese Bibliothek besteht aus dem t_programcontrol-Anteil. In diese Teilbibliothek sind Funktionen und Deklarationen ausgelagert, die In- und Output und Memory-Verwaltung betreffen wie Fortschrittsbalken oder das Einfärben von Text in der Konsole.
Der zweite Anteil t_numerics beinhaltet allerhand numerische Algorithmen zum Lösen von Integralen, ODEs der algebraischen Gleichungen.
Beide zusammen bilden die tlibrary, in welche die Object-Dateien von t_numerics und t_programcontrol als Shared Library gelinked wurden.

Welche Funktionalitäten die Bibliothek genau enthält, findet man am besten heraus, wenn man die Header-Dateien durchforstet. Darin befindet ist über jeder Funktion beschrieben, wie man die verwendet.

Wie man die Shared Library mit der Binary eines Programmes linked und während der Laufzeit verfügbar macht, ist im nächsten Abschnitt erklärt.

## Verwendung der tlibrary

In dem Schritt, wo man das Hauptprogramm `main.c` zu einer Executable kompiliert und externe Bibliotheken linkt, kann man auch die C-Library in Form einer Shared Library linken. Für die C-Bibliothek sollte das der bevorzugte Weg sein.

### Für den Fall, dass man auf dem System, auf dem man arbeitet, keine Adminrechte hat

```bash
gcc -L/PATH -o test main.c -ltlib -flto -lm
```

wobei `PATH` den Pfad angibt, in dem die Datei `libtlib.so` liegt. Die Flag `-ltlib` (link tlibrary)
bewirkt, dass gcc im hinterlegten `PATH` nach der Datei `libtlib.so` zum Linken sucht.
Der Name der .so-Datei (shared library) ist so merkwürding, weil gcc davon ausgeht, dass der Name aller libraries mit "lib" beginnen.

Anschließend muss man nur noch die Variable LD_LIBRARY_PATH auf `PATH` setzen. Vorher sollte man allerdings anschauen, ob diese bereits belegt ist mit 

```bash
echo $LD_LIBRARY_PATH
```
 und dann erst definieren mit

```bash
LD_LIBRARY_PATH=/home/username/foo:$LD_LIBRARY_PATH
```

in bash und

```bash
set LD_LIBRARY_PATH PATH
```

in fish, damit das System die Library auf während der Runtime findet, und an eine Umgebungsvariable exportieren mit

```bash
export LD_LIBRARY_PATH=PATH:$LD_LIBRARY_PATH
```
 Jetzt kann man die Executable wie gewohnt ausführen mit
 ```bash
 ./test
 ```

 **Bemerkung**: Auf MacOS heißt die Variable, in welcher man den Pfad, in dem die Bibliothek liegt, angibt: `DYLD_LIBRARY_PATH`.

### Mit Admin-Rechten

Besitzt man hingegen Admin-Rechte, kann man die Library in den Ordner `/usr/lib` legen mit

```bash
sudo cp PATH/libtlib.so /usr/lib
```
und anschließend die Berechtigung der Datei ändern mit

```bash
sudo chmod 0755 /usr/lib/libtlib.so
```

GCC und das System während der runtime finden dort die Library automatisch.

Die Berechtigung nach dem Code 0755 lauten wie folgt:  
0 --> no special permission  
7 --> full permissions (read, write, execute) for the owner of the file  
5 --> read and execute permissions for the group  
5 --> read and execute permissions for other users

**Bemerkung**: In MacOS darf man keine Datei in den `/usr/lib`-Ordner legen, auch nicht mit sudo access. Hier muss man die erste Methode wählen.

Zum Schluss muss man noch den Cache updaten mit

```bash
ldconfig
```

In dem Ordner "config" dieses Repositorys findet ihr ein fish-Skript unter dem Namen "sudo_update_libtlib.fish", dass diesen Prozess automatisiert. Ihr müsst lediglich dort, wo `PATH` steht, den Pfad einsetzen, wo dieses Repository liegt.