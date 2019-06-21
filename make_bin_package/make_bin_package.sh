#!/bin/sh

path=res
# 修改此脚本中的内容
script_name=install.sh
# * 修改此包名为所需要的包
rpm_name=
base_name=$(basename $rpm_name .rpm)

sed -i "s/^\(target_name=\).*/\1${rpm_name}/g" $path/$script_name
cat $path/$script_name $path/$rpm_name > ${base_name}.bin

chmod a+x ${base_name}.bin
