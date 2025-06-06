ESP-NOW
========

:link_to_translation:`en:[English]`

概述
--------

ESP-NOW 是一种由乐鑫公司定义的无连接 Wi-Fi 通信协议。在 ESP-NOW 中，应用程序数据被封装在各个供应商的动作帧中，然后在无连接的情况下，从一个 Wi-Fi 设备传输到另一个 Wi-Fi 设备。

CTR 与 CBC-MAC 协议 (CCMP) 可用来保护动作帧的安全。ESP-NOW 广泛应用于智能照明、远程控制、传感器等领域。

帧格式
------------

ESP-NOW 使用供应商的动作帧传输数据，默认比特率为 1 Mbps。

目前 ESP-NOW 支持两个版本：v1.0 和 v2.0。v2.0 的设备支持的最大数据包长度为 1470 (``ESP_NOW_MAX_DATA_LEN_V2``) 字节；v1.0 的设备支持的最大数据包长度为 250 (``ESP_NOW_MAX_DATA_LEN``) 字节。

v2.0 设备可以接收来自 v2.0 和 v1.0 设备的数据包。v1.0 设备只能接收来自 v1.0 设备的数据包。

当然，v1.0 设备也可以接收长度不超过 250 (``ESP_NOW_MAX_IE_DATA_LEN``) 的 v2.0 数据包，只是如果长度超过此值，就只接收前 250 (``ESP_NOW_MAX_IE_DATA_LEN``) 字节，或是直接丢弃数据包。

具体行为请参考对应 IDF 版本中的文档。

供应商的动作帧格式为：

.. highlight:: none

::

    -----------------------------------------------------------------------------
    |   MAC 报头   |  分类代码  |  组织标识符  |  随机值  |  供应商特定内容  |   FCS   |
    -----------------------------------------------------------------------------
       24 字节        1 字节        3 字节      4 字节      7-x 字节       4 字节

- 分类代码：分类代码字段可用于指示各个供应商的类别（比如 127）。
- 组织标识符：组织标识符包含一个唯一标识符（比如 0x18fe34），为乐鑫指定的 MAC 地址的前三个字节。
- 随机值：防止重放攻击。
- 供应商特定内容：供应商特定内容包含若干个（大于等于1）特定供应商元素字段，对于 v2.0 版本，x = 1512(1470+6*7)；对于 v1.0 版本，x = 257(250+7)。

特定供应商元素的帧格式为：

.. highlight:: none

::

    ESP-NOW v1.0：
    ---------------------------------------------------------------------------
    |  元素 ID  |  长度  |  组织标识符  |  类型  |  保留  |  版本    |     正文     |
    ---------------------------------------------------------------------------
                                              7~4 比特| 3~0 比特
       1 字节     1 字节     3 字节      1 字节       1 字节           0-250 字节

    ESP-NOW v2.0：
    -------------------------------------------------------------------------------------
    |  元素 ID  |  长度  |  组织标识符  |  类型    |  保留  |更多数据 | 版本    |     正文     |
    -------------------------------------------------------------------------------------
                                                7~5 比特 | 1 比特 | 3~0 比特
       1 字节     1 字节     3 字节      1 字节            1 字节               0-250 字节

- 元素 ID：元素 ID 字段可用于指示特定于供应商的元素。
- 长度：长度是组织标识符、类型、版本和正文的总长度，最大值为 255。
- 组织标识符：组织标识符包含一个唯一标识符（比如 0x18fe34），为乐鑫指定的 MAC 地址的前三个字节。
- 类型：类型字段设置为 4，代表 ESP-NOW。
- 版本：版本字段设置为 ESP-NOW 的版本。
- 正文：正文包含实际要发送的 ESP-NOW 数据。

由于 ESP-NOW 是无连接的，因此 MAC 报头与标准帧略有不同。FrameControl 字段的 FromDS 和 ToDS 位均为 0。第一个地址字段用于配置目标地址。第二个地址字段用于配置源地址。第三个地址字段用于配置广播地址 (0xff:0xff:0xff:0xff:0xff:0xff)。

安全
--------

ESP-NOW 采用 CCMP 方法保护供应商特定动作帧的安全，具体可参考 IEEE Std. 802.11-2012。Wi-Fi 设备维护一个初始主密钥 (PMK) 和若干本地主密钥 (LMKs，每个配对设备拥有一个 LMK)，长度均为 16 个字节。

    * PMK 可使用 AES-128 算法加密 LMK。请调用 :cpp:func:`esp_now_set_pmk()` 设置 PMK。如果未设置 PMK，将使用默认 PMK。
    * LMK 可通过 CCMP 方法对供应商特定的动作帧进行加密。如果未设置配对设备的 LMK，则动作帧不进行加密。

目前，不支持加密组播供应商特定的动作帧。

初始化和反初始化
------------------------------------

调用 :cpp:func:`esp_now_init()` 初始化 ESP-NOW，调用  :cpp:func:`esp_now_deinit()` 反初始化 ESP-NOW。ESP-NOW 数据必须在 Wi-Fi 启动后传输，因此建议在初始化 ESP-NOW 之前启动 Wi-Fi，并在反初始化 ESP-NOW 之后停止 Wi-Fi。

当调用 :cpp:func:`esp_now_deinit()` 时，配对设备的所有信息都将被删除。

