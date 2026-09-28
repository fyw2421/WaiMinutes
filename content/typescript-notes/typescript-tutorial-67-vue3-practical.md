---
title: "六十七、Vue3 实战"
weight: 67
description: "本章介绍 Vue3 实战。"
date: 2026-09-28
tags: ["TypeScript"]
featureimage: "covers/typescript-tutorial.svg"
---

Vue 3 对 TypeScript 有很好的支持，本教程介绍 Vue 3 + TypeScript 的开发实践。

Vue 3 的 Composition API 和 TypeScript 的类型系统完美结合，可以让代码更安全、更易维护。

## (一) 为什么需要在 Vue 3 中使用 TypeScript

Vue 3 从设计之初就全面拥抱 TypeScript，提供了极好的类型支持。

Composition API 的函数式写法与 TypeScript 类型系统完美配合：ref 有泛型支持、computed 有类型推断、props 有完整的类型检查。

> **Vue 3 原生支持：**Vue 3 的源码使用 TypeScript 编写。

## (二) 创建项目

```bash
npm create vite@latest my-vue-app -- --template vue-ts
cd my-vue-app
npm install
```

## (三) 组件类型

```vue
<template>
  <div class="user-card">
    <h3>{{ user.name }}</h3>
    <p>{{ user.email }}</p>
    <button @click="handleEdit">编辑</button>
  </div>
</template>
<script lang="ts">
import { defineComponent, PropType } from 'vue'
interface User { id: number; name: string; email: string }
export default defineComponent({
  name: 'UserCard',
  props: {
    user: { type: Object as PropType<User>, required: true }
  },
  emits: ['edit'],
  setup(props, { emit }) {
    const handleEdit = () => { emit('edit', props.user) }
    return { handleEdit }
  }
})
</script>
```

## (四) 组合式 API 类型

```vue
<template>
  <div>
    <p>计数: {{ count }}</p>
    <p>倍增: {{ doubled }}</p>
    <button @click="increment">+1</button>
    <button @click="decrement">-1</button>
  </div>
</template>
<script lang="ts">
import { ref, computed } from 'vue'
export default {
  setup() {
    const count = ref<number>(0)
    const doubled = computed(() => count.value * 2)
    const increment = () => { count.value++ }
    const decrement = () => { count.value-- }
    return { count, doubled, increment, decrement }
  }
}
</script>
```

## (五) 接口定义

```typescript
// src/types/index.ts
export interface User {
  id: number;
  name: string;
  email: string;
  avatar?: string;
}
export interface CreateUserDTO {
  name: string;
  email: string;
  password: string;
}
export interface ApiResponse {
  success: boolean;
  data?: T;
  error?: string;
}
```

## (六) 注意事项

- defineComponent：提供完整的类型推断
- PropType：复杂 Props 类型需要使用
- ref 泛型：复杂类型显式指定
- emits：声明事件类型

## (七) 总结

- **defineComponent：**获得完整的类型推断
- **PropType：**为 props 提供类型安全
- **ref：**泛型参数指定响应式类型
- **computed：**自动推断计算属性类型
