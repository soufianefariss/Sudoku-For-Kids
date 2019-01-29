#include <gtk/gtk.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <dirent.h>
#include <mysql/mysql.h>

// TODO: SOUND!! // LOAD BUTTON STILL HAS PROBLEMS


#define PUZZLE_PATH				     	"data/puzzles"
#define SAVE_PATH 						"data/saved_sudoku.txt"
#define BG_PATH 						"ressources/img/background.png"
#define	CURRENT_PUZZLE_PATH				"data/puzzles/current_puzzle.txt"
#define	FOUR_BOX_PUZZLES_FILE_PATH		"data/puzzles/4boxes.txt"
#define	FIVE_BOX_PUZZLES_FILE_PATH		"data/puzzles/5boxes.txt"

#define NEW_ICON_PATH 					"ressources/icons/new.png"
#define SAVE_ICON_PATH 					"ressources/icons/save.png"
#define LOAD_ICON_PATH 					"ressources/icons/load.png"
#define SUBMIT_ICON_PATH 				"ressources/icons/submit.png"
#define TOP10_ICON_PATH					"ressources/icons/top10.png"
#define QUIT_ICON_PATH 					"ressources/icons/quit.png"


#define GRID_X 		57
#define GRID_Y 		57
#define CMD_X 		50
#define CMD_Y 		50 
#define TOOL_X 		70
#define TOOL_Y 		50

#define APPLICATION_TITLE 				"C-doku Pour Enfants"
#define QUIT_BUTTON_TEXT 				"Quit"
#define LOAD_BUTTON_TEXT 				"Load"
#define SAVE_BUTTON_TEXT 				"Save"
#define NEW_BUTTON_TEXT 				"New"
#define SUBMIT_BUTTON_TEXT 				"Submit"
#define TOP10_BUTTON_TEXT 				"TOP 10"


#define	DEBUG

GtkWidget *imgg, *imgg2, *imgg3, *imgg4, *imgg5;

GtkWidget *window;
static GtkWidget *sudokuw[9][9];

int sudoku_total_files = 0;
char *sudoku_files[256];

// grid cell to act on, current sudoku file
int selected_x = 0, selected_y = 0, current_sudoku = -1;

// change grid cell
static void selection( GtkWidget *widget, gpointer data ) {
  	GdkColor color;
  	gdk_color_parse("#222222", &color); //#999999

  	gtk_widget_modify_bg(sudokuw[selected_x][selected_y], GTK_STATE_PRELIGHT, NULL);

  	selected_x = (int) ((int *) data)[0];
  	selected_y = (int) ((int *) data)[1];

  	gtk_widget_modify_bg(sudokuw[selected_x][selected_y], GTK_STATE_PRELIGHT, &color);
  	#ifdef DEBUG
    g_print("New selection: %i, %i\n", selected_x, selected_y);
  	#endif
}

// put number in grid cell
static void put_number( GtkWidget *widget, gpointer data )
{
	char *string = (char *) data;
	if (string[0] == 'X')
		string[0] = 0;
	gtk_button_set_label( GTK_BUTTON( sudokuw[selected_x][selected_y] ), string );
}

// save current puzzle progress
static void save( GtkWidget *widget, gpointer data ) {
	FILE *savefile = NULL;
	int i,  j;

/*	if( current_sudoku == -1 )	// no puzzle to save*/
/*		return;*/

	savefile = fopen( SAVE_PATH, "w" );
	if (!savefile)
		return;

	//fprintf( savefile, "%s\n0", sudoku_files[current_sudoku] );

	for( i=0; i<4; i++ ) {
		for( j=0; j<4; j++ ) {
				if( strcmp( "", gtk_button_get_label(GTK_BUTTON(sudokuw[i][j])) ) != 0 )	// if user wrote something
				{
					fprintf( savefile, "%s", gtk_button_get_label( GTK_BUTTON(sudokuw[i][j])) );
					continue;
				}
				fprintf( savefile, "." );
		}
	}
	fclose( savefile );
}

