#!@PYTHON_EXECUTABLE@

import sys
import json

writer_data = json.loads(sys.stdin.read())

def write():
     path = writer_data.get('path')
     mimetype = writer_data.get('mimetype', '')
     properties = writer_data.get('properties', '')

     output = open(path, 'w')
     output.write(json.dumps(properties))

     return_status = {}
     return_status['status'] = 'OK'
     return_status['error'] = ''

     print(json.dumps(return_status))

if __name__ == "__main__":
    write()
