<h1 style="text-align:center;">HW1</h1>
check ./app/README.md

<h1 style="text-align:center;">HW2</h1>
## How to build graphic app with applied pass ##

This command build app+pass
```bash 
mkdir build
cd ./build
# mac command:
cmake -DLLVM_DIR=/opt/homebrew/opt/llvm@<>/lib/cmake/llvm -DCMAKE_C_COMPILER=/opt/homebrew/opt/llvm@<>/bin/clang -DCMAKE_CXX_COMPILER=/opt/homebrew/opt/llvm@<>/bin/clang++ -DCMAKE_EXPORT_COMPILE_COMMANDS=ON ..
# others platform(should work)
cmake ..
make
```
## Generate llvm ir for app module ##
```bash
make get_app_ir
```

## Build only pass ##
```bash
make build_pass
```

## Get statistic ##
```bash
./graph_app/app/app > ./hi.log
python3 ../statistic/statistic.py ./hi.log
```

## Results ##
Results are reasonable. Because interaction with memory takes the biggest part of runtime time
```txt
Прочитано 56700769 строк из файла './hi.log'

Размер окна: 1
Топ-3 последовательности:
  1. getelementptr<-sext - 10967322 раз(а)
  2. sext<-load - 9139496 раз(а)
  3. icmp<-load - 5498460 раз(а)

Размер окна: 2
Топ-3 последовательности:
  1. sext<-load → getelementptr<-sext - 9139311 раз(а)
  2. getelementptr<-sext → getelementptr<-getelementptr - 5483569 раз(а)
  3. icmp<-load → br<-icmp - 3666703 раз(а)

Размер окна: 3
Топ-3 последовательности:
  1. sext<-load → getelementptr<-sext → getelementptr<-getelementptr - 5483569 раз(а)
  2. getelementptr<-sext → getelementptr<-getelementptr → sext<-load - 3655742 раз(а)
  3. getelementptr<-getelementptr → sext<-load → getelementptr<-sext - 3655742 раз(а)

Размер окна: 4
Топ-3 последовательности:
  1. sext<-load → getelementptr<-sext → getelementptr<-getelementptr → sext<-load - 3655742 раз(а)
  2. getelementptr<-sext → getelementptr<-getelementptr → sext<-load → getelementptr<-sext - 3655742 раз(а)
  3. add<-load → store<-add → icmp<-load → br<-icmp - 1831437 раз(а)

Размер окна: 5
Топ-3 последовательности:
  1. sext<-load → getelementptr<-sext → getelementptr<-getelementptr → sext<-load → getelementptr<-sext - 3655742 раз(а)
  2. getelementptr<-sext → store<-getelementptr → add<-load → store<-add → icmp<-load - 1828087 раз(а)
  3. store<-load → sext<-load → getelementptr<-sext → getelementptr<-getelementptr → sext<-load - 1827902 раз(а)
```

<h1 style="text-align:center;">HW3</h1>

## How to build graphic app with custom ir ##

### Compile to bin file and run
```bash 
mkdir build
cd ./build
# mac command:
cmake -DLLVM_DIR=/opt/homebrew/opt/llvm@<>/lib/cmake/llvm -DCMAKE_C_COMPILER=/opt/homebrew/opt/llvm@<>/bin/clang -DCMAKE_CXX_COMPILER=/opt/homebrew/opt/llvm@<>/bin/clang++ -DCMAKE_EXPORT_COMPILE_COMMANDS=ON ..
# others platform(should work)
cmake ..
make run_app_with_generated_ll
```
### Compile to .ll and run `lli`
```bash 
mkdir build
cd ./build
cmake -DSDL2_DYN_LIB_PATH=<>..
# for instance
cmake -DSDL2_DYN_LIB_PATH=/opt/homebrew/Cellar/sdl2/2.32.10/lib/libSDL2.dylib..
make run_app_with_generated_ll_interp
```
<h1 style="text-align:center;">HW4</h1>

## How to build asm2ir generator ##

### Compile to bin file and run
```bash 
cd asm2ir
mkdir build
cd ./build
# mac command:
cmake -DLLVM_DIR=/opt/homebrew/opt/llvm@<>/lib/cmake/llvm -DCMAKE_C_COMPILER=/opt/homebrew/opt/llvm@<>/bin/clang -DCMAKE_CXX_COMPILER=/opt/homebrew/opt/llvm@<>/bin/clang++ -DCMAKE_EXPORT_COMPILE_COMMANDS=ON ..
# others platform(should work)
cmake ..
make 
./asm2ir ../src/my_app.s
```


