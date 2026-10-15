---
title: "JavaNotes05 : 反射"
weight: 6
description: "Class 类的本质、获取 Class 对象、访问字段与方法、构造对象、动态代理的实现原理，以及反射的性能代价与适用边界。"
featureimage: "covers/java-basics.svg"
date: 2026-10-13
tags: ["Java"]
---

# 一、反射是什么

反射指程序在**运行期**获取类的内部结构（字段、方法、构造器、注解），并动态操作它们——而常规的 `new`、取值、调用都是**编译期**就确定好的。

```java
// 常规写法：编译期确定
Dog dog = new Dog();
dog.speak();

// 反射写法：运行期才知道要操作哪个类
Class<?> cls = Class.forName("com.example.Dog");
Object dog2 = cls.getDeclaredConstructor().newInstance();
cls.getMethod("speak").invoke(dog2);
```

# 二、获取 Class 对象

五种方式，按适用场景选择：

```java
Dog d = new Dog();

Dog.class;                        // 1. 类字面量（推荐，编译期检查，最快）
d.getClass();                     // 2. 实例的 getClass()
Class.forName("com.example.Dog");  // 3. 全限定名（字符串，拼错了要到运行期才发现）
Dog.class.getSuperclass();         // 4. 父类的 Class
Class.forName("[I");               // 5. 基本类型/数组的二进制名（[I = int[]）
```

`Class.forName` 有个重载容易踩：

```java
Class.forName("com.example.Dog");                    // 初始化（执行静态代码块）
Class.forName("com.example.Dog", false, loader);     // 不初始化
```

**两者对静态代码块的执行与否有本质差别**——若只是查询结构（如遍历字段），应当用 `false`，避免触发本不该运行的初始化逻辑。

基本类型与数组的 `Class` 名称：

| 类型 | `getName()` |
| --- | --- |
| `int.class` | `int` |
| `int[].class` | `[I` |
| `String[].class` | `[Ljava.lang.String;` |

# 三、访问字段

```java
class Dog {
    private String name;
    private static int count;
    public final int age = 3;
}

Class<?> cls = Dog.class;

// 公开字段
Field f = cls.getField("age");
System.out.println(f.get(new Dog()));     // 3

// 私有字段必须先 setAccessible
Field name = cls.getDeclaredField("name");
name.setAccessible(true);
System.out.println(name.get(new Dog()));

name.set(new Dog(), "旺财");              // 修改私有字段

// 静态字段不需要实例
Field count = cls.getDeclaredField("count");
count.setAccessible(true);
count.setInt(null, 10);                  // obj 参数传 null
```

## (一) 静态与 final 字段

- **静态字段**：`Field.get(obj)` 的 `obj` 参数必须传 `null`
- **final 字段**：限制与「是基本类型还是引用类型」**无关**，真正的条件是——`static final` 即使 `setAccessible(true)` 也改不了（抛 `IllegalAccessException`）；**非 static 的 final 实例字段可以改**（JDK 9 起）。record 与隐藏类的 final 字段则不行。

```java
Field list = MyBean.class.getDeclaredField("list");
list.setAccessible(true);
((List<String>) list.get(bean)).add("x");   // 合法：改的是内容不是引用
```

# 四、调用方法

```java
Class<?> cls = Dog.class;
Object dog = cls.getDeclaredConstructor().newInstance();

// 公开方法
Method speak = cls.getMethod("speak");            // 找不到抛 NoSuchMethodException
speak.invoke(dog);

// 私有方法
Method secret = cls.getDeclaredMethod("secret");
secret.setAccessible(true);
secret.invoke(dog);

// 带参数：注意 invoke 的参数是 Object[]
Method setName = cls.getMethod("setName", String.class);
setName.invoke(dog, "旺财");

// 静态方法
Method util = StringUtil.class.getMethod("join", String.class, String.class);
util.invoke(null, "a", "b");                      // obj 传 null
```

## (一) invoke 会吞掉异常

`invoke` 抛出的 `InvocationTargetException` 包装了**被调用方法真实抛出的异常**：

```java
try {
    method.invoke(obj);
} catch (InvocationTargetException e) {
    Throwable real = e.getCause();      // ★真正的异常在这里
    log.error("调用失败", real);
}
```

直接打印 `e` 只会看到 `InvocationTargetException`，真实原因被埋在 `getCause()` 里。

