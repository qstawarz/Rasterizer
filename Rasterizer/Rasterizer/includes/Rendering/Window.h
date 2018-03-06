#pragma once

#include "SDLManager.h"
#include "Rasterizer.h"

namespace Rendering
{
	class Window
	{
	private:
		unsigned int m_width;
		unsigned int m_height;

		SDLManager m_SDLManager;

		Rasterizer m_rasterizer;

		Scene m_scene;

		Texture m_texture;

		Mesh* m_cube;
		Mesh* m_sphere;
		Mesh* m_triangle;

		float m_startClock;
		float m_time;
		float m_deltaTime;

	public:
		explicit Window(const unsigned int& p_width = 500u,
		                const unsigned int& p_height = 500u);
		~Window();

		void Setup();
		void Update();
		void Display();
		void Run();

		unsigned int GetWidth() const;
		unsigned int GetHeight() const;
	};
}
