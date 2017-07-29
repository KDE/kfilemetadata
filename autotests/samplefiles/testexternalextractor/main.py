#!@PYTHON_EXECUTABLE@

import sys
import json

extractor_data = json.loads(sys.stdin.read())

def extract():
     path = extractor_data.get('path')
     mimetype = extractor_data.get('mimetype')

     doc = open(path)

     return_value = {}
     return_value['properties'] = {}
     return_value['properties']['text'] = doc.read()
     return_value['status'] = 'OK'
     return_value['error'] = ''

     print(json.dumps(return_value))

if __name__ == "__main__":
    extract()
