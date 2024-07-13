#include "app_menu.h"
#include <glibmm.h>
#include <gtkmm.h>

void run() {}

AppMenu::AppMenu(std::shared_ptr<Indicator> indicator)
    : m_performance_menu_item("Performance"), m_balanced_menu_item("Balanced"),
      m_power_saver_menu_item("Power Saver"), m_group1(),
      m_power_profile_manager(), m_current_profile("balanced") {
  auto profiles_available = m_power_profile_manager.get_all();
  m_indicator = indicator;

  m_balanced_menu_item.set_group(m_group1);
  m_power_saver_menu_item.set_group(m_group1);
  m_performance_menu_item.set_group(m_group1);

  if (std::find(profiles_available.begin(), profiles_available.end(),
                "balanced") != profiles_available.end())
    add(m_balanced_menu_item);
  if (std::find(profiles_available.begin(), profiles_available.end(),
                "power-saver") != profiles_available.end())
    add(m_power_saver_menu_item);
  if (std::find(profiles_available.begin(), profiles_available.end(),
                "performance") != profiles_available.end())
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
    m_indicator.get()->change_icon("speedometer");
    this->m_balanced_menu_item.set_active();
  } else if (this->m_current_profile == "power-saver") {
    m_indicator.get()->change_icon("battery-profile-powersave");
    this->m_power_saver_menu_item.set_active();
  } else if (this->m_current_profile == "performance") {
    m_indicator.get()->change_icon("battery-profile-performance");
    this->m_performance_menu_item.set_active();
  }
}
