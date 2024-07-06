#ifndef APP_WINDOW_H
#define APP_WINDOW_H

#include <gtkmm/menu.h>
#include <gtkmm/radiobuttongroup.h>
#include <gtkmm/radiomenuitem.h>
#include <libappindicator/app-indicator.h>

class AppMenu : public Gtk::Menu {
public:
  AppMenu(AppIndicator *indicator);
  virtual ~AppMenu();

private:
  AppIndicator *m_indicator;

  Gtk::RadioButtonGroup group1;

  Gtk::RadioMenuItem m_performance_menu;
  Gtk::RadioMenuItem m_balanced_menu;
  Gtk::RadioMenuItem m_power_saver_menu;

  static void update();
};

#endif // !APP_WINDOW_H
