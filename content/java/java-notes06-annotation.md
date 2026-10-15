---
title: "JavaNotes06 : 注解"
weight: 7
description: "注解的本质与保留期限、内置注解、定义与参数、使用注解与运行时处理注解，以及反射读取注解的完整示例。"
featureimage: "covers/java-basics.svg"
date: 2026-10-13
tags: ["Java"]
---

# 一、注解的本质

注解**不直接影响程序行为**，它是附加在元素上的元数据，供编译器或运行期读取。

从字节码看，注解被编译成一个**实现 `Annotation` 接口的类**：

```java
@Deprecated
public class Demo { }

// 等价于继承 java.lang.annotation.Annotation 的子类
interface MyAnnotation extends java.lang.annotation.Annotation { }
```

因此注解**不能继承、不能有构造方法、不能有抽象方法以外的方法**——接口的所有限制都适用。

# 二、保留期限

`@Retention` 决定注解活到哪一步，是最需要理解的属性：

| 取值 | 生命周期 | 谁能读到 |
| --- | --- | --- |
| `SOURCE`（默认） | 只在源码，**不进 class 文件** | 编译器、注解处理器 |
| `CLASS`（默认写 class 时） | 进 class 文件，**不加载到 JVM** | 反射读不到 |
| `RUNTIME` | 加载到 JVM，可被反射读取 | 运行时反射 |

```java
@Retention(RetentionPolicy.RUNTIME)   // 想反射读取必须写这个
@Target(ElementType.METHOD)
@interface MyAnno { }
```

`CLASS` 常被误解——注解确实写进了 `.class` 文件，但类加载时不保留，运行时 `getAnnotation()` 返回 null。

# 三、内置注解

| 注解 | 作用 |
| --- | --- |
| `@Override` | 编译器校验是否真的重写了父类方法 |
| `@Deprecated` | 标记过时，编译器警告 |
| `@SuppressWarnings` | 抑制警告 |
| `@FunctionalInterface` | 校验接口是否只有一个抽象方法 |
| `@SafeVarargs` | 允许可变参数泛型数组 |

`@Override` 不是必需的，但**强烈建议总是写**——它能在编译期抓出签名写错的问题：

```java
@Override
public void speak(String name) { }     // 父类没有 speak(String) → 编译报错
```

不加 `@Override` 时签名写错只是"重载"而非"重写"，编译能过但逻辑是错的。

# 四、定义注解

```java
@Retention(RetentionPolicy.RUNTIME)
@Target({ElementType.METHOD, ElementType.TYPE})
@Documented
@interface RateLimit {

    int value();                       // 必选：参数名以 value 开头时可省略参数名
    String message() default "";        // 可选：默认值
    Class<?>[] type() default {};       // 可选：数组默认值用 {}
}
```

`@interface` 的所有方法都是**抽象的**，且没有 `public`、`abstract` 等修饰符——写了会编译错误。

## (一) 元素类型 `@Target`

可取：`TYPE`、`FIELD`、`METHOD`、`PARAMETER`、`CONSTRUCTOR`、`LOCAL_VARIABLE`、`ANNOTATION_TYPE`、`PACKAGE`、`TYPE_PARAMETER`、`TYPE_USE`、`MODULE`、`RECORD_COMPONENT`。

不写 `@Target` 则**所有位置都能用**（宽松但容易误用）。

```java
@Target(ElementType.METHOD)      // 只允许标注方法
@interface MethodOnly { }
```

## (二) 是否继承 `@Inherited`

```java
@Inherited                       // 允许子类继承父类上的该注解
@Retention(RUNTIME)
@interface Marker { }
```

**默认不继承**。子类要显式标注。而且 `@Inherited` 只对**类**有效，方法上的注解永远不被继承。

## (三) 重复标注 `@Repeatable`

```java
@Repeatable(Tags.class)
@Retention(RUNTIME)
@interface Tag { String value(); }

@Retention(RUNTIME)
@interface Tags { Tag[] value(); }
```

Java 8 前要用容器注解手动模拟 `@Repeatable` 的效果。

# 五、使用与处理

## (一) 声明在元素上

```java
@RateLimit(value = 10, message = "每秒最多 10 次")
public void query() { }

@RateLimit(10)                          // 参数名以 value 开头时简写
public void query2() { }
```

## (二) 反射读取

```java
public class Processor {

    public static void main(String[] args) throws Exception {
        Method m = Api.class.getMethod("query");
        if (m.isAnnotationPresent(RateLimit.class)) {
            RateLimit anno = m.getAnnotation(RateLimit.class);
            System.out.println("限流值: " + anno.value());
            System.out.println("提示语: " + anno.message());
        }
    }
}
```

**三个前提缺一不可**：

1. 注解 `@Retention(RUNTIME)`
2. 目标元素要能被反射访问（`getDeclaredMethod`）
3. 调用 `getAnnotation` 而不是从 class 文件手工解析

## (三) 完整示例：自定义校验

```java
@Retention(RUNTIME)
@Target({ElementType.FIELD})
@interface NotBlank {
    String message() default "不能为空";
}

public class User {
    @NotBlank
    private String name;

    @NotBlank(message = "邮箱不能为空")
    private String email;
}

public static void validate(Object bean) throws IllegalAccessException {
    for (Field f : bean.getClass().getDeclaredFields()) {
        NotBlank anno = f.getAnnotation(NotBlank.class);
        if (anno == null) continue;
        f.setAccessible(true);
        Object v = f.get(bean);
        if (v == null || v.toString().isBlank()) {
            throw new IllegalArgumentException(f.getName() + ": " + anno.message());
        }
    }
}
```

# 六、注解处理器

编译期处理用 `javax.annotation.processing`，配合 `--processor-path` 启用，典型代表是 **Lombok**、MapStruct、Dagger。

```java
@SupportedAnnotationTypes("com.example.Mark")
@SupportedSourceVersion(SourceVersion.RELEASE_17)
public class MyProcessor extends AbstractProcessor {
    @Override
    public boolean process(Set<? extends TypeElement> annotations, RoundEnvironment env) {
        for (Element e : env.getElementsAnnotatedWith(Mark.class)) {
            // 生成新代码或修改 AST
        }
        return true;
    }
}
```

这类处理器运行在**编译器内部**，因此可以生成源码——普通反射做不到这一点，因为反射发生在运行期。

# 七、常见误区

| 误区 | 事实 |
| --- | --- |
| 注解能改变类行为 | 不能，它只是元数据，需要别的代码读取它才生效 |
| 注解是接口 | 是，但**是编译器生成的接口实现**，不是手写的 |
| `@Retention(CLASS)` 运行时可读 | 不可读，必须 `RUNTIME` |
| 参数可以有默认值 | 可以，但注解**方法的默认值在编译期确定**，不是运行期计算 |
| 注解里能写逻辑 | 不能，`@interface` 的方法全是抽象方法 |

# 八、参考资料

- [Java 官方教程：注解](https://docs.oracle.com/javase/tutorial/java/javaOO/annotations/index.html) — 定义、使用与处理的官方说明
- [Java 语言规范 JLS（SE 22）](https://docs.oracle.com/javase/specs/jls/se22/html/index.html) — 第 9 章「接口」中注解类型与元素语法的规范定义
- [Java SE 22 API 文档](https://docs.oracle.com/en/java/javase/22/docs/api/) — `java.lang.annotation` 包的完整 API，含 `RetentionPolicy`、`ElementType` 枚举
