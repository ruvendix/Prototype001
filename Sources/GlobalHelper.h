#pragma once

namespace global
{
	void SaveStringToFile(const std::string& str, FILE* pFile);
	std::string LoadStringFromFile(FILE* pFile);
}