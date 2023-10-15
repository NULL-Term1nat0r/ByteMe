#!/usr/bin/env python3

import os
import os.path as path
from hashlib import sha1

BASE_PATH     = 'frontend'
IGNORED_NAMES = ['.DS_Store', 'sensor_info.json']
MIME_TYPES    = {
    '.html': 'text/html',
    '.css':  'text/css',
    '.js':   'text/javascript',
    '.json': 'application/json',
    '.svg':  'image/svg+xml',
    '.png':  'image/png'
}

def dump_and_hash_file(server_path: str, content: bytes) -> tuple[str, int]:
    file_hash = sha1(content).hexdigest()
    file_length = len(content)
    print(f'static const uint8_t g_data_{file_hash}[{file_length}] = {{')
    # Write the file's path as comment
    print(f'  /* {server_path} */')
    # Write all bytes 16-byte rows
    for offset in range(0, len(content), 16):
        row = [f'0x{byte:02x}' for byte in content[offset : offset + 16]]
        print('  ' + ', '.join(row) + ',')
    print(f'}};')
    return file_hash, file_length

if __name__ == '__main__':
    listing = []
    print('#include "StaticResources.hpp"')
    print()
    # Pack all files in BASE_PATH
    for local_root_path, _, filenames in os.walk(BASE_PATH):
        server_root_path = local_root_path[len(BASE_PATH):]
        for name in filenames:
            # Skip over ignored files
            if name in IGNORED_NAMES:
                continue
            # Construct both local and server paths
            local_path = path.join(local_root_path, name)
            server_path = path.join(server_root_path, name)
            if not server_path.startswith('/'):
                server_path = '/' + server_path
            # Read and dump the file
            with open(local_path, 'rb') as io:
                content = io.read()
                file_hash, file_length = dump_and_hash_file(server_path, content)
                listing.append((server_path, file_hash, file_length))
                print()
    # Generate the static resources
    for server_path, file_hash, file_length in listing:
        mime_type = 'application/octet-stream'
        for extension, current_mime_type in MIME_TYPES.items():
            if server_path.endswith(extension):
                mime_type = current_mime_type
                break
        print(f'const StaticResource g_file_{file_hash} = {{')
        print(f'  "{server_path}", "{mime_type}", g_data_{file_hash}, {file_length}')
        print(f'}};')
        print()
    # Generate the listing of static resources
    print(f'const StaticResource *g_staticResources[] = {{')
    for _, file_hash, _ in listing:
        print(f'  &g_file_{file_hash},')
    print(f'  nullptr,')
    print(f'}};')
    print()
