#ifndef APP_MENU_H
#define APP_MENU_H

#include "indicator.h"
#include "power_profile_manager.h"
#include <gtkmm/menu.h>
#include <gtkmm/radiobuttongroup.h>
#include <gtkmm/radiomenuitem.h>
#include <memory>

class AppMenu : public Gtk::Menu {
public:
  AppMenu(std::shared_ptr<Indicator> indicator);
  virtual ~AppMenu();

private:
  std::shared_ptr<Indicator> m_indicator;

  Gtk::RadioButtonGroup m_group1;

  Gtk::RadioMenuItem m_performance_menu_item;
  Gtk::RadioMenuItem m_balanced_menu_item;
  Gtk::RadioMenuItem m_power_saver_menu_item;

  PowerProfileManager m_power_profile_manager;

  Glib::ustring m_current_profile;

  void update();
};

#endif // !APP_MENU_H
