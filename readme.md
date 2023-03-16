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
std::tuple<int, double> func() {
    return std::tuple(1, 2.2);
}

int main()
{
    auto [i, d] = func(); // 是 C++11 的 tie 吗？ 更高级
    cout << i << " " << d << endl;

    auto [i, f] = std::pair(1, 2.3f);
    cout << i << " " << f << endl;

    std::map<int, string> m = {{0, "a"},
                               {1, "b"}};
    for (const auto &[i, s] : m) {
        cout << i << " " << s << endl;
    }
}
```

**注意**

结构化绑定不能应用于 `constexpr`

```c++
constexpr auto[x, y] = std::pair(1, 2.3f); // compile error, C++20 可以
```



### C++17 之 `if-switch`语句初始化

C++17 前 `if` 语句需要这样写代码：

```c++
int a = GetValue();
if (a < 101) {
    cout << a;
}
```

C++17 之后可以这样：

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

C++17 前只有内联函数，现在有了内联变量，我们印象中 C++ 类的静态成员变量在头文件中是不能初始化的，但是有了内联变量，就可以达到此目的：

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
int a{sum(1, 2, 3, 4, 5)};
cout << a << endl;          // 15
std::string b{"hello "};
std::string c{"world"};
cout << sum(b, c) << endl;  // hello world
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

### C++17 之 `filesystem`

C++17 将文件系统的操作封装了进来，从而可以跨平台的操作文件系统，并将 `filesystem` 纳入标准中，提供了关于文件的大多数功能，基本上应有尽有，这里简单举几个例子：

```c++
#include <filesystem>
namespace fs = std::filesystem;

