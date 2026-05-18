#!/usr/bin/env python3
import re
fn = str(input("filename:\x20"))

with open(fn, 'r', encoding='utf-8') as f:
    lines = f.readlines()

cleaned_lines = []
for line in lines:
    stripped = line.rstrip()
    if stripped.startswith('#'):
        continue
    if '#' in line:
        in_string = False
        quote_char = None
        escaped = False
        result = []
        for i, char in enumerate(line):
            if escaped:
                result.append(char)
                escaped = False
                continue
            if char == '\\' and in_string:
                result.append(char)
                escaped = True
                continue
            if char in ('"', "'") and not in_string:
                in_string = True
                quote_char = char
                result.append(char)
            elif char == quote_char and in_string:
                in_string = False
                quote_char = None
                result.append(char)
            elif char == '#' and not in_string:
                result.append('\n')
                break
            else:
                result.append(char)
        cleaned_lines.append(''.join(result))
    else:
        cleaned_lines.append(line)
        
fn = str(input("output:\x20"))

with open(fn, 'w', encoding='utf-8') as f:
    f.writelines(cleaned_lines)

print("All comments removed from",fn)

