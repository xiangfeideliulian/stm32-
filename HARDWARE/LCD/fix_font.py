#!/usr/bin/env python3
# -*- coding: utf-8 -*-
import codecs

# Read the original file
with codecs.open('D:/qqfile/电子设计作业/STM32F103RC-LCD_TEST/STM32F103RC-LCD_TEST/HARDWARE/LCD/FONT.c', 'r', encoding='gbk', errors='ignore') as f:
    content = f.read()

# GBK encoded characters
钟 = b'\xD6\xD3'.decode('gbk', errors='ignore')
云 = b'\xD4\xC6'.decode('gbk', errors='ignore')
浩 = b'\xBA\xC6'.decode('gbk', errors='ignore')
魏 = b'\xCE\xBA'.decode('gbk', errors='ignore')
志 = b'\xD6\xBE'.decode('gbk', errors='ignore')
鹏 = b'\xC5\xF4'.decode('gbk', errors='ignore')
班 = b'\xB0\xE0'.decode('gbk', errors='ignore')

print(f"钟 GBK: {repr(钟)}")
print(f"云 GBK: {repr(云)}")
print(f"浩 GBK: {repr(浩)}")
print(f"魏 GBK: {repr(魏)}")
print(f"志 GBK: {repr(志)}")
print(f"鹏 GBK: {repr(鹏)}")
print(f"班 GBK: {repr(班)}")

# Check if characters exist in file
for char, name in [(钟, '钟'), (云, '云'), (浩, '浩'), (魏, '魏'), (志, '志'), (鹏, '鹏'), (班, '班')]:
    pattern = f'"{char}"'
    if pattern in content:
        print(f'Found {name} in file')
    else:
        print(f'{name} NOT found')
