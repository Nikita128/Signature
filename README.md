# Signature


Консольная программа на C++ для генерации сигнатуры указанного файла.

[![Build Status](https://github.com/Nikita128/Signature/actions/workflows/Ubuntu-latest.yml/badge.svg)](https://github.com/Nikita128/Signature/actions/workflows/Ubuntu-latest.yml)

Сигнатура генерируется следующим образом: исходный файл делится на блоки равной (фиксированной) длины. Если размер файла не кратен размеру блока, последний фрагмент может быть меньше. Для каждого блока вычисляется значение hash-функции и _дописывается в выходной файл-сигнатуру_. В качестве hash-функции использовался MD5.

## Пример

Если размер файла равен 6221846528 байт (~ 6 Гбайт) и размер блока - 1048576 байт (1 Мбайт), то сгенерируется файл размером 94944 байт.

## Требования

1) С++17
2) Boost 1.71
3) CMake 3.18
4) Linux

## Сборка

```
mkdir b
cd b
cmake ..
cmake --build .
```

## Интерфейс

Программа на вход принимает следующие аргументы в соответствующем порядке:
1) Путь до входного файла
2) Путь до выходного файла
3) Размер блока (по умолчанию, 1 Мбайт)

## Пример работы

```
./Signature /path/to/source/file /path/to/destination/file 1

# Compare two files
./Signature /path/to/source/file /path/to/destination/file_1 1
./Signature /path/to/source/file /path/to/destination/file_2 1
cmp --silent /path/to/destination/file_1 /path/to/destination/file_2 && echo "Files are identical" || echo "Files are different"
```
