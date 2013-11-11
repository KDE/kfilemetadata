/*
    Copyright (C) 2013  Vishesh Handa <me@vhanda.in>

    Code adapted from kdegraphics-mobipocket/strigi/
    Copyright (C) 2008 by Jakub Stachowski <qbast@go2.pl>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/


#include "mobiextractor.h"

#include <qmobipocket/mobipocket.h>
#include "qfilestream.h"

#include "nie.h"
#include "nfo.h"
#include "nco.h"

#include <Soprano/Vocabulary/NAO>

#include <KDebug>
#include <QtCore/QDateTime>
#include <QTextDocument>

using namespace Nepomuk2::Vocabulary;
using namespace Soprano::Vocabulary;

namespace Nepomuk2
{

MobiExtractor::MobiExtractor(QObject* parent, const QVariantList&)
    : ExtractorPlugin(parent)
{

}

QStringList MobiExtractor::mimetypes()
{
    QStringList types;
    types << QLatin1String("application/x-mobipocket-ebook");

    return types;
}

SimpleResourceGraph MobiExtractor::extract(const QUrl& resUri, const QUrl& fileUrl, const QString& mimeType)
{
    Q_UNUSED(mimeType);

    SimpleResource fileRes(resUri);
    SimpleResourceGraph graph;

    Mobipocket::QFileStream stream(fileUrl.toLocalFile());
    Mobipocket::Document doc(&stream);
    if (!doc.isValid())
        return graph;

    QMapIterator<Mobipocket::Document::MetaKey, QString> it(doc.metadata());
    while (it.hasNext()) {
        it.next();
        switch (it.key()) {
        case Mobipocket::Document::Title:
            fileRes.addProperty(NIE::title(), it.value());
            break;
        case Mobipocket::Document::Author: {
            SimpleResource con;
            con.addType(NCO::Contact());
            con.addProperty(NCO::fullname(), it.value());

            fileRes.addProperty(NCO::creator(), con);
            graph << con;
            break;
        }
        case Mobipocket::Document::Description: {
            QTextDocument document;
            document.setHtml(it.value());

            QString plain = document.toPlainText();
            if (!plain.isEmpty())
                fileRes.addProperty(NIE::comment(), plain);
            break;
        }
        case Mobipocket::Document::Subject:
            fileRes.addProperty(NIE::subject(), it.value());
            break;
        case Mobipocket::Document::Copyright:
            fileRes.addProperty(NIE::copyright(), it.value());
            break;
        }
    }

    if (!doc.hasDRM()) {
        QString html = doc.text(maxPlainTextSize());

        QTextDocument document;
        document.setHtml(html);

        QString plainText = document.toPlainText();
        if (!plainText.isEmpty())
            fileRes.addProperty(NIE::plainTextContent(), plainText);
    }

    graph << fileRes;
    return graph;
}

}

NEPOMUK_EXPORT_EXTRACTOR(Nepomuk2::MobiExtractor, "nepomukmobiextractor")
