#include <iostream>
#include <string>
#include <regex>

// 参考 : https://zh.cppreference.com/w/cpp/regex
// 主类
// basic_regex             正则表达式对象
// sub_match               标识子表达式所匹配的字符序列
// match_results           标识一个正则表达式匹配，包含所有子表达式匹配
// 算法
// regex_match             尝试匹配一个正则表达式到整个字符序列
// regex_search            尝试匹配一个正则表达式到字符序列的任何部分
// regex_replace           以格式化的替换文本来替换正则表达式匹配的出现位置
// 迭代器
// regex_iterator          迭代一个字符序列中的所有正则表达式匹配
// regex_token_iterator    迭代给定字符串中的所有正则表达式匹配中的指定子表达式，或迭代未匹配的子字符串
// 异常
// regex_error             报告正则表达式库生成的错误
// 特性
// regex_traits            提供正则表达式库所需的关于字符类型的元信息
// 常量
// synatx_option_type      控制正则表达式行为的通用选项
// match_flag_type         特定于匹配的选项
// error_type              描述不同类型的匹配错误



int main(int argc, char *argv[])
{
    std::string fnames[] = {"foo.txt", "bar.txt", "test", "a0.txt", "AAA.txt", "baz.dat", "tar.gz"};

    // regex : basic_regex<char>
    std::regex txt_regex("([a-z]+)\\.txt");
    // smatch : match_results<std::string::const_iterator>
    std::smatch txt_match;
    for (const auto &fname : fnames) {
        if (std::regex_match(fname, txt_match, txt_regex)) {
            std::cout << "成功匹配:" << fname << "\n";
            for (const auto &e : txt_match) {
                std::cout << "子匹配" << e << "\n";
            }
        }
    }
    return 0;
}
