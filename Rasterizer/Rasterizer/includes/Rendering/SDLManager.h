#pragma once

#include <string>

#include "../../../Dependencies/SDL2/include/SDL.h"
#include "../../../Dependencies/SDL2_ttf/include/SDL_ttf.h"
#include "../Graphics/Color.h"
#include "../Maths/Vec3.h"

using namespace Graphics;
using namespace Maths;

namespace Rendering
{
	class SDLManager
	{
	private:
		SDL_Window* m_window;

		SDL_Renderer* m_renderer;

		TTF_Font* m_font;

		const unsigned int m_windowW;
		const unsigned int m_windowH;

		bool m_run;
		bool m_wireframe;
		bool m_light;
		bool m_phong;
		bool m_normal;
		bool m_rotateX;
		bool m_rotateY;
		bool m_rotateZ;
		bool m_version0;
		bool m_version1;
		bool m_version2;
		bool m_version3;
		bool m_version4;
		bool m_version5;
		bool m_version6;
		bool m_version7;
		bool m_version8;
		bool m_version9;
		bool m_version10;

		Vec3 m_pos;
		Vec3 m_dir;
		Vec3 m_view;
		Vec3 m_angle;

		float m_speed;
		float m_scale;

	public:
		explicit SDLManager(const unsigned int& p_windowW,
		                    const unsigned int& p_windowH);
		~SDLManager();

		bool InitAll();
		bool InitSDL();
		bool InitSDLWindow();
		bool InitSDLRenderer();
		bool InitSDLFont();

		void Setup();
		void Update();
		void Display();
		void Clear(const Color& p_color = Color::Black());
		void CheckInput();
		void DrawPixel(const int& p_x,
		               const int& p_y,
		               const Color& p_color);
		void DrawText(const std::string& p_str,
		              const int& p_x,
		              const int& p_y,
		              const int& p_w,
		              const int& p_h,
		              const Color& p_color = Color::White());

		const SDL_Window* GetWindow() const;
		const SDL_Renderer* GetRenderer() const;

		const unsigned int& GetWindowWidth() const;
		const unsigned int& GetWindowHeight() const;

		const bool& IsRunning() const;
		const bool& IsWireframe() const;
		const bool& IsLight() const;
		const bool& IsPhong() const;
		const bool& IsNormal() const;
		const bool& IsRotateX() const;
		const bool& IsRotateY() const;
		const bool& IsRotateZ() const;
		const bool& IsVersion0() const;
		const bool& IsVersion1() const;
		const bool& IsVersion2() const;
		const bool& IsVersion3() const;
		const bool& IsVersion4() const;
		const bool& IsVersion5() const;
		const bool& IsVersion6() const;
		const bool& IsVersion7() const;
		const bool& IsVersion8() const;
		const bool& IsVersion9() const;
		const bool& IsVersion10() const;

		const Vec3& GetPos() const;
		const Vec3& GetDir() const;
		const Vec3& GetView() const;

		const float& GetScale() const;
	};
}
