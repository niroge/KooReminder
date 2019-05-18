/*
 * Copyright (C) 2019
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/* Lasciate ogni speranza, voi ch'entrate */

#include <gtk/gtk.h>

#include <time.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#define VERSION "0.0.1"
/* #include <niroge/bluetooth_unix.h> */

#define FULLSCREEN
#undef  FULLSCREEN

enum SCREEN_IDS {
    SCREEN_NONE,
    SCREEN_CLOCK,
    SCREEN_ALARMS,
    SCREEN_SETTINGS,
    SCREEN_ALL
};

const int    SCREEN_WIDTH  = 480;
const int    SCREEN_HEIGHT = 320;
const char * ALARMS_FILE   = "alarms.dat";
const char * THEMES_FILE   = "theme.css";

struct alarm_t {
    char *  time;
    char *  title;
    uint8_t repeat;
    time_t  secondsToAlarm;

    GtkWidget * box;
    GtkWidget * labelAlarm;
    GtkWidget * labelRepeat;
    GtkWidget * labelTitle;
};

GtkWidget * window            = NULL;
GtkWidget * buttonClock       = NULL;
GtkWidget * buttonAlarms      = NULL;
GtkWidget * buttonSettings    = NULL;
GtkWidget * buttonReset       = NULL;
GtkWidget * hboxTopBar        = NULL;
GtkWidget * vboxPageContent   = NULL;
GtkWidget * labelTime         = NULL;
GtkWidget * labelDate         = NULL;
GtkWidget * alarmsWindow      = NULL;
GtkWidget * selectedTopButton = NULL;
GtkWidget * labelVersion      = NULL;
GtkWidget * labelResetCuckoo  = NULL;
FILE *      alarmsFilePointer = NULL;
int         screenId          = SCREEN_NONE;
GtkStyleContext * contextBarSelected = NULL;

int              totalAlarms   = 0;
struct alarm_t * alarmsPointer = NULL;

bool FileExists( const char * );
void LoadAlarm( const char * );
void ResetCuckoo( gpointer );

void SetDateAndTime( void );
int DrawScreen( gpointer );
void ApplyCSSNaming( void );
void LoadFiles( void );
void ShowTopBar( int );
void LoadWidgets( void );
void ChangeScreenAlarms( gpointer );
void ChangeScreenClock( gpointer );
void ChangeScreenSettings( gpointer );
void CleanUpBody( int );
void CloseCuckoo( gpointer );

int main( int argc, char ** argv ) {
    gtk_init( &argc, &argv );
    
    buttonClock    = gtk_button_new_with_label( "Date & Time" );
    buttonAlarms   = gtk_button_new_with_label( "Alarms" );
    buttonSettings = gtk_button_new_with_label( "Settings" );
    buttonReset    = gtk_button_new_with_label( "Reset KooReminder" );

    window       = gtk_window_new( GTK_WINDOW_TOPLEVEL );
    alarmsWindow = gtk_scrolled_window_new( NULL, NULL );
    
    hboxTopBar = gtk_box_new( 0, 0 );
    vboxPageContent = gtk_box_new( 1, 0 );
    
    labelTime        = gtk_label_new( "hh:mm:ss" );
    labelDate        = gtk_label_new( "dd mmm yyyy" );
    labelResetCuckoo = gtk_label_new( "Factory reset KooReminder" );
    labelVersion     = gtk_label_new( "\n\nVersion: " VERSION );

    gtk_widget_set_halign( labelResetCuckoo, GTK_ALIGN_START );
    gtk_widget_set_halign( buttonReset, GTK_ALIGN_START );
    gtk_widget_set_halign( labelVersion, GTK_ALIGN_START );
    
    gtk_box_set_homogeneous( GTK_BOX( hboxTopBar ), true );
    
    g_signal_connect( window, "destroy", G_CALLBACK( CloseCuckoo ), NULL );

    gtk_container_set_border_width( GTK_CONTAINER( window ), 0 );
    gtk_window_set_title( GTK_WINDOW( window ), "KooReminder" );

    #ifdef FULLSCREEN
    gtk_window_fullscreen( GTK_WINDOW( window ) );
    #endif
    
    gtk_window_set_default_size( GTK_WINDOW( window ), 480, 320 );

    LoadWidgets();
    ApplyCSSNaming();
    LoadFiles();
    gtk_widget_show_all( window );
    g_timeout_add( 100, DrawScreen, NULL );
    gtk_main();

    return 0;
}

bool FileExists( const char * fileName ) {
    FILE * pointer = fopen( fileName, "r" );
    if ( pointer ) {
        fclose( pointer );
        return true;
    }

    return false;
}

void LoadAlarm( const char * alarmRow ) {
    char buffer[1024];
    struct alarm_t tmpAlarm;
    time_t secondsSinceEpoch;

    time( &secondsSinceEpoch );
    alarmsPointer = realloc( alarmsPointer, sizeof( struct alarm_t ) * (totalAlarms + 1) );
    memset( &tmpAlarm, '\0', sizeof tmpAlarm );

    sscanf( alarmRow, "%s %hhd %s", tmpAlarm.time, &tmpAlarm.repeat, tmpAlarm.title );
    /* Insert a complex algorithm */
    
    memcpy( alarmsPointer + sizeof( struct alarm_t ) * totalAlarms, &tmpAlarm, sizeof( struct alarm_t ) );
}

