---
title: "AndroidNotes24 : 权限与安全"
weight: 24
description: "Android 权限系统与安全最佳实践：运行时权限申请流程、权限组与使用说明、敏感权限处理、签名与加密、网络安全、组件安全、数据存储安全以及与现代架构的协同使用。"
featureimage: "covers/android-system.svg"
date: 2026-10-14
tags: ["Android"]
---

# 一、为什么需要重视权限与安全

移动应用安全直接关系到用户隐私和设备安全，Android 提供：

- **精细化权限控制**：从安装时授权演进到运行时动态申请
- **安全沙箱机制**：每个应用运行在独立的 Linux 用户空间
- **加密和凭据存储**：提供安全的密钥管理和数据保护
- **组件导出控制**：防止未授权访问应用内部组件
- **网络安全保障**：支持证书固定和明文流量限制

理解并正确使用这些机制是构建可信赖应用的基础。

# 二、权限系统详解

## (一) 权限分类

Android 权限分为三类：

1. **普通权限（Normal Permissions）**
   - 风险较低，系统自动授予
   - 例如：`INTERNET`, `ACCESS_NETWORK_STATE`, `WAKE_LOCK`
   ```xml
   <!-- 在 AndroidManifest.xml 中声明即可 -->
   <uses-permission android:name="android.permission.INTERNET" />
   ```

2. **危险权限（Dangerous Permissions）**
   - 涉及用户隐私或设备操作，需要运行时申请
   - 按组划分，同组权限一次性申请
   - 例如：`CAMERA`, `RECORD_AUDIO`, `ACCESS_FINE_LOCATION`, `READ_CONTACTS`
   ```xml
   <!-- 需要在清单中声明，但还需运行时申请 -->
   <uses-permission android:name="android.permission.CAMERA" />
   ```

3. **特殊权限（Special Permissions）**
   - 不属于危险权限组，但需要特殊申请方式
   - 例如：`SYSTEM_ALERT_WINDOW`（悬浮窗），`WRITE_SETTINGS`（修改系统设置）
   - 需要通过特殊 Intent 引导用户到系统设置页面授予

## (二) 权限组（Permission Groups）

危险权限按功能分组，同组权限只需申请一次：
```kotlin
// 定位权限组
Manifest.permission.ACCESS_FINE_LOCATION
Manifest.permission.ACCESS_COARSE_LOCATION

// 相机权限组
Manifest.permission.CAMERA

// 麦克风权限组
Manifest.permission.RECORD_AUDIO

// 联系人权限组
Manifest.permission.READ_CONTACTS
Manifest.permission.WRITE_CONTACTS
Manifest.permission.GET_ACCOUNTS

// 电话权限组
Manifest.permission.READ_CALL_LOG
Manifest.permission.WRITE_CALL_LOG
Manifest.permission.READ_PHONE_STATE
Manifest.permission.CALL_PHONE
Manifest.permission.ADD_VOICEMAIL
Manifest.permission.USE_SIP
Manifest.permission.PROCESS_OUTGOING_CALLS

// 传感器权限组
Manifest.permission.BODY_SENSORS

// 存储权限组（Android 10+ 有变化）
Manifest.permission.READ_EXTERNAL_STORAGE
Manifest.permission.WRITE_EXTERNAL_STORAGE
```

# 三、运行时权限申请

## (一) 运行时权限申请流程

使用 Activity Result API 申请权限：
```kotlin
class MainActivity : AppCompatActivity() {
    
    private val requestPermissionLauncher = registerForActivityResult(
        ActivityResultContracts.RequestPermission()
    ) { isGranted: Boolean ->
        if (isGranted) {
            // 权限已授予，可以执行相关操作
            startCamera()
        } else {
            // 用户拒绝了权限请求，应显示解释
            if (shouldShowRequestPermissionRationale(
                    Manifest.permission.CAMERA
            )) {
                // 显示解释为什么需要此权限
                showPermissionExplanationDialog()
            } else {
                // 用户勾选了“不再提示”，引导到设置页
                showPermissionDisabledExplanation()
            }
        }
    }
    
    private fun checkAndRequestCameraPermission() {
        when (ContextCompat.checkSelfPermission(
                this, Manifest.permission.CAMERA
        )) {
            PackageManager.PERMISSION_GRANTED -> {
                // 已有权限，直接使用
                startCamera()
            }
            PackageManager.PERMISSION_DENIED -> {
                // 需要申请权限
                requestPermissionLauncher.launch(
                    Manifest.permission.CAMERA
                )
            }
        }
    }
    
    private fun startCamera() {
        // 启动相机的逻辑
    }
}
```

