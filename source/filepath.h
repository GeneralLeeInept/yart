#pragma once

#include <string>

class FilePath
{
public:
	FilePath(const std::string& filename);

	operator const char*() const;
	operator const std::string&() const;

	void changeExtension(const std::string& extension);

	std::string directory() const;
	std::string extension() const;

private:
	std::string m_path;

	std::string makeCanonical(const std::string& filename);
};