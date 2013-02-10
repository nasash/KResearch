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

#ifndef BIBTEXPARSER_H_
#define BIBTEXPARSER_H_

#include <library/Document.h>

class BibtexParser
{
    public:
        BibtexParser();
        virtual ~BibtexParser();

        Document::List parse(const QString& str) const;

    private:
        QString cleanup(const QString& input) const;
        QStringList cleanup(const QStringList& input) const;
        QString removeQuotes(const QString& input) const;
        DocumentType typeFromString(const QString& type) const;

        Document::Ptr fillDocumentData(const QString& type, const QString& data) const;
};

#endif /* BIBTEXPARSER_H_ */