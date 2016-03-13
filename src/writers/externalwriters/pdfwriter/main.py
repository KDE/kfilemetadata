#!/usr/bin/python

import sys
import subprocess
import os.path
import os
import json

from PyPDF2 import PdfFileReader, PdfFileWriter
from PyPDF2.generic import NameObject, createStringObject

writer_data = json.loads(sys.stdin.read())

def write():
     path = writer_data.get('path')
     mimetype = writer_data.get('mimetype')
     properties = writer_data.get('properties')

     reader = PdfFileReader(path)
     document_info = reader.getDocumentInfo()

     for property in properties.keys():
         property_value = properties.get(property)

         if property == 'author':
              document_info[NameObject('/Author')] = createStringObject(property_value)

         if property == 'creationDate':
              document_info[NameObject('/CreationDate')] = createStringObject(property_value)

         if property == 'subject':
              document_info[NameObject('/Subject')] = createStringObject(property_value)

         if property == 'title':
              document_info[NameObject('/Title')] = createStringObject(property_value)

     output = PdfFileWriter()
     output.cloneDocumentFromReader(reader)
     output.addMetadata(document_info)

     outputStream = file(path, 'wb')
     output.write(outputStream)
     outputStream.close()

     return_status = {}
     return_status['status'] = 'OK'
     return_status['error'] = ''

     print(json.dumps(return_status))

if __name__ == "__main__":
    write()
