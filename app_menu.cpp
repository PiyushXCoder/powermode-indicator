#include "app_menu.h"
#include <glibmm.h>
#include <gtkmm.h>

void run() {}

AppMenu::AppMenu(AppIndicator *indicator)
    : m_indicator(indicator), m_performance_menu("Performance"),
      m_balanced_menu("Balanced"), m_power_saver_menu("Power Saver"),
      m_group1(), m_power_profile_manager(), m_current_profile("balanced") {

  m_performance_menu.set_group(m_group1);
  m_balanced_menu.set_group(m_group1);
  m_power_saver_menu.set_group(m_group1);

  add(m_performance_menu);
  add(m_balanced_menu);
  add(m_power_saver_menu);
  this->update();

  Glib::signal_timeout().connect(
      [this]() {
        auto active_profile = this->m_power_profile_manager.get_profile();
        if (this->m_current_profile != active_profile) {
          this->m_current_profile = active_profile;
          this->update();
        }
        return true;
      },
      1000);
  show_all();
}

AppMenu::~AppMenu() {}

void AppMenu::update() {
  if (this->m_current_profile == "balanced") {
    app_indicator_set_icon(m_indicator, "speedometer");
    this->m_balanced_menu.select();
  } else if (this->m_current_profile == "power-saver") {
    app_indicator_set_icon(m_indicator, "battery-profile-powersave");
    this->m_power_saver_menu.select();
  } else if (this->m_current_profile == "performance") {
    app_indicator_set_icon(m_indicator, "battery-profile-performance");
    this->m_performance_menu.select();
  }
}
