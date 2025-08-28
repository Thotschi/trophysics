# Python-Module

Es gibt zwei Möglichkeiten, Python-Module zu importieren.

Um einen der Python-Modul zu importieren (auch die .so-Datei), fügt man an den Anfang des Skriptes den Block

```python
import sys
sys.path.append("PATH")
```

ein, wobei `PATH` der Pfad ist, das Modul liegt.
Das Modul "sys" ist in Python integriert und muss daher nicht extra installiert werden.

## API-Key konfigurieren

Einige Funktionen benötigen einen API-Key. Dafür:

1. Erhalte einen API Key von groq unter https://console.groq.com/keys

2. Kopiere die Datei `.env.template` zu `.env`:
   ```bash
   cp io_utils/.env.template io_utensils/.env
   ```