## (二) 一次申请多个权限

申请同一组或不同组的多个权限：
```kotlin
private val multiplePermissionsLauncher = registerForActivityResult(
    ActivityResultContracts.RequestMultiplePermissions()
) { permissions ->
    val cameraGranted = permissions[Manifest.permission.CAMERA] ?: false
    val locationGranted = permissions[Manifest.permission.ACCESS_FINE_LOCATION] ?: false
    
    if (cameraGranted && locationGranted) {
        // 所有权限都已授予
        startAugmentedReality()
    } else {
        // 处理部分授予或全部拒绝的情况
        handlePartialPermissionResult(permissions)
    }
}

// 一次申请多个权限
fun requestCameraAndLocationPermissions() {
    multiplePermissionsLauncher.launch(
        arrayOf(
            Manifest.permission.CAMERA,
            Manifest.permission.ACCESS_FINE_LOCATION
        )
    )
}
```

## (三) 特殊权限申请

对于特殊权限，需要引导用户到系统设置：
```kotlin
private val SYSTEM_ALERT_WINDOW_REQUEST_CODE = 1001

fun checkAndRequestOverlayPermission() {
    if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M &&
        !Settings.canDrawOverlays(this)
    ) {
        // 没有悬浮窗权限，引导用户授予
        Intent(Intent.ACTION_MAIN)
            .apply {
                data = Uri.parse("package:$packageName")
            }
            .also { intent ->
                startActivityForResult(intent, SYSTEM_ALERT_WINDOW_REQUEST_CODE)
            }
    }
}

override fun onActivityResult(
    requestCode: Int, resultCode: Int, data: Intent?
) {
    super.onActivityResult(requestCode, resultCode, data)
    if (requestCode == SYSTEM_ALERT_WINDOW_REQUEST_CODE) {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M &&
            Settings.canDrawOverlays(this)
        ) {
            // 已获得权限，可以创建悬浮窗
            startOverlayService()
        } else {
            // 用户仍未授予权限
            showPermissionRequiredExplanation()
        }
    }
}
```

# 四、敏感权限处理

## (一) 后台定位权限（Android 10+）

从 Android 10 开始，后台定位需要单独申请：
```xml
<!-- 在 AndroidManifest.xml 中 -->
<uses-permission android:name="android.permission.ACCESS_BACKGROUND_LOCATION" />
```

申请流程：
```kotlin
private val backgroundLocationLauncher = registerForActivityResult(
    ActivityResultContracts.RequestPermission()
) { isGranted: Boolean ->
    if (isGranted) {
        // 可以开始后台定位
        startBackgroundLocationTracking()
    } else {
        // 处理拒绝情况
        handleLocationDenied()
    }
}

// 前提是已经授予了前台定位权限
fun requestBackgroundLocationPermission() {
    // 先确认有前台定位权限
    when (ContextCompat.checkSelfPermission(
            this, Manifest.permission.ACCESS_FINE_LOCATION
    )) {
        PackageManager.PERMISSION_GRANTED -> {
            // 有前台定位权限，申请后台定位
            backgroundLocationLauncher.launch(
                Manifest.permission.ACCESS_BACKGROUND_LOCATION
            )
        }
        PackageManager.PERMISSION_DENIED -> {
            // 先申请前台定位权限
            requestPermissionLauncher.launch(
                Manifest.permission.ACCESS_FINE_LOCATION
            )
        }
    }
}
```

## (二) 短信和通话权限（受限制）

从 Android 9（API 28）开始，短信和通话权限受到限制：
- 默认情况下，新申请这些权限的应用在 Google Play 上架时会被拒绝
- 只有特定默认处理器应用（如默认短信应用）才能获得这些权限
- 普通应用应使用替代方案（如使用 SMS Retriever API 接收验证码）

## (三) 身份权限（READ_PHONE_STATE, READ_LOGS 等）

某些身份相关权限在 Android 10 中受到限制：
- `READ_PHONE_STATE` 只有在满足特定条件时才能返回非遮蔽值
- 应考虑使用替代标识符：
  - `ANDROID_ID`（在 Android 8.0+ 中对于每个应用和每个用户是唯一的）
  - `UUID` 随机生成并存储
  - Firebase 安装 ID（FID）
  - Google Play 服务实例 ID（在某些情况下）

# 五、加密与凭据管理

## (一) Keystore 系统

