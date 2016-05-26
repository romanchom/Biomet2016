#pragma once

class Gnuplot;

class Chart
{
protected:

#pragma region protected

	std::vector<double> _y;
	std::vector<double> _x;

	const std::string* _namePtr;

	Gnuplot* _gnuplot = nullptr;


#pragma endregion

public:

#pragma region functionsPublic

	Chart();
	~Chart();

	void Initialize(const std::vector<short>* const y, double lengthSeconds, const std::string* const namePtr);
	void Shutdown();
	void Draw();

#pragma endregion

};

