# Befehle und Nützliches im Terminal für Betriebssystem und Shell

- Jupyter Notebook mit zusätzlichen Rechten starten, um Fehler bei LaTeX-Kompilierung vorzubeugen: `sudo jupyter notebook --allow-root`

Es sei erwähnt, dass es potenziell unsicher sein kann, Jupyter Notebooks mit sudo-access zu starten.

## GitHub Copilot in Command Line

Studierende haben kostenlosen Zugriff auf GitHub Pro, was unter anderem auch die Verwendung des Copilot in IDE, aber eben auch in der Command Line erlaubt. Damit kann man Hilfe für generische Shell- und Git-Befehle direkt über das Terminal bekommen.

Alle Infos zur Installation und Verwendung zu Copilot CLI findet ihr auf der [Github Page](https://docs.github.com/de/copilot/using-github-copilot/using-github-copilot-in-the-command-line).

## Shell Prompts

Shell Prompts sind die Zeilen im Terminal, in denen man Befehle eingeben kann. Sie enthalten den aktuellen Ordner, in dem das Terminal geöffnet ist oder den Status von git. Man kann sie auch ästhetischer und informativer machen, z.B. mit

Tide (für fish): https://github.com/IlanCosman/tide

Nicht vergessen, die für den Prompt notwendigen Fonts MesloLGS NF ... zu installieren!

## Module installieren

Grundsätzlich werden Module in Python mit

```bash
pip install <package-name>
```

installiert. 
In Jupyter-Cloud geht das mit dem Befehl

```python
!pip install <package-name>
```

den man einfach vor den import-Befehl in dieselbe Zelle einfügt.

### Trotz extern gemanagter Python-Umgebung global packages mit pip installieren

**Bei der Flag "--break-system-package" riskiert man, den Package manager oder das OS zu beschädigen!**

```bash
pip install <package-name> --break-system-packages
```
- mit pip packages upgraden:
```bash
pip install --upgrade <package-name> --user --break-system-packages
```

### Virtuelle Python-Umgebungen (venv)

Wenn die Python-Umgebuung extern gemanaged wird, wie zum Beispiel von homebrew, kann man
keine packages mit pip, dem python-eigenen Package mangager, installieren; abgesehen von
oben genanntem Workaround.

Die Lösung sind virtuelle Umgebungen. Hier folgt eine kleine Anleitung.

Im dem Verzeichnis, in dem die virtuelle Umgebung platziert werden soll, gibt man im
Terminal den Befehl

```bash
python -m venv .venv
```

ein. Dabei sollte python vom System als Interpreter der globalen
Python-Umgebung erkannt werden.

Anschließend muss man die Pythonumgebung im Terminal aktivieren mit

```bash
# UNIX mit bash shell
source .venv/bin/activate
# UNIX mit csh shell
source .venv/bin/activate.csh
# UNIX mit fish shell
source .venv/bin/activate.fish
```

Jetzt kann man wie gehabt `pip` verwenden.
Nach Benutzung kann man die Umgebung wieder deaktivieren mit

```bash
deactivate
```

und beim nächsten Mal, wenn die Umgebung wieder benötigt wird, mit
oben aufgelisteten Befehlen reaktivieren. Alle installierten
Packages bleiben dabei erhalten.

Weiteres zum Nachlesen: https://www.computerwoche.de/article/2826499/virtual-environments-in-python-erklaert.html

## makefile

Mit dem @ Zeichen zu Beginn eines Commands, der in der makefile geschrieben steht, wird der Befehl nicht ins Terminal geprinted.

Das ist zum Beispiel sinnvoll, wenn man mit echo ""
Textoutput von einer makefile erhalten möchte. Dann möchte man ja nicht den Befehl echo "" stehen, sondern nur den Textoutput.