mkdir build

cmake -B build -S . -DCMAKE_EXPORT_COMPILE_COMMANDS=1

cmake --build build
mv build/BoarCastle .