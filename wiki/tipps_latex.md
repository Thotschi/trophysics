# Tipps und Tricks in LaTex

## Selbstgeschriebene Packages einbuden

Ein LaTeX-package hat die Dateiendung `.sty`. Am Anfang des Header fügt man den Befehl

```LaTeX
\ProvidesPackage{headername}
```

ein und kann diesen anschließend wie gewohnt mit 

```LaTeX
\usepackage{PATH/headername}
```

einbinden.

## Figures beim Kompilieren auslassen

Mit der Option `draft` in der Documentclass gemäß

```LaTeX
\documentclass[draft]{scartl}
```
 oder nur für einzelne Abbildungen mit

 ```LaTeX
 \includegraphics[draft]{bildname}
 ```

wird beim Kompilieren anstatt der Bilder lediglich ein Rahmen an der selben Stelle eingefügt,
damit man beispielsweise noch die Formatierung im Text überprüfen kann. Das verringert
die Kompilationszeiten erheblich und ist vor allem für große Projekte mit vielen Abbildungen 
geeignet.