// start a new random sudoku
static void new( GtkWidget *widget, gpointer data)
{	
	
	FILE *sudoku_file = NULL;
	int i,j;
	char n[2] = { 0, '\0' };
	char c;

	GdkColor color;
	gdk_color_parse( "#eeeeee", &color );
	
	system("generator"); // I present to you the magical, the one and only: the SYSTEM() function <3 <3.
	
	sudoku_file = fopen(CURRENT_PUZZLE_PATH, "r" );
	
	// fill the grid
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
/*				imgg  = gtk_image_new_from_file(NEW_ICON_PATH);*/
/*				*/
/*				gtk_layout_put(GTK_LAYOUT(data), imgg, 100, 300);*/
/*				gtk_widget_show(data);*/
				gtk_widget_set_sensitive(sudokuw[i][j], TRUE);
				gtk_widget_modify_bg( sudokuw[i][j], GTK_STATE_NORMAL, &color );
			}
			gtk_button_set_label( GTK_BUTTON(sudokuw[i][j]), n );
			//gtk_button_set_image (GTK_BUTTON(sudokuw[i][j]), imgg);
		}
	}
	fclose(sudoku_file);
}

// load saved puzzle progress
static void load( GtkWidget *widget, gpointer data ) {
	int i,j;
	char c = 0, label[2] = { 0, '\0' };
	FILE *savefile = fopen(SAVE_PATH, "r" );

	if(!savefile)
		return;
	
	for( i=0; i<4; i++ )
		for( j=0; j<4; j++ )
		{
			c = fgetc(savefile);
			if( c != '.' )
			{
				label[0] = c;
				gtk_button_set_label( GTK_BUTTON(sudokuw[i][j]), label );
			}
		}
}

static void submit( GtkWidget *widget, gpointer data ) {
/*	int i, j;*/
/*	FILE *savefile = fopen(SAVE_PATH, "r" );*/

/*	if(!savefile)*/
/*		return;*/
	;
}

static void top10( GtkWidget *widget, gpointer data ) {
	;
}


// quit
static void quit( GtkWidget *widget, gpointer data )
{
  	gtk_main_quit();
}

static gboolean delete_event( GtkWidget *widget, GdkEvent *event, gpointer data )
{
  	return FALSE; // quit
}

static void destroy( GtkWidget *widget, gpointer data )
{
  	gtk_main_quit();
}

/* Determines if to continue the timer or not */
static gboolean continue_timer = FALSE;

/* Determines if the timer has started */
static gboolean start_timer = FALSE;

/* Display seconds expired */
static int sec_expired = 0;

static gboolean _label_update(gpointer data)
{
	GtkLabel *label = (GtkLabel*)data;
	char buf[256];
	memset(&buf, 0x0, 256);
	snprintf(buf, 255, "<span font_desc=\"Ubuntu Mono 14\"> Time elapsed: %d secs </span>", sec_expired++);
	gtk_label_set_markup(GTK_LABEL(label), buf);
	return continue_timer;

}

