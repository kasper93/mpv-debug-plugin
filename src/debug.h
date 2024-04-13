// Copyright (c) 2022-2023 tsl0922. All rights reserved.
// SPDX-License-Identifier: GPL-2.0-only

#pragma once
#include <vector>
#include <map>
#include <memory>
#include <string>
#include <mpv/client.h>
#include <imgui.h>

// imgui extensions
namespace ImGui {
inline ImVec2 EmVec2(float x, float y) { return ImVec2(GetFontSize() * x, GetFontSize() * y); }
inline float EmSize(float n) { return GetFontSize() * n; }
}  // namespace ImGui

class Debug {
   public:
    Debug(mpv_handle *mpv, int logLines);
    ~Debug();

    void draw();
    void show();
    void AddLog(const char *prefix, const char *level, const char *text);
    void update(mpv_event_property *prop);

   private:
    struct Console {
        Console(mpv_handle *mpv, int logLines);
        ~Console();

        void init(const char *level, int limit);
        void draw();

        void ClearLog();
        void AddLog(const char *level, const char *fmt, ...);
        void ExecCommand(const char *command_line);
        int TextEditCallback(ImGuiInputTextCallbackData *data);
        void initCommands(std::vector<std::pair<std::string, std::string>> &commands);

        ImVec4 LogColor(const char *level);

        const std::vector<std::string> builtinCommands = {"HELP", "CLEAR", "HISTORY"};

        struct LogItem {
            char *Str;
            const char *Lev;
        };

        mpv_handle *mpv;
        char InputBuf[256];
        ImVector<LogItem> Items;
        ImVector<char *> Commands;
        ImVector<char *> History;
        int HistoryPos = -1;  // -1: new line, 0..History.Size-1 browsing history.
        ImGuiTextFilter Filter;
        bool AutoScroll = true;
        bool ScrollToBottom = false;
        bool CommandInited = false;
        std::string LogLevel = "status";
        int LogLimit = 5000;
    };

    struct Binding {
        std::string section;
        std::string key;
        std::string cmd;
        std::string comment;
        int64_t priority;
        bool weak;
    };

    void drawHeader();
    void drawConsole();
    void drawBindings();
    void drawCommands();
    void drawProperties(const char *title, std::vector<std::string> &props);
    void drawPropNode(const char *name, mpv_node &node, int depth = 0);

    mpv_handle *mpv;
    bool m_open = true;
    Console *console = nullptr;
    std::unique_ptr<char, decltype(mpv_free)> version;
    bool m_demo = false;

    std::vector<std::string> options;
    std::vector<std::string> properties;
    std::vector<std::pair<std::string, std::string>> commands;
    std::vector<Binding> bindings;
};
