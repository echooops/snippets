#!/bin/sh
# 用法: * 号内容为选改, ** 为必填，其他则不需要修改

# * - 在目标机器释放到那个目录
target_path=/tmp
# 自动生成无需修改
target_name=
target=$target_path/$target_name

# 分离安装包到指定目录
sed -n -e '1,/^exit 0/!p' $0 > ${target} 2>/dev/null

# ** - 开始写 如何安装 terget_name 的逻辑
# BEGIN ...




# END ...
# 必须以exit 0 结尾
exit 0
