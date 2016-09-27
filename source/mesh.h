#pragma once

#include <unordered_map>
#include <memory>
#include <string>
#include <vector>

class Mesh
{
public:
	bool load(const char* filename, struct __RTCScene* scene);
};
