#include "power_profile_manager.h"

#include <iostream>

#define assert_ambiguous_output(condition, data, ret)                          \
  if (condition) {                                                             \
    std::cerr << "Dbus output is ambiguous!" << std::endl;                     \
    std::cerr << data.print();                                                 \
    return ret;                                                                \
  }

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

PowerProfileManager::~PowerProfileManager() {}

Glib::ustring PowerProfileManager::get_profile() {
  std::vector<Glib::VariantBase> params;
  params.push_back(Glib::Variant<Glib::ustring>::create(
      "org.freedesktop.UPower.PowerProfiles"));
  params.push_back(Glib::Variant<Glib::ustring>::create("ActiveProfile"));
  auto params_container = Glib::VariantContainerBase::create_tuple(params);
  auto result = this->m_proxy->call_sync("Get", params_container);

  assert_ambiguous_output(result.get_type_string() != "(v)", result, "");
  Glib::Variant<Glib::ustring> profile;
  assert_ambiguous_output(
      !Glib::VariantIter(result.get_child()).next_value(profile), result, "");
  return profile.get();
}

void PowerProfileManager::set_profile(Glib::ustring profile) {
  std::vector<Glib::VariantBase> params;
  params.push_back(Glib::Variant<Glib::ustring>::create(
      "org.freedesktop.UPower.PowerProfiles"));
  params.push_back(Glib::Variant<Glib::ustring>::create("ActiveProfile"));
  params.push_back(Glib::Variant<Glib::Variant<Glib::ustring>>::create(
      Glib::Variant<Glib::ustring>(
          Glib::Variant<Glib::ustring>::create(profile))));
  auto params_container = Glib::VariantContainerBase::create_tuple(params);
  auto result = this->m_proxy->call_sync("Set", params_container);
  assert_ambiguous_output(result.get_type_string() != "()", result, );
  assert_ambiguous_output(Glib::VariantIter(result).get_n_children() != 0,
                          result, );
}

std::vector<Glib::ustring> PowerProfileManager::get_all() {
  std::vector<Glib::ustring> profiles_available;

  std::vector<Glib::VariantBase> params;
  params.push_back(Glib::Variant<Glib::ustring>::create(
      "org.freedesktop.UPower.PowerProfiles"));
  auto params_container = Glib::VariantContainerBase::create_tuple(params);
  auto result = this->m_proxy->call_sync("GetAll", params_container);

  assert_ambiguous_output(result.get_type_string() != "(a{sv})", result,
                          profiles_available);

  Glib::Variant<std::map<Glib::ustring, Glib::VariantBase>> main_struct;
  result.get_child(main_struct);
  assert_ambiguous_output(main_struct.get().count("Profiles") != 1, result,
                          profiles_available);

  Glib::VariantIter profiles((main_struct.get())["Profiles"]);
  Glib::Variant<std::map<Glib::ustring, Glib::VariantBase>> profile;
  while (profiles.next_value(profile)) {
    assert_ambiguous_output(profile.get().count("Profile") != 1, result,
                            profiles_available);
    auto prof = Glib::VariantBase::cast_dynamic<Glib::Variant<Glib::ustring>>(
        (profile.get())["Profile"]);
    profiles_available.push_back(prof.get());
  }

  return profiles_available;
}
