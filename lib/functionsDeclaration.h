#ifndef _LIB_FUNCTIONSDECLARATION_H
#define _LIB_FUNCTIONSDECLARATION_H

/* for gpointer */
#include <gtk/gtk.h>

struct alarm_t;

/* alarms.h */
void LoadAlarm( const char * );
void LoadAlarms( void );
bool FileExists( const char * );
void ResetCuckoo( gpointer );

/* screens.h */
int DrawScreen( gpointer );
void ShowTopBar( int );
void CleanUpBody( int );
void ChangeScreenAlarms( gpointer );
void ChangeScreenClock( gpointer );
void ChangeScreenSettings( gpointer );

/* widgets.h */
void LoadFiles( void );
void ApplyCSSNaming( void );
void LoadWidgets( void );
void SetDateAndTime( void );
void CloseCuckoo( gpointer );

#endif
