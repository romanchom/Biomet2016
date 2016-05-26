#include "stdafx.h"
#include "Chart.h"
#include "gnuplot_i.hpp"

Chart::Chart()
{
	_gnuplot = new Gnuplot("lines");
}

Chart::~Chart()
{
	if (_gnuplot != nullptr)
	{
		delete _gnuplot;
		_gnuplot = nullptr;
	}
}

void Chart::Initialize(const std::vector<short>* const y, double lengthSeconds, const std::string* const namePtr)
{
	int size = y->size();

	_x.clear();
	_y.clear();

	for (int i = 0; i < size; ++i)
	{
		_y.push_back((double)(*y)[i]);
	}

	double xCurrent = 0.0;
	double xDelta = lengthSeconds / (double)size;

	for (int i = 0; i < size; ++i)
	{
		_x.push_back(xCurrent);
		xCurrent += xDelta;
	}

	_namePtr = namePtr;
}

void Chart::Shutdown()
{
	_gnuplot->remove_tmpfiles();
}

void Chart::Draw()
{
	if (_gnuplot != nullptr)
	{
		_gnuplot->reset_plot();
		_gnuplot->plot_xy(_x, _y, *_namePtr);
	}
}
