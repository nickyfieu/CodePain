#include "CodePainPCH.h"
#include "Logger.h"
#include "Imgui\imgui.h"
#include <string>
#include <codecvt>

cp::ExampleAppLog cp::Logger::my_log = cp::ExampleAppLog();

cp::Logger::~Logger()
{
}

void cp::Logger::Log(LogLevel level, const std::string& msg, bool showTime)
{
	if (level == cp::LogLevel::None)
		return;
#ifndef _DEBUG
	if (level == cp::LogLevel::Trace)
		return;
	if (level == cp::LogLevel::Debug)
		return;
#endif


	std::stringstream stream;

	if (showTime)
	{
		SYSTEMTIME st;
		GetSystemTime(&st);
		stream << "[" << st.wHour << ":" << st.wMinute << ":" << st.wSecond << ":" << st.wMilliseconds << "]";
	}

	switch (level)
	{
	case cp::LogLevel::Trace:
		stream << "[Trace]  ";
		break;
	case cp::LogLevel::Debug:
		stream << "[Debug]  ";
		break;
	case cp::LogLevel::Info:
		stream << "[Info]  ";
		break;
	case cp::LogLevel::Warning:
		stream << "[Warning]  ";
		break;
	case cp::LogLevel::Error:
		stream << "[Error]  ";
		break;
	case cp::LogLevel::Critical:
		stream << "[Critical]  ";
		break;
	}

	if (level == LogLevel::Error)
	{
		using convert_t = std::codecvt_utf8<wchar_t>;
		std::wstring_convert<convert_t, wchar_t> stringConverter;

		MessageBox(0, stringConverter.from_bytes(msg).c_str(), L"!Error!", 0);
	}

	stream << msg << '\n';
	my_log.AddLog(stream.str().c_str());
}

void cp::Logger::DrawLoggedInformation() const
{
	ImGui::SetNextWindowSize(ImVec2(320.f, 500.f));
	ImGui::Begin("Logger", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

	{	// drawing the log
		my_log.Draw("Log");
	}

	ImGui::End();
}


cp::Logger::Logger()
{
	my_log.AutoScroll = true;
}
