/*
    SPDX-FileCopyrightText: 2026 Méven Car <meven@kde.org>

    SPDX-License-Identifier: LGPL-2.1-or-later
*/

#ifndef KFILEMETADATA_HTMLTEXTDOCUMENT_H
#define KFILEMETADATA_HTMLTEXTDOCUMENT_H

#include <QTextDocument>
#include <QUrl>
#include <QVariant>

namespace KFileMetaData
{
/*
 * A QTextDocument to turn HTML from a file into plain text. It loads none of the images and style
 * sheets that the HTML refers to: QTextDocument would read local files and decode data: URLs for
 * them, with every installed image plugin, only for the text to be thrown away.
 */
class HtmlTextDocument : public QTextDocument
{
public:
    using QTextDocument::QTextDocument;

protected:
    QVariant loadResource(int type, const QUrl &name) override
    {
        Q_UNUSED(type)
        Q_UNUSED(name)
        return {};
    }
};
}

#endif
