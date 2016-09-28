#pragma once

#include <unordered_map>
#include <memory>
#include <string>
#include <vector>

class Mesh
{
public:
	bool loadObj(const char* filename, struct __RTCScene* scene);
	bool loadPly(const char* filename, struct __RTCScene* scene);
};
