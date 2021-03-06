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

#include "PublicationEditDialog.h"
#include "AuthorEditWidget.h"

#include <pdfwidget/PDFDocumentDialog.h>
#include <library/Repository.h>

#include <QDebug>
#include <QVBoxLayout>

#include <KIcon>
#include <KFileDialog>

PublicationEditDialog::PublicationEditDialog(Document::Ptr pubToEdit)
 : QDialog(), mPub(pubToEdit)
{
    setupUi(this);

    QVBoxLayout *authorEditLayout = new QVBoxLayout(authorEditWidget);
    mEditWidget = new AuthorEditWidget(authorEditWidget);

    authorEditLayout->addWidget(mEditWidget);
    authorEditLayout->setSpacing(0);
    authorEditLayout->setMargin(0);
    authorEditLayout->setContentsMargins(0, 0, 0, 0);

    setWindowTitle("Edit publication");

    acceptButton->setIcon(KIcon("dialog-ok"));
    cancelButton->setIcon(KIcon("dialog-cancel"));

    addAuthorButton->setIcon(KIcon("list-add"));
    removeAuthorButton->setIcon(KIcon("list-remove"));
    localURLButton->setIcon(KIcon("document-open"));

    typeComboBox->addItems(documentTypeStringValues());

    connect(acceptButton, SIGNAL(clicked()), this, SLOT(accept()));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));

    connect(titleButton, SIGNAL(clicked()), this, SLOT(acquireTitle()));
    connect(authorButton, SIGNAL(clicked()), mEditWidget, SLOT(acquireAuthor()));
    connect(abstractButton, SIGNAL(clicked()), this, SLOT(acquireAbstract()));
    connect(yearButton, SIGNAL(clicked()), this, SLOT(acquireYear()));
    connect(journalButton, SIGNAL(clicked()), this, SLOT(acquireJournal()));
    connect(conferenceButton, SIGNAL(clicked()), this, SLOT(acquireConference()));
    connect(publisherButton, SIGNAL(clicked()), this, SLOT(acquirePublisher()));
    connect(seriesButton, SIGNAL(clicked()), this, SLOT(acquireSeries()));
    connect(subseriesButton, SIGNAL(clicked()), this, SLOT(acquireSubseries()));
    connect(urlButton, SIGNAL(clicked()), this, SLOT(acquireUrl()));
    connect(doiButton, SIGNAL(clicked()), this, SLOT(acquireDoi()));
    connect(isbnButton, SIGNAL(clicked()), this, SLOT(acquireIsbn()));

    connect(addAuthorButton, SIGNAL(clicked()), mEditWidget, SLOT(addAuthor()));
    connect(removeAuthorButton, SIGNAL(clicked()), mEditWidget, SLOT(removeSelectedAuthor()));
    connect(localURLButton, SIGNAL(clicked()), this, SLOT(selectLocalURL()));

    connect(this, SIGNAL(accepted()), this, SLOT(save()));

    fill(pubToEdit);
    setupAutocompletions();
}

PublicationEditDialog::~PublicationEditDialog()
{
}

void PublicationEditDialog::fill(Document::Ptr pub)
{
    titleEdit->setText(pub->title());
    abstractTextEdit->setPlainText(pub->abstract());
    yearEdit->setText(QString::number(pub->year()));
    conferenceEdit->setText(pub->conference());
    //journalEdit->setText(pub->journal());
    publisherEdit->setText(pub->publisher());
    //seriesEdit->setText(pub->series());
    //subseriesEdit->setText(pub->subseries());
    volumeEdit->setText(QString::number(pub->volume()));
    numberEdit->setText(QString::number(pub->number()));
    urlEdit->setText(pub->url());
    doiEdit->setText(pub->doi());
    isbnEdit->setText(pub->isbn());
    localFileEdit->setText(pub->localUrl());

    int index = typeComboBox->findText(documentTypeToString(pub->type()));
    typeComboBox->setCurrentIndex(index);

    mEditWidget->setAuthorList(pub->authors());
    mEditWidget->setLocalUrl(pub->localUrl());
}

void PublicationEditDialog::setupAutocompletions()
{
     QStringList journals = Repository::self()->publicationDAO()->journals();

    QStringList conferences = Repository::self()->publicationDAO()->conferences();
}

void PublicationEditDialog::saveAuthors()
{
    QStringList authors = mEditWidget->currentAuthorList();
    Author::List authList;

    mPub->setAuthors(authors);
}

Document::Ptr PublicationEditDialog::editPublication(Document::Ptr pub)
{
    PublicationEditDialog dialog(pub);

    dialog.exec();

    return pub;
}

void PublicationEditDialog::selectLocalURL()
{
    QString localURL = KFileDialog::getOpenFileName(KUrl(), QString(), this, "Select local file");

    localFileEdit->setText(localURL);
}

void PublicationEditDialog::save()
{
    mPub->setTitle(titleEdit->text());
    mPub->setAbstract(abstractTextEdit->toPlainText());
    mPub->setYear(yearEdit->text().toInt());
    //mPub->setJournal(journalEdit->text());
    mPub->setConference(conferenceEdit->text());
    mPub->setPublisher(publisherEdit->text());
    //mPub->setSeries(seriesEdit->text());
    //mPub->setSubseries(subseriesEdit->text());
    mPub->setVolume(volumeEdit->text().toInt());
    mPub->setNumber(numberEdit->text().toInt());
    mPub->setUrl(urlEdit->text());
    mPub->setDoi(doiEdit->text());
    mPub->setIsbn(isbnEdit->text());
    mPub->setLocalUrl(localFileEdit->text());
    mPub->setType(stringToDocumentType(typeComboBox->currentText()));

    saveAuthors();

    Repository::self()->publicationDAO()->saveOrUpdate(mPub);
}

void PublicationEditDialog::acquireTitle()
{
    QString title = PDFDocumentDialog::selectString(mPub->localUrl());

    titleEdit->setText(title.simplified());
}

void PublicationEditDialog::acquireAbstract()
{
    QString abstract = PDFDocumentDialog::selectString(mPub->localUrl());

    abstractTextEdit->setText(abstract.simplified());
}

void PublicationEditDialog::acquireYear()
{

}

void PublicationEditDialog::acquireJournal()
{

}

void PublicationEditDialog::acquireConference()
{

}

void PublicationEditDialog::acquirePublisher()
{

}

void PublicationEditDialog::acquireSeries()
{

}

void PublicationEditDialog::acquireSubseries()
{

}

void PublicationEditDialog::acquireUrl()
{

}

void PublicationEditDialog::acquireDoi()
{

}

void PublicationEditDialog::acquireIsbn()
{

}
