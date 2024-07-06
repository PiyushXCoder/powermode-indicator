#include "app_menu.h"

AppMenu::AppMenu(AppIndicator *indicator)
    : m_indicator(indicator), m_performance_menu("Performance"),
      m_balanced_menu("Balanced"), m_power_saver_menu("Power Saver"), group1() {
  // app_indicator_set_icon(m_indicator, "speedometer");
  // app_indicator_set_icon(m_indicator, "battery-profile-powersave");
  app_indicator_set_icon(m_indicator, "battery-profile-performance");

  m_performance_menu.set_group(group1);
  m_balanced_menu.set_group(group1);
  m_power_saver_menu.set_group(group1);

  add(m_performance_menu);
  add(m_balanced_menu);
  add(m_power_saver_menu);

  show_all();
}

AppMenu::~AppMenu() {}

void AppMenu::update() {
  // TODO: Use Dbus to fetch data
}
