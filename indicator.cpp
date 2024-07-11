#include "indicator.h"

Indicator::Indicator()
    : m_app_indicator(app_indicator_new("powermode-indicator", "",
                                        APP_INDICATOR_CATEGORY_HARDWARE)) {
  app_indicator_set_status(m_app_indicator.get(), APP_INDICATOR_STATUS_ACTIVE);

  g_assert(IS_APP_INDICATOR(m_app_indicator.get()));
  g_assert(G_IS_OBJECT(m_app_indicator.get()));
}

Indicator::~Indicator() {}

void Indicator::add_menu(Gtk::Menu &menu) {
  app_indicator_set_menu(m_app_indicator.get(), menu.gobj());
}

void Indicator::change_icon(std::string icon) {
  app_indicator_set_icon(m_app_indicator.get(), icon.c_str());
}
