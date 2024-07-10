#ifndef APP_MENU_H
#define APP_MENU_H

#include "power_profile_manager.h"
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

  Gtk::RadioButtonGroup m_group1;

  Gtk::RadioMenuItem m_performance_menu;
  Gtk::RadioMenuItem m_balanced_menu;
  Gtk::RadioMenuItem m_power_saver_menu;

  PowerProfileManager m_power_profile_manager;

  Glib::ustring m_current_profile;

  void update();
};

#endif // !APP_MENU_H
