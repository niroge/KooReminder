#ifndef _LIB_GLOBALVARIABLES_H
#define _LIB_GLOBALVARIABLES_H

#define UPDATE_FILE "/tmp/updateAlarmsKooreminder.dat"

#include <gtk/gtk.h>

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

#endif
