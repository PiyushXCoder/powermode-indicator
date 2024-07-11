#include "app_menu.h"
#include "indicator.h"
#include <giomm.h>
#include <glibmm.h>
#include <gtkmm/application.h>
#include <libappindicator/app-indicator.h>

int main(int argc, char *argv[]) {
  Glib::RefPtr<Gtk::Application> app = Gtk::Application::create(argc, argv);

  std::shared_ptr<Indicator> indicator(new Indicator);

  AppMenu app_menu(indicator);

  indicator.get()->add_menu(app_menu);

  Glib::RefPtr<Glib::MainLoop> loop = Glib::MainLoop::create();
  loop->run();

  return 0;
}
