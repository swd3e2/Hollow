#pragma once

#include "Hollow/Graphics/GUI/ImGui/imgui.h"

using namespace Hollow;

namespace GUI {
	class LogTab
	{
	public:
		LogTab() {}

		void Draw()
		{
			ImGui::Begin("Log");
			ImGui::BeginChild("scrolling", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
			if (ImGui::Button("Clear")) {
				Hollow::Logger::instance()->clear();
			}
			ImGuiListClipper clipper;
			clipper.Begin(Hollow::Logger::instance()->size);
			while (clipper.Step())
			{
				for (int line_no = clipper.DisplayStart; line_no < clipper.DisplayEnd; line_no++)
				{
					const char* line_start = Hollow::Logger::instance()->buffer[line_no].c_str();
					const char* line_end = Hollow::Logger::instance()->buffer[line_no].size() > 0 ? Hollow::Logger::instance()->buffer[line_no].c_str() + Hollow::Logger::instance()->buffer[line_no].size() : 0;
					ImGui::TextUnformatted(line_start, line_end);
				}
			}
			clipper.End();
			ImGui::PopStyleVar();
			ImGui::EndChild();
			ImGui::End();
		}
	};
}
