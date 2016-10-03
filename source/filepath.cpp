#include "filepath.h"

#include <algorithm>

FilePath::FilePath(const std::string& filename)
{
	m_path = makeCanonical(filename);
}

FilePath::operator const char*() const
{
	return m_path.c_str();
}

FilePath::operator const std::string&() const
{
	return m_path;
}

std::string FilePath::directory() const
{
	size_t dirpart = m_path.find_last_of("/", std::string::npos);
	if (dirpart != std::string::npos)
		return m_path.substr(0, dirpart + 1);
	return std::string();
}

std::string FilePath::extension() const
{
	return m_path.substr(m_path.find_last_of('.'));
}

std::string FilePath::makeCanonical(const std::string& path)
{
	std::string canonical = path;
	std::replace(canonical.begin(), canonical.end(), '\\', '/');
	return canonical;
}
