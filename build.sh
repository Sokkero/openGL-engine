rm -rf cmake-build-debug/
mkdir -p cmake-build-debug/conanBuild && cd cmake-build-debug/conanBuild
conan install ../.. --build=missing
