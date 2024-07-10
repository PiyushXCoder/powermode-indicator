#include "app_menu.h"
#include <giomm.h>
#include <glibmm.h>
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

  Glib::RefPtr<Glib::MainLoop> loop = Glib::MainLoop::create();
  loop->run();
  return 0;
}
