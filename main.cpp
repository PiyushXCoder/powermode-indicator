#include "app_menu.h"
#include <glib-2.0/glib.h>
#include <glibmm/refptr.h>
#include <gtkmm/application.h>
#include <libappindicator/app-indicator.h>

int main(int argc, char *argv[]) {
  Gtk::Application::create(argc, argv);

  AppIndicator *indicator = app_indicator_new("powermode-indicator", "",
                                              APP_INDICATOR_CATEGORY_HARDWARE);
  g_assert(IS_APP_INDICATOR(indicator));
  g_assert(G_IS_OBJECT(indicator));

  app_indicator_set_status(indicator, APP_INDICATOR_STATUS_ACTIVE);

  AppMenu app_menu(indicator);

  app_indicator_set_menu(indicator, app_menu.gobj());

  GMainLoop *mainloop = g_main_loop_new(NULL, FALSE);
  g_main_loop_run(mainloop);

  return 0;
}
