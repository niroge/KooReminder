#ifndef _LIB_SCREENS_H
#define _LIB_SCREENS_H

#include <gtk/gtk.h>

#include <globalVariables.h>
#include <widgets.h>
#include <alarms.h>

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

void ShowTopBar( int id ) {
    GtkWidget *barButtons[] = { NULL, buttonClock, buttonAlarms, buttonSettings, NULL };
    
    selectedTopButton = barButtons[id];
}

void ChangeScreenAlarms( gpointer data ) {
    if ( screenId == SCREEN_ALARMS ) {
        /* insert another complex algorithm that shows alarms */
        if ( FileExists( UPDATE_FILE ) ) {
            system( "rm " UPDATE_FILE );
        }
        return;
    }

    CleanUpBody( screenId );
    ShowTopBar( SCREEN_ALARMS );
    LoadFiles();
    g_print( "Alarms screen pack\n" );
    gtk_scrolled_window_set_policy( GTK_SCROLLED_WINDOW( alarmsWindow ),
                                    GTK_POLICY_AUTOMATIC,
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


#endif
