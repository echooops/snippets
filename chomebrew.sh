echo "# 替换核心软件仓库 #"

echo "# 替换 cask 软件仓库（提供 macOS 应用和大型二进制文件） #"

cd "$(brew --repo)/Library/Taps/homebrew/homebrew-core"
git remote set-url origin https://mirrors.ustc.edu.cn/homebrew-core.git

echo "# 完成 ... #"


echo "# 替换 Bottles 源（Homebrew 预编译二进制软件包） #"

cd "$(brew --repo)/Library/Taps/homebrew/homebrew-cask"
git remote set-url origin https://mirrors.ustc.edu.cn/homebrew-cask.git

echo "# 完成 ... #"



echo "# 修改环境变量仓储 #"

echo "# bash（默认 shell）用户： #"

echo 'export HOMEBREW_BOTTLE_DOMAIN=https://mirrors.ustc.edu.cn/homebrew-bottles' >> ~/.bash_profile
source ~/.bash_profile

echo "# 完成 ... #"


echo "# zsh 用户： #"

echo 'export HOMEBREW_BOTTLE_DOMAIN=https://mirrors.ustc.edu.cn/homebrew-bottles' >> ~/.zshrc
source ~/.zshrc

echo "# 完成 ... #"
