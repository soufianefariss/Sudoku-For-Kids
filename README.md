---
authors:
- |
    **Soufiane FARISS**
    soufiane.fariss@um5s.net.ma
    Université Mohammed V
    ENSIAS
- |
    **Ismail FATRI**
    ismailfatri@gmail.com
    Université Mohammed V
    ENSIAS
date: Janvier 2019
title: 'C-doku pour enfants'
---

**Remerciements**

Nous adressons nos sincères remerciements à tous les professeurs,
intervenants et toutes les personnes qui par leurs paroles, leurs
écrits, leurs conseils et leurs critiques, ont guidé nos réflexions et
ont accepté de nous rencontrer et de répondre à nos questions durant nos
recherches.

Nous remercions nos très chers parents, qui ont toujours été là pour
nous.

**Encadrant: Mr. A. EL FAKER.**



Introduction
============

Le Sudoku est un jeu de réflexion créé en 1979 par un Américain. Ce jeu
consiste à remplir une grille de 4x4 par des icônes en respectant
certaines contraintes. La grille de jeu est un carré de 4 cases de côté,
subdivisé en autant de carrés identiques, appelés régions. On donne à la
disposition du joueur 4 différentes icônes. La règle du jeu est simple :
chaque ligne, colonne et région ne doit contenir qu’une seule fois
l’icône en question. Formulé autrement, chacun de ces ensembles doit
contenir tous les icônes sans aucune répétition.


l’objectif du projet (Cahier des charges)
=========================================

Il faut développer une application en C pour le jeu du sudoku (16
cases). Au départ des cases sont pré-remplies et ne peuvent être
changées.

Les 4 régions sont repérables par la couleur des briques. Chaque élément
(étoile, cercle, triangle, carré) doit être présent une et une seule
fois sur :

• chaque ligne ;

• chaque colonne ;

• chaque région ;

En plus de pouvoir jouer, on devra pouvoir :

• Recommencer à tout moment en cliquant sur le bouton “recommencer”

• Créer et enregistrer les joueurs et leurs caractéristiques (nom et
score) sur fichier

• Permettre le chargement d’un jeu sauvegardé auparavant.

• Donner la liste des dix meilleurs scores

Il est également recommandé de considérer :

• La sécurité des accès à l’aide de mots de passe

• La réaliser d’une interface de jeu conviviale

Fonctionnement
==============

Au début, vous possédez deux choix :

• soit s’inscrire si c’est votre première partie dans ce jeu : vous
allez entrer votre identifiant puis vous choisissez un mot de passe.

• Soit se connecter si vous y avez déjà joué : vous allez vous connectez
à l’aide de l’identifiant et mot de passe déjà fournis.

Ensuite, vous allez être redirigé vers l’interface de l’utilisateur qui
vous donne les possibilités suivantes:

• Lancer une nouvelle partie en générant une grille qui est pré remplie
par le biais du bouton ***nouveau***. Une fois que vous avez remplie les
cases vides vous devez cliquer sur le bouton ***soumettre*** pour
vérifier votre résultat et enregistrer votre score.

• Charger une partie sauvegardée auparavant à l’aide du bouton
***charger***

• Afficher le classement des 10 meilleurs joueurs en appuyant sur le
bouton ***TOP 10***.

• Quitter le jeu en appuyant sur le bouton ***Quitter***.

Outils et techniques de developpement
=====================================

Pour la réalisation du travail, nous avons utilisé le langage C dans
l’éditeur gedit (UNIX), et afin d’obtenir une interface graphique, nous
avons utilisé GTK+ qui est un ensemble de bibliothèques logicielles
permettant de réaliser des interfaces graphiques. GTK+ est écrit en
langage C et utilise pourtant le paradigme de la programmation orientée
objet. Il est également possible d’utiliser GTK+ dans de nombreux autres
langages de programmations: Perl, C++, Java, JavaScript, Python, PHP…

Scenario d’execution
====================

Inscription
-----------

*Il est nécessaire de s’inscrire pour pouvoir lancer une partie*.

Authentification
----------------

*S’il décide de s’inscrire, il doit entrer son identifiant et mot de
passe choisis*.


