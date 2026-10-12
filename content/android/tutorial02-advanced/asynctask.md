---
title: "AsyncTask"
weight: 4
description: "AsyncTask 是抽象类，三个泛型参数分别是输入、进度与结果；一次只能执行一个任务"
featureimage: "covers/android-advanced.svg"
date: 2016-11-02
tags: ["Android"]
---

`AsyncTask` 是一个抽象类：

```
AsyncTask<InParams, progress, Result>
```

实现类里要有三个泛型参数：第一个是传入参数的类型，第二个是任务执行进度的值类型，`Result` 是任务执行完时的返回值。

`AsyncTask` 必须在执行完一个任务后才能执行下一个任务，只能执行一次，多次执行会触发异常。

# 一、常用方法

- `getStatus()` 获取 AsyncTask 的运行状态
  - `AsyncTask.Status.PENDING`：还没有执行完毕
  - `AsyncTask.Status.RUNNING`
  - `AsycTask.Status.FINISHED`
- `cancel(boolea)` 暂停任务
- `isCancelled()` 判断任务是否暂停

# 二、实现类

必须重写的方法 `doInBackground` 在后台执行耗时操作，是任务的主要实现方法；`params` 是外界要启动这个任务时传进来的一个参数，是个可变长的数组。

```
public class myAsyncTask extends AsyncTask<String, Integer, Integer> {

    // 必须重写的方法，在后台执行耗时操作，任务的主要实现方法
    @Override
    protected Integer doInBackground(String... params) {
        // TODO Auto-generated method stub
        return null;
    }

    // 在任务执行前执行，做一些初始化操作
    @Override
    protected void onPreExecute() {
        // TODO Auto-generated method stub
        super.onPreExecute();
    }

    // 任务完成后执行该方法，并将 doInBackground 的返回值传进来
    @Override
    protected void onPostExecute(Integer result) {
        // TODO Auto-generated method stub
        super.onPostExecute(result);
    }

    // 这里可以执行进度界面更新的操作，
    // 在 doInBackground 方法中调用 publishProgress(values) 就会触发该方法
    @Override
    protected void onProgressUpdate(Integer... values) {
        // TODO Auto-generated method stub
        super.onProgressUpdate(values);
    }
}
```
