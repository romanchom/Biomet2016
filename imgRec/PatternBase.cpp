#include "PatternBase.h"

#include <cstring>
#include <map>
#include <iostream>
#include <algorithm>
#include <fstream>

void PatternBase::addPattern(double * desc, const std::string & className){
	Pattern p;
	memcpy(p.descriptor, desc, sizeof(double) * descriptorLength);
	bool found = false;
	for(size_t i = 0; i < classes.size(); ++i){
		if(classes[i] == className) {
			p.classId = i;
			found = true;
			break;
		}
	}
	if(!found){
		p.classId = classes.size();
		classes.push_back(className);
	}
	patterns.push_back(p);
}

bool cmpDesc(const std::pair<double, uint32_t> & first, const std::pair<double, uint32_t> & second){
	return first.first < second.first;
}

struct mapMax
{
    template <typename Pair>
    bool operator()(const Pair& a, const Pair& b)
    {
        return a.second < b.second;
    }
};

const std::string & PatternBase::classifyObject(double* desc, double tolerance){
	std::vector<std::pair<double, uint32_t>> results;
	for (int i = 0; i < patterns.size(); i++) {
		double distance = 0.0;
		Pattern & p = patterns[i];
		for (int j = 0; j < descriptorLength; j++) {
			double diff = p.descriptor[j] - desc[j];
			distance += diff * diff;
		}
		if(distance < tolerance)
			results.push_back(std::make_pair(distance, p.classId));
	}
	if(shouldAddPattern){
		std::string className;
		std::getline(std::cin, className);
		addPattern(desc, className);
		shouldAddPattern = false;
	}
	std::sort(results.begin(), results.end(), cmpDesc);
	results.resize(5);
	std::map<int, int> count;
    for (auto & v : results)
        ++count[v.second];

    size_t id = std::max_element(count.begin(), count.end(), mapMax())->first;
	return classes[id];
}

void PatternBase::readPatterns(){
	std::ifstream file(patternsPath);
	char c;
	while(true){
		Pattern p;
		for(size_t i = 0; i < descriptorLength; ++i){
			file >> p.descriptor[i];
			file >> c;
		}
		file >> p.classId;
		if(!file.good()) break;
		patterns.push_back(p);
	}
}

void PatternBase::writePatterns(){
	std::ofstream file(patternsPath);
	for(auto & p : patterns){
		for(size_t i = 0; i < descriptorLength; ++i){
			file << p.descriptor[i] << ',';
		}
		file << p.classId << '\n';
	}
}


void PatternBase::readClasses(){
	std::ifstream file(classesPath);
	std::string str;
	while(true){
		file >> str;
		if(!file.good()) break;
		classes.push_back(str);
	}
}

void PatternBase::writeClasses(){
	std::ofstream file(classesPath);
	for(auto & c : classes){
		file << c << '\n';
	}
}

PatternBase::PatternBase(const std::string & patterns, const std::string & classes) :
	patternsPath(patterns),
	classesPath(classes),
	shouldAddPattern(false)
{
	readPatterns();
	readClasses();
}

PatternBase::~PatternBase(){
	writePatterns();
	writeClasses();
}

void PatternBase::addNew(){
	shouldAddPattern = true;
}
