#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

class Mesh
{
public:
	bool loadObj(const char* filename, struct __RTCScene* scene);
	bool loadPly(const char* filename, struct __RTCScene* scene);
};
