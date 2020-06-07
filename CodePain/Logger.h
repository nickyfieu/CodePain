#pragma once
#include "Singleton.h"
#include "Imgui\imgui.h"
#include <iostream>
#include <fstream>

namespace cp
{
	enum class LogLevel
	{
		//Logs that contain the most detailed messages. These messages may contain sensitive application data.
		//These messages are disabled by default and should never be enabled in a production environment.
		Trace = 0,

		// Logs that are used for interactive investigation during development.
		//These logs should primarily contain information useful for debugging and have no long-term value.
		Debug = 1,

		// Logs that track the general flow of the application. These logs should have long-term value.
		Info = 2, 

		//Logs that highlight an abnormal or unexpected event in the application flow,
		//but do not otherwise cause the application execution to stop.
		Warning = 3,

		//Logs that highlight when the current flow of execution is stopped due to a failure.
		//These should indicate a failure in the current activity, not an application-wide failure.
		Error = 4,

		//Logs that describe an unrecoverable application or system crash,
		//or a catastrophic failure that requires immediate attention.
		Critical = 5,

		//Not used for writing log messages. Specifies that a logging category should not write any messages.
		None = 6 
	};

    // this "MOSTLY" comes from an imgui example file but changed to suit my needs
    struct ImGuiLogger final
    {
        ImGuiTextBuffer     Buf;
        ImGuiTextFilter     Filter;
        ImVector<int>       LineOffsets;        // Index to lines offset. We maintain this with AddLog() calls, allowing us to have a random access on lines
        bool                AutoScroll;     // Keep scrolling if already at the bottom

        ImGuiLogger()
        {
            AutoScroll = true;
            Clear();
        }

        void Clear()
        {
            Buf.clear();
            LineOffsets.clear();
            LineOffsets.push_back(0);
        }

        void Save()
        {
            std::stringstream stream;
            SYSTEMTIME st;
            GetSystemTime(&st);
            stream << "[LOGGER][" << st.wDay << "-" << st.wMonth << "-" << st.wYear << "]";
            stream << "_" << st.wHour << "-" << st.wMinute << "-" << st.wSecond << "-" << st.wMilliseconds << "_";
            if (Filter.Filters.size() > 0)
                if (!Filter.Filters[0].empty())
                    stream << Filter.Filters[0].b;

            stream << ".txt";
            
            std::ofstream write{};
            write.open(stream.str().c_str(), std::ios::out);
            if (write.is_open())
            {
                const char* buf = Buf.begin();
                const char* buf_end = Buf.end();
                if (Filter.IsActive())
                {
                    const char backSlash[1]{ '\n' };
                    for (int line_no = 0; line_no < LineOffsets.Size; line_no++)
                    {
                        const char* line_start = buf + LineOffsets[line_no];
                        const char* line_end = (line_no + 1 < LineOffsets.Size) ? (buf + LineOffsets[line_no + 1] - 1) : buf_end;
                        if (Filter.PassFilter(line_start, line_end))
                        {
                            auto it = line_start;
                            while (it != line_end)
                            {
                                write.write(it, 1);
                                it++;
                            }

                            write.write(backSlash, 1);
                        }
                    }
                }
                else
                {
                    auto it = buf;
                    while (it != buf_end)
                    {
                        write.write(it, 1);
                        it++;
                    }
                }

            }
            write.close();
        }

        void AddLog(const char* fmt, ...) IM_FMTARGS(2)
        {
            int old_size = Buf.size();
            va_list args;
            va_start(args, fmt);
            Buf.appendfv(fmt, args);
            va_end(args);
            for (int new_size = Buf.size(); old_size < new_size; old_size++)
                if (Buf[old_size] == '\n')
                    LineOffsets.push_back(old_size + 1);
        }

        void    Draw(const char* title, bool* p_open = NULL)
        {
            if (!ImGui::Begin(title, p_open))
            {
                ImGui::End();
                return;
            }

            // Options menu
            if (ImGui::BeginPopup("Options"))
            {
                ImGui::Checkbox("Auto-scroll", &AutoScroll);
                ImGui::EndPopup();
            }

            // Main window
            if (ImGui::Button("Options"))
                ImGui::OpenPopup("Options");
            ImGui::SameLine();
            Filter.Draw("Filter", -50);
            bool clear = ImGui::Button("Clear");
            ImGui::SameLine();
            bool save = ImGui::Button("Save");
            ImGui::SameLine();
            bool copy = ImGui::Button("Copy");

            ImGui::Separator();
            ImGui::BeginChild("scrolling", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);

            if (clear)
                Clear();
            if (save)
                Save();
            if(copy)
                ImGui::LogToClipboard();

            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
            const char* buf = Buf.begin();
            const char* buf_end = Buf.end();
            if (Filter.IsActive())
            {
                for (int line_no = 0; line_no < LineOffsets.Size; line_no++)
                {
                    const char* line_start = buf + LineOffsets[line_no];
                    const char* line_end = (line_no + 1 < LineOffsets.Size) ? (buf + LineOffsets[line_no + 1] - 1) : buf_end;
                    if (Filter.PassFilter(line_start, line_end))
                        ImGui::TextUnformatted(line_start, line_end);
                }
            }
            else
            {
                // The simplest and easy way to display the entire buffer:
                //   ImGui::TextUnformatted(buf_begin, buf_end);
                // And it'll just work. TextUnformatted() has specialization for large blob of text and will fast-forward to skip non-visible lines.
                // Here we instead demonstrate using the clipper to only process lines that are within the visible area.
                // If you have tens of thousands of items and their processing cost is non-negligible, coarse clipping them on your side is recommended.
                // Using ImGuiListClipper requires A) random access into your data, and B) items all being the  same height,
                // both of which we can handle since we an array pointing to the beginning of each line of text.
                // When using the filter (in the block of code above) we don't have random access into the data to display anymore, which is why we don't use the clipper.
                // Storing or skimming through the search result would make it possible (and would be recommended if you want to search through tens of thousands of entries)
                ImGuiListClipper clipper;
                clipper.Begin(LineOffsets.Size);
                while (clipper.Step())
                {
                    for (int line_no = clipper.DisplayStart; line_no < clipper.DisplayEnd; line_no++)
                    {
                        const char* line_start = buf + LineOffsets[line_no];
                        const char* line_end = (line_no + 1 < LineOffsets.Size) ? (buf + LineOffsets[line_no + 1] - 1) : buf_end;
                        ImGui::TextUnformatted(line_start, line_end);
                    }
                }
                clipper.End();
            }
            ImGui::PopStyleVar();

            if (AutoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
                ImGui::SetScrollHereY(1.0f);

            ImGui::EndChild();
            ImGui::End();
        }
    };

	class Logger final : public Singleton <Logger>
	{
	public:
		~Logger();
		Logger(const Logger& other) = delete;
		Logger(Logger&& other) noexcept = delete;
		Logger& operator=(const Logger& other) = delete;
		Logger& operator=(Logger&& other) noexcept = delete;

		void Log(LogLevel level, const std::string& msg, bool showTime = false);
		void DrawLoggedInformation() const;

	private:
		friend class Singleton<Logger>;
		Logger();

        static ImGuiLogger my_log;
	};
}