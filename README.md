# Game Engine Development

Dieser Ordner enthält das Startpaket zur Vorlesung "Game Engine Development" an der Technischen Hochschule Mittelhessen.


## Inhalt

- /app/ - Ihr Spiel: Code und Assets
- /engine/ - die Engine, die Sie Kapitel für Kapitel ausbauen
- /engine/libraries/ - Bibliotheken, die zum Kompilieren benötigt werden
- /Makefile - steuert den Build

Diese Trennung zieht sich durch den gesamten Kurs: In `app/` entsteht Ihr Spiel, in `engine/` das Werkzeug, mit dem Sie es bauen.


## Projekt starten

Öffnen Sie den Ordner in Visual Studio Code. Beim ersten Öffnen bietet VSCode Ihnen die empfohlenen Erweiterungen zur Installation an — bestätigen Sie das.

In der **Run and Debug** Ansicht lässt sich die Konfiguration `mygame` starten, alternativ mit **F5**. Das Projekt wird dabei automatisch gebaut und unter der Kontrolle des Debuggers gestartet.

Die Tasks zum Bauen, Starten und Aufräumen sind über das Menü **Terminal** ➔ **Run Task...** verfügbar.

Über die Kommandozeile lässt sich das Projekt wie folgt bauen, starten und aufräumen:

```
$ make

$ make run

$ make clean
```

Wenn alles geklappt hat, öffnet sich ein Fenster mit einem sich drehenden Raumschiff. Mit der **ESC**-Taste schließen Sie es wieder.


## Aufgabenstellung

Die Beschreibung der einzelnen Kapitel finden Sie auf [meiner Website](https://www.tobias-reimann.com/thm/game-engine-development/).


## Copyright Hinweise

Der Code dieses Projekts steht unter der GNU General Public License v3. Den vollständigen Lizenztext finden Sie in der Datei `LICENSE`.

Wenn Sie eine Datei verändern, tragen Sie sich im Dateikopf mit Ihrem vollen Namen und dem Jahr der Änderung ein — direkt unter meiner Zeile, die dabei stehen bleibt:

```
/**
 * Game Engine Development
 * Technische Hochschule Mittelhessen
 * Copyright © 2021-2026 Tobias Reimann
 * Copyright © 2026 Max Mustermann
 *
```

Eine Datei, die Sie selbst neu schreiben, bekommt denselben Kopf, aber nur mit Ihrer Zeile.

Die GPL verlangt einen solchen Änderungshinweis ohnehin, sobald Sie eine veränderte Fassung weitergeben, zum Beispiel in einem öffentlichen GitHub-Repository (Abschnitt 5a). Ersetzen Sie in diesem Fall auch die Einleitung der `README.md` durch Ihre eigene — GitHub zeigt sie als Startseite Ihres Repositorys.

Die Bibliotheken in `/engine/libraries/` wurden von den jeweiligen Autoren unter verschiedenen freien Lizenzen veröffentlicht. Die genauen Lizenzbedingungen finden Sie in den jeweiligen Verzeichnissen bzw. in den Quelldateien selbst.

Die Assets in `/app/assets/`, etwa Meshes und Texturen, stehen unter der Lizenz [Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International](https://creativecommons.org/licenses/by-nc-sa/4.0/deed.de) (CC BY-NC-SA 4.0). Sie dürfen sie in eigenen Projekten verwenden, verändern und weitergeben, solange Sie die Quelle nennen, sie nicht kommerziell nutzen und veränderte Fassungen unter derselben Lizenz weitergeben. Als Quellenangabe genügt:

```
Tobias Reimann, Game Engine Development, Technische Hochschule Mittelhessen, CC BY-NC-SA 4.0
```

Jedes Mesh nennt seine Lizenz auch im Dateikopf. Wenn Sie ein Mesh verändern, tragen Sie sich dort unter meiner Zeile ein, wie im Code.

Die Shader in `/app/assets/shaders/` sind Code und stehen wie die Engine unter der GPL. Die Datei `/app/assets/gamecontrollerdb.txt` stammt aus dem Projekt [SDL_GameControllerDB](https://github.com/mdqinc/SDL_GameControllerDB) und steht unter dessen Lizenz.
