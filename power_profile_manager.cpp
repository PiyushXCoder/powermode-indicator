#include "power_profile_manager.h"

#include <iostream>

PowerProfileManager::PowerProfileManager() {
  Gio::init();
  Glib::RefPtr<Gio::DBus::Connection> connection =
      Gio::DBus::Connection::get_sync(Gio::DBus::BusType::BUS_TYPE_SYSTEM);

  if (!connection) {
    std::cerr << "System Bus not available" << std::endl;
    exit(1);
  }

  this->m_proxy = Gio::DBus::Proxy::create_sync(
      connection, "org.freedesktop.UPower.PowerProfiles",
      "/org/freedesktop/UPower/PowerProfiles",
      "org.freedesktop.DBus.Properties");
}

Glib::ustring PowerProfileManager::get_profile() {
  std::vector<Glib::VariantBase> params;
  params.push_back(Glib::Variant<Glib::ustring>::create(
      "org.freedesktop.UPower.PowerProfiles"));
  params.push_back(Glib::Variant<Glib::ustring>::create("ActiveProfile"));

  auto params_container = Glib::VariantContainerBase::create_tuple(params);
  auto active_profile = this->m_proxy->call_sync("Get", params_container);

  Glib::VariantIter iterator1(active_profile);
  Glib::VariantContainerBase container1;
  if (!iterator1.next_value(container1)) {
    std::cerr << "Dbus output is ambiguous!" << std::endl;
    std::cerr << active_profile.print();
  }

  Glib::VariantIter iterator2(container1);
  Glib::Variant<Glib::ustring> profile;
  if (!iterator2.next_value(profile)) {
    std::cerr << "Dbus output is ambiguous!" << std::endl;
    std::cerr << active_profile.print();
  }

  return profile.get();
}

int PowerProfileManager::set_profile(Glib::ustring profile) { return 0; }