使用 Android Keystore 安全存储加密密钥：
```kotlin
class KeyStoreManager {
    
    private val KEYSTORE_PROVIDER = "AndroidKeyStore"
    private val KEY_NAME = "waiminutes_master_key"
    
    fun generateKey(): SecretKey {
        return try {
            val keyStore = KeyStore.getInstance(KEYSTORE_PROVIDER).apply {
                load(null)
            }
            
            val keyGenerator = KeyGenerator.getInstance(
                KeyProperties.KEY_ALGORITHM_AES, KEYSTORE_PROVIDER
            ).apply {
                // 初始化密钥生成器
                val builder = KeyGenParameterSpec.Builder(
                    KEY_NAME,
                    KeyProperties.PURPOSE_ENCRYPT or KeyProperties.PURPOSE_DECRYPT
                ).apply {
                    setBlockModes(KeyProperties.BLOCK_MODE_GCM)
                    setEncryptionPaddings(KeyProperties.ENCRYPTION_PADDING_NONE)
                    setRandomizedEncryptionRequired(false)
                }
                init(builder.build())
            }
            
            // 生成密钥
            keyGenerator.generateKey()
        } catch (e: Exception) {
            throw RuntimeException(e)
        }
    }
    
    fun getKey(): SecretKey {
        return try {
            val keyStore = KeyStore.getInstance(KEYSTORE_PROVIDER).apply {
                load(null)
            }
            
            val secretKeyEntry = keyStore.getEntry(
                KEY_NAME,
                null
            ) as? KeyStore.SecretKeyEntry
            
            secretKeyEntry?.secretKey
                ?: throw RuntimeException("密钥不存在，请先生成")
        } catch (e: Exception) {
            throw RuntimeException(e)
        }
    }
}

// 使用示例：密文格式 = [12 字节 IV][GCM 密文+Tag]
private const val IV_LENGTH = 12

fun encryptData(plaintext: String): ByteArray {
    val secretKey = KeyStoreManager().getKey()
    
    return try {
        val cipher = Cipher.getInstance("AES/GCM/NoPadding")

        // ★自己生成 IV，并且必须通过 init(...) 交给 cipher。
        //   只 new 一个 ByteArray 而不传给 cipher，cipher 会另外随机生成一个，
        //   两者对不上，解密必然失败。
        val iv = ByteArray(IV_LENGTH).also { SecureRandom().nextBytes(it) }
        cipher.init(Cipher.ENCRYPT_MODE, secretKey, GCMParameterSpec(128, iv))

        // AAD 用于把 IV 也纳入认证：改了 IV 解密就会失败。
        // 必须在 doFinal() 之前调用（init 之后是允许的）。
        cipher.updateAAD(iv)

        val ciphertext = cipher.doFinal(plaintext.toByteArray(Charsets.UTF_8))
        iv + ciphertext          // ★IV 必须随密文一起存下来
    } catch (e: Exception) {
        throw RuntimeException(e)
    }
}

fun decryptData(ciphertext: ByteArray): String {
    val secretKey = KeyStoreManager().getKey()
    
    return try {
        val cipher = Cipher.getInstance("AES/GCM/NoPadding")

        val iv = ciphertext.copyOfRange(0, IV_LENGTH)
        val actualCiphertext = ciphertext.copyOfRange(IV_LENGTH, ciphertext.size)

        cipher.init(Cipher.DECRYPT_MODE, secretKey, GCMParameterSpec(128, iv))
        cipher.updateAAD(iv)          // ★必须与加密端一致，否则 Tag 校验失败

        String(cipher.doFinal(actualCiphertext), Charsets.UTF_8)
    } catch (e: GeneralSecurityException) {
        // GCM 校验失败会抛 AEADBadTagException——说明密文被篡改或 IV 对不上
        throw RuntimeException("解密失败：数据可能被篡改", e)
    }
}
```

⚠ **GCM 的 IV 绝不能「用了就扔」**。同一密钥下重复使用同一个 IV 会彻底破坏 GCM 的安全性——攻击者可以通过密文差异推导出明文信息。每次加密都要重新随机生成 IV，并把 IV 随密文一起存储（通常放最前面）。

## (二) 凭据存储

