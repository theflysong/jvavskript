# jvavskript

这只是一个梗，衍生自jvav,我把它做了出来。

使用help获取帮助。

每个token（关键字）的格式是:token arg1 arg2 arg3 ...

每个arg以空格为界,每个语句以换行结束。

如果你要为此添砖jvav,请看以下说明。

## 参与jvavskript的开Fa♂

先为你的token编写一个token函数，函数格式是:void 名字\(token\*,args\)

token\*是你的token类的指针,args是一个typedef,指向的是std::vector\<string\>，args[1]就是第一个参数。

接下来在initTokens函数内使用registryToken(new token(用法,描述,名字,函数指针));来添加你的token，之后就完事了。

可以参考我的代码来写新token。

in是个istream\*,请使用\*in来获取代码，用cin来获取用户输入。