def xor_file(input_file, output_file, xor_value):
    """
    XOR an entire file with a given key value.
    """
    xor_byte = xor_value & 0xFF
    
    with open(input_file, 'rb') as f:
        data = f.read()
    
    xored_data = bytes([byte ^ xor_byte for byte in data])
    
    with open(output_file, 'wb') as f:
        f.write(xored_data)
    
    return len(data)

# Ask for XOR key
xfile = str(input('input file (default input.img): '))
if xfile=="":xfile="input.img"
xor_key = int(input("Entet uint64 key: "))%256 # the key

# Process c.iso
xor_file("input.img", f"{xor_key}_{xfile}", xor_key)
print("Done!")

