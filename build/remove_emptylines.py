#!/usr/bin/env python3
fn=str(input("filename:\x20"))
with open(fn, 'r', encoding='utf-8') as f:
    lines = f.readlines()

non_empty_lines = [line for line in lines if line.strip()]

with open(fn, 'w', encoding='utf-8') as f:
    f.writelines(non_empty_lines)

print(f"✅ Removed empty lines from",fn)
print(f"   Before: {len(lines)} lines")
print(f"   After: {len(non_empty_lines)} lines")
print(f"   Removed: {len(lines) - len(non_empty_lines)} empty lines")

