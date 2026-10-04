# ESP32 FreeRTOS Multi-Tasking & Queue-Based LED Control System

A robust, multi-threaded embedded system project developed for the **ESP32** microcontroller using the **ESP-IDF** framework and **FreeRTOS**. This project demonstrates real-time event handling by decoupling hardware interrupts (ISRs) from task execution using a thread-safe FreeRTOS Queue mechanism.

---

## 🚀 Key Features

* **Real-Time Operating System (RTOS):** Utilizes FreeRTOS tasks and queues for deterministic and non-blocking multi-tasking.
* **Hardware Interrupts (ISRs):** Fast response times using GPIO interrupts configured on falling edges (`NEGEDGE`) with internal pull-up resistors.
* **RAM Execution Optimization:** Interrupt service routines are mapped directly to instruction RAM via `IRAM_ATTR` to prevent cache misses and system crashes.
* **Inter-Task Communication:** Safe data transfer from ISR context to task context using `xQueueSendFromISR` and `xQueueReceive`.
* **Low Power / CPU Efficiency:** Tasks sleep indefinitely (`portMAX_DELAY`) until an event occurs, keeping CPU utilization minimal.

---

## 🛠️ Hardware Requirements

* **Microcontroller:** ESP32 Development Board
* **Actuators:** 2x LEDs (Connected to GPIO 2 and GPIO 26)
* **Inputs:** 
  * Push Button (Connected to GPIO 32)
  * Digital Sensor (Connected to GPIO 12)
* **Wiring Summary:**
  * Both input pins use internal **Pull-Up** configurations and trigger on falling edge (Active-Low).

---

## 📂 Project Architecture

```text
multitasking_led_project/
├── main/
│   ├── main.c           # Core application logic, ISRs, and FreeRTOS tasks
│   └── CMakeLists.txt   # Component-level build configuration
├── CMakeLists.txt       # Project-level build configuration
└── README.md            # Project documentation
