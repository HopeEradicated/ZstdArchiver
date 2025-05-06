# ZstdArchiver

Кроссплатформенное консольное приложение для сжатия и распаковки одного файла с использованием библиотеки [Zstandard (zstd)](https://github.com/facebook/zstd).

---

## 🎯 Возможности

- Архивирование (`a`) и распаковка (`e`) одного файла  
- Уровень сжатия `-l` (1–22) для команды `a`  
- Потоковая обработка блоками (1 MB) — поддержка файлов > 10 GB  
- Корректное прерывание операций по `Ctrl+C`  
- Минимальные внешние зависимости: CMake, C++17, FetchContent (или системный zstd)  
- Кроссплатформенность: Linux, Windows (MSVC) 

---

## 🛠 Зависимости

- **CMake** ≥ 3.15  
- Компилятор с поддержкой **C++17**  
- **Git** (для FetchContent)  

---

## ⚙ Сборка

### Вариант A: Системный пакет zstd (Linux/macOS)

```bash
sudo apt install libzstd-dev         # Ubuntu/Debian
brew install zstd                    # macOS (Homebrew)

git clone https://github.com/HopeEradicated/ZstdArchiver.git
cd ZstdArchiver
cmake -DCMAKE_BUILD_TYPE=Release -B build -S .
cmake --build build --config Release
```

### Вариант B: FetchContent (Windows)

```powershell
git clone https://github.com/HopeEradicated/ZstdArchiver.git
cd ZstdArchiver
cmake -DCMAKE_BUILD_TYPE=Release -B build -S .
cmake --build build --config Release
```

В результате в папке `build/` появится исполняемый файл `archiver` (или `archiver.exe` на Windows).

---

## 🚀 Использование

```bash
# Архивирование
./archiver a [-l <уровень>] <input_file> <archive_file>

# Распаковка
./archiver e <archive_file> <output_file>
```

### Параметры

- `a` — создать архив  
- `e` — распаковать архив  
- `-l <уровень>` — уровень сжатия (1–22), по умолчанию 1. Работает только вместе с `a`.  

### Примеры

```bash
# Простое сжатие уровня 1
./archiver a sample.txt sample.zst

# Сжатие уровня 9
./archiver a -l 9 bigfile.bin bigfile.zst

# Распаковка
./archiver e bigfile.zst bigfile_restored.bin
```

---

## 🛑 Прерывание операции

Во время выполнения архивирования или распаковки нажмите **Ctrl+C** — приложение аккуратно остановит текущую операцию и выйдет с сообщением об отмене.

---

## 🎓 Структура проекта

```
ZstdArchiver/
├── CMakeLists.txt
├── README.md
└── src/
    ├── main.cpp
    ├── archiver.h
    ├── archiver.cpp
    ├── logger.h
    └── signal_handler.h
```
