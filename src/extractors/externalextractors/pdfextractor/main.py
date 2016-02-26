#!/usr/bin/python

import sys
import subprocess
import os.path
import os
import json

from PyPDF2 import PdfFileReader
from PyPDF2.generic import NameObject

extractor_data = json.loads(sys.stdin.read())

def extract():
     path = extractor_data.get('path')
     mimetype = extractor_data.get('mimetype')

     reader = PdfFileReader(path)
     document_info = reader.getDocumentInfo()

     properties = {}
     for property in document_info:

         if property == NameObject('/Author'):
              properties['author'] = document_info[NameObject('/Author')]

         if property == NameObject('/CreationDate'):
              properties['creationDate'] = document_info[NameObject('/CreationDate')]

         if property == NameObject('/Subject'):
              properties['subject'] = document_info[NameObject('/Subject')]

         if property == NameObject('/Title'):
              properties['title'] = document_info[NameObject('/Title')]

     return_value = {}
     return_value['properties'] = properties
     return_value['status'] = 'OK'
     return_value['error'] = ''

     print(json.dumps(return_value))

if __name__ == "__main__":
    extract()