fs::path dir("c:/windows");
cout << boolalpha << fs::exists(dir) << endl;
cout << dir.string() << endl;
// 遍历目录
for (auto &e : fs::directory_iterator(dir)) {
    cout << e.path().string() << " " << e.file_size() << endl;
}
// 递归遍历子目录
for (auto &e : fs::recursive_directory_iterator(dir)) {
    cout << e.path().string() << endl;
}
```



### C++17 之 `std::shared_mutex`

C++17 引入了 `shared_mutex`，可以实现读写锁



---

## C++20

C++20 是 C++11 以来最大的一次改动，甚至比 C++11 还要大，毋庸置疑，C++20 将会和 C++11 一样，从根本上改变我们的 C++ 编程方式，学习 C++20 最好的方式就是多写多用新特性。

C++20 中引入了四大模块：

- `Ranges` 
- `Coroutines` 
- `Concepts` 
- `Modules`

### Ranges

[**Ranges**](https://en.cppreference.com/w/cpp/ranges) 库使其能够在容器上直接表达算法，通过管道符号组合算法，并将其用于数据流中。

要快速掌握 ranges，先要知道 3 个概念。

#### 可迭代对象（range）

就是有 `begin()` 和 `end()` 函数可以用迭代器来遍历的一切东东。`STL` 的大部分容器都是 `range`。

`STL` 的算法库 `algorithms` 使用 `[first,one_beyond_last)` 的范式描述一个操作的范围，但这种描述有些时候显得冗余，并且这些算法实现常常不能返回足够的有用信息。因此，C++20 提供了受约束的算法（Constrained algorithms），它使用了范围，并且修改了一部分函数的返回值。比如说以下代码：

```c++
vector vec{ 1, 8, 5, 3, 2, 9, 5, 2, 7 };
ranges::sort(vec);
```

它简化了对于 `sort` 的调用。由于容器本身满足范围的要求，因此不再需要传入两个迭代器作为参数。

#### 视图（view）

视图，其意义可以参考 `string_view`，它的拷贝代价是很低的，需要拷贝的时候直接传值即可，不必传引用。

`view` 本身也符合 `range` 的特征，可以用迭代器遍历。

`view` 对于数据的遍历都是 lazy evaluation（惰性求值），比如说 `views::iota(1)`，表示一个自然数序列（1, 2, 3, 4, ...）一直到无穷大，这个序列并不是定义的时候就全部产生出来的，而是需要遍历的时候才逐个往后增加的；再比如上面的例子，通过对各种 `view` 的数据处理规则定义了一个变量 `v`，此时所有的数据处理都未进行，直到运行 copy 的时候，才真正执行。这就是惰性求值。

`iota` 是一个仿函数，返回值是一个 `iota_view` 类型的可迭代视图，而 `iota_view` 的 `begin` 和 `end` 方法则返回一个合规的迭代器。

```c++
// 遍历 [0 - 100)
for (int i = 0; i < 100; i++); // C++20 before
// 约她 iota
for (auto i : views::iota(0, 100)); // C++20 after
```

#### 范围适配器（range adaptor）

范围适配器的作用就是将一个可迭代对象 `range` 转换为一个的可迭代视图 `view` 的东东（当然也可以将一个 `view` 转换为另一个 `view`）。

举一个简单的例子，找到所有的质数并且输出他们的平方。

```c++
vector vec{13, 2, 8, 10, 32, 21, 87};
for (auto i : views::transform(views::filter(vec, [](int i) { return i % 2; }), [](int i) { return i * i; })) {
    cout << i << endl;
}
```

这是使用传统的函数调用写法，这个很长而嵌套的函数调用看起来确实难受，毕竟要由内向外地一层一层理解它在干什么，这实在不够直观。而 C++20 Ranges 提供了管道运算符（Pipe operator）`|`，可以直观地按用以下方式完成代码：

```c++
for (auto i : vec 
     | views::filter([](int i) { return i % 2; })
     | views::transform([](int i) { return i * i; })) {
    cout << i << endl;
}
```

这样就能把操作和操作的顺序比较清楚地表达出来，并且提高了可读性。 `filter()` , `transform()`, and `take()` 就是所谓**范围适配器**，范围适配器以  `viewable_range` 为参数，并且可以用 `|` 来连接。标准库提供了许多范围适配器 （[Ranges](https://en.cppreference.com/w/cpp/ranges)）, 它们能够提供对特定范围的变换，并且这些变换很容易组合。比如说取交集：

```c++
vector<vector<int>> vec2{ {1, 2}, {3, 4}, {5, 6} };
for (auto i : vec2 | std::views::join) {
    cout << i << endl;
}
```

通过`std::views::elements` ，我们可以对范围里的元组（`tuple`）取出其中的特定元素：

```c++
vector<tuple<int, int, int>> vec3{ {1, 2, 3}, {4, 5, 6}, {7, 8, 9} };
for (auto e : vec3 | std::views::elements<1>)
cout << e << endl;
```

有了范围适配器，优雅地表达范围的操作成为了可能，而像是reverse这样的范围适配器更是可以减少使用传统的for循环的频率而更多地使用基于范围的for循环。但这么做会损失性能吗？

#### 惰性求值（lazy evaluation）

C++ 以”零成本抽象“为原则，因此使用 C++ 写代码的人会关注性能。范围库用惰性求值的方法确保性能，比如说：

```c++
auto v = vec | views::filter([](int i) { return i % 2; }) | views::transform([](int i) { return i * i; });
cout << *v.begin() << endl;
```

`auto v = ...` 这一整行得到了一个 `view`，但它既不修改 `vec` 中的元素，也不复制元素并储存到其他地方。也就是说，`v` 的构造不涉及到复制和存储中间值，更和 `vec` 的大小没有关系。
下一行中我们将 `v` 表示的一个元素输出在标准输出流上，这时候求值才会发生，对第一个元素的求值也仅发生这一次。

### Coroutines

[**Coroutines**](https://en.cppreference.com/w/cpp/language/coroutines) 库让 C++ 异步编程能力更加强大，甚至可能成为主流。`Coroutines` 是多任务合作、事件循环、管道的基础。

- **进程**：操作系统资源分配的基本单元。调度涉及到用户空间和内核空间的切换，资源消耗较大。
- **线程**：操作系统运行的基本单元。在同一个进程资源的框架下，实现抢占式多任务，相对进程，降低了执行单元切换的资源消耗。
- **协程**：和线程非常类似。但是转变一个思路实现协作式多任务，由用户来实现协作式调度，即主动交出控制权(或称为用户态的线程)。

协程在 C++20 中众望所归地加入标准，是一个受到很多讨论和期待的语言功能。简单地说，协程是可以挂起和恢复的函数。范围的使用并不局限于各种容器，毕竟 ”A range is a concept.“。协程和范围可以一起使用。比如说通过协程库中具备 `begin()` 和 `end()` 的 `generator`，我们可以写一个简单的斐波那契数列生成器：

#### **到底什么是协程？**

- 从语法角度讲，函数代码中含有 `co_await`、`co_yield`、`co_return` 中任何一个关键字，这个函数就是一个协程。
- 从系统角度讲，协程是运行在线程中的一堆代码，这些代码的执行过程可以被中断、恢复，从而实现单线程下的异步。这点是协程异步跟多线程异步的根本区别。在多线程异步的设计中，代码挂起意味着运行堆栈的保存与 CPU 等硬件资源的调度。保存与调度由系统负责，一个线程有且只有一个运行堆栈，线程恢复时，从上次挂起的地方继续执行。
  在协程中，代码的“挂起”与硬件资源的调度不再挂钩：我们挂起一段协程，线程继续运行，CPU 等硬件资源不会被剥夺。
- 从执行流程的角度讲，调用一个普通函数，只有两个状态：
  调用(`invoke`) => 终止(`finalize`)。
  调用一个协程，有四种状态：
  调用(`invoke`) => 挂起(`suspends`) <=> 恢复(`resume`) => 终止(`finalize`)。

![Caller-Callee](./Caller-Callee.png)

简单来说，协程就是一种特殊的函数，它可以在函数执行到某个地方的时候暂停执行，返回给调用者或恢复者（可以有一个返回值），并允许随后从暂停的地方恢复继续执行。
请注意，这个暂停执行不是指将函数所在的线程暂停执行，而是单纯的暂停执行函数本身。
**说白了，用处就是将“异步”风格的编程“同步”化。**

#### **C++20 协程的特点**

1）不需要内部栈分配，仅需要一个调用栈的顶层桢。
2）协程运行过程中，需要使用关键词来控制运行过程（比如 `co_return`）。
3）协程可能分配不同线程，触发资源竞争。
4）没有调度器，但是需要标准和编译器的支持。

#### **协程的特点在于是一个线程执行，那和多线程相比，协程有何优势？**

**优点：**
1）极高的执行效率：因为子程序切换不是线程切换，而是由程序自身控制，因此，没有线程切换的开销。和多线程比，线程数量越多，协程的性能优势就越明显。
2）不需要多线程的锁机制：因为只有一个线程，也不存在同时写变量冲突，在协程中控制共享资源不加锁，只需要判断状态就好了，所以执行效率比多线程高很多。

**缺点：**
1）无法利用多核资源：协程的本质是个单线程，它不能同时将单个 CPU 的多个核用上，协程需要和进程配合才能运行在多 CPU 上。当然我们日常所编写的绝大部分应用都没有这个必要，除非是 CPU 密集型应用。
2）进行阻塞（Blocking）操作（如 IO 时）会阻塞掉整个程序。

#### **协程中的关键字**

`co_wait` ：挂起协程, 等待其它计算完成。
`co_return` ：从协程返回 (协程 return 禁止使用)。
`co_yield` ：返回一个结果并且挂起当前的协程, 下一次调用继续协程的运行。
**注意：上述的协程关键字只能在协程中使用。这也就意味着，在 main 函数中直接调用 `co_await xxxx()`; 是不行的。**

#### 如何定义与使用协程？

**先了解几个基本的概念：**
1）一个线程只能有一个协程；
2）协程函数需要返回值是Promise；
3）协程的所有关键字必须在协程函数中使用；
4）在协程函数中可以按照同步的方式去调用异步函数，只需要将异步函数包装在 `Awaitable` 类中，使用 `co_wait` 关键字调用即可。

### Concepts

Concepts 将改变我们对模板编程的思考与方式。它们能够描述模板具有哪些特性，如果不满足这些特性，你会得到一个简短的错误信息，而不是一大堆不知所云的编译器错误。

### Modules

Modules  引进了 `import` 官架子，让 C++ 在 `#include` 的基础上支持更现代化的模块设计，减轻传统头文件带来的一些问题，提升编译的速度，并带来更简单的构建包的方式。

