# ParserFramwork

ParserFramwork是一个用C++实现的解析器组合子框架

## 使用方法

将 `Framwork` 文件夹下的三个文件 `Scanner.h` 、 `ParserCombinator.h` 、`Rule.h` 包含到项目中，并在需要使用的地方包含 `Rule.h` 头文件。

```c++
#include "Framwork/Rule.h"
using namespace byx;
```

### `Scanner<T> `类

`Scanner` 类是一个泛型类，封装了输入流，可以通过如下方式创建一个字符流：

```c++
string input = "1+2";
Scanner<char> scanner(vector<char>(input.begin(), input.end()));
```

通过如下方式创建一个单词（Token）流：

```c++
vector<Token> tokens = Lexer(...); // 通过词法分析获取单词流
Scanner<Token> scanner(tokens);
```





