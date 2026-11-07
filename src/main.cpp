#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdl3.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_properties.h>
#include <SDL3/SDL_video.h>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <string>

#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <SDL3/SDL_opengles2.h>
#else
#include <SDL3/SDL_opengl.h>
#endif

#ifdef __EMSCRIPTEN__
#include "../libs/emscripten/emscripten_mainloop_stub.h"
#endif

using namespace std;
// Main code
int main(int, char **) {
    // Setup SDL
    // [If using SDL_MAIN_USE_CALLBACKS: all code below until the main loop
    // starts would likely be your SDL_AppInit() function]
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD)) {
        printf("Error: SDL_Init(): %s\n", SDL_GetError());
        return 1;
    }

    // Select GL version + let the backend select a GLSL version
    const char *glsl_version = nullptr;
#if defined(IMGUI_IMPL_OPENGL_ES2)
    // GL ES 2.0 + GLSL 100 (WebGL 1.0)
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#elif defined(IMGUI_IMPL_OPENGL_ES3)
    // GL ES 3.0 + GLSL 300 es (WebGL 2.0)
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#elif defined(__APPLE__)
    // GL 3.2 Core + generally GLSL 150
    SDL_GL_SetAttribute(
        SDL_GL_CONTEXT_FLAGS,
        SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG); // Always required on Mac
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
                        SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
#else
    // GL 3.0 + generally GLSL 130
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
                        SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#endif

    // Create window with graphics context
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    SDL_WindowFlags window_flags = SDL_WINDOW_OPENGL | SDL_WINDOW_TRANSPARENT;
    SDL_PropertiesID props = SDL_CreateProperties();
    SDL_SetBooleanProperty(props, SDL_PROP_WINDOW_CREATE_TRANSPARENT_BOOLEAN,
                           true);
    SDL_Window *window =
        SDL_CreateWindow("Notes", (int)(300), (int)(300), window_flags);
    if (window == nullptr) {
        printf("Error: SDL_CreateWindow(): %s\n", SDL_GetError());
        return 1;
    }
    SDL_GLContext gl_context = SDL_GL_CreateContext(window);
    if (gl_context == nullptr) {
        printf("Error: SDL_GL_CreateContext(): %s\n", SDL_GetError());
        return 1;
    }

    SDL_GL_MakeCurrent(window, gl_context);
    SDL_ShowWindow(window);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    // ImGui::StyleColorsLight();
    ImGuiStyle &style = ImGui::GetStyle();
    style.FontSizeBase = 16.0f; // Default to 13.0f when
    // unspecified, which allows crisp legacy font for non-4K screens.
    //
    // Setup scaling
    // style.ScaleAllSizes(
    //     main_scale); // Bake a fixed style scale. (until we have a solution
    //     for
    //                  // dynamic style scaling, changing this requires
    //                  resetting
    //                  // Style + calling this again)
    // style.FontScaleDpi =
    //     main_scale; // Set initial font scale. (in docking branch: using
    // io.ConfigDpiScaleFonts=true automatically overrides this
    // for every window depending on the current monitor)

    // Setup Platform/Renderer backends
    ImGui_ImplSDL3_InitForOpenGL(window, gl_context);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Load Fonts
    // - If fonts are not explicitly loaded, Dear ImGui will select an embedded
    // font: either AddFontDefaultVector() or AddFontDefaultBitmap().
    //   This selection is based on (style.FontSizeBase * style.FontScaleMain *
    //   style.FontScaleDpi) reaching a threshold.
    // - You can load multiple fonts and use ImGui::PushFont()/PopFont() to
    // select them.
    // - If a file cannot be loaded, AddFont functions will return a nullptr.
    // Please handle those errors in your code (e.g. use an assertion, display
    // an error and quit).
    // - Read 'docs/FONTS.md' for more instructions and details.
    // - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use
    // FreeType for higher quality font rendering.
    // - Remember that in C/C++ if you want to include a backslash \ in a string
    // literal you need to write a double backslash \\ !
    // - Our Emscripten build process allows embedding fonts to be accessible at
    // runtime from the "fonts/" folder. See Makefile.emscripten for details.
    // io.Fonts->AddFontDefaultVector();
    // io.Fonts->AddFontDefaultBitmap();
    // io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf");
    // io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf");
    // io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf");
    // io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf");
    // ImFont* font =
    // io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf");
    // IM_ASSERT(font != nullptr);

    // Main loop
    bool done = false;

    static float offset = 40.0f;
    static bool wide = true;
#ifdef __EMSCRIPTEN__
    // For an Emscripten build we are disabling file-system access, so let's not
    // attempt to do a fopen() of the imgui.ini file. You may manually call
    // LoadIniSettingsFromMemory() to load settings from your own storage.
    io.IniFilename = nullptr;
    EMSCRIPTEN_MAINLOOP_BEGIN
#else
    while (!done)
