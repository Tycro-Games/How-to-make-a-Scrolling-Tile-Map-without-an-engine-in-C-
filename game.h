// Template, IGAD version 3
// Get the latest version from: https://github.com/jbikker/tmpl8
// IGAD/NHTV/BUAS/UU - Jacco Bikker - 2006-2023

#pragma once

namespace Tmpl8
{
	class Game : public TheApp
	{
	public:
		// game flow methods
		void Init() override;
		void ConvertCharToInt(const char* pch, uint& numberForm);
		void ExtractWidthHeight(const char* csvRaw, uint& w, uint& h);
		void LoadCSVFile(const char* csvPath);
		void Tick(float deltaTime) override;

		void Shutdown() override
		{
			/* implement if you want to do something on exit */
		}

		// input handling
		void MouseUp(int) override
		{
			/* implement if you want to detect mouse button presses */
		}

		void MouseDown(int) override
		{
			/* implement if you want to detect mouse button presses */
		}

		void MouseMove(int x, int y) override
		{
			mousePos.x = x, mousePos.y = y;
		}

		void MouseWheel(float) override
		{
			/* implement if you want to handle the mouse wheel */
		}

		void KeyUp(int) override
		{
			/* implement if you want to handle keys */
		}

		void KeyDown(int) override
		{
			/* implement if you want to handle keys */
		}

		// data members
		int2 mousePos;
	};
} // namespace Tmpl8
