#ifndef APP_INDICATOR_H
#define APP_INDICATOR_H

#include <glibmm.h>
#include <gtkmm.h>
#include <libappindicator/app-indicator.h>
#include <memory>
#include <string>

class Indicator {
public:
  Indicator();
  ~Indicator();

  void add_menu(Gtk::Menu &);
  void change_icon(std::string icon);

private:
  std::unique_ptr<AppIndicator> m_app_indicator;
  std::string m_icon_tmpdir;

  std::string extract_icons_to_tmpdir();
};

#endif // !APP_INDICATOR_H
