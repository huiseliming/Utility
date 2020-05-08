/*
 * MIT License
 *
 * Copyright(c) 2020 DaiMingze
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this softwareand associated documentation files(the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and /or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions :
 *
 * The above copyright noticeand this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#pragma once
#include <exception>
#include <string>
#include <sstream>

#define EXCEPT(...)  Exception(__LINE__,__FILE__,##__VA_ARGS__)
#define THROW_EXCEPT(...) throw EXCEPT(##__VA_ARGS__)

class Exception : public std::exception
{
public:
	Exception(int line, const char* file) noexcept :
		m_line(line), m_file(file)
	{}
	explicit Exception(int line, const char* file, char const* const Message) noexcept :
		m_line(line), m_file(file),std::exception(Message)
	{}
	const char* what() const noexcept override
	{
		std::ostringstream oss;
		oss << GetType() << std::endl
			<< "[Description] " <<std::exception::what() << std::endl
			<< GetOriginString() << std::endl;
		m_whatBuffer = oss.str();
		return m_whatBuffer.c_str();
	}
	virtual const char* GetType() const noexcept
	{
		return "Exception";
	}
	int GetLine() const noexcept
	{
		return m_line;
	}
	const std::string& GetFile() const noexcept
	{
		return m_file;
	}
	std::string GetOriginString() const noexcept
	{
		std::ostringstream oss;
		oss << "[File] " << m_file << std::endl
			<< "[Line] " << m_line;
		return oss.str();
	}
private:
	int m_line;
	std::string m_file;
protected:
	mutable std::string m_whatBuffer;
};


