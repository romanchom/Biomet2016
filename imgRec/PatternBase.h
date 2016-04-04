#pragma once

#include <vector>
#include <string>
#include <cstdint>

class PatternBase {
private:
	std::string patternsPath;
	std::string classesPath;
	enum {
		descriptorLength = 16,
	};
	struct Pattern {
		double descriptor[descriptorLength];
		uint32_t classId;
	};
	std::vector<Pattern> patterns;
	std::vector<std::string> classes;
	void addPattern(double * desc, const std::string & className);
	void readPatterns();
	void readClasses();
	void writePatterns();
	void writeClasses();
	bool shouldAddPattern;
public:
	PatternBase(const std::string & patterns, const std::string & classes);
	~PatternBase();
	void addNew();
	const std::string & classifyObject(double * desc);
};
