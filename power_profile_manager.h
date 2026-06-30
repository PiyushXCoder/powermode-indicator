#ifndef POWER_PROFILE_MANAGER
#define POWER_PROFILE_MANAGER

#include <functional>
#include <giomm.h>
#include <glibmm.h>

class PowerProfileManager {
public:
  PowerProfileManager();
  ~PowerProfileManager();

  Glib::ustring get_profile();
  void set_profile(Glib::ustring profile);
  std::vector<Glib::ustring> get_all();
  void connect_profile_changed(std::function<void(Glib::ustring)> callback);

private:
  Glib::RefPtr<Gio::DBus::Connection> m_connection;
  Glib::RefPtr<Gio::DBus::Proxy> m_proxy;
  guint m_signal_id = 0;
};

#endif // !POWER_PROFILE_MANAGER