使用 EncryptedSharedPreferences 安全存储偏好设置：
```kotlin
// 添加依赖
// implementation("androidx.security:security-crypto:1.1.0")

class SecurePrefsManager @Inject constructor(
    @ApplicationContext private val context: Context
) {
    
    private val encryptedSharedPrefs: EncryptedSharedPreferences by lazy {
        // ★MasterKeys 与 AES256_GCM_SPEC 在 security-crypto 1.1.0 中已整体 @Deprecated
        val masterKey = MasterKey.Builder(context)
            .setKeyScheme(MasterKey.KeyScheme.AES256_GCM)
            .build()

        EncryptedSharedPreferences.create(
            context,
            "secure_prefs",
            masterKey,
            EncryptedSharedPreferences.PrefKeyEncryptionScheme.AES256_SIV,
            EncryptedSharedPreferences.PrefValueEncryptionScheme.AES256_GCM
        )
    }
    
    fun saveString(key: String, value: String) {
        encryptedSharedPrefs.edit()
            .putString(key, value)
            .apply()
    }
    
    // ★SharedPreferences.getString 返回 String?，直接 return 会类型不匹配
    fun getString(key: String, defaultValue: String = ""): String {
        return encryptedSharedPrefs.getString(key, defaultValue) ?: defaultValue
    }
    
    fun saveInt(key: String, value: Int) {
        encryptedSharedPrefs.edit()
            .putInt(key, value)
            .apply()
    }
    
    fun getInt(key: String, defaultValue: Int = 0): Int {
        return encryptedSharedPrefs.getInt(key, defaultValue)
    }
    
    fun saveBoolean(key: String, value: Boolean) {
        encryptedSharedPrefs.edit()
            .putBoolean(key, value)
            .apply()
    }
    
    fun getBoolean(key: String, defaultValue: Boolean = false): Boolean {
        return encryptedSharedPrefs.getBoolean(key, defaultValue)
    }
    
    fun remove(key: String) {
        encryptedSharedPrefs.edit()
            .remove(key)
            .apply()
    }
    
    fun clear() {
        encryptedSharedPrefs.edit()
            .clear()
            .apply()
    }
}
```

## (三) 安全随机数

使用 SecureRandom 生成加密强度的随机数：
```kotlin
// 生成安全的随机密码
fun generateSecurePassword(length: Int = 16): String {
    val random = SecureRandom()
    val chars = ('A'..'Z') + ('a'..'z') + ('0'..'9') + "!@#$%^&*()"
    return (1..length)
        .map { chars[random.nextInt(chars.length)] }
        .joinToString("")
}

// 生成安全的随机 Token
fun generateSecureToken(): String {
    val random = SecureRandom()
    val bytes = ByteArray(32)
    random.nextBytes(bytes)
    return Base64.encodeToString(bytes, Base64.NO_WRAP)
}
```

# 六、网络安全

## (一) HTTPS 和证书固定

强制使用 HTTPS 并考虑证书固定：
```kotlin
// 使用 OkHttp 进行证书固定
class CertificatePinningInterceptor(
    private val certificatePinner: CertificatePinner
) : Interceptor {
    
    override fun intercept(chain: Interceptor.Chain): Response {
        val request = chain.request()
        // OkHttp 会自动进行证书固定验证
        // 如果证书不匹配，会抛出 SSLHandshakeException
        return chain.proceed(request)
    }
}

// 配置 OkHttp 客户端
fun createOkHttpClient(): OkHttpClient {
    val certificatePinner = CertificatePinner.Builder()
        .add(
            "api.waiminutes.com",
            "sha256/AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA="
            // 实际中应该是从服务器证书中提取的哈希值
        )
        .build()
    
    return OkHttpClient.Builder()
        .certificatePinner(certificatePinner)
        .addInterceptor { chain ->
            // 强制使用 HTTPS
            val original = chain.request()
            val urlWithoutScheme = original.url
                .replaceQueryParameter("scheme", "https")
            val httpsRequest = original.newBuilder()
                .url(urlWithoutScheme)
                .build()
            chain.proceed(httpsRequest)
        }
        .build()
}

// 或者使用 Network Security Config（推荐）
// 在 res/xml/network_security_config.xml 中：
/*
<?xml version="1.0" encoding="utf-8"?>
<network-security-config>
    <domain-config>
        <domain includeSubdomains="true">api.waiminutes.com</domain>
        <pin-set expiration="2026-01-01">
            <pin digest="SHA-256">AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA=</pin>
        </pin-set>
    </domain-config>
</network-security-config>
*/

// 在 AndroidManifest.xml 中引用：
/*
<application
    ...
    android:networkSecurityConfig="@xml/network_security_config"
    ... >
    ...
</application>
*/
```

## (二) 防止中间人攻击（MITM）