## (二) setAccessible 的限制

```java
Field f = SomeClass.class.getDeclaredField("x");
f.setAccessible(true);      // 在模块化环境下可能抛 InaccessibleObjectException
```

Java 9 引入模块系统与 `--add-opens`；**Java 16 起（JEP 396）默认强封装**，反射 JDK 内部非开放包会直接抛 `InaccessibleObjectException`（9–15 只是警告，仍可用 `--illegal-access=permit` 兼容）。要开放需在命令行或代码中显式声明：

```bash
java --add-opens java.base/java.lang=ALL-UNNAMED MyApp
```

# 五、构造对象

```java
Class<?> cls = Class.forName("com.example.Dog");

Dog d1 = (Dog) cls.getDeclaredConstructor().newInstance();          // 无参
Dog d2 = (Dog) cls.getDeclaredConstructor(String.class)
                  .newInstance("旺财");                             // 带参
```

`Class.newInstance()` 是旧 API（已废弃），它要求目标类有**公开无参构造方法**，且异常直接抛出而不包装。新 API `getDeclaredConstructor().newInstance()` 抛 `InvocationTargetException`，需要取 `getCause()`。

# 六、性能代价

反射的调用过程包含**访问权限检查**与**即时编译**，开销显著高于直接调用：

```java
Method m = cls.getMethod("compute");

// 首次调用：还要解析方法、生成字节码
m.invoke(obj);

// 直接调用
obj.compute();
```

实测通常有**一个数量级**的差距。优化手段：

1. **缓存 `Method`/`Field` 对象**，不要每次都 `getMethod`（后者开销更大）
2. 热点路径改用**方法句柄（MethodHandle）**，JVM 能内联优化
3. 高频动态调用场景，考虑生成字节码（如 ByteBuddy）或改用接口多态

```java
// 缓存示例
private static final Map<String, Method> CACHE = new ConcurrentHashMap<>();

static Method method(String name) throws NoSuchMethodException {
    return CACHE.computeIfAbsent(name, n -> {
        try {
            return Dog.class.getMethod(n);
        } catch (NoSuchMethodException e) {
            throw new IllegalStateException(e);
        }
    });
}
```

# 七、动态代理

`java.lang.reflect.Proxy` 在运行期生成实现了指定接口的代理类：

```java
interface Handler {
    Object handle(Object proxy, Method method, Object[] args);
}

InvocationHandler h = (proxy, method, args) -> {
    switch (method.getName()) {
        case "save":   return doSave(args[0]);
        case "delete": return doDelete(args[0]);
        default: throw new UnsupportedOperationException(method.getName());
    }
};

Repository repo = (Repository) Proxy.newProxyInstance(
        Repository.class.getClassLoader(),
        new Class[]{Repository.class},
        h);
```

**只能代理接口**，不能代理具体类（要代理类需用 ByteBuddy / CGLIB）。

代理方法里三个参数的作用：

| 参数 | 含义 |
| --- | --- |
| `proxy` | 生成的代理对象本身，回调中调用它会再次进入此方法，**注意别无限递归** |
| `method` | 被调用的方法，可据此分派 |
| `args` | 实参数组，可能为 `null`（无参方法） |

# 八、什么该用反射

**适合**：框架代码（Spring、Hibernate、Jackson 全靠它）、插件机制、序列化/映射、测试框架。

**不适合**：日常业务代码。理由是**编译期检查失效**——方法名写错、参数类型不对都要到运行期才暴露，且性能差、可读性低。

```java
// 反射的问题：编译通过，运行时才炸
method.invoke(obj, "参数数量或类型可能不对");
```

# 九、参考资料

- [Java 官方教程：反射](https://docs.oracle.com/javase/tutorial/reflection/index.html) — Class、字段、方法访问的官方说明
- [Java 语言规范 JLS（SE 22）](https://docs.oracle.com/javase/specs/jls/se22/html/index.html) — 模块系统与强反射的限制见第 7 章
- [Java SE 22 API 文档](https://docs.oracle.com/en/java/javase/22/docs/api/) — `Class`、`Field`、`Method`、`Proxy`、`InvocationTargetException` 的完整契约
- [OpenJDK](https://openjdk.org/) — 反射与动态代理在 JDK 中的实现源码
