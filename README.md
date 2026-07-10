
# 🍽 Utensils: 一个轻量级数学/计算机工具库
 
## 🔪 项目简介

Utensils 的目标是通过C语言编写高效率，高性能的核心底层代码，同时在上层使用C++，Python进行抽象封装，最终为用户提供一套兼有高性能内核与便利用户接口的数学与计算机工具库。

## 🍴 厨具展示

 - BLAS: 基础线性代数运算库。提供通用的BLAS接口和更高层级的Matrix, Tensor等类实现；
 - Data Structures: 基础数据结构。Utensils提供高性能的数据结构实现，包括Hash Table, Red-Black Tree等各种数据结构；
 - ...

## 😄 代码风格

这是一套轻量级的工具库，源码追求免去工程代码的繁重，同时保留工程代码的严谨、专业。本工程的源码阅读难度较低，上手快，适合入门后想要进阶的开发者朋友。

## ❓ 安装教程

编译之前，请确保已安装了较新版本的gcc/clang编译器。

gcc 开发使用版本：16.1.1 20260625

clang 开发使用版本：clang-2100.1.1.101

进入项目目录，运行makefile文件进行编译:

```bash
make
```

编译成功后，```libutensils.a```文件会在```lib/```目录下，开发者可自行链接使用。

接口文件目录：```include/```

库目录：```lib/```

## ✅ Recent Updates

### v0.1.0 (核心库和接口的初步实现)

implement core library build system and example linkage
 - Add Makefile to compile all sources into libutensils.a
 - Define public API in include/utensils/tree.h and tree/bst.h
 - Organize src/tree/bst/ with separate tree variants (AVL, RB, Treap, Splay)
 - Add examples/bst_demo.c to demonstrate library usage
 - Ensure library is linkable via -lutensils with correct -I/-L paths
 - First buildable and usable version of utensils library

在**v0.1.0**版本中，Utensils定义了平衡二叉搜索树，BST (Balanced Search Tree) 的接口规范，并在底层提供了四个基础的BST实现: **Red-Black Tree, AVL Tree, Treap, Splay**。

四种树的性能对比，开发者可以编译```benchmarks/bst/```下的```bst_benchmarks.c```文件并运行，在机器上查看不同树实现的性能差异。Utensils在该目录下提供了makefile脚本进行编译。

**注意**：编译benchmarks前请务必确保Utensils库本体已经成功编译并且在```lib/```目录下。否则benchmarks无法链接库文件。

运行

```bash
make
```

以编译代码。编译完成后，在同一目录下，运行：

```
./bench_all
```

这个命令的可用参数定义在```bst_benchmarks.c```中。



