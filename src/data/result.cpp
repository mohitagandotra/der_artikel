//******************************************************************************
/*! \file result.cpp Implementation of \ref Result_C
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
#include "result.h"
#include <QLocale>
#include <QDomElement>
#include "data/word.h"
#include "word_model.h"

//******************************************************************************
/*! \brief Constructor.
 *
 *  \author Vikas Pachdha
 *
 *  \param[in] parent : Parent object instance.
 ******************************************************************************/
Result_C::Result_C(QObject *parent):
    QObject(parent),
    _score(0.0),
    _correct_word_count(0),
    _mistakes_count(0),
    _unplayed_count(0),
    _experience_change(0),
    _score_string(tr("No game data.")),
    _mistake_string(""),
    _grade_string(tr("Result")),
    _unplayed_string("")
{
    _incorrect_words_model = new WordModel_C(this);
}

//******************************************************************************
/*! \brief Returns count of words marked correct.
 *
 *  \author Vikas Pachdha
 *
 *  \return unsigned int : Count of words marked correct.
 ******************************************************************************/
unsigned int Result_C::correctWordCount() const
{
    return _correct_word_count;
}

//******************************************************************************
/*! \brief Returs the score achieved.
 *
 *  \details Value is between 0 and 1 being maximum.
 *
 *  \author Vikas Pachdha
 *
 *  \return double : Score achieved.
 ******************************************************************************/
double Result_C::score() const
{
    return _score;
}

//******************************************************************************
/*! \brief Returns count of words marked incorrect.
 *
 *  \author Vikas Pachdha
 *
 *  \return unsigned int : Count of words marked incorrect.
 ******************************************************************************/
unsigned int Result_C::mistakesCount() const
{
    return _mistakes_count;
}

//******************************************************************************
/*! \brief Returns count of words left unplayed.
 *
 *  \author Vikas Pachdha
 *
 *  \return unsigned int : Count of words left unplayed.
 ******************************************************************************/
unsigned int Result_C::unplayedCount() const
{
    return _unplayed_count;
}

//******************************************************************************
/*! \brief Sets the experience points change.
 *
 *  \details Experience change can be negative to deduct experience points.
 *
 *  \author Vikas Pachdha
 *
 *  \param[in] change : Magnitude of experience change.
 ******************************************************************************/
void Result_C::setExperienceChange(int change)
{
    if(change != _experience_change) {
        _experience_change = change;
        emit experienceChanged();
    }
}

//******************************************************************************
/*! \brief Clears the data and reset it to default state.
 *
 *  \author Vikas Pachdha
 ******************************************************************************/
void Result_C::clear()
{
    _score=0.0;
    _correct_word_count=0;
    _mistakes_count=0;
    _unplayed_count=0;
    _experience_change = 0;

    _score_string = tr("No game data.");
    _mistake_string = "";
    _grade_string = tr("Result");
    _unplayed_string="";
}

//******************************************************************************
/*! \brief Updates the result parameters.
 *
 *  \details String data is also updated.
 *
 *  \author Vikas Pachdha
 *
 *  \param[in] score : Score of the gameplay value is between 0 and 1, i being maximum.
 *  \param[in] correct_word_count : Number of correct words.
 *  \param[in] unplayed_count : Number of unplayed words.
 *  \param[in] incorrect_words : Number of incorrect words.
 ******************************************************************************/
void Result_C::updateResult(double score, unsigned int correct_word_count, unsigned int unplayed_count, QList<const Word_C*>incorrect_words)
{
    if(score <0.0) {
        score = 0.0;
    }
    _score = score;
    _correct_word_count= correct_word_count;
    _mistakes_count= incorrect_words.count();
    _unplayed_count= unplayed_count;
    _incorrect_words_model->UpdateWords(incorrect_words);

    double percentage_score = _score *100.0;

    if(percentage_score > 94.99) {
        _grade = GRADE_AP;
    } else if(percentage_score > 89.99) {
        _grade = GRADE_A;
    } else if(percentage_score > 79.99) {
        _grade = GRADE_BP;
    } else if(percentage_score > 69.99) {
        _grade = GRADE_B;
    } else if(percentage_score > 59.99) {
        _grade = GRADE_C;
    } else if(percentage_score > 49.99) {
        _grade = GRADE_D;
    } else  {
        _grade = GRADE_E;
    }

    updateStringData();
    emit resultUpdated();
}

//******************************************************************************
/*! \brief Reads the result data from a xml node.
 *
 *  \author Vikas Pachdha
 *
 *  \param[in] element : Xml node to read data from.
 *
 *  \return bool : Returns whether read was a success.
 *  \retval success status.
 *                      <ul>
 *                         <li> False = Failure
 *                         <li> True = Success
 *                      </ul>
 ******************************************************************************/
