#include "hzpch.h"
#include "HazelException.h"

namespace Hazel {

	HazelException::HazelException(int line, const char* file) noexcept
		: 
		line(line),
		file(file)		
	{

	}

	const char* HazelException::what() const noexcept
	{
		std::stringstream oss;
		oss << GetType() << std::endl << GetOriginString();
		whatBuffer = oss.str();
		return whatBuffer.c_str();
	}

	const char* HazelException::GetType() const noexcept
	{
		return "Hazel Exception";
	}

	int HazelException::GetLine() const noexcept
	{
		return line;
	}

	const std::string& HazelException::GetFile() const noexcept
	{
		return file;
	}

	std::string HazelException::GetOriginString() const noexcept
	{
		std::ostringstream oss;
		oss << "[File] " << file << std::endl << "[Line] " << line;
		return oss.str();
	}
}