额外的防护措施：
```kotlin
// 检查主机名验证
fun createSafeOkHttpClient(): OkHttpClient {
    return OkHttpClient.Builder()
        .hostnameVerifier { hostname, session ->
            // 自定义主机名验证逻辑
            hostname == "api.waiminutes.com"
        }
        .certificatePinner(CertificatePinner.Builder()
            .add("api.waiminutes.com", "sha256/.../...")
            .build()
        )
        .build()
}

// 在 WebView 中启用安全设置
@SuppressLint("SetJavaScriptEnabled")
fun setupSecureWebView(webView: WebView) {
    webView.settings.apply {
        javaScriptEnabled = true
        allowUniversalAccessFromFileURLs = false
        allowFileAccessFromFileURLs = false
        allowContentAccess = true
        allowFileAccess = false
    }
    
    // 添加 WebView 客户端以处理 URL 加载
    webView.webViewClient = object : WebViewClient() {
        override fun shouldOverrideUrlLoading(
            view: WebView,
            request: WebResourceRequest
        ): Boolean {
            // 防止加载不安全的 URL
            val url = request.url.toString()
            return !url.startsWith("https://") && !url.startsWith("http://localhost")
        }
        
        override fun onReceivedSslError(
            view: WebView,
            handler: SslErrorHandler,
            error: SslError
        ) {
            // 不要简单地忽略 SSL 错误！
            // 正确做法是取消加载并提示用户
            handler.cancel() // 或者更好的处理方式
            Toast.makeText(
                view.context,
                "SSL 错误: ${error.primaryError}",
                Toast.LENGTH_LONG
            ).show()
        }
    }
}
```

## (三) 防止明文流量

Android 9 (API 28) 开始默认禁止明文流量：
```xml
<!-- 在 AndroidManifest.xml 中 -->
<application
    android:usesCleartextTraffic="false"
    ... >
    ...
</application>
```

如果需要明确允许特定域名的明文流量（不推荐）：
```xml
<!-- res/xml/network_security_config.xml -->
<?xml version="1.0" encoding="utf-8"?>
<network-security-config>
    <base-config cleartextTrafficPermitted="true" />
    <domain-config cleartextTrafficPermitted="true">
        <domain>localhost</domain>
    </domain-config>
</network-security-config>
```

# 七、组件安全

## (一) 防止组件被恶意调用

在 AndroidManifest.xml 中正确设置 exported 属性：
```xml
<!-- 明确设置 exported=false（除非需要导出） -->
<activity
    android:name=".InternalActivity"
    android:exported="false" />

<service
    android:name=".InternalService"
    android:exported="false" />

<receiver
    android:name=".InternalReceiver"
    android:exported="false" />

<provider
    android:name=".InternalProvider"
    android:exported="false" />
```

## (二) 使用权限保护导出组件

如果必须导出组件，使用权限进行保护：
```xml
<!-- 导出的 Activity，需要特定权限才能启动 -->
<activity
    android:name=".ExportedActivity"
    android:exported="true"
    android:permission="com.waiminutes.permission.VIEW_INTERNAL_DATA" />

<!-- 声明自定义权限 -->
<permission
    android:name="com.waiminutes.permission.VIEW_INTERNAL_DATA"
    android:protectionLevel="signature" />
```

# 八、安全的 Intent 使用

## (一) 安全的 Intent 使用

防止 Intent 劫持：
```kotlin
// 显式 Intent 而非隐式 Intent
val intent = Intent(this, TargetActivity::class.java)
// 而不是：Intent(ACTION_VIEW, data)

// 如果必须使用隐式 Intent，进行验证
fun isIntentSafe(intent: Intent, context: Context): Boolean {
    val packageManager = context.packageManager
    val activities = packageManager.queryIntentActivities(intent, 0)
    return if (activities.isEmpty()) {
        false // 没有 Activity 可以处理此 Intent
    } else {
        true // 至少有一个 Activity 可以处理
    }
}

// 使用 Pending Intent 时设置标志
val pendingIntent = PendingIntent.getActivity(
    context,
    0,
    intent,
    PendingIntent.FLAG_IMMUTABLE // 或 FLAG_MUTABLE 根据需求
    // Android 12+ 强制指定可变性
)
```

# 九、数据存储安全

## (一) 内部存储 vs 外部存储

- **内部存储**：应用私有目录，其他应用无法访问
  ```kotlin
  // 获取内部存储目录
  val privateDir: File = filesDir
  val cacheDir: File = cacheDir
  
  // 写入数据
  privateDir.resolve("secret.txt").outputStream().use { out ->
      out.write("敏感数据".toByteArray(Charsets.UTF_8))
  }
  ```