*Une fois inscrit, il pourra se connecter en utilisant l’identifiant et
mot de passe choisis auparavant*.

Game on!
--------

Dès qu’il accède à son compte, un menu sera affiché qui demande au
utilisateur soit jouer une partie, soit sauvegarder sa partie, soit se
quitter.

Une fois la partie commencée en cliquant sur le bouton
nouveau:L’utilisateur aura le choix soit de jouer, soit de sauvegarder
l’évolution de sa partie, soit charger une partie sauvegardée, soit
quitter.

### Button: *New*

Cette fonction génère une carte de sudoku aléatoire à l’aide du fichier
*generator.exe* et l’enregistre dans *savedsudoku.txt*.

``` {style="CStyle"}

static void new( GtkWidget *widget, gpointer data)
{
    FILE *sudoku_file = NULL;
    int i,j;
    char n[2] = { 0, '\0' };
    char c;

    GdkColor color;
    gdk_color_parse( "#eeeeee", &color );
    
    system("generator"); // Cet fonction execute le fichier generator.exe 
    
    sudoku_file = fopen(CURRENT_PUZZLE_PATH, "r" );
    
    // Remplissage de la grille
    for( i = 0; i < 4; i++ ) {
        for( j = 0; j < 4; j++ ) {
            c = fgetc(sudoku_file);
            if( c != '.' )
            {
                n[0] = c;
                gtk_widget_set_sensitive(sudokuw[i][j], FALSE);
            }
            else
            {
                n[0] = '\0';
                gtk_widget_set_sensitive(sudokuw[i][j], TRUE);
                gtk_widget_modify_bg( sudokuw[i][j], GTK_STATE_NORMAL, &color );
            }
            gtk_button_set_label( GTK_BUTTON(sudokuw[i][j]), n );
        }
    }
    fclose(sudoku_file);
}
```

### Button: *Save*

Cette fonction enregistre la progression du jeu dans le fichier
*savedsudoku.txt*.

``` {style="CStyle"}
static void save( GtkWidget *widget, gpointer data ) {
    FILE *savefile = NULL;
    int i,  j;
    savefile = fopen( SAVE_PATH, "w" );
    if (!savefile)
        return;

    //fprintf( savefile, "%s\n0", sudoku_files[current_sudoku] );

    for( i=0; i<4; i++ ) {
        for( j=0; j<4; j++ ) {
                if( strcmp( "", gtk_button_get_label(GTK_BUTTON(sudokuw[i][j])) ) != 0 )    // if user wrote something
                {
                    fprintf( savefile, "%s", gtk_button_get_label( GTK_BUTTON(sudokuw[i][j])) );
                    continue;
                }
                fprintf( savefile, "." );
        }
    }
    fclose( savefile );
}
```

### Button: *Sumbit*

Ce button vérifie votre résultat et enregistre votre score.

``` {style="CStyle"}
static void submit(GtkWidget *widget, gpointer data ) {
    srand(time(NULL));
    char board[4][4], *sound_effects[6] = {
                            "canberra-gtk-play -f ~/myProject/ressources/sounds/no-1.wav",
                            "canberra-gtk-play -f ~/myProject/ressources/sounds/no-2.wav", 
                            "canberra-gtk-play -f ~/myProject/ressources/sounds/no-3.wav", 
                            "canberra-gtk-play -f ~/myProject/ressources/sounds/no-4.wav", 
                            "canberra-gtk-play -f ~/myProject/ressources/sounds/no-5.wav", 
                            "canberra-gtk-play -f ~/myProject/ressources/sounds/no-6.wav"
                            };
                            
                            
    for(int i=0; i<4; i++) {
        for(int j=0; j<4; j++ ) {
                const gchar *c = gtk_button_get_label( GTK_BUTTON(sudokuw[i][j]));
                if (*c == 0) {
                    char r = rand() % 6;
                    system(sound_effects[r]);
                    return;
                }
                board[i][j] = *c;
        }
    }
    
    for (int row = 0; row < 4; row++) {
        for (int i = 0; i < 4 - 1; i++) {
            for (int j = i + 1; j < 4; j++) {
                if (board[row][i] == board[row][j]) {
                    char r = rand() % 6;
                    system(sound_effects[r]);
                    return; // 0 means a duplicate is found, quitting..
                }
            }
        }
    }
    
    int transposed[4][4];
    for (int i = 0; i < 4; i++) 
        for (int j = 0; j < 4; j++) 
            transposed[i][j] = board[j][i];
    
    // now, check columns... 
    for (int col = 0; col < 4; col++) {
        for (int i = 0; i < 4 - 1; i++) {
            for (int j = i + 1; j < 4; j++) {
                if (transposed[col][i] == transposed[col][j]) {
                    char r = rand() % 6;
                    system(sound_effects[r]);
                    return; // 0 means a duplicate is found, quitting..
                }
            }
        }
    }
    system("canberra-gtk-play -f ~/myProject/ressources/sounds/Ta-Da.wav");
    return; // means good board!
}
```