static void _start_timer (GtkWidget *button, gpointer data)
{
    (void) button; /*Avoid compiler warnings*/
    GtkWidget *label = data;
    if(!start_timer)
    {
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

static void _reset_timer (GtkWidget *button, gpointer data)
{
    (void)button; (void)data;
    
    sec_expired = 0;
    continue_timer = FALSE;
    start_timer = FALSE;
}

struct user{
    char username[10];
    char password[10];
} *pUser;

int userlogin() {
    FILE *fp;
    char uName[10], pwd[10];
    int i;
    char c;

    pUser = (struct user *) malloc(sizeof(struct user));
	printf("\n*******************************************************************");
	printf("\n*                                                                 *");
	printf("\n*                     Welcome to Sudoku Game                      *");
	printf("\n*                                                                 *");
	printf("\n*******************************************************************\n");

    printf("\n\n\t1. Login Through An Existing Account\n\t2. Create New account\n\n\t");
    scanf("%d",&i);
    printf("\n");
    switch(i){
        case 1:
            if ( ( fp=fopen("user.dat", "r+")) == NULL) {
                if ( ( fp=fopen("user.dat", "w+")) == NULL) {
                    printf ("Could not open file\n");
                    break;
                }
            }
            printf("\tUsername: ");
            scanf("%s",&uName);
            printf("\tPassword: ");
            scanf("%s",&pwd);
            while ( fread (pUser, sizeof(struct user), 1, fp) == 1) {
                if( strcmp ( pUser->username, uName) == 0) {
                    if( strcmp ( pUser->password, pwd) == 0) {
                        printf  ("\n\tLogin successful! Welcome.\n");
                        
                    }
                }
            }
            break;

        case 2:
            do
            {
                if ( ( fp=fopen("user.dat", "a+")) == NULL) {
                    if ( ( fp=fopen("user.dat", "w+")) == NULL) {
                        printf ("Could not open file\n");
						break;
                    }
                }
                printf("Choose A Username: ");
                scanf("%s",pUser->username);
                printf("Choose A Password: ");
                scanf("%s",pUser->password);
                fwrite (pUser, sizeof(struct user), 1, fp);
                printf("\tAdd another account? (Y/N): ");
                scanf(" %c",&c);
            } while(c=='Y'||c=='y');
            
            break;
    }
	free (pUser);
	fclose(fp);
}

int main( int argc, char *argv[] ) {

	FILE *fp;
    char uName[10], pwd[10];
    int var;
    char c;

    pUser = (struct user *) malloc(sizeof(struct user));
	printf("\n*******************************************************************");
	printf("\n*                                                                 *");
	printf("\n*                     Welcome to Sudoku Game                      *");
	printf("\n*                                                                 *");
	printf("\n*******************************************************************\n");

    printf("\n\n\t1. Login Through An Existing Account\n\t2. Create New account\n\n\t");
    scanf("%d",&var);
    printf("\n");
    switch(var) {
        case 1:
            if ( ( fp=fopen("user.dat", "r+")) == NULL) {
                if ( ( fp=fopen("user.dat", "w+")) == NULL) {
                    printf ("Could not open file\n");
                    exit(0);
                }
            }
            printf("\tUsername: ");
            scanf("%s",&uName);
            printf("\tPassword: ");
            scanf("%s",&pwd);
            while ( fread (pUser, sizeof(struct user), 1, fp) == 1) {
                if( strcmp ( pUser->username, uName) == 0) {
                    if( strcmp ( pUser->password, pwd) == 0) {
                        printf  ("\n\tLogin successful! Welcome.\n");
                        goto BEGIN;
                    }
                }
            }
            exit(0);

        case 2:
            do
            {
                if ( ( fp=fopen("user.dat", "a+")) == NULL) {
                    if ( ( fp=fopen("user.dat", "w+")) == NULL) {
                        printf ("Could not open file\n");
						exit(0);
                    }
                }
                printf("\tChoose A Username: ");
                scanf("%s",pUser->username);
                printf("\tChoose A Password: ");
                scanf("%s",pUser->password);
                fwrite (pUser, sizeof(struct user), 1, fp);
                printf("\tAdd another account? (Y/N): ");
                scanf(" %c",&c);
            } while(c=='Y'||c=='y');
            exit(0);
    }
	free (pUser);
	BEGIN:fclose(fp);
	
	
	GtkWidget *layout; 													// layout
    GtkWidget *image;												    // background image
    GtkWidget *lTitle, *label;											// titles & lables
    GtkWidget *bQuit, *bLoad, *bSave, *bNew, *bSubmit, *bTop10;			// toolbox buttons
    GtkWidget *bStart, *bPause, *bReset;			                    // timer buttons
    GtkWidget *vbox, *hbox, *separator, *cmdbox, *toolbox, *timerbox;	// boxes
    GtkWidget *numbers[4];												// numbers buttons

    int i,j;
    int *data = NULL;
    char num[2] = { 0, '\0' };
    char *n = NULL;
    
    // random seed
    srand(time(NULL));

    gtk_init( &argc, &argv );

    window = gtk_window_new( GTK_WINDOW_TOPLEVEL );
    gtk_window_set_default_size(GTK_WINDOW(window), 700, 680);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	gtk_window_set_resizable (GTK_WINDOW(window), TRUE);
	
	layout = gtk_layout_new(NULL, NULL);
    gtk_container_add(GTK_CONTAINER (window), layout);
    gtk_widget_show(layout);
	image = gtk_image_new_from_file("ressources/img/background.png");
    gtk_layout_put(GTK_LAYOUT(layout), image, 0, 0);
    
    
    g_signal_connect( window, "delete-event", G_CALLBACK(delete_event), NULL );
    g_signal_connect( window, "destroy", G_CALLBACK(destroy), NULL);
    gtk_container_set_border_width(GTK_CONTAINER(window), 0);
    
    lTitle = gtk_label_new( APPLICATION_TITLE );

    bQuit = gtk_button_new_with_label( QUIT_BUTTON_TEXT );
    gtk_widget_set_size_request( bQuit, TOOL_X, TOOL_Y );
    g_signal_connect (bQuit, "clicked", G_CALLBACK(quit), NULL);

    bLoad = gtk_button_new_with_label( LOAD_BUTTON_TEXT );
    gtk_widget_set_size_request( bLoad, TOOL_X, TOOL_Y );
    g_signal_connect (bLoad, "clicked", G_CALLBACK(load), NULL);

    bSave = gtk_button_new_with_label( SAVE_BUTTON_TEXT );
    gtk_widget_set_size_request( bSave, TOOL_X, TOOL_Y );
    g_signal_connect (bSave, "clicked", G_CALLBACK(save), NULL);

    bNew = gtk_button_new_with_label( NEW_BUTTON_TEXT );
    gtk_widget_set_size_request( bNew, TOOL_X, TOOL_Y );
    g_signal_connect (bNew, "clicked", G_CALLBACK(new), NULL);
	
	
/*	
	GtkWidget *eb1 = gtk_event_box_new();
	gtk_widget_add_events (eb1, GDK_BUTTON_PRESS_MASK);
	gtk_container_add(GTK_CONTAINER(window), eb1);
	imgg  = gtk_image_new_from_file(NEW_ICON_PATH);
	g_signal_connect (G_OBJECT(eb1), "button_press_event", G_CALLBACK(new), NULL);
	gtk_container_add(GTK_CONTAINER(eb1), imgg);
	gtk_layout_put(GTK_LAYOUT(layout), eb1, 300, 480);
	
	GtkWidget *eb2 = gtk_event_box_new();
	gtk_widget_add_events (eb2, GDK_BUTTON_PRESS_MASK);
	gtk_container_add(GTK_CONTAINER(window), eb2);
	imgg2  = gtk_image_new_from_file(SAVE_ICON_PATH);
	g_signal_connect (G_OBJECT(eb2), "button_press_event", G_CALLBACK(save), NULL);
	gtk_container_add(GTK_CONTAINER(eb2), imgg2);
	gtk_layout_put(GTK_LAYOUT(layout), eb2, 300 + 40, 480);
	
	GtkWidget *eb3 = gtk_event_box_new();
	gtk_widget_add_events (eb3, GDK_BUTTON_PRESS_MASK);
	gtk_container_add(GTK_CONTAINER(window), eb3);
	imgg3  = gtk_image_new_from_file(LOAD_ICON_PATH);
	g_signal_connect (G_OBJECT(eb3), "button_press_event", G_CALLBACK(load), NULL);
	gtk_container_add(GTK_CONTAINER(eb3), imgg3);
	gtk_layout_put(GTK_LAYOUT(layout), eb3, 300 + 40 * 20, 480);
*/

    bSubmit = gtk_button_new_with_label( SUBMIT_BUTTON_TEXT );
    gtk_widget_set_size_request( bSubmit, TOOL_X, TOOL_Y );
    g_signal_connect (bSubmit, "clicked", G_CALLBACK(submit), NULL);
    
    bTop10 = gtk_button_new_with_label( TOP10_BUTTON_TEXT );
    gtk_widget_set_size_request( bTop10, TOOL_X, TOOL_Y );
    g_signal_connect (bSubmit, "clicked", G_CALLBACK(top10), NULL);
    
    // timer buttons
/*    label = gtk_label_new("Time elapsed: 0 secs");*/
/*    gtk_widget_modify_font (GtkWidget *widget, PangoFontDescription *font_desc);*/
/*    	*/
    label = gtk_label_new(NULL);
    
        
    bStart = gtk_button_new_with_label("Start");
    gtk_widget_set_size_request( bStart, TOOL_X, TOOL_Y );
    g_signal_connect(G_OBJECT(bStart), "clicked", G_CALLBACK(_start_timer), label);

    bPause = gtk_button_new_with_label("Pause / Resume");
    gtk_widget_set_size_request( bPause, TOOL_X + 50, TOOL_Y);
    g_signal_connect(G_OBJECT(bPause), "clicked", G_CALLBACK(_pause_resume_timer), label);

    bReset = gtk_button_new_with_label("Reset");
    gtk_widget_set_size_request( bReset, TOOL_X, TOOL_Y );
    g_signal_connect(G_OBJECT(bReset), "clicked", G_CALLBACK(_reset_timer), label);
    
    
    vbox = gtk_vbox_new( FALSE, 0 ); //false
    //gtk_box_pack_start(GTK_BOX(vbox), lTitle, 20,20,55); //0, 0, 5
    gtk_layout_put(GTK_LAYOUT(layout), vbox, 0, 0);
    

    // prepare the grid (with buttons)
    for( i=0; i<4; i++ ) //
    {
    	hbox = gtk_hbox_new( FALSE, 0 );
    	for( j=0; j<4; j++ )
    	{
    		sudokuw[i][j] = gtk_button_new();
    		gtk_widget_set_sensitive(sudokuw[i][j], FALSE); // ORG: FALSE
    		gtk_widget_set_size_request( sudokuw[i][j], GRID_X, GRID_Y);
    		data = (int*) malloc(sizeof(int)*2);
    		data[0] = i; data[1] = j;
    		g_signal_connect (sudokuw[i][j], "clicked", G_CALLBACK(selection), data);
    		gtk_box_pack_start(GTK_BOX(hbox), sudokuw[i][j], 0, 0, 0); // 0 0 0 
    		if(j + 1 == 2) // 8 -> 2 / 3 -> 2 AND (j+1) % 2 == 0 && j < 2 which means id j is in the middle.
    		{
    			separator = gtk_vseparator_new();
    			gtk_box_pack_start(GTK_BOX(hbox), separator, 0, 0, 5);
    		}
    	}
    	gtk_box_pack_start(GTK_BOX(vbox), hbox, 0, 0, 0);
    	if (i + 1 == 2) // 8 -> 2 / 3 -> 2 AND (i+1)%2 == 0 && i<2
    	{
    		separator = gtk_hseparator_new();
    		gtk_box_pack_start(GTK_BOX(vbox), separator, 0, 0, 5);
    	}
    }
    
    // prepare the number buttons
    cmdbox = gtk_hbox_new( FALSE, 0 );
    for( i = 0; i < 5; i++ )
    {
    	if( i == 4 )
    		num[0] = 'X';
    	else
    		num[0] = 0x31+i;
    	numbers[i] = gtk_button_new_with_label(num);
    	gtk_widget_set_size_request( numbers[i], CMD_X, CMD_Y);
    	gtk_box_pack_start(GTK_BOX(cmdbox), numbers[i], 0, 0, 5);
    	n = (char*) malloc(sizeof(char) * 2);
    	n[1] = '\0';
    	n[0] = num[0];
    	g_signal_connect (numbers[i], "clicked", G_CALLBACK(put_number), n);
    }


    gtk_box_pack_start(GTK_BOX(vbox), cmdbox, 0, 0, 10);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    // prepare the toolbox
    toolbox = gtk_hbox_new( FALSE, 20 );
    gtk_box_pack_start(GTK_BOX(toolbox), bNew, 0, 0, 5);
    gtk_box_pack_start(GTK_BOX(toolbox), bSave, 0, 0, 5);
    gtk_box_pack_start(GTK_BOX(toolbox), bLoad, 0, 0, 5);
    gtk_box_pack_start(GTK_BOX(toolbox), bSubmit, 0, 0, 5);
    gtk_box_pack_start(GTK_BOX(toolbox), bTop10, 0, 0, 5);
    gtk_box_pack_start(GTK_BOX(toolbox), bQuit, 0, 0, 5);
    
    // prepare the timer
    timerbox = gtk_hbox_new( FALSE, 20 );
    gtk_box_pack_start(GTK_BOX(timerbox), bStart, 0, 0, 10);
    gtk_box_pack_start(GTK_BOX(timerbox), bPause, 0, 0, 10);
    gtk_box_pack_start(GTK_BOX(timerbox), bReset, 0, 0, 10);

    //gtk_box_pack_start(GTK_BOX(vbox), toolbox, 0, 0, 10);// 0, 0, 10
    
    gtk_layout_put(GTK_LAYOUT(layout), vbox, 225, 150);
    gtk_layout_put(GTK_LAYOUT(layout), toolbox, 55, 460);
    gtk_layout_put(GTK_LAYOUT(layout), timerbox, 170, 520);
    gtk_layout_put(GTK_LAYOUT(layout), label, 255, 610 + 50);
    
    g_timeout_add_seconds(1, _label_update, label);
    continue_timer = TRUE;
    start_timer = TRUE;

    gtk_button_set_image (GTK_BUTTON(bNew), imgg);
    gtk_window_set_title( GTK_WINDOW(window), "C-doku pour enfants");
    gtk_widget_show_all(window);
	
    gtk_main();
    return 0;
}
