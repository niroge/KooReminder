#ifndef _LIB_ALARMS_H
#define _LIB_ALARMS_H

#include <gtk/gtk.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>

#include <globalVariables.h>
#include <screens.h>

/* Alarm structure */
struct alarm_t {
    char * time;
    char * title;
    char   repeat[8];

    GtkWidget * boxRow;
    GtkWidget * boxCol;
    GtkWidget * labelTime;
    GtkWidget * labelRepeat;
    GtkWidget * labelTitle;
};

/* Function declaration */
void LoadAlarm( const char * alarmRow ) {
    char           buffer[1024];
    char repeatBuffer[8]    = { 0 };
    struct alarm_t tmpAlarm;
    time_t         secondsSinceEpoch;

    time( &secondsSinceEpoch );
    alarmsPointer = realloc( alarmsPointer, sizeof( struct alarm_t ) * (totalAlarms + 1) );
    memset( &tmpAlarm, '\0', sizeof tmpAlarm );

    sscanf( alarmRow, "%s %7c %s", tmpAlarm.time, tmpAlarm.repeat, tmpAlarm.title );
    tmpAlarm.labelTime = gtk_label_new( tmpAlarm.time );
    tmpAlarm.labelRepeat = gtk_label_new( tmpAlarm.repeat );
    tmpAlarm.labelTitle = gtk_label_new( tmpAlarm.title );
    tmpAlarm.boxRow = gtk_box_new( 0, 0 );
    tmpAlarm.boxCol = gtk_box_new( 1, 0 );
    gtk_box_pack_start( GTK_BOX( alarmsBox ), tmpAlarm.boxRow, false, true, false );
    gtk_box_pack_start( GTK_BOX( tmpAlarm.boxRow ), tmpAlarm.boxCol, false, false, false );
    gtk_box_pack_start( GTK_BOX( tmpAlarm.boxRow ), tmpAlarm.labelTitle, false, false, false );
    gtk_box_pack_start( GTK_BOX( tmpAlarm.boxCol ), tmpAlarm.labelTime, false, false, false );
    gtk_box_pack_start( GTK_BOX( tmpAlarm.boxCol ), tmpAlarm.labelRepeat, false, false, false );

    tmpAlarm.labelRepeat = gtk_label_new( repeatBuffer );
    tmpAlarm.labelTitle = gtk_label_new( tmpAlarm.title );
    
    /* Insert a complex algorithm to check time */
    /* LMFAO not today */

    /* Copy the tmp buffer to the alarms segment in memory */
    memcpy( alarmsPointer + sizeof( struct alarm_t ) * totalAlarms, &tmpAlarm, sizeof( struct alarm_t ) );
    g_print( "Loaded alarm: %s %s %s\n", tmpAlarm.time, tmpAlarm.repeat, tmpAlarm.title );
}

void LoadAlarms( void ) {
    char buffer[1024];
    struct alarm_t * alarmPointer = alarmsPointer;

    g_print( "Before cleanup\n" );
    /* Cleanup alarmsPointer */
    if ( alarmsPointer ) {
        g_print( "sizeof alarm_t -> %lu\n", sizeof( struct alarm_t ) );
        while ( totalAlarms-- ) {
            g_print( "In the loop\n" );
            g_print( "[%p] alarm pointer\n", alarmPointer );
            gtk_widget_destroy( alarmPointer->labelTime );
            gtk_widget_destroy( alarmPointer->labelRepeat );
            gtk_widget_destroy( alarmPointer->labelTitle );
            gtk_widget_destroy( alarmPointer->boxCol );
            gtk_widget_destroy( alarmPointer->boxRow );
            alarmPointer++;
        }

        g_print( "Out of the loop\n" );
        
        free( alarmsPointer );
    }

    g_print( "Allocating memory...\n" );
    alarmsPointer = malloc( sizeof( struct alarm_t ) );

    /* Load the new alarms */
    if ( alarmsFilePointer ) {
        fclose( alarmsFilePointer );
    }
    
    alarmsFilePointer = fopen( ALARMS_FILE, "rw" );
    if ( alarmsFilePointer == NULL ) {
        g_print( "Cannot open alarmsFilePointer\n" );
        return;
    }
    
    while ( fgets( buffer, sizeof buffer, alarmsFilePointer ) != NULL ) {
        LoadAlarm( buffer );
        totalAlarms++;
    }

    g_print( "Amount of alarms: %d\n", totalAlarms );
}

/* void LoadAlarm( const char * fileRow ) { */
/*     g_print( "Called load_alarm for %s\n", fileRow ); */
/* } */

/* void LoadAlarms( void ) { */
/*     char buffer[1024]; */
/*     struct alarm_t * alarmPointer = alarmsPointer; */
/*     static bool firstTime = true; */
    
/*     if ( !FileExists( ALARMS_FILE ) ) { */
/*         g_print( "Alarms file missing!\n" ); */
/*     } */

/*     if ( firstTime ) { */
/*         firstTime = false; */
/*         totalAlarms = 0; */
/*     } */

/*     g_print( "Alarms file present! Cleaning up memory...\n" ); */

/*     while ( totalAlarms-- > 0 ) { */
/*         gtk_widget_destroy( alarmPointer->labelTitle ); */
/*         gtk_widget_destroy( alarmPointer->labelTime ); */
/*         gtk_widget_destroy( alarmPointer->labelRepeat ); */
/*         gtk_widget_destroy( alarmPointer->boxCol ); */
/*         gtk_widget_destroy( alarmPointer->boxRow ); */
/*         alarmPointer++; */
/*     } */

/*     g_print( "Cleaning up memory...\n" ); */
/*     free( alarmsPointer ); */

/*     g_print( "opening alarms file\n" ); */
/*     /\* if ( alarmsFilePointer ) { *\/ */
/*     /\*     g_print( "closing it first...\n" ); *\/ */
/*     /\*     fclose( alarmsFilePointer ); *\/ */
/*     /\* } *\/ */

/*     /\* alarmsFilePointer = fopen( ALARMS_FILE, "r" ); *\/ */

/*     if ( alarmPointer == NULL ) { */
/*         g_print( "Error opening alarms file!\n" ); */
/*         return; */
/*     } */
    
/*     while ( fgets( buffer, sizeof buffer, alarmsFilePointer ) ) { */
/*         LoadAlarm( buffer ); */
/*     } */

/*     /\* fclose( alarmsFilePointer ); *\/ */
/* } */

bool FileExists( const char * fileName ) {
    FILE * pointer = fopen( fileName, "r" );
    if ( pointer ) {
        fclose( pointer );
        return true;
    }

    return false;
}

void ResetCuckoo( gpointer data ) {
    /* system( "rm theme.css" ); */
    /* system( "rm alarms.dat" ); */

    CloseCuckoo( NULL );
}

#endif
