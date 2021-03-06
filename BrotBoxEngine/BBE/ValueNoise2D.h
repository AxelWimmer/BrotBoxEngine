#pragma once

#include "../BBE/List.h"
#include "../BBE/DynamicArray.h"

namespace bbe
{
	class ValueNoise2D
	{
	private:
		float *m_pdata      = nullptr;
		int    m_width      = 0;
		int    m_height     = 0;
		bool   m_wasCreated = false;

		float min = 0;
		float max = 0;
		bool wasStandardized = false;

		List<DynamicArray<float>> nodes;

	public:
		ValueNoise2D();

		ValueNoise2D(const ValueNoise2D &)            = delete;
		ValueNoise2D(ValueNoise2D &&)                 = delete;
		ValueNoise2D& operator=(const ValueNoise2D &) = delete;
		ValueNoise2D& operator=(ValueNoise2D &&)      = delete;

		~ValueNoise2D();

		void create(int width, int height);
		void create(int width, int height, int seed);
		void destroy();
		void unload();

		float get(int x, int y) const;
		void preCalculate(); 
		void standardize();
		void set(int x, int y, float val);

		float* getRaw();
	};
}