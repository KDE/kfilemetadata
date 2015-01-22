/*
 * This file is part of KFileMetaData
 * Copyright (C) 2014 Vishesh Handa <me@vhanda.in>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#ifndef KFILEMETADATA_TYPES
#define KFILEMETADATA_TYPES

namespace KFileMetaData {
namespace Type {

/**
 * A Type represents a way to represent a way to group files based on
 * a higher level view, which the user generally expects.
 *
 * Every extractor provides a list of types applicable for each file.
 */
enum Type {
    FirstType = 0,

    /**
     * Any file which contains a compressed collection of other files
     * eg - tar, zip, rar, gz
     */
    Archive = 0,

    /**
     * Used to mark any file which just contains audio. Do not use this
     * type if the file also contains Video
     */
    Audio,

    /**
     * Any file which contains Video. It may also contain Audio
     */
    Video,

    /**
     * Any Image file. This includes both raster and vector formats.
     */
    Image,

    /**
     * Any file which counts as a document. Documents are generally
     * files which contain rich text, formatting and maybe images
     */
    Document,

    /**
     * A SpreadSheet file. This is a specialization of the Document type
     * Any file which has this type should also have the Document type
     */
    Spreadsheet,

    /**
     * A Presentation file. This is a specialization of the Document type.
     * Any file which has this type should also have the Document type
     */
    Presentation,

    /**
     * Any file which just contains plain text data counts
     * as a Text file
     */
    Text,

    LastType = Text
};

}
}

#endif
