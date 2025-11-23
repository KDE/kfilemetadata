/*
    SPDX-FileCopyrightText: 2025 Stefan Brüns <stefan.bruens@rwth-aachen.de>

    SPDX-License-Identifier: LGPL-2.1-or-later
*/

#ifndef CSVDECODER_P_H
#define CSVDECODER_P_H

#include <QRegularExpression>

namespace KFileMetaData
{
namespace Helper
{

namespace
{
    auto QUOTE = QLatin1Char('\"');
    auto COMMA = QLatin1Char(',');
    auto SEMICOLON = QLatin1Char(';');
    auto TABULATOR = QLatin1Char('\t');
    auto CR = QLatin1String("\r");
    auto NL = QLatin1String("\n");
    auto CRNL = QLatin1String("\r\n");
    const QRegularExpression CrnlRe(QStringLiteral("\r?\n|\r"));

    auto isEscapedQuote(const QStringView buffer, qsizetype pos) -> bool
    {
        if (buffer.size() <= pos + 1)
            return false;
        return buffer[pos + 1] == QUOTE;
    }
}

struct CsvStyle
{
    enum Delimiter {
        Comma,
        Semicolon,
        Tabulator,
    } delimiter = Comma;
    enum LineTerminator {
        CR,
        NL,
        CRNL,
    } lineTerminator = NL;
};


/**
  * Try to determine the delimiter and line separator of a CSV file
  *
  * The fields may be delimited with either a Comma \c "," or
  * a Semicolon ";" character, while the other may be used as a
  * decimal separator.
  */
CsvStyle detectCsvStyle(const QStringView buffer)
{
    auto analyzeLine = [&](const QStringView buffer) -> std::tuple<QStringView, CsvStyle::LineTerminator, size_t, size_t, size_t> {
        qsizetype unquotedPos = 0;
        qsizetype nlPos = 0;

        size_t commaCount = 0;
        size_t semicolonCount = 0;
        size_t tabulatorCount = 0;

        while (unquotedPos < buffer.size()) {
            nlPos = buffer.indexOf(CrnlRe, nlPos);
            nlPos = nlPos == -1 ? buffer.size() : nlPos;
            auto quotePos = buffer.indexOf(QUOTE, unquotedPos);
            quotePos = quotePos == -1 ? nlPos : quotePos;

            // count possible delimiters up to newline or next quote
            auto end = std::min(nlPos, quotePos);
            auto part = buffer.sliced(unquotedPos, end - unquotedPos);
            commaCount += part.count(COMMA);
            semicolonCount += part.count(SEMICOLON);
            tabulatorCount += part.count(TABULATOR);

            if (nlPos <= quotePos) {
                auto match = CrnlRe.matchView(buffer, nlPos, QRegularExpression::PartialPreferFirstMatch);
                auto lineSepMatch = match.captured(0);
                auto lineSep = (lineSepMatch == CR) ? CsvStyle::LineTerminator::CR :
                               (lineSepMatch == CRNL) ? CsvStyle::LineTerminator::CRNL : CsvStyle::LineTerminator::NL;
                return {buffer.sliced(nlPos + lineSepMatch.size()), lineSep, commaCount, semicolonCount, tabulatorCount};
            }

            unquotedPos = quotePos + 1;
            // unquotedPos points 1 after opening quote
            do {
                quotePos = buffer.indexOf(QUOTE, unquotedPos);
                if (!isEscapedQuote(buffer, quotePos)) {
                    unquotedPos = quotePos + 1;
                    break;
                }
                unquotedPos = quotePos + 2;
            } while (true);
            // unquotedPos points 1 after closing quote

            nlPos = std::max(nlPos, unquotedPos);
        }
        return {{}, CsvStyle::LineTerminator::NL, commaCount, semicolonCount, tabulatorCount};
    };

    auto [tail, nl, cc, sc, tc] = analyzeLine(buffer);

    for (int line = 0; line < 10; line++) {
        if (tail.isEmpty()) {
            break;
        }
        const auto [tail2, _, cc2, sc2, tc2] = analyzeLine(tail);

        if (sc2 != sc && tc2 != tc && cc) {
            return {CsvStyle::Comma, nl};
        } else if (cc2 != cc && tc2 != tc && sc) {
            return {CsvStyle::Semicolon, nl};
        } else if (cc && !sc && !tc) {
            return {CsvStyle::Comma, nl};
        } else if (sc && !cc && !tc) {
            return {CsvStyle::Semicolon, nl};
        } else if (tc && !cc && !sc) {
            return {CsvStyle::Tabulator, nl};
        }
        tail = tail2;
        cc = cc2;
        sc = sc2;
        tc = tc2;
    }

    if ((sc > cc) && (sc > tc))
        return {CsvStyle::Semicolon, nl};
    if ((tc > cc) && (tc > sc))
        return {CsvStyle::Tabulator, nl};
    return {CsvStyle::Comma, nl};
}

/**
 *  Split the CSV data in @p buffer into lines and fields
 *  @return list of row, each containing a list of fields
 */
auto decodeCsv(const QStringView buffer, CsvStyle style) -> std::pair<QList<QStringList>, qsizetype>
{
    const auto delimiter = (style.delimiter == CsvStyle::Comma) ? COMMA :
                           (style.delimiter == CsvStyle::Semicolon) ? SEMICOLON : TABULATOR;
    const auto lineTerminator = (style.lineTerminator == CsvStyle::CR) ? CR :
                                (style.lineTerminator == CsvStyle::NL) ? NL : CRNL;

    QList<QStringList> result{QStringList{}};

    qsizetype consumed = 0;
    qsizetype startOfField = 0;
    qsizetype nlPos = 0;

    while (startOfField < buffer.size()) {
        nlPos = buffer.indexOf(lineTerminator, nlPos);
        nlPos = nlPos == -1 ? buffer.size() : nlPos;

        qsizetype endOfField = startOfField + 1;

        if (buffer[startOfField] == QUOTE) {
            do {
                endOfField = buffer.indexOf(QUOTE, endOfField);
                if (endOfField == -1) {
                    endOfField = buffer.size();
                    break;
                }
                if (!isEscapedQuote(buffer, endOfField)) {
                    break;
                }
                endOfField += 2;
            } while (true);
            auto part = buffer.sliced(startOfField + 1, endOfField - startOfField - 1).toString();
            part.replace(QLatin1String("\"\""), QLatin1String("\""));
            result.back().append(part);

            startOfField = endOfField + 1;

        } else {
            endOfField = buffer.indexOf(delimiter, endOfField);
            if ((endOfField == -1) || (endOfField > nlPos)) {
                endOfField = nlPos;
            }

            auto part = buffer.sliced(startOfField, endOfField - startOfField);
            result.back().append(QString{part});

            startOfField = endOfField;
        }

        // startOfField points to line terminator or field delimiter,
        // advance to actual start of next field
        if ((startOfField + lineTerminator.size() <= buffer.size()) &&
            (buffer.sliced(startOfField, lineTerminator.size()) == lineTerminator))
        {
            startOfField += lineTerminator.size();
            consumed = startOfField;
            if (startOfField < buffer.size()) {
                result.append(QStringList{});
            }
        } else {
            startOfField += 1;
        }

        nlPos = std::max(nlPos, startOfField);
    }

    return {result, consumed};
}

} // namespace Helper
} // namespace KFileMetaData

Q_DECLARE_METATYPE(KFileMetaData::Helper::CsvStyle::Delimiter)

#endif // CSVDECODER_P_H
