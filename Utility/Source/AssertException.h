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
#include "Exception.h"


#define ASSERT_EXCEPT(Expr) AssertException(__LINE__,__FILE__,#Expr)
#define THROW_ASSERT_EXCEPT(Expr) throw ASSERT_EXCEPT(Expr)

#ifndef ASSERT
#define ASSERT(Expr) if(!(Expr)) THROW_ASSERT_EXCEPT(Expr)
#endif 


class AssertException : public Exception
{
public:
	AssertException(int line, const char* file, const char* assertstr) noexcept :
		Exception(line, file),
		m_assert(assertstr)
	{
	}
	const char* what() const noexcept
	{
		std::ostringstream oss;
		oss << GetType() << std::endl
			<< "[Assert] " << GetAssert() << std::endl
			<< GetOriginString();

		m_whatBuffer = oss.str();
		return m_whatBuffer.c_str();
	}
	const char* GetType() const noexcept
	{
		return "AssertException";
	}
	const char* GetAssert() const noexcept
	{
		return m_assert.c_str();
	}

private:
	std::string m_assert;
};