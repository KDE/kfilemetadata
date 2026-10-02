/*
    SPDX-FileCopyrightText: 2026 Méven Car <meven@kde.org>

    SPDX-License-Identifier: LGPL-2.1-or-later
*/

#include "extractors/htmltextdocument.h"

#include <QTest>
#include <QTextFrame>

using namespace KFileMetaData;

class HtmlTextDocumentTest : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void testImagesAreNotLoaded();
};

// A 1x1 PNG as the page background, and an image in the text.
static const QString s_html = QStringLiteral(
    "<html><body background=\"data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAAEAAAABCAIAAACQd1PeAAAADElEQVR4nGP4z8AAAAMBAQDJ/pLvAAAAAElFTkSuQmCC\">"
    "<p>Chapter one</p><p><img src=\"data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAAEAAAABCAIAAACQd1PeAAAADElEQVR4nGP4z8AAAAMBAQDJ/pLvAAAAAElFTkSuQmCC\"/> "
    "starts here.</p></body></html>");

void HtmlTextDocumentTest::testImagesAreNotLoaded()
{
    // A plain QTextDocument decodes the background, which is what this test relies on.
    QTextDocument plain;
    plain.setHtml(s_html);
    QCOMPARE(plain.rootFrame()->frameFormat().background().style(), Qt::TexturePattern);

    HtmlTextDocument document;
    document.setHtml(s_html);
    QCOMPARE(document.rootFrame()->frameFormat().background().style(), Qt::NoBrush);

    // The text that the extractors keep is the same.
    QCOMPARE(document.toPlainText(), plain.toPlainText());
    QVERIFY(document.toPlainText().contains(QLatin1String("Chapter one")));
}

QTEST_MAIN(HtmlTextDocumentTest)

#include "htmltextdocumenttest.moc"
