#include <iostream>



void problem_5th()
{
    std::cout << "编写一个程序，开启三个线程，这三个线程的ID分别为A、B、C，每个线程将自己的"
              << "Id在屏幕上打印10遍，要求输出结果必须按照 ABC 的顺序显示；如：ABCABC..."
              << "以此递推。\n";
}
void problem_9th()
{
    std::cout << "09 > C++ 中的 struct 和 class 有什么区别？" << "\n";
}
void problem_10th()
{
    std::cout << "10 > 内存的分配方式有几种？" << "\n";
}
void problem_11th()
{
    std::cout << "11 > 内联函数在编译时是否做参数类型检查？" << "\n";
}

void problem_12th()
{
    std::cout << "12 >" << "\n";
    class CTest
    {
    public:
        CTest(int nNum)
            : m_nB(nNum),
              m_nC(m_nB + 1),
              m_nA(m_nC) {
            std::cout << "a = " << m_nA << "\n";
            std::cout << "b = " << m_nB << "\n";
            std::cout << "c = " << m_nC << "\n";
        }
    public:
        // 原函数：
        // void setA(int nNum) const {
        //     m_nA = nNum;
        // }
        // 更正：
        void setA(int nNum) {
            m_nA = nNum;
        }
    private:
        int m_nA;
        int m_nB;
        int m_nC;
    };
    CTest test(5);
    std::cout << "假如上述程序能够正常运行，请问输出结果是什么？" << "\n";
    std::cout << "成员函数setA(int nNum)存在什么问题？该如何修改？" << "\n";
}
void problem_13th()
{
    std::cout << "13 >" << "\n";
    struct SInfoA {
        int nA;
        double dB;
        char cBuf;
        char* pCh;
    };
    struct SInfoB {
        int nA;
        double dB;
        char cBuf;
        char* pCh;
    };
    std::cout << "如何计算结构体SInfoA、SInfoB大小？分别在windows中32位与64位计算结果是否相同？" << "\n";
    std::cout << "SInfoA size :" << sizeof(SInfoA) << "\n";
    std::cout << "SInfoB size :" << sizeof(SInfoB) << "\n";
    std::cout << "假如结构体SinfoB存在对象B中，由于str存储的字符不确定，如何计算B所占内存的真实大小？" << "\n";
}
void problem_14th()
{
    std::cout << "14 >" << "\n";
    class A
    {
    public:
        A() {
            pChA = new char[10];
        }
        ~A() {
            if (pChA) {
                delete pChA;
            }
            std::cout << "delete A" << "\n";
        }
        virtual void Disp() {
            std::cout << "A disp ... ..." << "\n";
        }
    private:
        char *pChA;
    };
    class B : public A
    {
    public:
        B() {
            pChB = new char[10];
        }
        ~B() {
            if (pChB) {
                delete pChB;
            }
            std::cout << "delete B" << "\n";
        }
        virtual void Disp() {
            std::cout << "B disp ... ..." << "\n";
        }
    private:
        char *pChB;
    };
    A *a = new B;
    a->Disp();
    delete a;
    std::cout << "上述程序输出结果？" << "\n";
    std::cout << "是否存在内存泄漏？如果存在，该如何处理？" << "\n";
}
void problem_16th()
{
    std::cout << "16 > 以下三条输出语句分别输出什么？" << "\n";
    char str1[] = "abc";
    char str2[] = "abc";
    const char str3[] = "abc";
    const char str4[] = "abc";
    const char *str5 = "abc";
    const char *str6 = "abc";
    std::cout << std::boolalpha << (str1 == str2) << "\n";
    std::cout << std::boolalpha << (str3 == str4) << "\n";
    std::cout << std::boolalpha << (str5 == str6) << "\n";
}

int main(int argc, char *argv[])
{
    problem_5th();
    problem_9th();
    problem_10th();
    problem_11th();
    problem_12th();
    problem_13th();
    problem_14th();
    problem_16th();
    return 0;
}
