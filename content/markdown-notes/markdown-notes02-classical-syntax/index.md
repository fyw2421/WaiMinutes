---
title: "Markdown02 : Classical Syntax"
weight: 3
description: "Emphasis Headers Lists Code Links Image"
date: 2018-05-02
tags: ["markdown"]
featureimage: "covers/markdown-notes02-classical-syntax.svg"
---
## 一、Phrase Emphasis

Markdown treats asterisks \* and underscores \_ as indicators of emphasis.

### 1.1 Bold

* **Syntax**:

  ```markdown
  **Markdown Bold**  or  __Markdown Bold__
  ```

* **Shortcut**: ` ctrl + B `

### 1.2 Italic

* **Syntax**:

  ```markdown
  *Markdown italic* or _Markdown italic_
  ```

* **Shortcut**: `Ctrl + I`

### 1.3 Strikethrough

* **Syntax**:

  ```markdown
  ~~Markdown strikethrough~~
  ```

* **Output**:

    ~~Markdown strikethrough~~

## 二、Headers

Markdown supports two styles of headers, **Setext** and **Atx**.

### 2.1 Setext-style headers

Any number of underlining =’s or -’s will work.

```markdown
This is an H1
===
This is an H2
```

**Shortcut**: `Ctrl + R`

## 七、Links

Markdown supports two style of links: **inline** and **reference**.

### 7.1 Inline Links

* **Syntax**:

  ```markdown
   This is [an example](//example.com/ "Title") inline link.
   [This link](//example.net/) has no title attribute.
  ```

* **Shortcut**: `Ctrl + L`

* **Sample**:

  ` [百度](www.baidu.com "百度一下，你就知道" ) `  [百度](www.baidu.com "百度一下，你就知道" )

### 7.2 Reference Links

* **Syntax**:

  ```markdown
  This is [an example][id] reference-style link.
  [id]: //example.com/+space+"Optional Title Here"
  ```

* **Sample**:

  ```markdown
  [Stackeidt] [id]
  [id]: stackedit.io/edito+space+"Markdown Editor"
  ```

* **引用链接内容定义的形式为**:

  1. Square brackets containing the link identifier (optionally indented from the left margin using **up to three spaces);**

  2. followed by a colon;

  3. ollowed by one or more spaces (or tabs);

  4. followed by the URL for the link;

  5. optionally followed by a **title attribute** for the link, enclosed in **double** or **single quotes**, or enclosed in **parentheses**.

* **Other Style**:

  The following three link definitions are equivalent:

  ```markdown
  [foo]: //example.com/+space+"Optional Title Here"
  [foo]: //example.com/+space+'Optional Title Here'
  [foo]: //example.com/+space+(Optional Title Here)
  ```

## 八、Image

　　Markdown uses an image syntax that is intended to resemble the syntax for links, allowing for two styles: **inline** and **reference**.

### 8.1 Inline Image

* **Syntax**:

  ```markdown
  ![Alt text](/path/to/img.jpg)
  ![Alt text](/path/to/img.jpg "Optional title")
  ```

* **Shortcut**:`Ctrl + G`

* **Sample**:

  ```markdown
  ![Monchhichi](//i1.piimg.com/567571/2d67e28eb36f0b21.jpg "soldier")
  ```

  ![Monchhichi](markdown-syntax-image.webp "soldier")

* **Remark**:

  链接地址应为[图床](//baike.baidu.com/link?url=Gv5rDeP8wsM5yk_6t5wM0y8QbTaXfnwJkNtrbVaXPM0in59BVFPg53CpQ0F0u6B05XKZk4HAZNpOANOL5hoxyLtuWPX-xS8zb5FvDYVpfKa)，否则复制之后图片地址失效，造成无法显示。

  Test ImagePlace :  < //yotuku.cn/#!/>

### 8.2 Reference Image

* **Syntax**:

  ```markdown
  ![Alt text][id]
  [id]: url/to/image+space+"Optional title attribute"
  ```

  That is:

  1.  An exclamation mark: !;

  2.  followed by a set of square brackets, containing the alt attribute text for the image;

  3.  followed by a set of parentheses, containing the URL or path to the image, and an optional title attribute enclosed in double or single quotes.

* **Sample**:

  ```markdown
  ![ xyz ][ ImageXID ]

  [ ImageXID ] : //i1.piimg.com/567571/2d67e28eb36f0b21.jpg
  ```

  ![ abc ][ ImageReferenceID ]

  [ ImageReferenceID ]: markdown-syntax-image.webp "soldier"


## 九、Backslash Escapes

　　Markdown allows you to use backslash escapes to generate literal characters which would otherwise have special meaning in Markdown’s formatting syntax.

<center>` \*literal asterisks\* `</center>

Markdown provides backslash escapes for the following characters:

Characters  | Name
:---:       |---
`\`         | backslash
\`          | backtick
`\*`        | asterisk
`\_`        | underscore
`\{\}`      | curly braces
`\[\]`      | square brackets
`\(\)`      | parentheses
`\#`        | hash mark
`\+`        | plus sign
`\-`        | minus sign (hyphen)
`\`.        | dot
`\!`        | exclamation mark

***

> [**JOHN GRUBER Blog**](//daringfireball.net/projects/markdown/syntax)




