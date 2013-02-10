/*
 * Copyright (C) 2011-2012 Matias Valdenegro <matias.valdenegro@gmail.com>
 * This file is part of KResearch.
 *
 * kesearch is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3 of the License.
 *
 * kresearch is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with KResearch.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "BibtexParser.h"

#include <QMap>
#include <QStringList>
#include <QDebug>

BibtexParser::BibtexParser()
{
}

BibtexParser::~BibtexParser()
{
}

static QRegExp entryRegexp = QRegExp("@([a-zA-Z]+)\\s*\\{(.+)\\}");

Document::List BibtexParser::parse(const QString& str) const
{
    Document::List ret;
    int pos = entryRegexp.indexIn(str);

    if(pos == -1) {
        return ret;
    }

    QStringList entries = entryRegexp.capturedTexts();
    //Skip first entry since it contains the whole matched text.
    entries.removeAt(0);

    //Entries size should be a multiple of 2, a pair of bibtex entry type and bibtex entry data.
    if((entries.count() % 2) == 1) {
        return ret;
    }

    for(int i = 0; i < entries.count(); i += 2) {
        QString type = cleanup(entries[i]).toLower();
        QString data = entries[i + 1];

        qDebug() << "Type" << type;
        qDebug() << "Data" << data;

        Document::Ptr document = fillDocumentData(type, data);

        if(document != nullptr) {
            ret.append(document);
        }
    }

    return ret;
}

QString BibtexParser::cleanup(const QString& input) const
{
    QString tmp = input;
    tmp = tmp.remove('\n');
    tmp = tmp.remove('}');
    tmp = tmp.remove('{');

    return tmp.trimmed();
}

QStringList BibtexParser::cleanup(const QStringList& input) const
{
    QStringList ret;

    foreach(QString s, input) {
        ret += cleanup(s);
    }

    return ret;
}

QString BibtexParser::removeQuotes(const QString& input) const
{
    return QString(input).remove('"');
}

typedef QMap<QString, QString> StringMap;

class BibtexDocumentFiller
{
    public:
        BibtexDocumentFiller()
        {
        }

        virtual ~BibtexDocumentFiller()
        {
        }

        virtual Document::Ptr fillDocument(const StringMap& data) const = 0;

        Author::List parseAuthors(const QString& authorString) const
        {
            Author::List authors;
            QStringList authorParts = authorString.split("and");

            foreach(QString authorPart, authorParts) {
                Author::Ptr author = new Author();
                author->setFirstName(authorPart);

                authors.append(author);
            }

            return authors;
        }
};

class BibtexArticleFiller : public BibtexDocumentFiller
{
    public:
        BibtexArticleFiller() {}

        virtual ~BibtexArticleFiller() {}

        virtual Document::Ptr fillDocument(const StringMap& data) const
        {
            Document::Ptr document = new Document();

            //Type
            document->setType(DocumentType::Article);

            //Required fields
            document->setTitle(data["title"]);

            Journal::Ptr journal = new Journal();
            journal->setName(data["journal"]);
            document->setJournal(journal);

            document->setYear(data["year"].toInt());
            document->setAuthors(parseAuthors(data["author"]));

            //Optional fields
            document->setVolume(data["volume"].toInt());
            document->setNumber(data["number"].toInt());

            return document;
        }
};

class BibtexBookFiller : public BibtexDocumentFiller
{
    public:
        BibtexBookFiller() {}

        virtual ~BibtexBookFiller() {}

        virtual Document::Ptr fillDocument(const StringMap& data) const
        {
            Document::Ptr document = new Document();

            //Type
            document->setType(DocumentType::Book);

            //Required fields
            document->setTitle(data["title"]);
            document->setPublisher(data["publisher"]);
            document->setYear(data["year"].toInt());

            if(data.contains("author")) {
                document->setAuthors(parseAuthors(data["author"]));
            } else {
                document->setAuthors(parseAuthors(data["editor"]));
            }

            //Optional fields
            document->setVolume(data["volume"].toInt());
            document->setNumber(data["number"].toInt());

            return document;
        }
};

class BibtexConferenceFiller : public BibtexDocumentFiller
{
    public:
        BibtexConferenceFiller() {}

        virtual ~BibtexConferenceFiller() {}

        virtual Document::Ptr fillDocument(const StringMap& data) const
        {
            Document::Ptr document = new Document();

            //Type
            document->setType(DocumentType::ConferenceArticle);

            //Required fields
            document->setTitle(data["title"]);
            document->setConference(data["booktitle"]);
            document->setYear(data["year"].toInt());
            document->setAuthors(parseAuthors(data["author"]));

            //Optional fields
            document->setVolume(data["volume"].toInt());
            document->setNumber(data["number"].toInt());

            return document;
        }
};

class BibtexNullFiller : public BibtexDocumentFiller
{
    public:
        BibtexNullFiller() {}

        virtual ~BibtexNullFiller() {}

        virtual Document::Ptr fillDocument(const StringMap& data) const
        {
            return nullptr;
        }
};

static BibtexDocumentFiller *nullFiller = new BibtexNullFiller();

inline QMap<QString, BibtexDocumentFiller *> createFillerMap()
{
    QMap<QString, BibtexDocumentFiller *> map;
    map.insert("book"           , new BibtexBookFiller());
    map.insert("article"        , new BibtexArticleFiller());
    map.insert("booklet"        , nullptr);
    map.insert("conference"     , new BibtexConferenceFiller());
    map.insert("inbook"         , nullptr);
    map.insert("incollection"   , nullptr);
    map.insert("inproceedings"  , new BibtexConferenceFiller());
    map.insert("manual"         , nullptr);
    map.insert("masterthesis"   , nullptr);
    map.insert("misc"           , nullptr);
    map.insert("phdthesis"      , nullptr);
    map.insert("proceedings"    , nullptr);
    map.insert("techreport"     , nullptr);
    map.insert("unpublished"    , nullptr);

    return map;
}

static QMap<QString, BibtexDocumentFiller *> fillerMap = createFillerMap();


BibtexDocumentFiller *dispatchFiller(const QString& type)
{
    if(fillerMap.contains(type)) {
        return fillerMap[type];
    }

    return nullFiller;
}

const QChar comma = QChar(',');
const QChar quote = QChar('"');
const QChar openingBrace = QChar('{');
const QChar closingBrace = QChar('}');

//This function splits a QString at each comma, but skips splitting between pairs of quotes (" ... "),
//or between braces ({ .... }).
inline QStringList splitWithComas(const QString& str)
{
    QStringList ret;
    int start = 0;
    int i = 0;

    for(i = 0; i < str.length(); i++) {
        if(str[i] == comma) {
            QString part = str.mid(start, i - start);
            start = i+1;
            ret.append(part);
        }

        if(str[i] == quote) {
            for(i++; str[i] != QChar('"'); i++) {}
        }

        if(str[i] == openingBrace) {
            for(i++; str[i] != closingBrace; i++) {}
        }
    }

    ret.append(str.mid(start, i - start));

    return ret;
}

Document::Ptr BibtexParser::fillDocumentData(const QString& type, const QString& rawData) const
{
    QStringList data = splitWithComas(rawData);
    StringMap dataMap;

    if(data.count() == 0) {
        return nullptr;
    }

    //Assume that the first element
    QString citeKey = data[0];

    qDebug() << "Splitting data" << rawData;

    for(int i = 1; i < data.count(); i++) {
        QStringList parts = data[i].split('=');
        parts = cleanup(parts);

        if(parts.count() != 2) {

            qDebug() << "Parts don't match" << parts;

            return nullptr;
        }

        QString tag = parts[0].toLower();
        QString tagValue = removeQuotes(parts[1]);

        qDebug() << "Tag" << tag;
        qDebug() << "TagValue" << tagValue;

        dataMap.insert(tag, tagValue);
    }

    Document::Ptr document = dispatchFiller(type)->fillDocument(dataMap);

    return document;
}