- **外部存储**：共享存储，需要权限且其他应用可能访问
  ```kotlin
  // Android 10+ 使用存储访问框架（SAF）
  // 或使用应用专属的外部存储目录（不需要权限）
  val externalCacheDir: File = externalCacheDir
  
  // 访问媒体文件使用 MediaStore
  val collection = if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.Q) {
      MediaStore.Images.Media.getContentUri(
          MediaStore.VOLUME_EXTERNAL_PRIMARY
      )
  } else {
      MediaStore.Images.Media.EXTERNAL_CONTENT_URI
  }
  ```

## (二) 防止意外数据泄漏

```kotlin
// 防止在日志中泄漏敏感信息
fun logSafe(message: String, sensitiveData: String? = null) {
    val safeMessage = if (sensitiveData == null || sensitiveData.isEmpty()) {
        message
    } else {
        message.replace(sensitiveData, "[REDACTED]")
    }
    Log.d(TAG, safeMessage)
}

// 防止剪贴板泄漏敏感数据
fun clearClipboardIfNecessary(context: Context) {
    val clipboard = context.getSystemService(Context.CLIPBOARD_SERVICE) as ClipboardManager
    if (clipboard.hasPrimaryClip()) {
        val clip = clipboard.primaryClip
        // 检查是否包含敏感数据并清除
        // （实际中可能需要更复杂的检测逻辑）
    }
}

// 在截图时隐藏敏感内容
@RequiresApi(Build.VERSION_CODES.O)
fun secureWindow(window: Window) {
    // 设置窗口为安全，防止截屏和录制
    window.setFlags(
        WindowManager.LayoutParams.FLAG_SECURE,
        WindowManager.LayoutParams.FLAG_SECURE
    )
}

// 在 Activity 中使用
class SecureActivity : AppCompatActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        secureWindow(window)
        setContentView(R.layout.activity_secure)
    }
}
```

# 十、与现代架构的协同使用

## (一) 在 Repository 中封装权限检查

```kotlin
class PermissionsRepository @Inject constructor(
    @ApplicationContext private val context: Context
) {
    
    fun checkPermission(permission: String): Boolean {
        return ContextCompat.checkSelfPermission(
            context, permission
        ) == PackageManager.PERMISSION_GRANTED
    }
    
    fun checkPermissions(vararg permissions: String): Boolean {
        return permissions.all { checkPermission(it) }
    }
    
    fun requestPermission(
        permission: String,
        onGranted: () -> Unit,
        onDenied: () -> Unit
    ) {
        if (checkPermission(permission)) {
            onGranted()
        } else {
            // 这里需要 Activity 上下文，通常在 ViewModel 中不可用
            // 更好的做法是返回状态让 UI 层处理
            // 或者使用特殊的 UI 事件流
        }
    }
    
    // 更好的方式：返回权限状态流
    fun observePermissionStatus(permission: String): Flow<Boolean> = callbackFlow {
        // 初始状态
        trySend(checkPermission(permission))
        
        // 监听权限变化（需要广播或其他机制）
        // 实际上权限变化不会自动通知，需要重新检查
        // 这种方法主要用于初始检查，实际中 UI 层通常负责请求
        awaitClose()
    }
}
```

## (二) 在 ViewModel 中处理权限结果

```kotlin
@HiltViewModel
class CameraViewModel @Inject constructor(
    private val permissionsRepository: PermissionsRepository,
    private val savedStateHandle: SavedStateHandle
) : ViewModel() {
    
    private val _uiState = MutableStateFlow<UiState>(
        UiState.ReadyToRequest(
            permissionsRepository.checkPermission(
                Manifest.permission.CAMERA
            )
        )
    )
    val uiState: StateFlow<UiState> = _uiState.asStateFlow()
    
    fun requestCameraPermission() {
        // 实际上 ViewModel 不能直接请求权限
        // 这里需要通过 UI 事件或回调来处理
        _uiState.value = UiState.RequestingPermission
        // 实际权限请求由 Activity/Fragment 发起
        // 结果通过某种方式返回到 ViewModel
    }
    
    // 处理权限请求结果
    fun onPermissionResult(isGranted: Boolean) {
        _uiState.value = when (isGranted) {
            true -> UiState.PermissionGranted
            false -> UiState.PermissionDenied
        }
    }
}

// UI 层处理
@Composable
fun CameraScreen(
    viewModel: CameraViewModel = hiltViewModel(),
    // 需要某种方式从 Activity 获取权限结果
    // 例如通过 rememberUpdatedState 或 SideEffect
) {
    val uiState by viewModel.uiState.collectAsState()
    
    when (uiState) {
        is UiState.ReadyToRequest -> {
            // 显示请求权限的按钮
            Button(onClick = { /* 触发权限请求 */ }) {
                Text("打开相机")
            }
        }
        is UiState.RequestingPermission -> {
            CircularProgressIndicator(modifier = Modifier.fillMaxSize())
        }
        is UiState.PermissionGranted -> {
            CameraPreview()
        }
        is UiState.PermissionDenied -> {
            Column(
                modifier = Modifier
                    .fillMaxSize()
                    .padding(16.dp)
            ) {
                Text("需要相机权限才能使用此功能")
                Button(onClick = { /* 再次请求权限 */ }) {
                    Text("重试")
                }
                Button(onClick = { /* 引导到设置 */ }) {
                    Text("去设置开启")
                }
            }
        }
    }
}
```

