/*
 * Copyright (C) 2011-2012 Matias Valdenegro <matias.valdenegro@gmail.com>
 * This file is part of kresearch.
 *
 * kresearch is free software: you can redistribute it and/or modify
 * it under the terms of the GNU  General Public License as published by
 * the Free Software Foundation, version 3 of the License.
 *
 * kresearch is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU  General Public License for more details.
 *
 * You should have received a copy of the GNU  General Public License
 * along with kresearch.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "Repository.h"

Repository *Repository::mInstance = 0;

Repository::Repository()
{
    mAuthorCache = new RepositoryCache<Author>();
    mPubCache = new RepositoryCache<Document>();
    mJournalCache = new RepositoryCache<Journal>();
    mCollectionCache = new RepositoryCache<DocumentCollection>();

    mInstance = this;
}

Repository::~Repository()
{
}

