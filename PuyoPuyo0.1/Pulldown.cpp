#include "Pulldown.hpp"

namespace GUI {
	void Pulldown::update() {
		if (isEmpty())
		{
			return;
		}

		if (m_rect.leftClicked())
		{
			m_isOpen = (not m_isOpen);
		}

		Point pos = m_rect.pos.movedBy(0, m_rect.h);

		if (m_isOpen)
		{
			for (auto i : step(m_items.size()))
			{
				if (const Rect rect{ pos, m_rect.w, m_rect.h };
					rect.leftClicked())
				{
					m_index = i;
					m_isOpen = false;
					break;
				}

				pos.y += m_rect.h;
			}
		}
	}

	void Pulldown::draw() const{
		m_rect.draw();

		if (isEmpty())
		{
			return;
		}

		m_rect.drawFrame(1, 0, m_isOpen ? Palette::Orange : Palette::Gray);

		Point pos = m_rect.pos;

		m_font(m_items[m_index]).draw(pos + m_padding, Palette::Black);

		Triangle{ (m_rect.x + m_rect.w - m_downButtonSize / 2.0 - m_padding.x), (m_rect.y + m_rect.h / 2.0),
			(m_downButtonSize * 0.5), 180_deg }.draw(Palette::Black);

		pos.y += m_rect.h;

		if (m_isOpen)
		{
			const Rect backRect{ pos, m_rect.w, (m_rect.h * m_items.size()) };

			backRect.drawShadow({ 1, 1 }, 4, 1).draw();

			for (const auto& item : m_items)
			{
				if (const Rect rect{ pos, m_rect.size };
					rect.mouseOver())
				{
					rect.draw(Palette::Skyblue);
				}

				m_font(item).draw((pos + m_padding), Palette::Black);

				pos.y += m_rect.h;
			}

			backRect.drawFrame(1, 0, Palette::Gray);
		}
	}
}
