#ifndef POWER_PROFILE_MANAGER
#define POWER_PROFILE_MANAGER

#include <giomm.h>
#include <glibmm.h>

class PowerProfileManager {
public:
  PowerProfileManager();

  ~PowerProfileManager();

  Glib::ustring get_profile();
  void set_profile(Glib::ustring profile);
  std::vector<Glib::ustring> get_all();

private:
  Glib::RefPtr<Gio::DBus::Proxy> m_proxy;
};

#endif // !POWER_PROFILE_MANAGER