#endif
    {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            ImGui_ImplSDL3_ProcessEvent(&event);
            if (event.type == SDL_EVENT_QUIT)
                done = true;
            if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED &&
                event.window.windowID == SDL_GetWindowID(window))
                done = true;
        }

        if (SDL_GetWindowFlags(window) & SDL_WINDOW_MINIMIZED) {
            SDL_Delay(10);
            continue;
        }

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();

        ImVec2 want;
        static ImVec2 last = ImVec2(300, 300);
        static ImVec4 bg(0.102f, 0.102f, 0.102f, 1.000f);
        {
            ImGui::SetNextWindowPos(ImVec2(0, 0));
            ImGui::SetNextWindowSize(last);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
            ImGui::PushStyleColor(ImGuiCol_FrameBg, bg);

            ImGui::Begin("Notes", nullptr,
                         ImGuiWindowFlags_NoCollapse |
                             ImGuiWindowFlags_NoResize |
                             ImGuiWindowFlags_NoSavedSettings |
                             ImGuiWindowFlags_NoScrollbar);

            ImGui::BeginChild("##input", ImVec2(last.x - offset, last.y), false,
                              ImGuiWindowFlags_NoScrollbar);

            static char str[10 * 1024 * 1024] = {};
            ImGui::InputTextMultiline("##i", &str[0], IM_COUNTOF(str),
                                      ImVec2(-1, -1));
            ImGui::EndChild();
            ImGui::SameLine();
            ImGui::BeginChild("##buttons", ImVec2(40, last.y), false);

            ImGui::Dummy(ImVec2(10, 2));

            if (ImGui::Button("X"))
                return 0;

			// THIS IS THE BEST PROGRAMMING THE WORLD HAS EVER SEEN !!!
			// HARDCODED FILE NAMES
            const std::string &name = "test.txt";
            if (ImGui::Button("S")) {
                ifstream q(name.c_str());
                if (!q.good()) {
                    cout << "file doesn't exist \n";
                    ofstream file;
                    file.open(name);
                    file << str << endl;
                    cout << "writing...\n";
                    file.close();
                    ImGui::OpenPopup("p");
                } else {
                    cout << "file exists\n";
                    ImGui::OpenPopup("overwrite");
                }
            }

            if (ImGui::Button("I")) {
                bg = ImVec4(rand() / (float)RAND_MAX, rand() / (float)RAND_MAX,
                            rand() / (float)RAND_MAX, 1.0f);
                cout << bg.x << endl
                     << bg.y << endl
                     << bg.z << endl
                     << bg.w << endl
                     << endl;
            }

            if (ImGui::BeginPopup("overwrite")) {
                ImGui::Dummy(ImVec2(30, 5));
                ImGui::Text("  file exists . OVERWRITE ?  ");
                ImGui::Separator();
                ImGui::Dummy(ImVec2(30, 3));
                ImGui::Text("  ");
                ImGui::SameLine();
                if (ImGui::Button("no"))
                    ImGui::CloseCurrentPopup();
                ImGui::SameLine();
                ImGui::Text("  ");
                ImGui::SameLine();
                if (ImGui::Button("yes")) {
                    ofstream file;
                    file.open(name);
                    file << str << endl;
                    cout << "wrote file .\n";
                    file.close();
                    ImGui::CloseCurrentPopup();
                }
                ImGui::Dummy(ImVec2(30, 5));
                ImGui::EndPopup();
            }

            if (ImGui::BeginPopup("p")) {
                ImGui::Dummy(ImVec2(30, 5));
                ImGui::Text("  saved file .  ");
                ImGui::Dummy(ImVec2(30, 5));
                ImGui::EndPopup();
            }

            ImGui::EndChild();

            want = ImGui::GetItemRectSize();
            ImVec2 padd = ImGui::GetStyle().WindowPadding;
            want.x -= padd.x * 2;
            want.y -= padd.y * 2;

            if (ImGui::IsKeyPressed(ImGuiKey_F1)) {
                cout << "wide: " << wide << endl;
                cout << "offset: " << offset << endl << endl;
                offset = 0.0f;
                if (wide == true) {
                    offset = 0.0f;
                    wide = false;
                } else {
                    offset = 40.0f;
                    wide = true;
                }
            }

            ImGui::End();
            ImGui::PopStyleVar();
            ImGui::PopStyleColor();
        }
        want = last;

        if (want.x != last.x || want.y != last.y) {
            SDL_SetWindowSize(window, (int)want.x, (int)want.y);
            last = want;
        }

        ImGui::Render();
        // glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        SDL_GL_SwapWindow(window);
    }
#ifdef __EMSCRIPTEN__
    EMSCRIPTEN_MAINLOOP_END;
#endif

    // Cleanup
    // [If using SDL_MAIN_USE_CALLBACKS: all code below would likely be your
    // SDL_AppQuit() function]
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DestroyContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