void ResetCuckoo( gpointer data ) {
    /* system( "rm theme.css" ); */
    /* system( "rm alarms.dat" ); */

    CloseCuckoo( NULL );
}

void SetDateAndTime( void ) {
    const char * weekDays[]   = { "Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun" };
    const char * monthNames[] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };
    
    char tmpBufferTime[9];
    char tmpBufferDate[16];
    time_t secondsSinceEpoch;
    struct tm * localTime;
    
    time( &secondsSinceEpoch );
    localTime = localtime( &secondsSinceEpoch );
    sprintf( tmpBufferTime, "%02d:%02d:%02d", localTime->tm_hour, localTime->tm_min, localTime->tm_sec );
    sprintf( tmpBufferDate, "%s %02d %s %d", weekDays[localTime->tm_wday], localTime->tm_mday, monthNames[localTime->tm_mon], localTime->tm_year + 1900 );

    gtk_label_set_text( GTK_LABEL( labelTime ), tmpBufferTime );
    gtk_label_set_text( GTK_LABEL( labelDate ), tmpBufferDate );
}

int DrawScreen( gpointer uselessChunk ) {
    static bool screenChanged = false;

    g_signal_connect( buttonAlarms, "clicked", G_CALLBACK( ChangeScreenAlarms ), NULL );
    g_signal_connect( buttonClock, "clicked", G_CALLBACK( ChangeScreenClock ), NULL );
    g_signal_connect( buttonSettings, "clicked", G_CALLBACK( ChangeScreenSettings ), NULL );

    ApplyCSSNaming();
    switch ( screenId ) {
    case SCREEN_NONE:
        ChangeScreenAlarms( NULL );
        ChangeScreenClock( NULL );
        ChangeScreenSettings( NULL );
        ChangeScreenClock( NULL );
        break;
        
    case SCREEN_ALARMS:
        ChangeScreenAlarms( NULL );
        break;
        
    case SCREEN_CLOCK:
        ChangeScreenClock( NULL );
        break;

    case SCREEN_SETTINGS:
        ChangeScreenSettings( NULL );
        break;
    }
    
    return G_SOURCE_CONTINUE;
}

void ApplyCSSNaming( void ) {
    /* Only IDs, classes are declared in LoadFiles */
    gtk_widget_set_name( GTK_WIDGET( window ), "id_window" );
    gtk_widget_set_name( GTK_WIDGET( alarmsWindow ), "id_alarms_screen" );
    gtk_widget_set_name( GTK_WIDGET( buttonClock ), "id_buttonClock" );
    gtk_widget_set_name( GTK_WIDGET( buttonAlarms ), "id_buttonAlarms" );
    gtk_widget_set_name( GTK_WIDGET( buttonSettings ), "id_buttonSettings" );
    gtk_widget_set_name( GTK_WIDGET( vboxPageContent ), "id_body" );
    gtk_widget_set_name( GTK_WIDGET( hboxTopBar ), "id_topBar" );
    gtk_widget_set_name( GTK_WIDGET( labelDate ), "id_labelDate" );
    gtk_widget_set_name( GTK_WIDGET( labelTime ), "id_labelTime" );
    gtk_widget_set_name( GTK_WIDGET( selectedTopButton ), "id_barSelected" );
    gtk_widget_set_name( GTK_WIDGET( labelVersion ), "id_labelVersion" );
}

void LoadFiles( void ) {
    /* First load the theme file */
    if ( FileExists( THEMES_FILE ) ) {
        g_print( "Loading theme...\n" );
        GtkCssProvider * provider = gtk_css_provider_new ();
        GdkDisplay *     display  = gdk_display_get_default ();
        GdkScreen *      screen   = gdk_display_get_default_screen (display);
        GtkWidget *barButtons[] = { buttonClock, buttonAlarms, buttonSettings };

        GtkStyleContext * contextTopBarElement = NULL;

        GError *error = 0;

        gtk_css_provider_load_from_file( provider, g_file_new_for_path( THEMES_FILE ), &error );

        for ( int i = 0; i < 3; i++ ) {
            g_print( "Loop id: %d\n", i + 1 );
            contextTopBarElement = gtk_widget_get_style_context( barButtons[i] );
            gtk_style_context_add_class( contextTopBarElement, "barButton" );
        }
        
        gtk_style_context_add_provider_for_screen( screen, GTK_STYLE_PROVIDER( provider ), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION );
        
        if ( error ) {
            g_print( "Error loading theme: %s\n", error->message );
        }
        
        g_object_unref( provider );
    } else {
        g_print( "Theme file not found! Fuck, where is Davide when you need him?\n" );
    }

    /* Then load the alarms file */
    if ( FileExists( ALARMS_FILE ) ) {
        alarmsFilePointer = fopen( ALARMS_FILE, "rw" );
        char buffer[1024];

        while ( fgets( buffer, sizeof buffer, alarmsFilePointer ) != NULL ) {
            LoadAlarm( buffer );
            totalAlarms++;
        }

        g_print( "Amount of alarms: %d\n", totalAlarms );
    }
}