Module（即模块）避免了传统头文件机制的诸多缺点，一个 Module 是一个独立的翻译单元，包含一个到多个 module interface file（即模块接口文件），包含 0 个到多个 module implementation file（即模块实现文件），使用 Import 关键字即可导入一个模块、使用这个模块暴露的方法。



### C++20 之新增 `Attribute`

```c++
// [[no_unique_address]]
// [[likely]] 和 [[unlikely]]：在分支预测时，用于告诉编译器哪个分支更容易被执行，哪个不容易执行，方便编译器做优化。
constexpr long long fact(long long n) noexcept 
{
    if (n > 1) [[likely]]
        return n * fact(n - 1);
    else [[unlikely]]
        return 1;
}
```

### `for` 循环

括号里可以初始化，优雅。

```c++
for (vector v{ 1,2,3 }; auto & e : v) {
    cout << e << endl;
}
```

### `<=>` 三路比较运算符（Three-way comparison）

三路比较运算符 <=> 也被称为宇宙飞船运算符（spaceship operator）。

三路比较运算符表达式的形式为：*左操作数* **<=>** *右操作数*

**为什么引入**

如果项目中使用 `struct` 的值用作 `std::map` 的 `key`，因此就需要自己实现比较运算符。如果实现得不对（可能会出现自相矛盾，a < b 且 b < a），程序可能会崩溃。

