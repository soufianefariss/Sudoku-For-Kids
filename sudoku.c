#include <gtk/gtk.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <dirent.h>

#define PUZZLE_PATH "data/puzzles"
#define SAVE_PATH 	"data/saved_sudoku.txt"
#define BG_PATH 	"ressources/img/background.png"
#define GRID_X 57
#define GRID_Y 57
#define CMD_X 50 //43
#define CMD_Y 50 //43
#define TOOL_X 94
#define TOOL_Y 50

#define APPLICATION_TITLE "C-doku Pour Enfants"
#define QUIT_BUTTON_TEXT "Quit"
#define LOAD_BUTTON_TEXT "Load"
#define SAVE_BUTTON_TEXT "Save"
#define NEW_BUTTON_TEXT "New"

GtkWidget *window;
static GtkWidget *sudokuw[9][9];

int sudoku_total_files = 0;
char *sudoku_files[256];

// grid cell to act on, current sudoku file
int selected_x = 0, selected_y = 0, current_sudoku = -1;

// change grid cell
static void selection( GtkWidget *widget, gpointer data ) {
  	GdkColor color;
  	gdk_color_parse("#999999", &color);

  	gtk_widget_modify_bg(sudokuw[selected_x][selected_y], GTK_STATE_PRELIGHT, NULL);

  	selected_x = (int)((int*)data)[0];
  	selected_y = (int)((int*)data)[1];

  	gtk_widget_modify_bg(sudokuw[selected_x][selected_y], GTK_STATE_PRELIGHT, &color);
  	#ifdef DEBUG
    g_print("New selection: %i, %i\n", selected_x, selected_y);
  	#endif
}

// put number in grid cell
static void put_number( GtkWidget *widget, gpointer data )
{
	char *string = (char*)data;
	if( string[0] == 'X' )
		string[0] = 0;
	gtk_button_set_label( GTK_BUTTON( sudokuw[selected_x][selected_y] ), string );
}

// save current puzzle progress
static void save( GtkWidget *widget, gpointer data )
{
	FILE *savefile = NULL;
	int i,  j;

	if( current_sudoku == -1 )	// no puzzle to save
		return;

	savefile = fopen( SAVE_PATH, "w" );
	if( !savefile )
		return;

	fprintf( savefile, "%s\n0", sudoku_files[current_sudoku] );

	for( i=0; i<4; i++ ) //
	{
		for( j=0; j<4; j++ ) //
		{
			if( gtk_widget_get_sensitive(sudokuw[i][j]) == TRUE )	// if the cell is editable
			{
				if( strcmp( "", gtk_button_get_label(GTK_BUTTON(sudokuw[i][j])) ) != 0 )	// if user wrote something
				{
					fprintf( savefile, "%s", gtk_button_get_label( GTK_BUTTON(sudokuw[i][j])) );
					continue;
				}
			}
			fprintf( savefile, "." );
		}
	}
	fclose( savefile );
}

