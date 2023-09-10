#pragma once
#include <string>

struct _TTF_Font;
namespace that
{
	/**
	 * Simple RAII wrapper for a _TTF_Font
	 */
	class Font final
	{
	public:
		_TTF_Font* GetFont() const;
		explicit Font(const std::string& fullPath, unsigned int size);
		~Font();

		unsigned int GetSize() { return m_Size; }

		Font(const Font &) = delete;
		Font(Font &&) = delete;
		Font & operator= (const Font &) = delete;
		Font & operator= (const Font &&) = delete;
	private:
		_TTF_Font* m_Font{};
		std::string m_FontName{};
		unsigned int m_Size{};
	};
}
