apt-get update
apt-get install wget

apt-get install -y software-properties-common
wget -O - https://apt.llvm.org/llvm-snapshot.gpg.key | apt-key add -
add-apt-repository "deb http://apt.llvm.org/bionic/ llvm-toolchain-bionic-15 main"

apt-get update
apt-get install -y clang-15 lldb-15 lld-15

update-alternatives --install /usr/bin/cc cc /usr/bin/clang-15 100
update-alternatives --install /usr/bin/c++ c++ /usr/bin/clang++-15 100

update-alternatives --config cc
update-alternatives --config c++

cc --version
c++ --version
