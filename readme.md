[TOC]

---
## C++11

### C++11 之左值右值

众所周知C++11新增了右值引用，这里涉及到很多概念：

- 左值：可以取地址并且有名字的东西就是左值。
- 右值：不能取地址的没有名字的东西就是右值。
- 纯右值：运算表达式产生的临时变量、不和对象关联的原始字面量、非引用返回的临时变量、`lambda`表达式等都是纯右值。
- 将亡值：可以理解为即将要销毁的值。
- 左值引用：对左值进行引用的类型。
- 右值引用：对右值进行引用的类型。
- 移动语义：转移资源所有权，类似于转让或者资源窃取的意思，对于那块资源，转为自己所拥有，别人不再拥有也不会再使用。
- 完美转发：可以写一个接受任意实参的函数模板，并转发到其它函数，目标函数会收到与转发函数完全相同的实参。
- 返回值优化(`RVO`)：当函数需要返回一个对象实例时候，就会创建一个临时对象并通过复制构造函数将目标对象复制到临时对象，这里有复制构造函数和析构函数会被多余的调用到，有代价，而通过返回值优化，C++标准允许省略调用这些复制构造函数。

### C++11 之列表初始化

### C++11 之 `std::function` & `std::bind` & `lambda` 表达式


### C++11 之模板改进

C++11关于模板有一些细节的改进：

- 模板的右尖括号
- 模板的别名
- 函数模板的默认模板参数

#### 模板的右尖括号

C++11之前是不允许两个右尖括号出现的，会被认为是右移操作符，所以需要中间加个空格进行分割，避免发生编译错误。

```c++
int main() {
    std::vector<std::vector<int>> a; // error
    std::vector<std::vector<int> > b; // ok
}
```
这个我之前都不知道，我开始学编程的时候就已经是C++11的时代啦。
#### 模板的别名

#### 函数模板的默认模板参数

### C++11 之智能指针

C++11引入了三种智能指针：

- `std::shared_ptr`
- `std::weak_ptr`
- `std::unique_ptr`

#### `shared_ptr`

**关于`shared_ptr`有几点需要注意：**

- 不要用一个裸指针初始化多个`shared_ptr`，会出现`double_free`导致程序崩溃
- 通过`shared_from_this()`返回this指针，不要把this指针作为`shared_ptr`返回出来，因为`this`指针本质就是裸指针，通过`this`返回可能 会导致重复析构，不能把`this`指针交给智能指针管理。

#### `weak_ptr`

`weak_ptr`是用来监视`shared_ptr`的生命周期，它不管理`shared_ptr`内部的指针，它的拷贝的析构都不会影响引用计数，纯粹是作为一个旁观者监视`shared_ptr`中管理的资源是否存在，可以用来返回`this`指针和解决循环引用问题。

- 作用1：返回`this`指针，上面介绍的`shared_from_this()`其实就是通过`weak_ptr`返回的`this`指针，这里参考我之前写的源码分析`shared_ptr`实现的文章，最后附上链接。
- 作用2：解决循环引用问题。

#### `unique_ptr`

`unique_ptr`是一个独占型的智能指针，它不允许其它智能指针共享其内部指针，也不允许`unique_ptr`的拷贝和赋值。使用方法和`shared_ptr`类似，区别是不可以拷贝：

### C++11 之线程相关所有知识点

C++11关于并发引入了好多好东西，这里按照如下顺序介绍：

- `std::thread`
- `std::mutex`
- `std::lock`
- `std::atomic`
- `std::call_once`
- `volatile`
- `std::condition_variable`
- `std::future`
- `std::async`

### C++11 之基于范围的 for 循环

### C++11 之 `auto` & `decltype`

### C++11 之 `final` & `override`

### C++11 之 `default`

### C++11 之 `delete`

### C++11 之 `explicit`

### C++11 之 `constexpr` & `const`

### C++11 之 `enum class`

### C++11 之非受限联合体

c++11之前union中数据成员的类型不允许有非POD类型，而这个限制在c++11被取消，允许数据成员类型有非POD类型：

```c++
struct A {
   	int a;
   	int *b;
};

union U {
    A   a;  // 非POD类型 c++11之前不可以这样定义联合体
   	int b;
};
```

### C++11 之 `sizeof`

C++11 中 `sizeof` 可以用的类的数据成员上：

```c++
struct A {
   	int data[10];
   	int a;
};

int main() {
    // before C++11:
    A a;
	cout << "size " << sizeof(a.data) << endl;
   	return 0;
    // after C++11:
    cout << "size " << sizeof(A::data) << endl;
    return 0;
}
```

### C++11 之 `assertion`

```c++
static_assert(true/false, message);
```

c++11引入 `static_assert` 声明，用于在编译期间检查，如果第一个参数值为 `false`，则打印 `message`，编译失败。

### C++11 之自定义字面量

C++11 可以自定义字面量，我们平时 C++ 中都或多或少使用过 `chrono` 中的时间，例如：

