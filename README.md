## 🛠 Advanced Hardware Abstraction (Layer 1)
The core strength of this protocol lies in its **Hardware Driver Layer**, which I specifically optimized for industrial-grade reliability:

- **Seamless Interface Switching:** Supports both **UART** and **I2C** physical layers. Switching the entire system's communication mode requires only **one line of code** in `bkit_config.h`.
- **DMA Ring Buffer (Zero-Copy):** Utilizes STM32 DMA in Circular mode. By tracking the DMA counter directly, the driver provides a non-blocking "Ring Buffer" that prevents data loss even at high baud rates.
- **Auto-Recovery Logic:** - **UART Watchdog:** Automatically clears ORE (Overrun), NE (Noise), and FE (Frame) flags and restarts DMA in real-time.
    - **I2C Listen Mode:** Implements `EnableListen_IT` to ensure the Slave is always ready to respond to the Master even after unexpected bus resets.
- **Protobuf Serialization:** High-efficiency data encoding using **Nanopb**, making the protocol lightweight and scalable for complex sensor data types.

## 📐 Layered Architecture
1. **Application Layer (`app_main.c`):** High-level API for sending/receiving sensor objects.
2. **Protocol Layer (`bkit_protocol.c`):** Framing, CRC16 calculation, and Protobuf encoding/decoding.
3. **Hardware Driver (`hw_driver.c`):** Direct register/HAL interaction with DMA and peripheral management.
