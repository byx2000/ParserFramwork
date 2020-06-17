# ParserFramwork

ParserFramwork是一个用C++实现的解析器组合子框架，支持自定义解析器组合子，支持自定义类型。

解析器组合子（Parser Combinator）是一种实现解析器的方法，通过定义少量的原子解析器和组合手段，用户可以任意组合这些基本解析器，从而实现复杂的解析功能。

用解析器组合子实现解析器有很多优点，包括实现简洁、可扩展性高等，缺点是效率较低（与递归下降法相比）、调试困难。

ParserFramwork用C++泛型实现了基本的解析器组合子，因此可以支持对用户自定义类型的解析。例如，ParserFramwork不仅支持对字符串的解析，同时也支持对词法分析后的单词流（Token）进行解析。

ParserFramwork在实现时对性能做了一定考虑，减少了大量不必要的复制操作，因此效率与其它解析器组合子库相比有一定优势。

ParserFramwork是对解析器组合子的简单实现，不包含完备的错误处理和错误恢复功能，仅供学习交流，请勿用于正规大型项目。

## 项目基本情况

## 项目基本情况

该项目用Visual Studio 2019开发，请使用最新版本的Visual Studio 2019导入该项目。

项目源码全部保存在`src`目录中：下面是`src`目录的各个子目录和文件介绍：

| 子目录/文件 |        简介        |
| :---------: | :----------------: |
|  Framwork   | 解析器框架核心代码 |
|   Samples   |      使用示例      |
|    Test     |      测试代码      |
|  main.cpp   |      启动代码      |

Framework目录包含了解析器框架的核心代码，主要包括 Scanner.h 、ParserCombinator.h和Rule.h三个文件：

|        文件        |            简介            |
| :----------------: | :------------------------: |
|     Scanner.h      |     输入流类定义与实现     |
| ParserCombinator.h | 基本解析器组合子定义与实现 |
|       Rule.h       |  基本解析器组合子的包装类  |

Samples目录包含了若干使用示例，下面是所有示例的简介：

|   示例名称   |            说明            |
| :----------: | :------------------------: |
| BracketMatch |   检验括号表达式是否匹配   |
|  ExprCalc1   |         表达式求值         |
|  ExprCalc2   | 表达式求值（包含词法分析） |
|    CLexer    |      C语言词法分析器       |
|    Regex     |      简易正规式解析器      |

Test目录包含了整个项目的所有单元测试，包括解析器组合子的单元测试和使用示例的单元测试。

main.cpp文件包含了运行所有单元测试以及检测内存泄漏的代码。

## 使用方法

将整个`src/Framwork`目录包含到项目中，并在需要使用的地方包含 `Rule.h` 头文件。

注意，所有核心类都包含在名称空间`byx`中。

```c++
#include "Framwork/Rule.h"
using namespace byx;
```

### Hello World

下面是一个最简单的使用示例：

```c++
int main()
{
    Rule<char> rule = Rule<char>::Symbol('a') + Rule<char>::Symbol('b').zeroOrMore() + Rule<char>::Symbol('c');
    string s = "abbbbbc";
    vector<char> input(s.begin(), s.end());
    int res = rule.parse(input);
    if (res < 0)
    {
        cout << "parse failed" << endl;
    }
    return 0;
}
```

该示例创建了一个正规式`a(b*)c`的规则（rule），然后构造了输入格式（input），最后调用`rule.parse`方法对指定字符串进行解析。

`+`用于连接两个解析器；`|`用于选择两个解析器中的一个，等价于正规式中的`|`；`zeroOrMore`用于对一个解析器应用0次或多次，等价于正规式中的`*`。

若解析失败，`parse`方法会返回一个负值，否则会返回解析后的输入索引。

若想获取更多用法，请参考`src/Samples`目录下的使用示例。

