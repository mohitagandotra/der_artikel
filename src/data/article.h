//******************************************************************************
/*! \file article.h Declaration of \ref Article_C
 *
 *  \author Vikas Pachdha
 *
 *  \copyright Copyright (C) 2014 Vikas Pachdha, Mohita Gandotra.
 * Contact: http://www.vikaspachdha.com
 *
 * This file is part of the application der_artikel.
 *
 *  \copyright GNU Lesser General Public License Usage
 * This file may be used under the terms of the GNU Lesser
 * General Public License version 2.1 as published by the Free Software
 * Foundation and appearing in the file LICENSE.LGPL included in the
 * packaging of this file.  Please review the following information to
 * ensure the GNU Lesser General Public License version 2.1 requirements
 * will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
 *
 *  \copyright GNU General Public License Usage
 * Alternatively, this file may be used under the terms of the GNU
 * General Public License version 3.0 as published by the Free Software
 * Foundation and appearing in the file LICENSE.GPL included in the
 * packaging of this file.  Please review the following information to
 * ensure the GNU General Public License version 3.0 requirements will be
 * met: http://www.gnu.org/copyleft/gpl.html.
 *
 ******************************************************************************/

#ifndef ARTICLE_H
#define ARTICLE_H

#include <QObject>

//******************************************************************************
/*! \brief Defines German article.
 *
 *  \author Vikas Pachdha
 ******************************************************************************/
class Article_C : public QObject
{
    Q_OBJECT
    Q_ENUMS(Artikel)
    Q_DISABLE_COPY(Article_C)
public:
    //******************************************************************************
    /*! \brief Enumeration defining German article.
     *
     *  \author Vikas Pachdha
     *
     *  \see \ref Article_C::ArtikelText
     ******************************************************************************/
    enum Artikel
    {
        DER = 1,
        DIE,
        DAS,
        NA,
        INVALID
    };

private:
    explicit Article_C(QObject *parent = 0);

public:
    static QString ArtikelText(Article_C::Artikel artikel);
};

#endif // ARTICLE_H
