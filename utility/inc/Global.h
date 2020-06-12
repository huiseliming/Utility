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
#include <cassert>

/*
 * 用于在main函数之前构造的全局类
 */
template<typename T>
class Global
{
public:

    inline static void Set(T* pVariable)
    {
        //每个类仅能创建一个全局类
        assert(m_pT == nullptr);
        m_pT = pVariable;
    }

    inline static T* Get()
    {
        return m_pT;
    }
    inline static T& Ref()
    {
        return *m_pT;
    }
private:
    static T* m_pT;
};

template<class T>
T* Global<T>::m_pT = nullptr;

/*
 * main函数之前构造的全局类的内存是动态申请的，使用全局静态变量记录这块内存用于释放
 */
template<typename T>
class GlobalDeleter
{
public:
    GlobalDeleter(T* pT)
    {
        m_pT = pT;
    }
    ~GlobalDeleter()
    {
        delete m_pT;
    }
    T* m_pT;
};
/*
 * 通过宏使用函数包装器定义一个全局类，
 */
#define GLOBAL_INITIALIZE(CLASS_NAME,...)\
static std::function<GlobalDeleter<CLASS_NAME>()> GlobalInitialize =\
[&]\
{\
    CLASS_NAME* ptr = new CLASS_NAME(##__VA_ARGS__);\
    Global<CLASS_NAME>::Set(ptr);\
    return GlobalDeleter<CLASS_NAME>(ptr);\
};\
static GlobalDeleter<CLASS_NAME> ##CLASS_NAME##_Deleter = GlobalInitialize();