## (三) 使用封装的权限处理库

考虑使用如下库简化权限处理：
- [Accompanist Permissions](https://github.com/google/accompanist) —— 用注解声明所需权限，对 Compose 友好
- [EasyPermissions](https://github.com/googlesamples/easypermissions) —— 传统 View 系统
- [RxPermissions](https://github.com/tbruyelle/RxPermissions) —— 基于 RxJava
- [Android 权限设计指南](https://developer.android.com/training/permissions/requesting) —— 官方对权限分组、最小化申请与一次性授权的说明

# 十一、最佳实践与常见问题

## (一) 最佳实践

✅ 最小化权限原则：只申请确实需要的权限  
✅ 明确解释为什么需要权限：在申请前向用户说明用途  
✅ 优先使用系统默认处理器：如使用系统相机而非自行实现  
✅ 处理拒绝情况：区分临时拒绝和永久拒绝（“不再提示”）  
✅ 使用权限组：同组权限一次申请  
✅ 检查设备和系统版本：不同 Android 版本行为不同  
✅ 清理敏感数据：及时清理内存中的敏感信息  
✅ 使用安全存储：如 Keystore、EncryptedSharedPreferences  
✅ 强制使用 HTTPS：并考虑证书固定  
✅ 最小化导出组件：除非必要，否则设置 exported=false  
✅ 定期安全审计：使用工具如 MobSF、Qark 进行检测  
✅ 保持依赖更新：及时更新库以获得安全修复  

## (二) 常见问题及解决方案

### 1. 问题：用户反复拒绝权限导致功能无法使用
**原因**：未正确处理永久拒绝状态或未提供足够说明  
**解决方案**：
- 使用 `shouldShowRequestPermissionRationale()` 区分拒绝类型
- 对于永久拒绝，引导用户到应用设置页面
- 提供清晰的解释说明为什么需要此权限
- 考虑设计无需该权限的替代功能

### 2. 问题：在 Android 10+ 上后台定位不工作
**原因**：缺少 ACCESS_BACKGROUND_LOCATION 权限或前台定位权限  
**解决方案**：
- 确认已声明和申请了前台定位权限 (ACCESS_FINE_LOCATION/ACCESS_COARSE_LOCATION)
- 确认已声明和申请了后台定位权限 (ACCESS_BACKGROUND_LOCATION)
- 检查是否启动了前台服务（后台定位通常需要前台服务）
- 验证是否正确处理了位置回调

### 3. 问题：应用在 Google Play 上架时被拒，理由是权限滥用
**原因**：申请了受限制的权限如短信或通话权限而未被默认处理器授予  
**解决方案**：
- 移除不必要的受限制权限
- 如果应用确实需要这些权限（如默认短信应用），在 Google Play 控制台申请默认处理器资格
- 使用替代方案：如 SMS Retriever API 用于验证码自动填充

### 4. 问题：密钥在设备重置后丢失
**原因**：Android Keystore 与设备锁定状态绑定  
**解决方案**：
- 理解 Keystore 的行为：当设备锁定（如 PIN、图案、密码）被更改或清除时，存储的密钥将被删除
- 对于需要持久化的数据，考虑使用服务器端存储而非仅本地存储
- 实现密钥恢复机制：如在首次启动时生成并安全地将密钥发送到服务器进行备份
- 考虑使用基于用户凭据的加密（如用户密码派生的密钥），但需注意安全性

### 5. 问题：根设备上数据被轻松提取
**原因**：root 设备可以绕过许多安全机制  
**解决方案**：
- 使用 SafetyNet Attestation API 或 Play Integrity API 检测设备完整性
- 对于高安全性应用，考虑在不安全设备上限制功能
- 将关键操作放在服务器端执行
- 使用混淆和防篡改技术增加逆向工程难度
- 接受在完全不受控制的环境中无法提供绝对安全

## (三) 易错点对照

| 误区 | 事实 |
| --- | --- |
| 用户拒绝了权限就一直弹窗 | 系统规定：拒绝两次后不再弹，此时必须引导用户去设置页手动开 |
| 申请全部权限一次通过最省事 | 拒绝率会显著上升。应按功能最小集申请，用到哪个要哪个 |
| 后台定位只要申请 `ACCESS_BACKGROUND_LOCATION` | 该权限**不能**直接申请，必须先 granted 前台定位，再二次请求 |
| 短信/通话权限可以直接申请 | **Android 8.0/9.0** 起属受限权限：非默认处理器应用在**安装时**就被 allowlist 排除，运行时申请恒为 denied。这与 `targetSdk` 无关（`targetSdk 33+` 对应的是 `POST_NOTIFICATIONS`） |
| 把密钥写进 `BuildConfig` 就安全了 | `BuildConfig` 会被打进 APK，反编译即可见。必须进 Keystore |
| 用 `EncryptedSharedPreferences` 存密钥就万无一失 | 它只能加密存储，不能防「解锁后读取」。密钥本体的保护要靠 Keystore + 设备证明 |
| 校验证书只是为了防 MITM | 证书固定（pinning）必须做**备份域名**（backup pin），否则证书轮换会让已装用户全部无法访问 |
| 混淆能保护敏感数据 | 混淆只增加逆向成本，不提供任何机密性。机密性只能靠加密 |

# 十二、参考资料

- [Android 权限指南](https://developer.android.com/guide/topics/permissions/overview)
- [运行时权限请求指南](https://developer.android.com/guide/topics/permissions/requesting)
- [权限组列表](https://developer.android.com/reference/android/Manifest.permission)
- [特殊权限说明](https://developer.android.com/guide/topics/permissions/overview)
- [Android Keystore 系统](https://developer.android.com/training/articles/keystore)
- [加密共享偏好设置](https://developer.android.com/reference/androidx/security/crypto/EncryptedSharedPreferences)
- [网络安全配置](https://developer.android.com/training/articles/security-config)
- [后台定位限制](https://developer.android.com/about/versions/10/behavior-changes-10)
- [短信和通话权限限制](https://developer.android.com/about/versions/nougat/android-7.0#runtime_permissions)
- [数据存储最佳实践](https://developer.android.com/training/data-storage)
- [应用安全最佳实践](https://developer.android.com/training/articles/security-tips)
- [SafetyNet  attestation API](https://developer.android.com/training/safetynet/attestation)
- [Play Integrity API](https://developer.android.com/google/play/integrity)
- [Android 应用加固指南](https://developer.android.com/studio/build/shrink-code)
- [有效的 SSL/TLS 配置](https://developer.android.com/training/articles/security-ssl)
- [ContentProvider 安全指南](https://developer.android.com/guide/topics/providers/content-providers)
- [Intent 和 Intent Filter 安全使用](https://developer.android.com/guide/components/intents-filters#security)
- [Pending Intent 可变性说明](https://developer.android.com/reference/android/app/PendingIntent#FLAG_IMMUTABLE)
- [窗口安全标志](https://developer.android.com/reference/android/view/WindowManager.LayoutParams#FLAG_SECURE)
- [剪贴板访问指南](https://developer.android.com/reference/android/content/ClipboardManager)
- [存储访问框架（SAF）](https://developer.android.com/guide/topics/providers/document-provider)
- MediaStore 访问指南
- [后台执行限制说明](https://developer.android.com/about/versions/oreo/background)
- [前台服务开发指南](https://developer.android.com/guide/components/foreground-services)
- [WorkManager 与后台任务](https://developer.android.com/develop/background-work/background-tasks/persistent/getting-started)
- [EncryptedFile 使用指南](https://developer.android.com/reference/androidx/security/crypto/EncryptedFile)
- [MasterKey 生成说明](https://developer.android.com/reference/androidx/security/crypto/MasterKeys)
- [BiometricPrompt 生物识别认证](https://developer.android.com/training/sign-in/biometric-auth)
- [凭据存储最佳实践](https://developer.android.com/training/articles/keystore#StoringPasswords)