```c++
std::this_thread::sleep_for(std::chrono::milliseconds(100)); // 100ms
std::this_thread::sleep_for(std::chrono::seconds(100)); // 100s
```

其实没必要这么麻烦，也可以这么写：

```c++
std::this_thread::sleep_for(100ms); // C++14 里可以这么使用，这里只是举个自定义字面量使用的例子
std::this_thread::sleep_for(100s);
```

这就是自定义字面量的使用，示例如下：

```c++
struct myUnit {
    unsigned long long value;
};
constexpr myUnit operator"" _myUnit ( unsigned long long n ) {
    return myUnit{n};
}
myUnit mm = 123_myUnit;
cout << mm.value << endl;
```

### C++11 之内存对齐

### C++11 之 `thread_local`

c++11引入thread_local，用thread_local修饰的变量具有thread周期，每一个线程都拥有并只拥有一个该变量的独立实例，一般用于需要保证线程安全的函数中。

### C++11 之随机数功能

C++11关于随机数功能则较之前丰富了很多，典型的可以选择概率分布类型，先看如下代码：

```c++
#include <time.h>

#include <iostream>
#include <random>

using namespace std;

int main() {
    std::default_random_engine random(time(nullptr));

    std::uniform_int_distribution<int> int_dis(0, 100); // 整数均匀分布
    std::uniform_real_distribution<float> real_dis(0.0, 1.0); // 浮点数均匀分布

    for (int i = 0; i < 10; ++i) {
        cout << int_dis(random) << ' ';
    }
    cout << endl;

    for (int i = 0; i < 10; ++i) {
        cout << real_dis(random) << ' ';
    }
    cout << endl;
    return 0;
}
```

代码中举例的是整数均匀分布和浮点数均匀分布，c++11提供的概率分布类型还有好多，例如伯努利分布、正态分布等，具体可以见最后的参考资料。

### C++11 正则表达式

C++11引入了 `regex` 库更好的支持正则表达式

### C++11 时间库

C++11关于时间引入了 `chrono` 库，源于 `boost`，功能强大，`chrono` 主要有三个点：

- `duration`
- `time_point`
- `clocks`



---

## C++14

### C++14 之函数返回值类型推导

### C++14 之 `lambda` 参数 `auto`

### C++14 之变量模板

C++14支持变量模板：

```c++

template<class T>
constexpr T pi = T(3.1415926535897932385L);

int main()
{
    cout << pi<int> << endl; // 3
    cout << pi<double> << endl; // 3.14159
    return 0;
}
```

### C++14 之别名模板

### C++14 之 `constexpr` 的限制

### C++14 之 `[[deprecated]]` 标记

### C++14 之二进制字面量与整形字面量分隔符

### C++14 之 `std::make_unique`

### C++14 之 `std::shared_timed_mutex` 与 `std::shared_lock`

### C++14 之 `std::integer_sequence`

### C++14 之 `std::exchange`

### C++14 之 `std::quoted`



---

## C++17

### C++17 之构造函数模板推导

在 C++17 前构造一个模板类对象需要指明类型：

```c++
pair<int, double> p(1, 2.2); // before c++17
```

C++17 就不需要特殊指定，直接可以推导出类型，代码如下：

```c++
pair p(1, 2.2); // c++17 自动推导
vector v = {1, 2, 3}; // c++17
```

### C++17 之结构化绑定

通过结构化绑定，对于tuple、map等类型，获取相应值会方便很多：

```c++
```

### C++17 之 `if-switch`语句初始化

C++17前if语句需要这样写代码：

```c++
int a = GetValue();
if (a < 101) {
    cout << a;
}
```

C++17之后可以这样：

```c++
// if (init; condition)

if (int a = GetValue()); a < 101) {
    cout << a;
}

string str = "Hi World";
if (auto [pos, size] = pair(str.find("Hi"), str.size()); pos != string::npos) {
    std::cout << pos << " Hello, size is " << size;
}
```

使用这种方式可以尽可能约束作用域，让代码更简洁，但是可读性略有下降。

### C++17 之内联变量

C++17 前只有内联函数，现在有了内联变量，我们印象中C++类的静态成员变量在头文件中是不能初始化的，但是有了内联变量，就可以达到此目的：

```c++
// header file
struct A {
    static const int value;  
};
inline int const A::value = 10;

// ==========或者========
struct A {
    inline static const int value = 10;
}
```

### C++17 之折叠表达式

C++17引入了折叠表达式使可变参数模板编程更方便：

```c++
template <typename ... Ts>
auto sum(Ts ... ts) {
    return (ts + ...);
}
int a {sum(1, 2, 3, 4, 5)}; // 15
std::string a{"hello "};
std::string b{"world"};
cout << sum(a, b) << endl; // hello world
```

### C++17 之 `constexpr lambda` 表达式

C++17 前 `lambda` 表达式只能在运行时使用，C++17 引入了 `constexpr lambda` 表达式，可以用于在编译期进行计算。

