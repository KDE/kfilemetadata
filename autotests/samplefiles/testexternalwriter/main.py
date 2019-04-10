#!@PYTHON_EXECUTABLE@
#
# Copyright (C) 2016 Varun Joshi <varunj.1011@gmail.com>
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) version 3, or any
# later version accepted by the membership of KDE e.V. (or its
# successor approved by the membership of KDE e.V.), which shall
# act as a proxy defined in Section 6 of version 3 of the license.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public 
# License along with this library.  If not, see <https://www.gnu.org/licenses/>.

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
