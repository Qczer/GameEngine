#include "gepch.h"
#include "GameEngine/Utils/PlatformUtils.h"
#include "GameEngine/Core/Application.h"

#include <gtk/gtk.h>

namespace GameEngine {

    std::string OpenFile(const char* filter)
    {
        GtkFileChooserNative *native = gtk_file_chooser_native_new(
            "Wybierz plik",
            //Application::GetWindow()->GetNativeWindow(),
            nullptr,
            GTK_FILE_CHOOSER_ACTION_OPEN,
            "_Open",
            "_Cancel"
        );

        gint res = gtk_native_dialog_run(GTK_NATIVE_DIALOG(native));

        if (res == GTK_RESPONSE_ACCEPT)
        {
            char *filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(native));
            g_print("Wybrano: %s\n", filename);
            g_free(filename);

            g_object_unref(native);
        }

        return "";
    }

    std::string SaveFile(const char* filter)
    {
        return "";
    }
}