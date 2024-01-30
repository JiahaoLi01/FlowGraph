# FlowGraph
![58~1(YB$)4I)RTE19OKCQ)C](https://github.com/JiahaoLi01/FlowGraph/assets/103988839/fd00610e-226a-4e7e-8d6c-7685eebc7095)

Flow Graph是一个使用图结构的Gameplay脚本工具，该工具用于替换Unreal Engine中的关卡蓝图，该插件只实现了非常基本的框架和配套的编辑器内容，与Gameplay相关的内容应该根据你的项目来实现。

## Flow Graph的工作原理

Flow Graph的原理受其他于巫师3在GDC上关于对话系统的演讲，每个脚本内容为一个Flow Graph Template，当一个Flow Graph Template被注册到对应的世界子系统的时候，会复制一个自己，成为一个Flow Graph Instance。
每一个Flow Graph Instance中会含有一个或多个Flow Graph Iterator，Iterator可以走向不同的分支，以此来实现并行。该结构对于实现例如游戏中监听多个条件的任务系统非常有效。Flow Graph Iterator是可拓展的。

## Flow Graph的拓展

可以通过实现你自己的Flow Graph Node来拓展该插件，Flow Graph Node也可以使用蓝图来拓展，为那些没有专业程序的团队提供简单支持，但总体上而言，拓展Flow Graph插件需要你的团队有专业的程序支持。

