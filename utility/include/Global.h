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
#include <vector>
#include <functional>


namespace Utility
{
/*
 *
 */

std::vector<std::function<void()>>& GetConstructorVector();
std::vector<std::function<void()>>& GetDestructorVector();

void ConstructorGlobalObject();
void DestructorGlobalObject();

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
 * 记录全局对象，以正确析构
 */
template<typename T>
class Deleter
{
public:
    Deleter(T* pT)
    {
        m_pT = pT;
    }
    ~Deleter()
    {
        delete m_pT;
    }
private:
    T* m_pT;
};

/*
 * 注册全局对象，以在ConstructorGlobalObject()中构造
 */

template<typename T>
class Register
{
public:
    Register(bool IsRegister)
    {
        m_bIsRegister = IsRegister;
    }
private:
    bool m_bIsRegister = false;
};

}

#define GLOBAL_INITIALIZE(CLASS_NAME,...) \
static std::function<Utility::Register<CLASS_NAME>()> ##CLASS_NAME##_Global_Initializer =\
[]\
{\
    Utility::GetConstructorVector().emplace_back(\
        []()\
        {\
            CLASS_NAME* ##CLASS_NAME##_Global_Ptr = new CLASS_NAME(##__VA_ARGS__); \
            Utility::Global<CLASS_NAME>::Set(##CLASS_NAME##_Global_Ptr);\
        }\
    );\
    Utility::GetDestructorVector().emplace_back(\
        []()\
        {\
            delete Utility::Global<CLASS_NAME>::Get(); \
        }\
    );\
    return Utility::Register<CLASS_NAME>(true);\
};\
static Utility::Register<CLASS_NAME> ##CLASS_NAME##_Register = ##CLASS_NAME##_Global_Initializer();

/*
 * 使用GLOBAL_INITIALIZE_BEFORE_MAIN定义的全局访问类在main()函数执行之前构造，在main()返回之后析构
 */
#define GLOBAL_INITIALIZE_BEFORE_MAIN(CLASS_NAME,...)\
static std::function<Utility::Deleter<CLASS_NAME>()> ##CLASS_NAME##_Global_Initializer =\
[]\
{\
    CLASS_NAME* ptr = new CLASS_NAME(##__VA_ARGS__);\
    Utility::Global<CLASS_NAME>::Set(ptr);\
    return Utility::Deleter<CLASS_NAME>(ptr);\
};\
static Utility::Deleter<CLASS_NAME> ##CLASS_NAME##_Deleter = ##CLASS_NAME##_Global_Initializer();