```c++
int a = 0, b = 1, c = 1;

cout << boolalpha << ((a <=> b) < 0) << endl;
cout << boolalpha << ((b <=> c) == 0) << endl;
cout << boolalpha << ((b <=> a) > 0) << endl;

vector A = { 1, 2, 3 }, B = { 1, 2 };
auto cmp = A <=> B;

cout << boolalpha << (cmp > 0) << endl;
```

### 指定初始值设定项（designated initializers）

其实 designated initializers 在 C++17 以前就实装到编译器了，只是在 C++20 才明确写入 C++ 标准，类似的情况还有很多。

```c++
// 这种乱序初始化方式在 C 语言中可以，但是在 C++ 中是不可以的，C++ 里一定要按顺序初始化。
struct A { int x; int y; int z; };
A a{ .y = 2, .x = 1 }; // error; designator order does not match declaration order
A b{ .x = 1, .z = 2 };

// C++11 的时候增加了 default-initialized 概念，数值的默认初始化值被设为0。
int i{}; // i 将被初始化为 0
int j;   // j 将被初始化为未知的一个值
```

### `lambda` 表达式

C++20 之前 `[=]` 会隐式捕获 `this`，而 C++20 需要显式捕获，这样 `[=, this]`

`lambda` 能直接包含在类型中，这样的机制允许我们可以快速写出各种各样 C 库的 `RAII` 包装器，并且由于传递的均是可调用对象，这允许编译器对其优化，直接把我们的删除器代码内联到 `unique_ptr` 内部，太强了。

```c++
// 之前封装的一个 opus_wrapper

// 需要使用仿函数定义删除器
struct CoderDestroyer {
    void operator()(OpusDecoder *decoder) const noexcept
    {
        opus_decoder_destroy(decoder);
    }
};

class Decoder final {
public:
    Decoder(opus_int32 sampleRate, int numChannels) : numChannels_(numChannels)
    {
        int error = OPUS_OK;
        decoder_.reset(opus_decoder_create(sampleRate, numChannels, &error));
        isValid_ = (error == OPUS_OK);
    }
private:
    std::unique_ptr<OpusDecoder, CoderDestroyer>    decoder_{ nullptr };        // 解码器句柄
}

// C++20 after

 std::unique_ptr<OpusDecoder, CoderDestroyer>    decoder_{ nullptr };
```

`lambda` 表达式可以使用模板：

```c++
// generic lambda, operator() is a template with two parameters
auto glambda = []<class T>(T a, auto && b) { return a < b; };

// generic lambda, operator() is a template with one parameter pack
auto f = []<typename ...Ts>(Ts&& ...ts)
{
    return foo(std::forward<Ts>(ts)...);
};
```

### C++20 之 `consteval`

`consteval` 修饰的函数只会在编译期间执行，如果不能编译期间执行，则编译失败。

### C++20 之 `constint`

断言一个变量有静态初始化，即零初始化和常量初始化，否则程序是有问题的。

### C++20 之协程

协程具体是什么不做过多介绍，它最大的特点就是可以使用顺序代码的逻辑执行异步的任务，让我们写异步代码非常的方便。

**限制：**

协程不能使用可变参数、普通返回语句或占位符返回类型( `auto` 或 `Concept` )。`Constexpr` 函数、构造函数、析构函数和主函数不能是协程。

### C++20 之线程同步

- `barrier`：屏障
- `latch`：`CountDownLatch`
- `counting_semaphore`：信号量

### C++20 之 `Modules`

直接看代码：

```c++
// helloworld.cpp
export module helloworld;  // module declaration
import <iostream>;         // import declaration
 
export void hello()
{      
    // export declaration
    std::cout << "Hello world!\n";
}
```

```c++
// main.cpp
import helloworld;  // import declaration
 
int main() {
    hello();
}
```

越来越有 `Python` 的味道

`modules` 使用方式和 `include` 差不多，但 `modules` 使用比 `include` 头文件速度更快，C++ 全球开发者大会中，C++ 之父贴出来过测试数据，`modules` 效率比  `include` 高了25倍。

### C++20 之 `using` 可以引用 `enum`

