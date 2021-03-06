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

#ifndef KEYWORDDAO_H_
#define KEYWORDDAO_H_

#include <QStringList>

#include <library/dao/GenericDAO.h>
#include <library/dao/BaseDAO.h>
#include <library/Keyword.h>

class KeywordDAO : public BaseDAO, public GenericDAO<Keyword>
{
    public:
        KeywordDAO();
        virtual ~KeywordDAO();

        //virtual bool remove(typename Document::Ptr entity) = 0;
};

#endif /* KEYWORDDAO_H_ */