// start a new random sudoku
static void new( GtkWidget *widget, gpointer data )
{
	FILE *sudoku_file = NULL;
	char path[256];
	int i,j;
	char n[2] = { 0, '\0' };
	char c;

	GdkColor color;
	gdk_color_parse( "#eeeeee", &color );

	int file = -1;

	if( data == NULL )
	{
		do
		{
			file = rand() % sudoku_total_files;
		}while( file == current_sudoku ); // different from the current one
	}
	else
	{
		file = (int) data;
	}

	current_sudoku = file;
	#ifdef DEBUG
	g_print("New sudoku: %i: %s\n", file, sudoku_files[file]);
	#endif

	// fill the grid
	sudoku_file = fopen( sudoku_files[file], "r" );
	for( i = 0; i < 4; i++ ) { //
		for( j = 0; j < 4; j++ ) { //
			c = fgetc( sudoku_file );
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

// load saved puzzle progress
static void load( GtkWidget *widget, gpointer data )
{
	int i,j, file = -1;
	char buffer[256];
	char c = 0, label[2] = { 0, '\0' };
	FILE *savefile = fopen(SAVE_PATH, "r" );

	if( !savefile )
		return;

	fscanf( savefile, "%s", buffer );

	for( i=0; i<sudoku_total_files; i++ )
		if( strcmp( sudoku_files[i], buffer ) == 0 )
		{
			file = i;
			break;
		}

	new( NULL, (gpointer)file );

	while( c != '0' )
		c = fgetc( savefile );

	for( i=0; i<4; i++ )
		for( j=0; j<4; j++ )
		{
			c = fgetc(savefile);
			if( c != '.' )
			{
				label[0] = c;
				#ifdef DEBUG
				g_print( "c = %c - pos = %i, %i\n", c, i, j );
				#endif
				gtk_button_set_label( GTK_BUTTON(sudokuw[i][j]), label );
			}
		}
}

// quit
static void quit( GtkWidget *widget, gpointer data )
{
  	#ifdef DEBUG
	   g_print("Quitting.. Bye\n");
  	#endif
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

int main( int argc, char *argv[] ) {
	GtkWidget *layout; 												// layout
    GtkWidget *image;												// background image
    GtkWidget *lTitle;												// title lable
    GtkWidget *bQuit, *bLoad, *bSave, *bNew;						// toolbox buttons
    GtkWidget *vbox, *hbox, *separator, *cmdbox, *toolbox;			// boxes
    GtkWidget *numbers[4];											// numbers buttons

    int i,j;
    int *data = NULL;
    char num[2] = { 0, '\0' };
    char *n = NULL;

    DIR *dir;
    struct dirent *ep;

    // count sudoku files and save paths to them
    dir = opendir(PUZZLE_PATH);
    while( ep = readdir(dir) )
    {
    	if( strcmp(ep->d_name, ".") == 0 || strcmp(ep->d_name, "..") == 0 )
    		continue;
    	sudoku_files[sudoku_total_files] = (char*)malloc(sizeof(char)*256);
    	sprintf( sudoku_files[sudoku_total_files], "%s/%s", PUZZLE_PATH, ep->d_name );
    	#ifdef DEBUG
    	g_print("%s\n", sudoku_files[sudoku_total_files]);
    	#endif
    	sudoku_total_files++;
    }
    closedir(dir);

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
    /*
    GdkPixmap *backPixMap = gdk_pixmap_create_from_xpm ( window , NULL , NULL , BG_PATH );
	gdk_window_set_back_pixmap( GTK_WIDGET( window )->window , backPixMap , FALSE ); //GTK_WIDGET( window )->window 
	*/
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

    vbox = gtk_vbox_new( FALSE, 0 ); //false
    gtk_box_pack_start(GTK_BOX(vbox), lTitle, 0,0,5);
    gtk_layout_put(GTK_LAYOUT(layout), vbox, 0, 0);

    // prepare the grid (with buttons)
    for( i=0; i<4; i++ ) //
    {
    	hbox = gtk_hbox_new( FALSE, 0 );
    	for( j=0; j<4; j++ )
    	{
    		sudokuw[i][j] = gtk_button_new();
    		gtk_widget_set_sensitive(sudokuw[i][j], FALSE); // ORG: FALSE
    		gtk_widget_set_size_request( sudokuw[i][j], GRID_X, GRID_Y );
    		data = (int*) malloc(sizeof(int)*2);
    		data[0] = i; data[1] = j;
    		g_signal_connect (sudokuw[i][j], "clicked", G_CALLBACK(selection), data);
    		gtk_box_pack_start(GTK_BOX(hbox), sudokuw[i][j], 0,0,0);
    		if((j+1) % 2 == 0 && j < 2) // 8 -> 2 / 3 -> 2
    		{
    			separator = gtk_vseparator_new();
    			gtk_box_pack_start(GTK_BOX(hbox), separator, 0, 0, 5);
    		}
    	}
    	gtk_box_pack_start(GTK_BOX(vbox), hbox, 0, 0, 0);
    	if( (i+1)%2 == 0 && i<2 )// 8 -> 2 / 3 -> 2
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


    gtk_box_pack_start(GTK_BOX(vbox), cmdbox, 0, 0, 10 );
    gtk_container_add(GTK_CONTAINER(window), vbox);

    // prepare the toolbox
    toolbox = gtk_hbox_new( FALSE, 20 );
    gtk_box_pack_start(GTK_BOX(toolbox), bNew, 0, 0, 5);
    gtk_box_pack_start(GTK_BOX(toolbox), bSave, 0, 0, 5);
    gtk_box_pack_start(GTK_BOX(toolbox), bLoad, 0, 0, 5);
    gtk_box_pack_start(GTK_BOX(toolbox), bQuit, 0, 0, 5);

    gtk_box_pack_start(GTK_BOX(vbox), toolbox, 0, 0, 10 );

    gtk_window_set_title( GTK_WINDOW(window), "C-doku pour enfants");
    gtk_widget_show_all(window);

    gtk_main();
    return 0;
}
