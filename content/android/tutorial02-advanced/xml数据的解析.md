---
title: "xml数据的解析"
weight: 19
description: "用 DocumentBuilder 解析 XML：先取到根元素，再按标签名取出节点列表并读取属性"
featureimage: "covers/android-advanced.svg"
date: 2016-11-02
tags: ["Android"]
---

```
<? version="1.0" encoding="utf-8"?>

<Family>
    <Father name="jinfa" age="34"></Father>
    <Mother name="manxiu" age="32"></Mother>
    <Son name="xianyi" age="18"></Son>
</Family>
```

尖括号里的 “Family”、“Father”、“Mother”、“Son” 是属于标签（tag），而 “name”、“age” 则是各个标签对应的属性。

```
DocumentBuilderFactory factory = DocumentBuilderFactory.newInstance();

try {
    DocumentBuilder builder = factory.newDocumentBuilder();

    // 这个方法里面可以传入一个 xml 格式的字符串，也可以是文件
    org.w3c.dom.Document document = builder.parse(getAssets().open("family.xml"));

    // 获取根元素
    org.w3c.dom.Element e = document.getDocumentElement();

    // NodeList list = e.getElementsByTagName("Family");
    String totalString = "";

    NodeList element = e.getElementsByTagName("Father");
    String attributeString = ((org.w3c.dom.Element) element.item(0)).getAttribute("name");
    String ageString = ((org.w3c.dom.Element) element.item(0)).getAttribute("age");
    totalString += attributeString + ":" + ageString + "\n";

    element = e.getElementsByTagName("Mother");
    attributeString = ((org.w3c.dom.Element) element.item(0)).getAttribute("name");
    ageString = ((org.w3c.dom.Element) element.item(0)).getAttribute("age");
    totalString += attributeString + ":" + ageString + "\n";

    element = e.getElementsByTagName("Son");
    attributeString = ((org.w3c.dom.Element) element.item(0)).getAttribute("name");
    ageString = ((org.w3c.dom.Element) element.item(0)).getAttribute("age");
    totalString += attributeString + ":" + ageString + "\n";
```
