---
title: "Markdown Custom Writing Format"
weight: 1
description: "Writing Template"
date: 2018-05-02
tags: ["markdown"]
featureimage: "covers/markdown-custom-writing-format.svg"
---
# 一、一级标题

## (一) 文字段落

　　全角空格缩进段落

　　余下内容为纯文字

## (二) 文字+列表/表格/代码

如果文字段落后为列表等内容,文字左对齐

```cpp
代码左对齐.
```

name|content
---|---
a|b

## (三) 列表

1. order list content1,项目间加空行

   [三个空格]+内容

2. order list content2

   * Unorder list1

     [`*`后加两个空格]+内容

   * Unorder list2

## (四) 代码

```cpp
代码左对齐.
```

## (五) 表格

标题后为表格,左对齐

name|content
---|---
a|b

# 二、特殊格式

## (一) 居中

`<center>content</center>`

## (二) 换行

`换<br>行`

## (三) 箭头

`**View** &rarr; [**More**](address)`

## (四) 嵌套代码

嵌套代码使用`html`

```html
<pre><code>
markdown code
</code></pre>
```

# 三、注意事项

* 在表格,代码等等输入完成后加入空行.

* 标点符号均为英文,除了首行缩进(全角空格).

* 两种格式之间切换要加空行,比如两个标题之间,列表之间

* 序号后加空格,如`# 1.1 XXX`.文章结尾加换行



