#pragma once

namespace GUI {
	class Pulldown
	{
	public:

		Pulldown() = default;

		Pulldown(const Array<String>& items, const Font& font, const Point& pos = { 0,0 })
			: m_font{ font }
			, m_items{ items }
			, m_rect{ pos, 0, (m_font.height() + m_padding.y * 2) }
		{
			for (const auto& item : m_items)
			{
				m_rect.w = Max(m_rect.w, static_cast<int32>(m_font(item).region().w));
			}

			m_rect.w += (m_padding.x * 2 + m_downButtonSize);
		}

		bool isEmpty() const { return m_items.empty(); }

		void update();

		void draw() const;

		void setPos(const Point& pos) { m_rect.setPos(pos); }

		const Rect& getRect() const { return m_rect; }

		size_t getIndex() const { return m_index; }

		String getItem() const
		{
			if (isEmpty())
			{
				return{};
			}

			return m_items[m_index];
		}

		void pushItem(String item) { m_items.push_back(item); }

	private:

		Font m_font;

		Array<String> m_items;

		size_t m_index = 0;

		Size m_padding{ 6, 2 };

		Rect m_rect;

		int32 m_downButtonSize = 16;

		bool m_isOpen = false;
	};

}
