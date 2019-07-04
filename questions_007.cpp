#include <iostream>

void program_1th()
{
    std::cout << "01 > 编程实现：对n个整数进行排序，要求写出核心算法即可。\n";
}
void program_2th()
{
    std::cout << "02 > 松鼠第一天摘下若干个松子，当即吃了一半，还不过瘾，又多吃了一个，第二天早上\n"
              << "     又将剩下的松子吃掉一半，又多吃一个。以后每天早上都吃了前一天剩下的一半零一\n"
              << "     个。到第10天早上想再吃时，见只剩下一个松子了。编程实现计算第一天共摘了多少？\n";
}
void program_3th()
{
    std::cout << "03 > 编程实现 String 类，已知类 String 的原型为：\n"
              << "         class String {\n"
              << "         public:\n"
              << "             String(const char *str = NULL);\n"
              << "             String(const String &other);\n"
              << "             ~String(void);\n"
              << "             String& operate =(const String &other);\n"
              << "         private:\n"
              << "             char *m_data;\n"
              << "         };\n"
              << "     请编写 String 的上述 4 个函数。";
}
void program_4th()
{
    std::cout << "04 > 现有 STL 的 map<int, string>，自定义六个键值对初始化，其中两个字符串开\n"
              << "     头是 's'，一个第二个字母为 's'，实现如下功能：\n"
              << "     a > 找出字符串开头为 's' 的值对应的 key 并输出：\n"
              << "     b > 将第一个节点的值和倒数第二个节点的值互换。\n";
}

void program_5th()
{
    std::cout << "05 > 编写一个程序，开启三个线程，这三个线程的ID分别为A、B、C，每个线程将自己的\n"
              << "     ID在屏幕上打印10遍，要求输出结果必须按照 ABC 的顺序显示；如：ABCABC...\n"
              << "     以此递推。\n";
}
void problem_1th()
{
    std::cout << "01 > 简述#include <filename.h> 和 #include \"filename.h\" 有什么区别？\n";
}
void problem_2th()
{
    std::cout << "02 > 简述引用和指针的区别，并写出使用指针和引用作为参数的函数。\n";
}
void problem_3th()
{
    std::cout << "03 > 写出 bool，float，指针变量与 “零值” 比较的 if 语句。\n";
}
void problem_4th()
{
    std::cout << "04 > 简述 C++ 中的深拷贝和浅拷贝?\n";
}
void problem_5th()
{
    std::cout << "05 > 什么是 “野指针”，如何避免？\n";
}
void problem_6th()
{
    std::cout << "06 > 简述面向对象语言的特征以及作用。\n";
}
void problem_7th()
{
    std::cout << "07 > 什么事纯虚函数？什么是抽象类？并说明他们有何作用以及相互之间的关系？\n";
}
void problem_8th()
{
    std::cout << "08 > C++中的四大类型转换分别是什么？有什么作用？\n";
}
void problem_9th()
{
    std::cout << "09 > C++ 中的 struct 和 class 有什么区别？\n";
}
void problem_10th()
{
    std::cout << "10 > 内存的分配方式有几种？\n";
}
void problem_11th()
{
    std::cout << "11 > 内联函数在编译时是否做参数类型检查？\n";
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
void problem_15th()
{
    std::cout << "15 > 识别函数或指针：\n"
              << "         void * (* (* fp1)(int))[10];\n"
              << "         float (* (* fp2)(int, int, int))(int);\n"
              << "         int (* (* fp3)())[10]();\n"
              << "     分别表示什么意思？\n";
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
    std::cout << "编程题：\n";
    program_1th();
    program_2th();
    program_3th();
    program_4th();
    program_5th();
    std::cout << "简答题：\n";
    problem_1th();
    problem_2th();
    problem_3th();
    problem_4th();
    problem_5th();
    problem_6th();
    problem_7th();
    problem_8th();
    problem_9th();
    problem_10th();
    problem_11th();
    problem_12th();
    problem_13th();
    problem_14th();
    problem_15th();
    problem_16th();
    return 0;
}
