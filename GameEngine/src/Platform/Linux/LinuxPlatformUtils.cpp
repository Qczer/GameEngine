#include "gepch.h"
#include "GameEngine/Utils/PlatformUtils.h"
#include "GameEngine/Core/Application.h"

#include <gtkmm.h>
#include <gtkmm/filechoosernative.h>

#include <cstdlib>

#include <cstdio>
#include <fcntl.h>
#include <unistd.h>
#include <limits.h>
#include <filesystem>

namespace GameEngine {

    std::optional<std::string> FileDialogs::OpenFile(const char* filter)
    {
        // Silencing warning for this function (the library causes the warnings so i cant fix them)
        int oldStderr = dup(fileno(stderr));
        freopen("/dev/null", "w", stderr);

        setenv("GSK_RENDERER", "cairo", 1);

        if (!Gtk::Application::get_default())
        {
            if (!gtk_init_check()) {
                return "";
            }
        }

        setlocale(LC_NUMERIC, "C");

        auto dialog = Gtk::FileChooserNative::create(
            "Open File",
            Gtk::FileChooser::Action::OPEN,
            "Accept",
            "Cancel");

		#ifdef GE_PROJECT_DIR
			std::filesystem::path projectDir = GE_PROJECT_DIR;
			std::filesystem::path startDir = projectDir / "GameEngine-Editor/assets";
		#else
			std::filesystem::path startDir = std::filesystem::current_path();
		#endif

        if (std::filesystem::exists(startDir))
            dialog->set_current_folder(Gio::File::create_for_path(startDir.string()));
        else
            GE_CORE_ERROR("Folder does not exist: {}", startDir.string());

        dialog->set_modal(true);

        auto engineFilter = Gtk::FileFilter::create();
        engineFilter->set_name("GameEngine Scene (*.gameengine)");
        engineFilter->add_pattern("*.gameengine");

        dialog->add_filter(engineFilter);

        auto allFilter = Gtk::FileFilter::create();
        allFilter->set_name("All Files");
        allFilter->add_pattern("*");
        dialog->add_filter(allFilter);

        dialog->set_filter(engineFilter);

        bool finished = false;
        int response_id = -1;

        dialog->signal_response().connect([&](int response) {
            response_id = response;
            finished = true;
        });

        dialog->show();

        while (!finished) {
            // Przetwórz jedno zdarzenie GTK (blocking = true oszczędza CPU)
            g_main_context_iteration(NULL, TRUE);
        }

        std::string result = "";
        bool success = false;

        if (response_id == Gtk::ResponseType::ACCEPT) {
            auto file = dialog->get_file();
            if (file) {
                result = file->get_path();
                success = true;
            }
        }

        dialog->hide();
        while (g_main_context_iteration(NULL, FALSE));

        setlocale(LC_ALL, "C");
        std::locale::global(std::locale("C"));

        dup2(oldStderr, fileno(stderr));
        close(oldStderr);

        if (success)
            return result;
        return std::nullopt;
    }

    std::optional<std::string> FileDialogs::SaveFile(const char* filter)
    {
        // Silencing GTK warnings
        int oldStderr = dup(fileno(stderr));
        freopen("/dev/null", "w", stderr);

        setenv("GSK_RENDERER", "cairo", 1);

        if (!Gtk::Application::get_default())
        {
            if (!gtk_init_check()) {
                return "";
            }
        }

        setlocale(LC_NUMERIC, "C");

        auto dialog = Gtk::FileChooserNative::create(
            "Save File",
            Gtk::FileChooser::Action::SAVE,
            "Save",
            "Cancel");

		#ifdef GE_PROJECT_DIR
			std::filesystem::path projectDir = GE_PROJECT_DIR;
			std::filesystem::path startDir = projectDir / "GameEngine-Editor/assets";
		#else
			std::filesystem::path startDir = std::filesystem::current_path();
		#endif

        if (std::filesystem::exists(startDir))
            dialog->set_current_folder(Gio::File::create_for_path(startDir.string()));
        else
            GE_CORE_ERROR("Folder does not exist: {}", startDir.string());

        dialog->set_modal(true);

        auto engineFilter = Gtk::FileFilter::create();
        engineFilter->set_name("GameEngine Scene (*.gameengine)");
        engineFilter->add_pattern("*.gameengine");
        dialog->add_filter(engineFilter);

        auto allFilter = Gtk::FileFilter::create();
        allFilter->set_name("All Files");
        allFilter->add_pattern("*");
        dialog->add_filter(allFilter);

        dialog->set_filter(engineFilter);

        bool finished = false;
        int response_id = -1;

        dialog->signal_response().connect([&](int response) {
            response_id = response;
            finished = true;
        });

        dialog->show();

        while (!finished) {
            g_main_context_iteration(NULL, TRUE);
        }

        std::string result = "";
        bool success = false;

        if (response_id == Gtk::ResponseType::ACCEPT) {
            auto file = dialog->get_file();
            if (file) {
                result = file->get_path();
                success = true;
            }
        }

        dialog->hide();
        while (g_main_context_iteration(NULL, FALSE));

        setlocale(LC_ALL, "C");
        std::locale::global(std::locale("C"));

        dup2(oldStderr, fileno(stderr));
        close(oldStderr);

        if (success)
            return result;
        return std::nullopt;
    }
}