void ShowTopBar( int id ) {
    GtkWidget *barButtons[] = { NULL, buttonClock, buttonAlarms, buttonSettings, NULL };
    
    selectedTopButton = barButtons[id];
}

void LoadWidgets( void ) {
    const int expand = 0, fill = 1, padding = 0;
    g_print( "Loading widgets!\n" );
    gtk_container_add( GTK_CONTAINER( window ), vboxPageContent );

    gtk_box_pack_start( GTK_BOX( vboxPageContent ), hboxTopBar, expand, fill, padding );
    gtk_box_pack_start( GTK_BOX( hboxTopBar ), buttonClock, expand, fill, padding);
    gtk_box_pack_start( GTK_BOX( hboxTopBar ), buttonAlarms, expand, fill, padding);
    gtk_box_pack_start( GTK_BOX( hboxTopBar ), buttonSettings, expand, fill, padding);

    gtk_box_pack_start( GTK_BOX( vboxPageContent ), alarmsWindow, true, true, false );
    gtk_box_pack_start( GTK_BOX( vboxPageContent ), labelTime, true, true, true );
    gtk_box_pack_start( GTK_BOX( vboxPageContent ), labelDate, false, false, false );

    gtk_box_pack_start( GTK_BOX( vboxPageContent ), labelResetCuckoo, false, false, false );
    gtk_box_pack_start( GTK_BOX( vboxPageContent ), buttonReset, false, false, false );
    gtk_box_pack_start( GTK_BOX( vboxPageContent ), labelVersion, false, false, false );
    
    g_signal_connect( buttonReset, "clicked", G_CALLBACK( ResetCuckoo ), NULL );

    g_print( "Cleaning up the body...\n" );
    CleanUpBody( SCREEN_ALL );
    ShowTopBar( SCREEN_CLOCK );
    g_print( "Top bar updated!\n" );
}

void ChangeScreenAlarms( gpointer data ) {
    if ( screenId == SCREEN_ALARMS ) {
        /* insert another complex algorithm that shows alarms */
        return;
    }

    CleanUpBody( screenId );
    ShowTopBar( SCREEN_ALARMS );
    g_print( "Alarms screen pack\n" );
    gtk_scrolled_window_set_policy( GTK_SCROLLED_WINDOW( alarmsWindow ),
                                    GTK_POLICY_NEVER,
                                    GTK_POLICY_AUTOMATIC );

    gtk_widget_show( GTK_WIDGET( alarmsWindow ) );

    screenId = SCREEN_ALARMS;
    g_print( "changed to alarms\n" );
}

void ChangeScreenClock( gpointer data ) {
    if ( screenId == SCREEN_CLOCK ) {
        SetDateAndTime();
        return;
    }
    
    CleanUpBody( screenId );
    ShowTopBar( SCREEN_CLOCK );

    SetDateAndTime();
    gtk_widget_show( GTK_WIDGET( labelTime ) );
    gtk_widget_show( GTK_WIDGET( labelDate ) );

    screenId = SCREEN_CLOCK;
}

void ChangeScreenSettings( gpointer data ) {
    if ( screenId == SCREEN_SETTINGS ) {
        return;
    }

    CleanUpBody( screenId );
    ShowTopBar( SCREEN_SETTINGS );
    screenId = SCREEN_SETTINGS;
    gtk_widget_show( GTK_WIDGET( labelVersion ) );
    gtk_widget_show( GTK_WIDGET( labelResetCuckoo ) );
    gtk_widget_show( GTK_WIDGET( buttonReset ) );

    g_print( "changed to settings\n" );
}

void CleanUpBody( int bodyId ) {
    switch ( bodyId ) {
    case SCREEN_NONE:
        g_print( "CleanUp Nothing\n" );
        break;
        
    case SCREEN_ALARMS:
        g_print( "CleanUp alarms" );
        gtk_widget_hide( GTK_WIDGET( alarmsWindow ) );
        break;

    case SCREEN_CLOCK:
        g_print( "CleanUp Clock\n" );
        gtk_widget_hide( GTK_WIDGET( labelTime ) );
        gtk_widget_hide( GTK_WIDGET( labelDate ) );
        break;

    case SCREEN_SETTINGS:
        gtk_widget_hide( GTK_WIDGET( labelVersion ) );
        gtk_widget_hide( GTK_WIDGET( buttonReset ) );
        gtk_widget_hide( GTK_WIDGET( labelResetCuckoo ) );
        break;

    case SCREEN_ALL:
        g_print( "CleanUp Everything\n" );
        CleanUpBody( SCREEN_ALARMS );
        CleanUpBody( SCREEN_CLOCK );
        CleanUpBody( SCREEN_SETTINGS );
        break;
    }
}

void CloseCuckoo( gpointer data ) {
    CleanUpBody( screenId );
    gtk_main_quit();
}
