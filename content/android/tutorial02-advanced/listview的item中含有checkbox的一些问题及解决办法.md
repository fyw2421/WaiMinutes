---
title: "listView的item中含有checkbox的一些问题及解决办法"
weight: 12
description: "ListView 的 item 中含 CheckBox 时，点击事件不触发、复用导致选中状态错乱等三个问题及解决办法"
featureimage: "covers/android-advanced.svg"
date: 2016-11-02
tags: ["Android"]
---

# 一、问题1

因为checkbox的点击事件优先级高于listview的item点击事件

所以，checkbox点击后，item的事件将不能触发。

解决方法：

要想checkbox和item的事件都能触发，只需要在item的跟布局中添加

```
android:descendantFocusability="blocksDescendants"
```

# 二、问题2

由于baseAdapter里的getView()方法里的convertview参数可以重用item，

导致你选中一个item里的checkbox后，每隔一定长度的item里的checkbox也会显示选中状态

解决方法：

用一个hashmap记录每个item里checkbox的状态，在每次取出item时，自己根据hashmap里的值初始化checkbox的状态。

```
//初始化checkbox的状态值
h.checkBox.setChecked(checkBoxState.get(position));

//用一个hashmap记录checkbox的变化值
h.checkBox.setOnCheckedChangeListener(new OnCheckedChangeListener() {
    @Override
    public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
        checkBoxState.put(position, isChecked);
        Toast.makeText(mainActivity.this, checkBoxState.get(position)+"", 10).show();
    }
});
```

# 三、问题3

listView滚动时，item里含有checkbox的话，他会自动调用setOnCheckedChangeListener()；

这样的话checkbox的状态就会不断变化。

解决方法：

```
h.checkBox.setOnCheckedChangeListener(null);
h.checkBox.setChecked(checkBoxState.get(position));
h.checkBox.setOnCheckedChangeListener(new OnCheckedChangeListener() {
    @Override
    public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
        checkBoxState.put(position, isChecked);
        Toast.makeText(mainActivity.this, checkBoxState.get(position)+"", 10).show();
    }
});
```

在初始化checkbox的状态值前，添加

```
h.checkBox.setOnCheckedChangeListener(null);
```

所以问题2和问题3一般会一起出现，都要一起解决。

```
public View getView(final int position, View convertView, ViewGroup parent) {
    // TODO Auto-generated method stub
    if (convertView == null) {
        convertView = getLayoutInflater().from(mainActivity.this)
                .inflate(R.layout.itemview, null, false);
        h = new hold();
        h.nameView = (TextView) convertView
                .findViewById(R.id.nameItemTV);
        h.noticeButton = (Button) convertView
                .findViewById(R.id.ItemnoticeBT);
        h.checkBox=(CheckBox) convertView.findViewById(R.id.itemCB);
        h.checkBox.setId(position);
        //	h.editText = (EditText) convertView.findViewById(R.id.itemet);
        convertView.setTag(h);
    } else {
        h = (hold) convertView.getTag();
    }
    h.checkBox.setOnCheckedChangeListener(null);
    h.checkBox.setChecked(checkBoxState.get(position));
    h.checkBox.setOnCheckedChangeListener(new OnCheckedChangeListener() {
        @Override
        public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
            checkBoxState.put(position, isChecked);
            Toast.makeText(mainActivity.this, checkBoxState.get(position)+"", 10).show();
        }
    });
    h.nameView.setText(nameitems.get(position));
    h.noticeButton.setOnClickListener(new OnClickListener() {
        @Override
        public void onClick(View v) {
            Intent intent=new Intent(mainActivity.this,PinglunActivity.class);
            startActivity(intent);
            overridePendingTransition(R.anim.bottomin, R.anim.bubian);
        }
    });
    return convertView;
}
```