```c++
int main() 
{ 
    // c++17可编译，编译期完成计算
    constexpr auto lamb = [] (int n) { return n * n; };
    static_assert(lamb(3) == 9, "a");
}
```

**注意**

`constexpr` 函数有如下限制：

函数体不能包含汇编语句、`goto` 语句、`label`、`try` 块、静态变量、线程局部存储、没有初始化的普通变量，不能动态分配内存，不能有 `new` `delete` 等，不能虚函数。

### C++17 之 `namespace` 嵌套

```c++
namespace A {
    namespace B {
        namespace C {
            void func();
        }
    }
}

// c++17，更方便更舒适
namespace A::B::C {
    void func();)
}
```

### C++17 之 `__has_include` 预处理表达式

可以判断是否有某个头文件，代码可能会在不同编译器下工作，不同编译器的可用头文件有可能不同，所以可以使用此来判断：

```c++
#if defined __has_include
#if __has_include(<charconv>)
#define has_charconv 1
#include <charconv>
#endif
#endif

std::optional<int> ConvertToInt(const std::string& str)
{
    int value{};
#ifdef has_charconv
    const auto last = str.data() + str.size();
    const auto res = std::from_chars(str.data(), last, value);
    if (res.ec == std::errc{} && res.ptr == last) return value;
#else
    // alternative implementation...
    // do something...
#endif
    return std::nullopt;
}
```

### C++17 之在 `lambda` 表达式用 `*this` 捕获对象副本

正常情况下，`lambda` 表达式中访问类的对象成员变量需要捕获 `this`，但是这里捕获的是 `this` 指针，指向的是对象的引用，正常情况下可能没问题，但是如果多线程情况下，函数的作用域超过了对象的作用域，对象已经被析构了，还访问了成员变量，就会有问题。



所以 C++17 增加了新特性，捕获 `*this`，不持有 `this` 指针，而是持有对象的拷贝，这样生命周期就与对象的生命周期不相关啦。

### C++17 之新增 `Attribute`

我们可能平时在项目中见过 `declspec`，`attribute`，`#pragma` 指示符，使用它们来给编译器提供一些额外的信息，来产生一些优化或特定的代码，也可以给其它开发者一些提示信息。

```c++
struct A { short f[3]; } __attribute__((aligned(8)));

void fatal() __attribute__((noreturn));
```

在 C++11 和 C++14 中有更方便的方法：

```c++
[[carries_dependency]] 让编译期跳过不必要的内存栅栏指令
[[noreturn]] 函数不会返回
[[deprecated]] 函数将弃用的警告

[[noreturn]] void terminate() noexcept;
[[deprecated("use new func instead")]] void func() {}
```

C++17 又新增了三个使得编译器和其它开发者都可以理解开发者的意图：

```c++
// [[fallthrough]]：用在switch中提示可以直接落下去，不需要break，让编译期忽略警告
switch (i) {
    case 1:
        xxx; // warning
    case 2:
        xxx;
        [[fallthrough]];      // 警告消除
    case 3:
        xxx;
       break;
}

// [[nodiscard]] ：表示修饰的内容不能被忽略，可用于修饰函数，标明返回值一定要被处理
[[nodiscard]] int func();
void F() {
    func(); // warning 没有处理函数返回值
}

// [[maybe_unused]] ：提示编译器修饰的内容可能暂时没有使用，避免产生警告
void func1() {}
[[maybe_unused]] void func2() {} // 警告消除
void func3() {
    int x = 1;
    [[maybe_unused]] int y = 2; // 警告消除
}
```

### C++17 之字符串转换

新增 `from_chars` 函数和 `to_chars` 函数，直接看代码：

### C++17 之 `std::variant`

C++17 增加 `std::variant` 实现类似 `union` 的功能，但却比 `union` 更高级，举个例子 `union` 里面不能有 `string` 这种类型，但 `std::variant` 却可以，还可以支持更多复杂类型，如 `map` 等，看代码：

### C++17 之 `std::optional`

我们有时候可能会有需求，让函数返回一个对象，如下：

### C++17 之 `std::any`

### C++17 之 `std::apply`

### C++17 之 `std::make_from_tuple`

### C++17 之 `std::string_view`

### C++17 之 `as_const`

### C++17 之 `file_system`

C++17 正式将 `file_system` 纳入标准中，提供了关于文件的大多数功能，基本上应有尽有，这里简单举几个例子：

### C++17 之 `std::shared_mutex`

C++17 引入了 `shared_mutex`，可以实现读写锁



---

## C++20

### C++20 之比较运算 `<=>`

对于 (a <=> b)，如果a > b ，则运算结果>0，如果a < b，则运算结果<0，如果a==b，则运算结果等于0，注意下，运算符的结果类型会根据a和b的类型来决定，所以我们平时使用时候最好直接用auto，方便快捷。

### C++20 之指定初始化

