#include "app_menu.h"
#include <glibmm.h>
#include <gtkmm.h>
#include <iostream>

void run() {}

AppMenu::AppMenu(AppIndicator *indicator)
    : m_indicator(indicator), m_performance_menu_item("Performance"),
      m_balanced_menu_item("Balanced"), m_power_saver_menu_item("Power Saver"),
      m_group1(), m_power_profile_manager(), m_current_profile("balanced") {

  m_balanced_menu_item.set_group(m_group1);
  m_power_saver_menu_item.set_group(m_group1);
  m_performance_menu_item.set_group(m_group1);

  add(m_balanced_menu_item);
  add(m_power_saver_menu_item);
  add(m_performance_menu_item);
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

  m_balanced_menu_item.signal_activate().connect(
      [this]() { this->m_power_profile_manager.set_profile("balanced"); });

  m_power_saver_menu_item.signal_activate().connect(
      [this]() { this->m_power_profile_manager.set_profile("power-saver"); });

  m_performance_menu_item.signal_activate().connect(
      [this]() { this->m_power_profile_manager.set_profile("performance"); });
}

AppMenu::~AppMenu() {}

void AppMenu::update() {
  if (this->m_current_profile == "balanced") {
    app_indicator_set_icon(m_indicator, "speedometer");
    this->m_balanced_menu_item.set_active();
  } else if (this->m_current_profile == "power-saver") {
    app_indicator_set_icon(m_indicator, "battery-profile-powersave");
    this->m_power_saver_menu_item.set_active();
  } else if (this->m_current_profile == "performance") {
    app_indicator_set_icon(m_indicator, "battery-profile-performance");
    this->m_performance_menu_item.set_active();
  }
}
