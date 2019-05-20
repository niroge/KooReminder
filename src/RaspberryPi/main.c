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

#include <globalVariables.h>
#include <functionsDeclaration.h>
#include <alarms.h>
#include <screens.h>
#include <widgets.h>

#define VERSION "0.0.1"

#define FULLSCREEN
#undef  FULLSCREEN

int main( int argc, char ** argv ) {
    gtk_init( &argc, &argv );
    
    buttonClock    = gtk_button_new_with_label( "Date & Time" );
    buttonAlarms   = gtk_button_new_with_label( "Alarms" );
    buttonSettings = gtk_button_new_with_label( "Settings" );
    buttonReset    = gtk_button_new_with_label( "Reset KooReminder" );

    window       = gtk_window_new( GTK_WINDOW_TOPLEVEL );
    alarmsWindow = gtk_scrolled_window_new( NULL, NULL );

    gtk_window_set_default_size( GTK_WINDOW( window ), 480, 320 );
    gtk_window_set_resizable( GTK_WINDOW( window ), FALSE );
    
    hboxTopBar      = gtk_box_new( 0, 0 );
    vboxPageContent = gtk_box_new( 1, 0 );
    alarmsBox       = gtk_box_new( 1, 0 );
    
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
    
    LoadWidgets();
    ApplyCSSNaming();

    gtk_widget_show_all( window );
    g_timeout_add( 100, DrawScreen, NULL );
    gtk_main();

    return 0;
}
