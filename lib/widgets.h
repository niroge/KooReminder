#ifndef _LIB_WIDGETS_H
#define _LIB_WIDGETS_H

#include <globalVariables.h>

#include <gtk/gtk.h>
#include <time.h>

void ApplyCSSNaming( void ) {
    /* Only IDs, classes are declared in LoadFiles */
    gtk_widget_set_name( GTK_WIDGET( window ), "id_window" );
    gtk_widget_set_name( GTK_WIDGET( alarmsWindow ), "id_alarms_screen" );
    gtk_widget_set_name( GTK_WIDGET( buttonClock ), "id_buttonClock" );
    gtk_widget_set_name( GTK_WIDGET( buttonAlarms ), "id_buttonAlarms" );
    gtk_widget_set_name( GTK_WIDGET( buttonSettings ), "id_buttonSettings" );
    gtk_widget_set_name( GTK_WIDGET( buttonReset ), "id_buttonReset" );
    gtk_widget_set_name( GTK_WIDGET( vboxPageContent ), "id_body" );
    gtk_widget_set_name( GTK_WIDGET( hboxTopBar ), "id_topBar" );
    gtk_widget_set_name( GTK_WIDGET( labelDate ), "id_labelDate" );
    gtk_widget_set_name( GTK_WIDGET( labelTime ), "id_labelTime" );
    gtk_widget_set_name( GTK_WIDGET( selectedTopButton ), "id_barSelected" );
    gtk_widget_set_name( GTK_WIDGET( labelVersion ), "id_labelVersion" );
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
        LoadAlarms();
    } else {
        g_print( "Alarms file missing!\n" );
    }
}

void LoadWidgets( void ) {
    const int expand = 0, fill = 1, padding = 0;
    g_print( "Loading widgets!\n" );
    gtk_container_add( GTK_CONTAINER( window ), vboxPageContent );
    gtk_container_add( GTK_CONTAINER( alarmsWindow ), alarmsBox );

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

void CloseCuckoo( gpointer data ) {
    CleanUpBody( screenId );
    gtk_main_quit();
}

#endif