添加配对设备
-----------------

在将数据发送到其他设备之前，请先调用  :cpp:func:`esp_now_add_peer()` 将其添加到配对设备列表中。如果启用了加密，则必须设置 LMK。

ESP-NOW 数据可以从 Station 或 SoftAP 接口发送。确保在发送 ESP-NOW 数据之前已启用该接口。

配对设备的信道范围是从 0 ~ 14。如果信道设置为 0，数据将在当前信道上发送。否则，必须使用本地设备所在的通道。

对于接收设备，调用 :cpp:func:`esp_now_add_peer()` 不是必需的。如果没有添加配对设备，只能接收广播包和不加密的单播包。如果需要接收加密的单播包，则必须添加配对设备并设置相同的 LMK。

.. only:: esp32c2

    配对设备的最大数量是 20，其中加密设备的数量不超过 4，默认值是 2。如果想要修改加密设备的数量，在 Wi-Fi menuconfig 设置 :ref:`CONFIG_ESP_WIFI_ESPNOW_MAX_ENCRYPT_NUM`。

.. only:: esp32 or esp32s2 or esp32s3 or esp32c3 or esp32c6

    配对设备的最大数量是 20，其中加密设备的数量不超过 17，默认值是 7。如果想要修改加密设备的数量，在 Wi-Fi menuconfig 设置 :ref:`CONFIG_ESP_WIFI_ESPNOW_MAX_ENCRYPT_NUM`。

发送 ESP-NOW 数据
-----------------

调用 :cpp:func:`esp_now_send()` 发送 ESP-NOW 数据，调用  :cpp:func:`esp_now_register_send_cb` 注册发送回调函数。如果 MAC 层成功接收到数据，则该函数将返回 `ESP_NOW_SEND_SUCCESS` 事件。否则，它将返回 `ESP_NOW_SEND_FAIL`。ESP-NOW 数据发送失败可能有几种原因，比如目标设备不存在、设备的信道不相同、动作帧在传输过程中丢失等。应用层并不一定可以总能接收到数据。如果需要，应用层可在接收 ESP-NOW 数据时发回一个应答 (ACK) 数据。如果接收 ACK 数据超时，则将重新传输 ESP-NOW 数据。可以为 ESP-NOW 数据设置序列号，从而删除重复的数据。

如果有大量 ESP-NOW 数据要发送，调用 ``esp_now_send()`` 时需注意单次发送的数据不能超过最大数据包长（v1.0 是 250 bytes， v2.0 是 1470 字节）。请注意，两个 ESP-NOW 数据包的发送间隔太短可能导致回调函数返回混乱。因此，建议在等到上一次回调函数返回 ACK 后再发送下一个 ESP-NOW 数据。发送回调函数从高优先级的 Wi-Fi 任务中运行。因此，不要在回调函数中执行冗长的操作。相反，将必要的数据发布到队列，并交给优先级较低的任务处理。

接收 ESP-NOW 数据
----------------------

调用 :cpp:func:`esp_now_register_recv_cb()` 注册接收回调函数。当接收 ESP-NOW 数据时，需要调用接收回调函数。接收回调函数也在 Wi-Fi 任务任务中运行。因此，不要在回调函数中执行冗长的操作。
相反，将必要的数据发布到队列，并交给优先级较低的任务处理。

配置 ESP-NOW 速率
----------------------

.. only:: esp32 or esp32s2 or esp32s3 or esp32c2 or esp32c3

    调用 :cpp:func:`esp_wifi_config_espnow_rate()` 配置指定接口的 ESP-NOW 速率。确保在配置速率之前启用接口。这个 API 应该在 :cpp:func:`esp_wifi_start()` 之后调用。

.. only:: esp32c6

    调用 :cpp:func:`esp_now_set_peer_rate_config()` 配置指定 peer 的 ESP-NOW 速率。确保在配置速率之前添加 peer。这个 API 应该在 :cpp:func:`esp_wifi_start()` 和 :cpp:func:`esp_now_add_peer()` 之后调用。

    .. note::

        :cpp:func:`esp_wifi_config_espnow_rate()` 已弃用，请使用 :cpp:func:`esp_now_set_peer_rate_config()`

配置 ESP-NOW 功耗参数
----------------------

当且仅当 {IDF_TARGET_NAME} 配置为 STA 模式时，允许其进行休眠。

进行休眠时，调用 :cpp:func:`esp_now_set_wake_window()` 为 ESP-NOW 收包配置 Window。默认情况下 Window 为最大值，将允许一直收包。

如果对 ESP-NOW 进功耗管理，也需要调用 :cpp:func:`esp_wifi_connectionless_module_set_wake_interval()`。

.. only:: SOC_WIFI_SUPPORTED

    请参考 :ref:`非连接模块功耗管理 <connectionless-module-power-save-cn>` 获取更多信息。

应用示例
----------

- :example:`wifi/espnow` 演示了如何使用 {IDF_TARGET_NAME} 的 Wi-Fi 的 ESPNOW 功能，包括启动 Wi-Fi、初始化 ESP-NOW、注册 ESP-NOW 发送或接收回调函数、添加 ESP-NOW 对等信息，以及在两台设备之间发送和接收 ESP-NOW 数据。

API 参考
-------------

.. include-build-file:: inc/esp_now.inc
