import base64
import sys

def xor_with_int8_and_base64(input_file, xor_value):
    """
    Read a binary file, XOR each byte with an int8 value, and encode as base64.
    
    Args:
        input_file: Path to the binary file (helloworld.exe)
        xor_value: Integer between -128 and 127 (int8)
    
    Returns:
        Base64 encoded string of the XORed data
    """
    # Validate xor_value is within int8 range
    if not -128 <= xor_value <= 127:
        raise ValueError(f"xor_value ({xor_value}) must be between -128 and 127")
    
    # Convert the int8 value to a byte (0-255 range for XOR operation)
    xor_byte = xor_value & 0xFF
    
    # Read the binary file
    with open(input_file, 'rb') as f:
        binary_data = f.read()
    
    # XOR each byte with the xor_byte
    xored_data = bytes([byte ^ xor_byte for byte in binary_data])
    
    # Encode to base64
    #base64_result = base64.b64encode(xored_data).decode('ascii')
    base64_result = xored_data
    return base64_result

def main():
    # Example usage
    input_file = str(input('filename:\x20'))
    xor_value = int(input('xorvalue:\x20')) # Example int8 value (-128 to 127)
    
    try:
        # Perform XOR and base64 encode
        result = xor_with_int8_and_base64(input_file, xor_value)
        
        # Print the base64 result
        print(f"XOR value used: {xor_value}")
        print(f"Base64 encoded result:\n{result}")
        
        # Optionally save to a file
        output_file = f"{input_file}_{str(hex(xor_value))[2:]}h.b64"
        with open(output_file, 'wb') as f:
            f.write(result)
        print(f"\nResult also saved to: {output_file}")
        
    except FileNotFoundError:
        print(f"Error: {input_file} not found!")
        sys.exit(1)
    except ValueError as e:
        print(f"Error: {e}")
        sys.exit(1)
    except Exception as e:
        print(f"Unexpected error: {e}")
        sys.exit(1)

if __name__ == "__main__":
    main()