### C++20 之 `Constraints and concepts` 约束和概念

### C++20 之缩写函数模板

```c++
void f1(auto); // same as template<class T> void f(T)
void f2(C1 auto); // same as template<C1 T> void f2(T), if C1 is a concept
void f3(C2 auto...); // same as template<C2... Ts> void f3(Ts...), if C2 is a concept
void f4(const C3 auto*, C4 auto&); // same as template<C3 T, C4 U> void f4(const T*, U&);
template <class T, C U>
void g(T x, U y, C auto z); // same as template<class T, C U, C W> void g(T x, U y, W z);
```

### C++20 之增加日历和时区的支持

### C++20 之 `source_location`

可作为 `__LINE__` 、`__func__` 这些宏的替代：

### C++20 之 `span`

类模板 `span` 可以表示一个片段。

### C++20 之 `endian`

可获取当前平台是大端序还是小端序。

```c++
#include <bit>
#include <iostream>
 
int main() {
    if constexpr (std::endian::native == std::endian::big) {
        std::cout << "big-endian" << '\n';
    }
    else if constexpr (std::endian::native == std::endian::little) {
        std::cout << "little-endian"  << '\n';
    }
    else {
        std::cout << "mixed-endian"  << '\n';
    }
}
```

### C++20 之 `std::format`

`std::format` 可用于替代 `printf`，它的目的是补充现有的 C++ I/O 流库，并重用它的一些基础设施，如用户定义类型的重载插入操作符。

```c++
std::string message = std::format("The answer is {}.", 42);
```

### C++20 之 `std::atomic`

让智能指针线程安全。

### C++20 之 `std::make_shared`

支持构造数组。

### C++20 之 `std::remove_cvref`

去除CV，去除引用。

```c++
#include <iostream>
#include <type_traits>
 
int main()
{
    std::cout << std::boolalpha
              << std::is_same_v<std::remove_cvref_t<int>, int> << '\n'
              << std::is_same_v<std::remove_cvref_t<int&>, int> << '\n'
              << std::is_same_v<std::remove_cvref_t<int&&>, int> << '\n'
              << std::is_same_v<std::remove_cvref_t<const int&>, int> << '\n'
              << std::is_same_v<std::remove_cvref_t<const int[2]>, int[2]> << '\n'
              << std::is_same_v<std::remove_cvref_t<const int(&)[2]>, int[2]> << '\n'
              << std::is_same_v<std::remove_cvref_t<int(int)>, int(int)> << '\n';
}
// 结果全是 true
```

### C++20 之 `std::to_address`

获得由 `p` 表示的地址，而不形成对 `p` 所指向的对象的引用。

### C++20 之 `std::jthread`

之前的 `std::thread` 在析构时如果没有 `join` 或者 `detach` 会 `crash`，而 `jthread` 在析构时会自动 `join`。`jthread` 也可以取消线程：`request_stop()`。

C++20也引进了一些中断线程执行的相关类：

- `stop_token`：查询线程是否中断
- `stop_source`：请求线程停止运行
- `stop_callback`：`stop_token` 执行时，可以触发的回调函数

### C++20 之 `basic_osyncstream`

它是对 `std::basic_syncbuf` 的再包装，直接使用 `std::cout` 多线程下可能出现数据交叉，`osyncstream` 不会发生这种情况。

```c+
{
    std::osyncstream synced_out(std::cout); // synchronized wrapper for std::cout
    synced_out << "Hello, ";
    synced_out << "World!";
    synced_out << std::endl; // flush is noted, but not yet performed
    synced_out << "and more!\n";
} // characters are transferred and std::cout is flushed
```

### C++20 之 `std::string` 的系列操作

- `string::starts_with`
- `string::ends_with`
- `string_view::starts_with`
- `string::view::ends_with`

### C++20 之 `std::assume_aligned`

### C++20 之 `bind_front`

### C++20 之 `std::ssize`

### C++20 之 `midpoint`

### C++20 之 `lerp`

### C++20 之 `Ranges` 库

`ranges` 库提供了用于处理元素范围的组件，包括各种视图适配器。表示连续元素或者连续元素的片段。

### C++20 之 `std::is_bounded_array` & `std::is_unbounded_array`

检查数组是不是有界

### C++20 之 `numbers` 头文件

- `e`
- `log2e`
- `log10e`
- `pi`
- `inv_pi`
- `inv_sqrtpi`
- `ln2`
- `ln10`
- `sqrt2`
- `sqrt3`
- `inv_sqrt3`
- `egamma`
- `phi`