bool Result_C::read(const QDomElement &element)
{
    bool success = false;

    if(!element.isNull()) {
        bool ok = false;
        QDomElement dom_date_time = element.firstChildElement("DateTime");
        if(!dom_date_time.isNull()) {
            qint64 msecs = dom_date_time.text().toLongLong(&ok);
            if(ok) {
                _result_date_time = QDateTime::fromMSecsSinceEpoch(msecs);
                success = true;
            } else {
                success = false;
            }
        }

        if(success) {
            QDomElement dom_score = element.firstChildElement("Score");
            if(!dom_score.isNull()) {
                ok= false;
                double score = dom_score.text().toDouble(&ok);
                if(ok) {
                    _score = score;
                    success = true;
                } else {
                    success = false;
                }
            }
        }

        if(success) {
            QDomElement dom_correct_count = element.firstChildElement("CorrectCount");
            if(!dom_correct_count.isNull()) {
                ok= false;
                unsigned int count = dom_correct_count.text().toUInt(&ok);
                if(ok) {
                    _correct_word_count = count;
                    success = true;
                } else {
                    success = false;
                }
            }
        }

        if(success) {
            QDomElement dom_mistakes_count = element.firstChildElement("MistakesCount");
            if(!dom_mistakes_count.isNull()) {
                ok= false;
                unsigned int count = dom_mistakes_count.text().toUInt(&ok);
                if(ok) {
                    _mistakes_count = count;
                    success = true;
                } else {
                    success = false;
                }
            }
        }

        if(success) {
            QDomElement dom_unplayed_count = element.firstChildElement("UnplayedCount");
            if(!dom_unplayed_count.isNull()) {
                ok= false;
                unsigned int count = dom_unplayed_count.text().toUInt(&ok);
                if(ok) {
                    _unplayed_count = count;
                    success = true;
                } else {
                    success = false;
                }
            }
        }

        if(success) {
            updateStringData();
        }

    }
    return success;
}

//******************************************************************************
/*! \brief Writes the result data to xml node.
 *
 *  \author Vikas Pachdha
 *
 *  \param[in] element : Xml node to write data under.
 *
 *  \return bool : Returns whether write was a success.
 *  \retval success status.
 *                      <ul>
 *                         <li> False = Failure
 *                         <li> True = Success
 *                      </ul>
 ******************************************************************************/
bool Result_C::write(QDomElement &element)
{
    bool success = false;

    if( !element.isNull()) {
        QDomDocument domDocument = element.ownerDocument();

        if(!domDocument.isNull()) {
            QDomElement dom_result = domDocument.createElement("Result");

            QDomElement dom_date_time = domDocument.createElement("DateTime");
            QDomText text_date_time = domDocument.createTextNode(QString::number(_result_date_time.toMSecsSinceEpoch()));
            dom_date_time.appendChild(text_date_time);
            dom_result.appendChild(dom_date_time);

            QDomElement dom_score = domDocument.createElement("Score");
            QDomText text_score = domDocument.createTextNode(QString::number(_score,'f',2));
            dom_score.appendChild(text_score);
            dom_result.appendChild(dom_score);

            QDomElement dom_correct_count = domDocument.createElement("CorrectCount");
            QDomText text_correct_count = domDocument.createTextNode(QString::number(_correct_word_count));
            dom_correct_count.appendChild(text_correct_count);
            dom_result.appendChild(dom_correct_count);

            QDomElement dom_mistakes_count = domDocument.createElement("MistakesCount");
            QDomText text_mistakes_count = domDocument.createTextNode(QString::number(_mistakes_count));
            dom_mistakes_count.appendChild(text_mistakes_count);
            dom_result.appendChild(dom_mistakes_count);

            QDomElement dom_unplayed_count = domDocument.createElement("UnplayedCount");
            QDomText text_unplayed_count = domDocument.createTextNode(QString::number(_unplayed_count));
            dom_unplayed_count.appendChild(text_unplayed_count);
            dom_result.appendChild(dom_unplayed_count);

            element.appendChild(dom_result);

            success = true;
        }
    }

    return success;
}

//******************************************************************************
/*! \brief Helper method to update string data as per the current result data.
 *
 *  \author Vikas Pachdha
 ******************************************************************************/
void Result_C::updateStringData()
{
    QLocale locale;
    double score = _score*100;
    _score_string = tr("Score : %1 %").arg(locale.toString(score,'f',2));
    _mistake_string = tr("Mistakes : %1").arg(_mistakes_count);
    _unplayed_string = tr("Unplayed : %1").arg(_unplayed_count);

    if(_grade >= GRADE_AP) {
        _grade_string = tr("Execelent");
    } else if(_grade >= GRADE_A) {
        _grade_string = tr("Very good");
    } else if (_grade >= GRADE_BP) {
        _grade_string = tr("Good");
    } else if (_grade >= GRADE_C) {
        _grade_string = tr("Satisfactory");
    } else if(_grade >= GRADE_D){
        _grade_string = tr("Sufficient");
    } else {
        _grade_string = tr("Failed");
    }

    _grade_string.prepend(tr("Result : "));
}



