#!/usr/bin/env python3

from struct import pack, pack_into
from json import load
from sys import argv
from uuid import UUID
from os import write

# Use like this to generate a C array:
#   ./gen_auth_descriptor.py example_auth_descriptor.json | xxd -i

FW_MAGIC = b'ByteMe\xffAudi\x00'
FW_REV   = 1002

if __name__ == '__main__':
    if len(argv) != 2:
        print(f'usage: {argv[0]} <input-json>')
        exit(1)
    with open(argv[1], 'r') as io:
        input_data = load(io)
    name = input_data['name'].encode('utf-8')
    part = input_data['part'].encode('utf-8')
    desc = input_data['desc'].encode('utf-8')
    output_data = bytes()
    output_data += FW_MAGIC
    output_data += pack('<I', FW_REV)
    output_data += pack('<I', 0)
    output_data += pack('<B', len(name))
    output_data += pack('<B', len(part))
    output_data += pack('<H', len(desc))
    output_data += UUID(input_data['uuid']).bytes_le
    output_data += pack('<B', len(input_data['values']))
    output_data += name
    output_data += part
    output_data += desc
    for value in input_data['values']:
        name = value['name'].encode('utf-8')
        output_data += pack('<B', len(name))
        output_data += name
    output_data = bytearray(output_data)
    pack_into('<I', output_data, 16, len(output_data))
    write(1, output_data)
