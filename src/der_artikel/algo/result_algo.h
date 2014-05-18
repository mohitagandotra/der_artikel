//******************************************************************************
/*! \file result_algo.h Declaration \ref ResultAlgo_I interface
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
#ifndef RESULTALGO_I_H
#define RESULTALGO_I_H

// Forward declarations
class Thema_I;
class Result_C;

//******************************************************************************
/*! \brief Result Algorithm interface.
 *
 *  \details Algorithm decides the scoring and time alocated to complete a thema.
 *
 *  \author Vikas Pachdha
 *
 *  \see \ref ModerateResultAlgo_C , \ref StrictResultAlgo_C , \ref EasyResultAlgo_C
 ******************************************************************************/
class ResultAlgo_I
{
public:
    ResultAlgo_I() {}
    virtual ~ResultAlgo_I() {}

public:
    virtual void calculate(Thema_I& thema, Result_C& result) = 0;
    virtual int playTime(Thema_I& thema) = 0;
};

#endif // RESULTALGO_I_H
