#include "TiledLevel.h"

TiledLevel::TiledLevel(const unsigned short r, const unsigned short c, const int w, const int h, 
	const char* tileData, const char* levelData, const char* tileKey) : m_rows(r), m_cols(c), m_tileKey(tileKey)
{
	ifstream inFile(tileData); // create the 16 prototype tiles
	if (inFile.is_open())
	{
		char key;
		int x, y;
		bool obs, haz;
		while (!inFile.eof())
		{
			inFile >> key >> x >> y >> obs >> haz;
			m_tiles.emplace(key, new Tile({x * w, y * h, w, h}, {0.0f, 0.0f, (float)w, (float)h}, obs, haz));
		}
	}
	inFile.close();
	
	inFile.open(levelData); //create the who tiled level now from the text file
	if (inFile.is_open())
	{
		char key;
		m_level.resize(m_rows); //sets size of the first dimension
		for (unsigned short row = 0; row < m_rows; row++)
		{
			m_level[row].resize(m_cols);
			for (unsigned short col = 0; col < m_cols; col++)
			{
				inFile >> key;
				m_level[row][col] = m_tiles[key]->Clone(); //common prototype method
				m_level[row][col]->SetXY((float)col * w, (float)row * h);
				if (m_level[row][col]->IsObstacle())
				{
					m_obstacles.push_back(m_level[row][col]);
				}
			}
		}
	}
	inFile.close();
}

TiledLevel::~TiledLevel()
{
	//clear the tiled m_level of clones. this also would account for those m_obstacles
	for (unsigned short row = 0; row < m_rows; row++)
	{

		for (unsigned short col = 0; col < m_cols; col++)
		{
			delete m_level[row][col];
			m_level[row][col] = nullptr;
		}

	}
	m_level.clear();
	m_obstacles.clear();

	//clear the original tile prototypes.
	for (map<char, Tile*>::iterator i = m_tiles.begin(); i != m_tiles.end(); i++)
	{
		delete i->second;
		i->second = nullptr;
	}
	m_tiles.clear();
}

void TiledLevel::Render()
{
	for (unsigned short row = 0; row < m_rows; row++)
	{

		for (unsigned short col = 0; col < m_cols; col++)
		{
			SDL_RenderCopyF(Engine::Instance().GetRenderer(), TEMA::GetTexture(m_tileKey),
				m_level[row][col]->GetSrc(), m_level[row][col]->GetDst());
		}

	}
}
