#include "indicator.h"
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <gio/gio.h>
#include <gtk/gtk.h>

static const char *ICON_NAMES[] = {
    "battery-profile-balanced",
    "battery-profile-powersave",
    "battery-profile-performance",
    nullptr,
};

std::string Indicator::extract_icons_to_tmpdir() {
  char buf[] = "/tmp/powermode-XXXXXX";
  if (!mkdtemp(buf))
    return {};

  std::string icondir = std::string(buf) + "/hicolor/22x22/status";
  std::filesystem::create_directories(icondir);

  {
    std::ofstream f(std::string(buf) + "/hicolor/index.theme");
    f << "[Icon Theme]\nName=hicolor\nDirectories=22x22/status\n\n"
         "[22x22/status]\nSize=22\nType=Fixed\nContext=Status\n";
  }

  for (int i = 0; ICON_NAMES[i]; i++) {
    std::string res = std::string("/org/powermode-indicator/icons/hicolor"
                                  "/22x22/status/") +
                      ICON_NAMES[i] + ".svg";
    GError *err = nullptr;
    GBytes *bytes = g_resources_lookup_data(res.c_str(),
                                            G_RESOURCE_LOOKUP_FLAGS_NONE, &err);
    if (!bytes) {
      if (err)
        g_error_free(err);
      continue;
    }
    gsize size;
    const void *data = g_bytes_get_data(bytes, &size);
    std::ofstream f(icondir + "/" + ICON_NAMES[i] + ".svg", std::ios::binary);
    f.write(static_cast<const char *>(data), size);
    g_bytes_unref(bytes);
  }

  return buf;
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
