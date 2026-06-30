#include "indicator.h"
#include <algorithm>
#include <cctype>
#include <filesystem>
#include <fstream>
#include <gio/gio.h>
#include <gtk/gtk.h>

static bool detect_dark_panel() {
  GtkSettings *settings = gtk_settings_get_default();
  gboolean prefer_dark = FALSE;
  g_object_get(settings, "gtk-application-prefer-dark-theme", &prefer_dark,
               nullptr);
  if (prefer_dark)
    return true;

  gchar *theme_name = nullptr;
  g_object_get(settings, "gtk-theme-name", &theme_name, nullptr);
  bool dark = false;
  if (theme_name) {
    std::string t = theme_name;
    std::transform(t.begin(), t.end(), t.begin(), ::tolower);
    dark = t.find("dark") != std::string::npos;
    g_free(theme_name);
  }
  return dark;
}

static bool system_has_icons() {
  GtkIconTheme *theme = gtk_icon_theme_get_default();
  return gtk_icon_theme_has_icon(theme, "speedometer") &&
         gtk_icon_theme_has_icon(theme, "battery-profile-powersave") &&
         gtk_icon_theme_has_icon(theme, "battery-profile-performance");
}

static void write_resource(const std::string &res, const std::string &dest) {
  GError *err = nullptr;
  GBytes *bytes =
      g_resources_lookup_data(res.c_str(), G_RESOURCE_LOOKUP_FLAGS_NONE, &err);
  if (!bytes) {
    if (err)
      g_error_free(err);
    return;
  }
  gsize size;
  const void *data = g_bytes_get_data(bytes, &size);
  std::ofstream f(dest, std::ios::binary);
  f.write(static_cast<const char *>(data), size);
  g_bytes_unref(bytes);
}

std::string Indicator::extract_icons_to_tmpdir() {
  if (system_has_icons())
    return {};

  char buf[] = "/tmp/powermode-XXXXXX";
  if (!mkdtemp(buf))
    return {};

  bool dark = detect_dark_panel();
  std::string base = buf;
  std::string icondir = base + "/hicolor/22x22/status";
  std::filesystem::create_directories(icondir);

  {
    std::ofstream f(base + "/hicolor/index.theme");
    f << "[Icon Theme]\nName=hicolor\nDirectories=22x22/status\n\n"
         "[22x22/status]\nSize=22\nType=Fixed\nContext=Status\n";
  }

  static const std::string RES =
      "/org/powermode-indicator/icons/hicolor/22x22/status/";

  write_resource(RES + (dark ? "speedometer-dark.svg" : "speedometer-light.svg"),
                 icondir + "/speedometer.svg");
  write_resource(RES + "battery-profile-balanced.svg",
                 icondir + "/battery-profile-balanced.svg");
  write_resource(RES + (dark ? "battery-profile-powersave.svg"
                              : "battery-profile-powersave-light.svg"),
                 icondir + "/battery-profile-powersave.svg");
  write_resource(RES + (dark ? "battery-profile-performance.svg"
                              : "battery-profile-performance-light.svg"),
                 icondir + "/battery-profile-performance.svg");

  return base;
}

Indicator::Indicator()
    : m_app_indicator(app_indicator_new("powermode-indicator", "",
                                        APP_INDICATOR_CATEGORY_HARDWARE)),
      m_icon_tmpdir(extract_icons_to_tmpdir()) {
  if (!m_icon_tmpdir.empty())
    app_indicator_set_icon_theme_path(m_app_indicator.get(),
                                      m_icon_tmpdir.c_str());
  gtk_icon_theme_add_resource_path(gtk_icon_theme_get_default(),
                                   "/org/powermode-indicator/icons");
  app_indicator_set_status(m_app_indicator.get(), APP_INDICATOR_STATUS_ACTIVE);

  g_assert(IS_APP_INDICATOR(m_app_indicator.get()));
  g_assert(G_IS_OBJECT(m_app_indicator.get()));
}

Indicator::~Indicator() {
  if (!m_icon_tmpdir.empty())
    std::filesystem::remove_all(m_icon_tmpdir);
}

void Indicator::add_menu(Gtk::Menu &menu) {
  app_indicator_set_menu(m_app_indicator.get(), menu.gobj());
}

void Indicator::change_icon(std::string icon) {
  app_indicator_set_icon(m_app_indicator.get(), icon.c_str());
}

void Indicator::set_tooltip(const std::string &text) {
  app_indicator_set_title(m_app_indicator.get(), text.c_str());
}
