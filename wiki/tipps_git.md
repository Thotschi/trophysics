# Tipps und Tricks für Git

## Orphan Branches

Ein Orphan-Branch ist ein Branch, der keine Commit-History hat und keine Dateien enthält. Weil dieser Branch keine Commit-History mit einem anderen Branch teilt, kann man diesen **nicht** mit einem anderen Branch mergen. Er steht für sich allein, wodurch der Name Orphan kommt.

So einen Branch zu erstellen, geht über

```bash
git switch --orphan <orphan-branch>
```

Möchte man diesen publishen, ohne einen Inhalt zu comitten, geht das wiederum über

```bash
git commit --allow-empty -m "Initial commit on orphan branch"
git push origin <orphan-branch>
```

## Änderungen, die noch nicht commited wurde, rückgängig machen

`git stash` speichert vorübergehende Änderungen in einem working directory, ohne sie zu committen, beispielsweise wenn man den Branch wechseln will, die Änderung aber nicht verloren gehen soll.

`git stash pop` löscht gestashte Änderungen aus dem Stash. Die Änderungen bleiben aber erhalten und können dann committed werden.

Mit `git stash drop` werden die Änderungen dann tatsächlich gelöscht.

```bash
git stash
git stash drop
```

Lies mehr zu Stash in der [Git Documentation](https://git-scm.com/docs/git-stash).

