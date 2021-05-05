## Welcome to GitHub Pages

You can use the [editor on GitHub](https://github.com/CodingJosh42/GameProgramming/edit/gh-pages/index.md) to maintain and preview the content for your website in Markdown files.

Whenever you commit to this repository, GitHub Pages will run [Jekyll](https://jekyllrb.com/) to rebuild the pages in your site, from the content in your Markdown files.

### Markdown

Markdown is a lightweight and easy-to-use syntax for styling your writing. It includes conventions for

```markdown
Syntax highlighted code block

# Header 1
## Header 2
### Header 3

- Bulleted
- List

1. Numbered
2. List

**Bold** and _Italic_ and `Code` text

[Link](url) and ![Image](src)
```

For more details see [GitHub Flavored Markdown](https://guides.github.com/features/mastering-markdown/).

### Jekyll Themes

Your Pages site will use the layout and styles from the Jekyll theme you have selected in your [repository settings](https://github.com/CodingJosh42/GameProgramming/settings/pages). The name of this theme is saved in the Jekyll `_config.yml` configuration file.

### Support or Contact

Having trouble with Pages? Check out our [documentation](https://docs.github.com/categories/github-pages-basics/) or [contact support](https://support.github.com/contact) and we’ll help you sort it out.

# Entwicklungsblog

## 20.04.2021
In den vergangenen Tagen habe ich über meine Idee für ein Spiel nachgedacht und reifen lassen. Nun bin ich zu der folgenden Idee gekommen: Es soll ein Spiel werden, in dem der Spieler eine Figur mit der Hilfe der WASD Tasten steuern kann. Dabei handelt es sich um eine 2D Umgebung, in der sich der Spieler von links nach rechts bewegen kann. Allerdings gibt es mehrere Ebenen. Das heißt die Figur kann auf eine höhere Ebene springen und auch wieder eine Ebene tiefer fallen. Die Figur hat eine Waffe, welche Projektile abschießen kann. Diese sollten genutzt werden um die computergesteuerten Gegner zu besiegen. Allerdings können die Gegner auch Projektile abfeuern und damit den Spieler treffen wenn er nicht ausweicht. Bei zu vielen Treffern hat der Spieler verloren. Das Ziel des Spielers ist es eine gewisse Ziellinie/-abschnitt zu erreichen. Als besondere Mechanik des Spiels soll es bestimmte Tore geben. Wenn der Spieler durch eines dieser Tore läuft ist er invertiert. Das heißt, wenn er nach links mit A laufen will, läuft er nach rechts. Die eigentliche Mechanik besteht nun darin, dass auch das Schießen "rückwärts" funktioniert. In dem normalen Zustand beginnen die Projektile bei der Waffe des Spielers und enden am Bildschirmrand. Im invertierten Zustand beginnen die Projektile am Bildschirmrand und enden bei der Waffe des Spielers. Durch eine gute Strukturierung des Levels soll der Spieler bestimmte Gegner nur im invertierten Modus besiegen können.

## 02.05.2021
Heute habe ich die allgemeine Main-Loop und den Anfang der Klasse Game implementiert. In der Main-Loop wird im Prinizip nichts anderes gemacht als Funktionen der Game Klasse aufzurufen. Dabei muss vor der Main-Loop die init-Funktion ausgeführt werden, welche SDL initialisiert und ein Window erstellt. In der Main-Loop werden nun, solange game->running True zurückliefert, immer wieder die Funktionen handleEvents(), update() und render() ausgeführt. All diese drei Funktion machen allerdings noch nicht viel. HandleEvents behandelt z.B. nur das Event, welches durch das Schließen der Anwendung ausgelöst wird. Sollte die Anwendung geschlossen werden wird die Main-Loop unterbrochen und die clear Funktion gibt belegten Speicher wieder frei.

## 05.05.2021
Heute habe ich mit der Hilfe von Pixel Edit einen ersten Entwurf eines Spielcharakters entwickelt. Diesen habe ich anschließend versucht in meine Anwendung mit einzubinden. Dazu habe ich die Klasse TextureManager definiert, welche die statische Methode LoadTexture besitzt. Diese soll dazu dienen, dass weitere Texturen in Zukunft schnell und einfach geladen werden können. Um meinen Spieler zu laden habe die soeben genannte Funktion in der init Funktion von Game aufgerufen. Damit die Texture gerendert wird, wird die Funktion renderCopy verwendet. Außerdem habe ich dieser Funktion ein SDL_Rect als "Destination-Rechteck" mitgegeben. Dadurch kann ich in meiner update-Funktion die Postion meiner Figur ändern, sodass sie jetzt immer wieder von links nach rechts durch das Fenster läuft. 

<img src="https://github.com/CodingJosh42/GameProgramming/tree/main/Game%20Programming/Game%20Programming/assets" alt="Figur" width="200" height="200" />
