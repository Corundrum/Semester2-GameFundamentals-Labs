#include "Witch.h"

Witch::Witch(int x, int y) :m_src({0,32,24,32})
{
	m_dst = { x,y,m_src.w * 3,m_src.h * 3 };
}
