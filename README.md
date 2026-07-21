# ESP32 Waveshare

ESP-IDF project for the **Waveshare ESP32-S3 Touch LCD 7** using LVGL and the custom `DeclarativeUI` framework.

## Requirements

- ESP-IDF 5.5+
- ESP32-S3
- CMake
- Ninja

## Components

- **DeclarativeUI** – Declarative C++ UI framework
- **LVGL** – Graphics library
- **GT911** – Touch controller driver
- **ESP LCD Touch** – ESP-IDF touch abstraction

## Initialize ESP-IDF

Initialize the ESP-IDF environment by sourcing your local installation:

```bash
source ~/Developer/SDKs/esp-idf/export.sh
```

Verify:

```bash
idf.py --version
```

## Basic Commands

Configure target:

```bash
idf.py set-target esp32s3
```

Configure project:

```bash
idf.py menuconfig
```

Reconfigure CMake and dependencies:

```bash
idf.py reconfigure
```

Build:

```bash
idf.py build
```

Clean build:

```bash
idf.py fullclean
```

Flash:

```bash
idf.py flash
```

Monitor:

```bash
idf.py monitor
```

Flash + Monitor:

```bash
idf.py flash monitor
```

Full rebuild:

```bash
idf.py fullclean
idf.py reconfigure
idf.py build flash monitor
```

## Project Layout

```text
components/
    declarative_ui/
    lvgl__lvgl/
    espressif__esp_lcd_touch_gt911/

main/

idf_component.yml
dependencies.lock
sdkconfig
```

## Notes

Do **not** commit:

```text
build/
managed_components/
sdkconfig.old
sdkconfig.backup
```

Keep versioned:

```text
sdkconfig
idf_component.yml
dependencies.lock
CMakeLists.txt
```