### Button: *TOP 10*

Ce button afficher les TOP 10 scores.

``` {style="CStyle"}


// TODO: A COMPLETER
```

### Button: *Quitter*

Ce bouton permet de quitter le jeu.

``` {style="CStyle"}
// quit
static void quit( GtkWidget *widget, gpointer data )
{
    gtk_main_quit();
}
```

### Chronomètre

Cette section présente diverses fonctions utilisées pour implémenter un
chronomètre dans GTK +. Ce bloc de code consiste à afficher un
chronomètre à l’utilisateur avec possibilité d’arrêt ou de
réinitialisation.

``` {style="CStyle"}
/* Determines if to continue the timer or not */
static gboolean continue_timer = FALSE;

/* Determines if the timer has started */
static gboolean start_timer = FALSE;

/* Display seconds expired */
static int sec_expired = 0;

static gboolean _label_update(gpointer data) {
    GtkLabel *label = (GtkLabel*)data;
    char buf[256];
    memset(&buf, 0x0, 256);
    snprintf(buf, 255, "<span font_desc=\"Ubuntu Mono 14\"> Time elapsed: %d secs </span>", sec_expired++);
    gtk_label_set_markup(GTK_LABEL(label), buf);
    return continue_timer;
}

static void _start_timer (GtkWidget *button, gpointer data) {
    (void) button; /*Avoid compiler warnings*/
    GtkWidget *label = data;
    if(!start_timer) {
        g_timeout_add_seconds(1, _label_update, label);
        start_timer = TRUE;
        continue_timer = TRUE;
    }
}

static void _pause_resume_timer (GtkWidget *button, gpointer data)
{
    (void)button;
    if(start_timer)
    {
        GtkWidget *label = data;
        continue_timer = !continue_timer;
        if(continue_timer)
        {
            g_timeout_add_seconds(1, _label_update, label);
        }
        else
        {
            /*Decrementing because timer will be hit one more time before expiring*/
            sec_expired--;
        }
    }
}
```

Le générateur
=============

Ce générateur génère un tableau aléatoire en utilisant un fichier
contenant 256 formations possibles. Nous prenons d’abord un état de
départ aléatoire (exemples 2, 4, 4, 1), puis nous remplissons une
variable appelée init\_boad et nous utilisons quelques fonctions afin de
générer un tableau de départ valide. *(Voir generator.c)*

Ce générateur est u simplifié et peut générer des états de départ
***invalides***.

Comment Exécuter?
=================

Pour compiler le projet, exécuter la commande suivante:

``` {.bash language="bash"}
  $ make sudoku
```

Pour supprimer le projet, exécuter la commande suivante:

``` {.bash language="bash"}
  $ make clean
```


Conclusion
==========

A travers ce projet, nous avons pu comprendre et expérimenter les
différentes étapes de la conception d’un jeu pour enfants qu’est le
SUDOKU. De plus la réalisation du programme nous a permis non seulement
d’approfondir nos connaissances acquises depuis le début de notre
formation, mais aussi de nous familiariser avec la bibliothèque utilisée
GTK. En plus d’être un projet pédagogique il est aussi ludique et nous a
donné beaucoup de liberté dans le code et dans la conception.

