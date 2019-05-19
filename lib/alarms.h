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
    char *  time;
    char *  title;
    uint8_t repeat;

    GtkWidget * boxRow;
    GtkWidget * boxCol;
    GtkWidget * labelAlarm;
    GtkWidget * labelRepeat;
    GtkWidget * labelTitle;
};

/* Function declaration */
void LoadAlarm( const char * alarmRow ) {
    char           buffer[1024];
    char repeatBuffer[8]    = { 0 };
    struct alarm_t tmpAlarm;
    time_t         secondsSinceEpoch;
    const char *   weekDays = "MTWTFSD";

    time( &secondsSinceEpoch );
    alarmsPointer = realloc( alarmsPointer, sizeof( struct alarm_t ) * (totalAlarms + 1) );
    memset( &tmpAlarm, '\0', sizeof tmpAlarm );

    sscanf( alarmRow, "%s %hhd %s", tmpAlarm.time, &tmpAlarm.repeat, tmpAlarm.title );
    tmpAlarm.labelAlarm = gtk_label_new( tmpAlarm.time );

    for ( int i = 0b00000001; i <= 0b01000000; i <<= 1 ) {
        if ( i & tmpAlarm.repeat ) {
            repeatBuffer[i/2] = weekDays[i/2];
        } else {
            repeatBuffer[i/2] = ' ';
        }
    }

    g_print( "Repeat: %s\n", repeatBuffer );
    tmpAlarm.labelRepeat = gtk_label_new( repeatBuffer );
    tmpAlarm.labelTitle = gtk_label_new( tmpAlarm.title );
    
    /* Insert a complex algorithm */

    /* Copy the tmp buffer to the alarms segment in memory */
    memcpy( alarmsPointer + sizeof( struct alarm_t ) * totalAlarms, &tmpAlarm, sizeof( struct alarm_t ) );
}

void LoadAlarms( void ) {
    char buffer[1024];
    struct alarm_t * alarmPointer = alarmsPointer;

    g_print( "Before cleanup\n" );
    /* Cleanup alarmsPointer */
    while ( totalAlarms ) {
        gtk_widget_destroy( alarmPointer->labelAlarm );
        gtk_widget_destroy( alarmPointer->labelRepeat );
        gtk_widget_destroy( alarmPointer->labelTitle );
        gtk_widget_destroy( alarmPointer->boxCol );
        gtk_widget_destroy( alarmPointer->boxRow );
        alarmPointer++;
        totalAlarms--;
    }

    free( alarmsPointer );

    /* Load the new alarms */
    alarmsFilePointer = fopen( ALARMS_FILE, "rw" );
    
    while ( fgets( buffer, sizeof buffer, alarmsFilePointer ) != NULL ) {
        LoadAlarm( buffer );
        totalAlarms++;
    }

    g_print( "Amount of alarms: %d\n", totalAlarms );
}

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
